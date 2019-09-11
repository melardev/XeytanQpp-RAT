#include "NetService.h"

#include "ProcessService.h"
#include "DesktopMediaService.h"
#include "CameraMediaService.h"


#include "shared/net/packets/PacketPresentation.h"
#include "shared/net/packets/PacketDesktopRequest.h"
#include "shared/net/packets/PacketMediaResponse.h"
#include "shared/net/packets/PacketDesktopConfigResponse.h"
#include "shared/net/packets/PacketCameraConfigResponse.h"
#include "shared/net/packets/PacketProcessResponse.h"
#include "shared/net/packets/PacketConnection.h"
#include "shared/net/packets/PacketMediaRequest.h"
#include "shared/net/packets/PacketCameraRequest.h"
#include "shared/net/packets/PacketCameraError.h"
#include "shared/net/processors/QDataStreamPacketProcessor.h"

#include "shared/net/packets/PacketShell.h"
#include "shared/net/packets/PacketChat.h"

NetService::~NetService()
{
}

NetService::NetService(QObject* parent)
	: QObject(parent),
	  clientSocket(new QTcpSocket(this)),
	  desktopMediaService(Q_NULLPTR),
	  packetProcessor(new QDataStreamPacketProcessor), cameraMediaService(Q_NULLPTR)
{
	// We would need to know this which is the minimum data we can read at the beginning of a readyRead operation
	QByteArray byteArray;
	QDataStream writeStream(&byteArray, QIODevice::WriteOnly);
	writeStream
		<< quint64(0);

	hasReadHeader = false;
	packetHeaderLength = byteArray.size();
}

// SELF UTILS BEGIN
void NetService::startService()
{
	clientSocket->connectToHost(QHostAddress::LocalHost, 3002);
	//	connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	connectedConnection = connect(clientSocket, &QTcpSocket::connected, this, &NetService::onSocketConnected);
	disconnectionConnection = connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	readConnection = connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));

	/* // Not working, error messagign stating onSocketError slot does not exist, what?! below alternative works though
	errorConnection = connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
	                          SLOT(NetService::onSocketError(QAbstractSocket::SocketError)));
	*/

	errorConnection = connect(clientSocket, qOverload<QAbstractSocket::SocketError>(&QAbstractSocket::error), this,
	                          &NetService::onSocketError);
}

void NetService::onSocketError(QAbstractSocket::SocketError error)
{
	onSocketDisconnected();
}

void NetService::onSocketConnected()
{
	PacketPresentation packet;
	packet.setOs(QSysInfo::prettyProductName());
	packet.setPcName(QSysInfo::machineHostName());
	// packet.setPcName(QHostInfo::localHostName());
	packet.setPassword("");

	sendPacket(packet);
}

void NetService::onSocketReadyRead()
{
	auto socket = static_cast<QTcpSocket *>(sender());

	const quint64 available = socket->bytesAvailable();
	if (!hasReadHeader && available < packetHeaderLength)
		return;


	QDataStream readStream(socket);
	readStream.setVersion(QDataStream::Qt_4_0);

	if (!hasReadHeader)
	{
		readStream >> packetSize;
		if (packetSize == 0)
		{
		}

		// If network buffer does not contain all expected bytes server told us then return until all is received
		if (available < packetSize)
			return;
	}


	qint16 typeInt;

	readStream >> typeInt;
	const PacketType type = (PacketType)typeInt;

	if (type == PacketType::Login)
		handleLogin(readStream);
	else if (type == PacketType::DesktopConfig)
	{
		// initDesktopHandler();
		initDesktopService();
		emit requestDesktopConfig();
	}
	else if (type == PacketType::CameraConfig)
	{
		initCameraService();
		emit requestCameraConfig();
	}
	else if (type == PacketType::Desktop)
		handleDesktop(readStream);
	else if (type == PacketType::Presentation)
		handlePresentation(readStream);
	else if (type == PacketType::Shell)
		handleShell(readStream);
	else if (type == PacketType::Camera)
		handleCamera(readStream);
	else if (type == PacketType::ProcessList)
		handleProcessList(readStream);
	else if (type == PacketType::Connection)
		handleConnection(readStream);
	else if (type == PacketType::Chat)
		handleChat(readStream);

	// For our next packet
	hasReadHeader = false;
	packetSize = 0;
}

void NetService::onSocketDisconnected()
{
	disconnect(connectedConnection);
	disconnect(disconnectionConnection);
	disconnect(readConnection);

	clientSocket->abort(); // abort() may also call behind the scenes close() method
	clientSocket->close();

	emit disconnected();
}

