#pragma once
#include <QMap>
#include <QObject>


enum class UiWindow;
class NetEvent;
class GuiEvent;
class ClientNetEvent;

class RemoteDesktopDialog;
class RemoteCameraDialog;
class ClientInfoDlg;
class ProcessListDlg;
class ReverseShellDlg;
class MainGui;
class ChatDialog;

class GuiMediator : public QObject
{
Q_OBJECT
public:
	GuiMediator(QObject* parent = Q_NULLPTR);

	virtual ~GuiMediator();
	QWidget* getWindow(UiWindow window);
	QWidget* getClientWindow(qint64 clientId, UiWindow qt_window);

	MainGui* getOrOpenMainUi();
	ClientInfoDlg* getOrOpenClientInfoDlg(qint64 get_id);
	ReverseShellDlg* getOrOpenReverseShellDlg(qint64 clientId);
	RemoteDesktopDialog* getOrOpenDesktopDialog(const qint64 clientId);
	ProcessListDlg* getOrOpenProcessListDialog(const qint64 clientId);
	ChatDialog* getOrOpenChatDialog(qint64 clientId);
	RemoteCameraDialog* getOrOpenCameraDlg(const qint64 clientId);
signals:
	void queueEvent(const GuiEvent& evt);
	void sendToApp(const GuiEvent& evt);
	void sendPingToApp();
	void sendToAppPtr(const QSharedPointer<GuiEvent>& event);

public slots:
	void onPingTimer();

	void receiveEventFromApp(const GuiEvent& event);
	void closeAllWindowsFromClient(qint64 clientId);


	void receiveEventFromUi(const GuiEvent& event);


	void receiveEventFromUiPtr(const QSharedPointer<GuiEvent>& event);

	void recvClientEventFromApp(const ClientNetEvent& event);
	void recvClientEventFromAppPtr(const QSharedPointer<ClientNetEvent>& event);

	void onWindowClose(int result);;
	void recvPingFromApp();

protected:
	QMap<int, QMap<UiWindow, QWidget*>> openedWindows;
	QMap<UiWindow, QWidget*>::mapped_type tempWindow;
};
