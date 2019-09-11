#include "Reporter.h"



Reporter::Reporter()
{
}


Reporter::~Reporter()
{
}

void Reporter::Error(const QString& message)
{
	qDebug() << message;
}

void Reporter::Debug(const QString& message)
{
	qDebug() << message;
}

void Reporter::Trace(const QString& message)
{
	qDebug() << message;
}

void Reporter::Info(const QString& message)
{
	qDebug() << message;
}

void Reporter::Info(char* str)
{
	qDebug() << QString(str);
}
