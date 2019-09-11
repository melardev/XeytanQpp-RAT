#pragma once

#include <QString>
#include "globals.h"
// #include <winbase.h>
#include <QDateTime>
#include <qtextstream.h>
#include <qfile.h>
#include <qfile.h>

class QString;

enum LogLevel
{
	Trace = 0,
	Debug = 1,
	Info = 2,
	Warn = 3,
	Error = 4
};

class Log
{
private:
	LogLevel level;
	QString message;
	QDateTime timeStamp;

public:
	Log(QString message, LogLevel level);
	Log(const Log& copy);

	QString levelName();
	LogLevel getLevel() { return level; }
	QString getMessage() { return message; }
	QDateTime getTimeStamp() { return timeStamp; }
	QDate getDate() { return timeStamp.date(); }
	QTime getTime() { return timeStamp.time(); }
	void writeTo(QFile* file);
};

class Logger
{
public:
	Logger(QString message, LogLevel level);
	Logger(const Logger& copy);
	Logger();
	static void error(QString msg);
	static void warning(QString message);
	static void info(QString message);
	static void debug(QString message);
	static void logOsMessage(uint errorId, QString message, LogLevel level);
	QString levelName();
	void writeTo(QFile* file);
	~Logger();
	QString message;
	LogLevel level;
    QDateTime timeStamp;
    Logger &current();
};
