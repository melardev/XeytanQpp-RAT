#pragma once
#include <shared/media/media_defs.h>
#include <QTcpSocket>
#include <QUuid>
#include "shared/net/processors/QDataStreamPacketProcessor.h"
#include "shared/net/packets/PacketCameraError.h"


class PacketChat;
class PacketShell;
class PacketDesktopResponse;
class PacketProcessResponse;
class NetServerService;
class PacketPresentation;
class PacketDesktopConfigResponse;
class PacketMediaResponse;
class PacketCameraConfigResponse;

class NetClientService : public QObject, public QEnableSharedFromThis<NetClientService>
{
Q_OBJECT
public:
	NetClientService();
	NetClientService(const QSharedPointer<NetServerService>& server, QTcpSocket* clientTcpSocket);
	~NetClientService();

	void interact();
	void handleLogin();

	QTcpSocket* getSocketObject() const;
	void set_socket_obj(QTcpSocket* socket);


	void handlePresentation(quint64 payloadLen);

	void sendPacket(const Packet& packet);
	qint64 getSocketDescriptor();
	void handleRemoteDesktop(quint64 payloadLen);

	void handleRemoteCameraConfig(quint64 packetLength);

	void handleRemoteCamera(quint64 payloadLen);

	void handleCameraError(quint64 payloadLen);

	void handleProcessList(quint64 packetLength);
	void handleShell(quint64 packetLength);
	void handleRemoteDesktopConfig(quint64 packetLength);
	void handleChat(quint64 packetLength);

	void closeConnection(bool sendCloseConnectionPacket);

signals:
	void packetReceived(const Packet& packet);
	void packetReceivedPtr(const QSharedPointer<Packet>& packet);

	void presentPacketReceived(PacketPresentation& packet);

	void desktopConfigPacketReceived(const PacketDesktopConfigResponse& packet);
	void desktopPacketReceived(PacketDesktopResponse& packet);

	void cameraConfigPacketReceived(const PacketCameraConfigResponse& packet);
	void cameraPacketReceived(const PacketMediaResponse& packet);
	void cameraErrorPacketReceived(const PacketCameraError& packet);

	void processPacketReceived(const PacketProcessResponse& packet);
	void shellPacketReceived(const PacketShell& packet);
	void chatPacketReceived(const PacketChat& packet);
	
public slots:
	void onSocketReadyRead();
	void onSocketError(QAbstractSocket::SocketError);
	void onDisconnected();

private:
	QTcpSocket* clientSocket;
	qintptr clientSocketDescriptor;
	QSharedPointer<NetServerService> netServerService;
	QSharedPointer<QDataStreamPacketProcessor> packetProcessor;
	quint64 packetSize = 0;
	unsigned int packetHeaderLength;
	bool hasReadHeader;
	qint64 packetTypeLength;
	qint64 packetSizeLength;
};
