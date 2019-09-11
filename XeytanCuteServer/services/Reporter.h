#pragma once
#include <QDebug>
class QString;

class Reporter
{
public:
	Reporter();
	~Reporter();

	void Error(const QString&);
	void Debug(const QString&);
	void Trace(const QString&);
	void Info(const QString&);
	void Info(char* str);
};
