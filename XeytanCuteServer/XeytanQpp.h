#pragma once
#include "../ui/qt/GuiMediator.h"
#include "../services/NetServerService.h"


class GuiEvent;
class XeytanQpp;
extern QSharedPointer<XeytanQpp> appInstance;

class XeytanQpp : public QObject, public QEnableSharedFromThis<XeytanQpp>
{
Q_OBJECT

public:
	XeytanQpp(QObject* parent = Q_NULLPTR);
	~XeytanQpp();
	static QSharedPointer<XeytanQpp> launch();

	static QSharedPointer<XeytanQpp> getInstance();

	void bootstrapUiSubsystem();
	void sendTestNetEvent();
	void bootstrapNetSubsystem();
	void run();
signals: // this runs in the Main Thread
	void eventQueued(GuiEvent* evt);
	void appStarted();
	void sendEventToUiMediator(const GuiEvent& event);
	void sendClientEventToUiMediator(const ClientNetEvent& event);

	void requestCloseClientConnection(qint64 clientId, bool sendClosePacket = true);
	void sendPingToUi();
	void sendPingToNet();

public slots: // Called in the Background Thread
	void receiveFromGui(const GuiEvent& event);
	void receiveEventFromUiPtr(const QSharedPointer<GuiEvent>& event);

	void onThreadFinished();
	void receiveNetEvent(const NetEvent& ev);

	void recvNetClientEvent(const ClientNetEvent& event);
	void onClientDataReceivedPtr(QSharedPointer<ClientNetEvent> event);

	void onPingUiTimer();
	void onPingNetTimer();

	void recvPingFromNet();
	void recvPingFromUi() const;
	// void onClientDataReceived(const ClientNetEvent& event);

private:
	GuiMediator* mediator;
	QSharedPointer<NetServerService> serverService;
	Reporter* reporter;
	QThread* netThread;
};