void NetService::sendPacket(const Packet& packet)
{
	QByteArray serializedPacket = packetProcessor->serialize(packet);
	qDebug() << serializedPacket.size();

	// For debugging it may be useful to dump it to a file
	/*
	QFile testFile("./packet.txt");
	testFile.open(QIODevice::WriteOnly);
	if (testFile.isOpen())
		testFile.write(serializedPacket);
	testFile.close();
	*/

	clientSocket->write(serializedPacket);
	clientSocket->waitForBytesWritten();
}

void NetService::exitApplication()
{
	if (desktopMediaService != Q_NULLPTR && desktopMediaService->isActive())
		desktopMediaService->stopService();

	if (cameraMediaService != Q_NULLPTR && cameraMediaService->isActive())
		cameraMediaService->stopService();

	packetProcessor.reset();
	cameraMediaService.reset();
	desktopMediaService.reset();
	// qApp->quit();
	QCoreApplication::quit();
}

// SELF UTILS END

void NetService::handlePresentation(QDataStream& data_stream)
{
	PacketPresentation packet;
	data_stream >> packet;
	packet.setOs(QSysInfo::prettyProductName());
	packet.setPcName(QSysInfo::machineHostName());
	if (packet.getDetails() == Extended)
	{
		const QProcessEnvironment sysEnv = QProcessEnvironment::systemEnvironment();
		QStringList keys = sysEnv.keys();
		for (const auto& key : keys)
		{
			packet.addEnv(key, sysEnv.value(key));
		}
	}
	sendPacket(packet);
}

// PROCESS LIST HANDLING BEGIN
void NetService::handleProcessList(const QDataStream& dataStream)
{
	QList<Process> processes = ProcessService::getProcessList();
	PacketProcessResponse packet;
	packet.processes = processes;
	sendPacket(packet);
}

// PROCESS LIST HANDLING END

// CONNECTION HANDLING BEGIN
void NetService::handleConnection(QDataStream& data_stream)
{
	PacketConnection packet;
	data_stream >> packet;

	if (packet.action == ConnectionAction::Close)
	{
		clientSocket->close();
		exitApplication();
	}
	else if (packet.action == ConnectionAction::Uninstall)
	{
		// TODO: Remove persistence(which is not implemented yet)
		clientSocket->close();
		exitApplication();
	}
}

// CONNECTION HANDLING END

// DESKTOP HANDLING BEGIN
void NetService::handleDesktop(QDataStream& readStream)
{
	PacketDesktopRequest packet;
	readStream >> packet;
	Q_ASSERT(packet.action >= MediaAction::MinMediaAction && packet.action <= MediaAction::MaxMediaAction);
	if (packet.action == MediaAction::Start || packet.action == MediaAction::Resume)
	{
		// desktopHandler = initDesktopHandler();
		initDesktopService();
		emit setDesktopConfig(packet.displayNames, packet.interval, packet.width, packet.height);
		emit setDesktopServiceState(packet.action);
	}
	else if (packet.action == MediaAction::Pause)
	{
		// Q_ASSERT(desktopHandler != nullptr);
		emit setDesktopServiceState(MediaAction::Pause);
	}
	else if (packet.action == MediaAction::Stop)
	{
		// Q_ASSERT(desktopHandler != nullptr);
		emit setDesktopServiceState(packet.action);
	}
}

void NetService::onScreenShotAvailable(const QPixmap& pixmap)
{
	PacketMediaResponse packet(PacketType::Desktop, pixmap);
	sendPacket(packet);
}

void NetService::onScreenConfigAvailable(const QList<ScreenInfo>& screens)
{
	const PacketDesktopConfigResponse packet(screens);
	sendPacket(packet);
}

QScopedPointer<DesktopMediaService>& NetService::initDesktopService()
{
	if (desktopMediaService == Q_NULLPTR)
	{
		desktopMediaService.reset(new DesktopMediaService);
		/*
		desktopHandlerThread = new QThread;
		desktopHandler->moveToThread(desktopHandlerThread);
		*/

		// We should be able to change how screenshots are taken
		connect(this, &NetService::setDesktopConfig, desktopMediaService.get(),
		        &DesktopMediaService::onConfigChange);

		// We should be able to start, pause, resume and stop the desktop service
		connect(this, &NetService::setDesktopServiceState, desktopMediaService.get(),
		        &AbstractMediaService::onSetMediaServiceState);

		// we should be able to ask for screen info
		connect(this, &NetService::requestDesktopConfig, desktopMediaService.get(),
		        &AbstractMediaService::onConfigRequested);

		// We should be able to receive images and configuration data
		connect(desktopMediaService.get(), &AbstractMediaService::imageReady, this, &NetService::onScreenShotAvailable);
		connect(desktopMediaService.get(), &DesktopMediaService::screenConfigAvailable, this,
		        &NetService::onScreenConfigAvailable);

		// desktopHandlerThread->start();
	}
	return desktopMediaService;
}

