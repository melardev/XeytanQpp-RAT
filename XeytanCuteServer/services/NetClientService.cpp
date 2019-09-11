#include "NetClientService.h"
#include "NetServerService.h"
#include "shared/net/packets/PacketConnection.h"
#include "shared/net/packets/PacketPresentation.h"
#include "shared/net/packets/PacketProcessResponse.h"
#include "shared/net/packets/PacketDesktopResponse.h"
#include "shared/net/packets/PacketLoginRequest.h"
#include "shared/net/packets/PacketMediaResponse.h"
#include "shared/net/packets/PacketDesktopConfigResponse.h"
#include "shared/net/packets/PacketCameraConfigResponse.h"
#include "shared/net/packets/PacketCameraError.h"
#include "shared/net/packets/PacketShell.h"
#include "shared/net/packets/packet_defs.h"
#include "shared/net/packets/PacketChat.h"

NetClientService::NetClientService()
{
}

NetClientService::NetClientService(const QSharedPointer<NetServerService>& server,
                                   QTcpSocket* clientTcpSocket)
	: clientSocket(clientTcpSocket), clientSocketDescriptor(clientTcpSocket->socketDescriptor()),
	  netServerService(server)
{
	packetProcessor = QSharedPointer<QDataStreamPacketProcessor>::create();

	// We would need to know this which is the minimum data we can read at the beginning of a readyRead operation
	QByteArray byteArray;
	QDataStream writeStream(&byteArray, QIODevice::WriteOnly);
	writeStream.setVersion(QDataStream::Qt_4_0);
	writeStream
		<< quint64(0);

	packetSizeLength = byteArray.size();
	writeStream << qint16(0);

	packetTypeLength = byteArray.size() - packetSizeLength;

	hasReadHeader = false;
	packetHeaderLength = byteArray.size();
}


NetClientService::~NetClientService()
{
	qDebug() << QString("NetClientService with socket descriptor %1 is being destroyed ").arg(getSocketDescriptor());
}

void NetClientService::interact()
{
	connect(clientSocket, &QTcpSocket::readyRead, this, &NetClientService::onSocketReadyRead);
	connect(clientSocket, &QTcpSocket::disconnected, this, &NetClientService::onDisconnected);
	connect(clientSocket, qOverload<QAbstractSocket::SocketError>(&QTcpSocket::error), this,
	        &NetClientService::onSocketError);
	// connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnSocketError(QAbstractSocket::SocketError)));
}


void NetClientService::onSocketReadyRead()
{
	// QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	QDataStream socketInputStream(clientSocket);
	socketInputStream.setVersion(QDataStream::Qt_4_0);
	const quint64 available = clientSocket->bytesAvailable();

	// If we are reading a packet from scratch and we don't have yet 4 bytes for the
	// packet header length then return, the next time onSocketReadyRead() is called hopefully
	// we have those 4 bytes
	if (!hasReadHeader && available < packetHeaderLength)
		return;

	// If we are reading a packet from scratch and we have those 4 first bytes then read them
	if (!hasReadHeader)
	{
		socketInputStream >> packetSize;
		// if packetSize 0 it is an invalid, state, discard the packet
		if (packetSize == 0)
		{
			packetSize = 0;
			return;
		}

		hasReadHeader = true;
		// if what is available is less than the whole packet(packetSize == available)
		// then return, keep the data in the socket buffer, it is important to not read anything until
		// we get the whole packet, because any byte we read from the socket will be discarded and will
		// not be available in the next onSocketReadyRead() call
		if ((available - packetSizeLength) < packetSize)
			return;
	}
	else
	{
		// If we have already read the packet Size header from a previous call to onSocketReadyRead
		// then wait for the packet to be complete, if not return, keeping the data in the socket buffer
		if ((available - packetSizeLength) < packetSize)
			return;
	}

	// Now have the packetSize field stating the size of the packet, and we are sure
	// we have the whole packet in the socket buffer, let's read from it


	qint16 packetTypeInt;
	socketInputStream >> packetTypeInt;
	PacketType packetType = (PacketType)packetTypeInt;

	// The packetSize included the length needed to write packetType(qint16)
	// we calculated how much space does it take before, so substract it
	packetSize = packetSize - packetTypeLength;

	// qDebug() << payloadLen;

	switch (packetType)
	{
	case PacketType::Login:
		handleLogin();
		break;
	case PacketType::Presentation:
		handlePresentation(packetSize);
		break;
	case PacketType::DesktopConfig:
		handleRemoteDesktopConfig(packetSize);
		break;
	case PacketType::Desktop:
		handleRemoteDesktop(packetSize);
		break;
	case PacketType::CameraConfig:
		handleRemoteCameraConfig(packetSize);
		break;
	case PacketType::Camera:
		handleRemoteCamera(packetSize);
		break;
	case PacketType::CameraError:
		handleCameraError(packetSize);
	case PacketType::Shell:
		handleShell(packetSize);
		break;
	case PacketType::ProcessList:
		handleProcessList(packetSize);
		break;
	case PacketType::Chat:
		handleChat(packetSize);
		break;
	default:
		{
			qDebug() << "Packet Type not handled: " << (qint16)packetType << " with packet size: " << packetSize;
			// Free the socket buffer and begin from scratch
			clientSocket->readAll();
			break;
		}
	}

	packetSize = 0;
	hasReadHeader = false;

	// Ensure handle methods have read all bytes, otherwise those will remain for the next onSocketReadyRead call
	// that will be received the next time this function is called leading to unexpected behaviour
	if (clientSocket->bytesAvailable() > 0)
	{
		qDebug() << "There are " << clientSocket->bytesAvailable() << " bytes not read yet from the socket "
			<< " Packet Type: " << (qint16)packetType;

		// We have more data, let's try to read read it also
		// TODO: handle a possible DoS
		onSocketReadyRead();
	}
}

