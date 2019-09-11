#pragma once

#include <QDataStream>
#include <QIcon>

class Process
{
public:
	Process() = default;
	~Process() = default;

	QString path;
	qint64 pid;
	bool hasUi;
	QString title;
	QPixmap icon;

	friend QDataStream& operator<<(QDataStream& stream, const Process& process);
	friend QDataStream& operator>>(QDataStream& stream, Process& process);
};


inline QDataStream& operator<<(QDataStream& stream, const Process& process)
{
	stream << process.pid << process.path << process.icon << process.hasUi;

	if (process.hasUi)
		stream << process.title;
	return stream;
}

inline QDataStream& operator>>(QDataStream& stream, Process& process)
{
	stream >> process.pid >> process.path >> process.icon >> process.hasUi;

	if (process.hasUi)
		stream >> process.title;


	return stream;
}

class ProcessResult
{
public:
	ProcessResult() = default;
	~ProcessResult() = default;
};