// DESKTOP HANDLING END

// CAMERA HANDLING BEGIN
QScopedPointer<CameraMediaService>& NetService::initCameraService()
{
	if (cameraMediaService == Q_NULLPTR)
	{
		// cameraService = QSharedPointer<CameraService>(new CameraService);
		cameraMediaService.reset(new CameraMediaService);

		// we should be able to ask for screen info
		// Q_ASSERT(connect(this, &NetService::requestCameraConfig, cameraService.get(), &CameraService::onCameraConfigRequested));

		Q_ASSERT(connect(this, &NetService::requestCameraConfig,
			cameraMediaService.get(), &AbstractMediaService::onConfigRequested));
		// We should be able to change how camera images are taken
		Q_ASSERT(connect(this, &NetService::setCameraConfig,
			cameraMediaService.get(), &AbstractMediaService::onConfigChange));

		// We should be able to start, pause, resume and stop the camera service
		Q_ASSERT(connect(this, &NetService::setCameraServiceState,
			cameraMediaService.get(), &AbstractMediaService::onSetMediaServiceState));

		Q_ASSERT(connect(cameraMediaService.get(), &CameraMediaService::cameraError,
			this, &NetService::onCameraError));

		// We should be able to receive images and configuration data
		Q_ASSERT(connect(cameraMediaService.get(), &AbstractMediaService::imageReady,
			this, &NetService::onCameraPictureAvailable));
		Q_ASSERT(connect(cameraMediaService.get(), &CameraMediaService::cameraConfigReady,
			this, &NetService::onCamerasConfigAvailable));
	}
	return cameraMediaService;
}

void NetService::handleCamera(QDataStream& stream)
{
	PacketCameraRequest packet;
	stream >> packet;
	Q_ASSERT(packet.action >= MediaAction::MinMediaAction && packet.action <= MediaAction::MaxMediaAction);
	if (packet.action == MediaAction::Start || packet.action == MediaAction::Resume)
	{
		initCameraService();
		QList<QString> devices;
		devices.append(packet.selectedCameraDeviceName);
		emit setCameraConfig(devices, packet.interval, packet.width, packet.height);
		emit setCameraServiceState(packet.action);
	}
	else if (packet.action == MediaAction::Pause)
	{
		Q_ASSERT(cameraMediaService != nullptr);
		emit setCameraServiceState(MediaAction::Pause);
	}
	else if (packet.action == MediaAction::Stop)
	{
		Q_ASSERT(cameraMediaService != nullptr);
		emit setCameraServiceState(MediaAction::Stop);
	}
}

void NetService::onCamerasConfigAvailable(const QList<CameraInfo>& availableCameras)
{
	PacketCameraConfigResponse packet(availableCameras);
	sendPacket(packet);
}

void NetService::onCameraPictureAvailable(const QPixmap& pixmap)
{
	const PacketMediaResponse packet(PacketType::Camera, pixmap);
	sendPacket(packet);
}

void NetService::onCameraError(const CameraErrorDetails& error)
{
	const PacketCameraError packet(error);
	sendPacket(packet);
}

// CAMERA HANDLING END

// SHELL HANDLING BEGIN


void NetService::handleShell(QDataStream& readStream)
{
	PacketShell packet;
	readStream >> packet;

	emit requestShellReady(packet.shellAction, packet.data);
}

void NetService::sendShellOutput(const QString& data)
{
	PacketShell packetResponse;
	packetResponse.shellAction = ShellAction::MoreData;
	packetResponse.data = data;
	sendPacket(packetResponse);
}

void NetService::sendShellStop()
{
	PacketShell packetResponse;
	packetResponse.shellAction = ShellAction::Stop;
	sendPacket(packetResponse);
}

// SHELL HANDLING END

// CHAT HANDLING BEGIN
void NetService::handleChat(QDataStream& data_stream)
{
	PacketChat packet;
	data_stream >> packet;

	const ChatAction chatAction = packet.action;
	const QString message = packet.message;

	emit chatRequest(chatAction, message);
}

void NetService::sendChatMessage(const QString& message)
{
	PacketChat packet(ChatAction::AddMessage);
	packet.message = message;
	sendPacket(packet);
}

void NetService::chatClosed()
{
	const PacketChat packet(ChatAction::Close);
	sendPacket(packet);
}

// CHAT HANDLING END

void NetService::handleLogin(QDataStream& readStream)
{
	/*
		PacketLoginResponse packet;
		readStream >> packet;

		if (!packet.loginWasSuccess())
		{
			socket->abort();
			return;
		}
		*/
}

/*void NetService::onMessageSent(PacketLoginRequest& packet)
{
	//packet.setSender(identifier.getUsername());
	//write(socket, packet);
}
*/