void NetClientService::handleLogin()
{
	Q_ASSERT(false);
	QDataStream readStream(clientSocket);
	readStream.setVersion(QDataStream::Qt_4_0);
	PacketLoginRequest packet;
	// QSharedPointer<PacketLoginRequest> packet;
	// readStream >> *packet;

	readStream >> packet;
	emit packetReceived(packet);
}

void NetClientService::handlePresentation(quint64 payloadLen)
{
	PacketPresentation packet;
	QByteArray data = clientSocket->read(payloadLen);
	qDebug() << data.size();

	packetProcessor->deserialize(data, packet);
	// emit packetReceived(packet);
	emit presentPacketReceived(packet);
}

// =================
// Desktop handling
// =================
void NetClientService::handleRemoteDesktopConfig(quint64 packetLength)
{
	PacketDesktopConfigResponse packet;
	const QByteArray data = clientSocket->read(packetLength);
	if (clientSocket->bytesAvailable() > 0)
	{
		qDebug() << "";
	}

	packetProcessor->deserialize(data, packet);
	emit desktopConfigPacketReceived(packet);
}

void NetClientService::handleChat(quint64 packetLength)
{
	PacketChat packet;
	const QByteArray data = clientSocket->read(packetLength);

	packetProcessor->deserialize(data, packet);
	emit chatPacketReceived(packet);
}

void NetClientService::handleRemoteDesktop(quint64 payloadLen)
{
	PacketDesktopResponse packet;
	QByteArray data = clientSocket->read(payloadLen);
	qDebug() << data.size();

	packetProcessor->deserialize(data, packet);
	emit desktopPacketReceived(packet);
}

// =================
// Camera handling
// =================
void NetClientService::handleRemoteCameraConfig(quint64 packetLength)
{
	PacketCameraConfigResponse packet;
	QByteArray data = clientSocket->readAll();
	Q_ASSERT(data.size() == packetLength);

	packetProcessor->deserialize(data, packet);
	emit cameraConfigPacketReceived(packet);
}

void NetClientService::handleRemoteCamera(quint64 payloadLen)
{
	PacketMediaResponse packet(PacketType::Camera);
	QByteArray data = clientSocket->read(payloadLen);
	qDebug() << data.size();

	packetProcessor->deserialize(data, packet);
	emit cameraPacketReceived(packet);
}

void NetClientService::handleCameraError(quint64 payloadLen)
{
	PacketCameraError packet;
	QByteArray data = clientSocket->read(payloadLen);
	qDebug() << data.size();

	packetProcessor->deserialize(data, packet);
	emit cameraErrorPacketReceived(packet);
}

// =======
//	Process handling
// =======
void NetClientService::handleProcessList(quint64 packetLength)
{
	PacketProcessResponse packet;
	const QByteArray data = clientSocket->read(packetLength);
	if (clientSocket->bytesAvailable() > 0)
	{
		qDebug() << "There were more data available into the socket buffer";
	}

	packetProcessor->deserialize(data, packet);
	emit processPacketReceived(packet);
}

void NetClientService::handleShell(quint64 packetLength)
{
	PacketShell packet;
	const QByteArray data = clientSocket->read(packetLength);
	if (clientSocket->bytesAvailable() > 0)
	{
		qDebug() << "There were more data available into the socket buffer";
	}

	packetProcessor->deserialize(data, packet);
	emit shellPacketReceived(packet);
}

void NetClientService::closeConnection(bool sendCloseConnectionPacket)
{
	if (sendCloseConnectionPacket)
	{
		const PacketConnection packet(ConnectionAction::Close);
		sendPacket(packet);
	}
	clientSocket->close();
}

void NetClientService::onSocketError(QAbstractSocket::SocketError err)
{
	qDebug() << "onSocketError " << clientSocketDescriptor;
}

void NetClientService::onDisconnected()
{
	// auto disconnectedSocket = static_cast<QTcpSocket*>(sender());
	clientSocket->close();
	netServerService->onClientDisconnected(sharedFromThis());
}

QTcpSocket* NetClientService::getSocketObject() const
{
	return clientSocket;
}

void NetClientService::set_socket_obj(QTcpSocket* socket)
{
	this->clientSocket = socket;
}


void NetClientService::sendPacket(const Packet& packet)
{
	// Send with QDataStream serializer
	QByteArray byteArray;
	QDataStream writeStream(&byteArray, QIODevice::WriteOnly);
	writeStream.setVersion(QDataStream::Qt_4_0);
	writeStream
		<< quint64(0) // Packet size place holder
		<< (qint16)packet.type() // write packet type as qint16
		<< packet; // serialize packet payload

	// Go at the beginning of the stream and write the packet size
	writeStream.device()->seek(0); // write packet size
	writeStream << quint64(byteArray.size() - packetHeaderLength); // writing data size excluding the packetHeaderLength
	clientSocket->write(byteArray);
	clientSocket->waitForBytesWritten();
}

qint64 NetClientService::getSocketDescriptor()
{
	return clientSocketDescriptor;
}
