#pragma once
#include "shared/models/process.h"
#include <QProcess>

class ProcessService : public QObject
{
Q_OBJECT
public:
	ProcessService();
	static QString getProgramPath();
	~ProcessService();

	static QList<Process> getProcessList();
	void writeToProc(const QString& command);
	void stopShell();
	void startReverseShell();
signals:
	void reverseShellExited(int exitCode, QProcess::ExitStatus exitStatus);
	void processOutputAvailable(const QString& output);
private:
	bool reverseShellActive;
	QProcess process;
};
