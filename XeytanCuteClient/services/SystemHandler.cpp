#include "SystemHandler.h"
#include <qglobal.h>


SystemHandler::SystemHandler()
{
}


SystemHandler::~SystemHandler()
{
}

#include <QProcessEnvironment>
void getEnv()
{
	qgetenv("USERPROFILE");
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	QStringList envKeys = env.keys();
	for(const QString key: envKeys)
	{
		QString value = env.value(key);
	}
}
