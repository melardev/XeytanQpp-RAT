#include "Logger.h"



/*--------------------------------------------------------------------------+
| Constructor                                                               |
+--------------------------------------------------------------------------*/
Log::Log(QString message, LogLevel level) :
	message(message),
	level(level)
{
	timeStamp = QDateTime::currentDateTime();
}

/*--------------------------------------------------------------------------+
| Copy Constructor                                                          |
+--------------------------------------------------------------------------*/
Log::Log(const Log& copy)
{
	message = copy.message;
	level = copy.level;
	timeStamp = copy.timeStamp;
}

Logger::Logger()
{
}


Logger::~Logger()
{
}

void Logger::logOsMessage(uint errorId, QString message, LogLevel level)
{
    /*
	if (errorId == 0)
		return;

	QString osMessage;
	WCHAR* charData = new WCHAR[1024];
	DWORD lenght = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorId, NULL,
	                             charData, 1024, NULL);
	if (lenght)
	{
		//in QHelpers
		message = wCharToString(charData);
	}
	else
	{
		message = "No such error id " + QString::number(errorId) + ")";
	}
	QString fullStop = (message.endsWith('.') ? "" : ".");
	log(message + fullStop + "\nOS Error: " + osMessage, level);
	delete charData;
    */
}

QString Log::levelName()
{
	switch (level)
	{
	case Error: return "Error";
	case Warn: return "Warning";
	case Info: return "Info";
	case Debug: return "Debug";
	default: return "";
	}
}

/*--------------------------------------------------------------------------+
| Writes this log message and level to the given stream.                    |
| Newlines in the message will be stripped and replaced with space.         |
+--------------------------------------------------------------------------*/
void Log::writeTo(QFile* file)
{
	QTextStream stream(file);
	stream << timeStamp.toString(Qt::SystemLocaleShortDate) << " - "
		<< levelName() << ":\t"
		<< message.simplified() << "\n";
}

/*--------------------------------------------------------------------------+
| Return the singleton instance, instantiating on first use.                |
+--------------------------------------------------------------------------*/
Logger& Logger::current() {
	static Logger* instance = new Logger();
	return *instance;
}



/*--------------------------------------------------------------------------+
| Constructor                                                               |
+--------------------------------------------------------------------------*/
/*Logger::Logger() :
	logs(),
	maxLogs(1000),
	file(NULL),
	listener(NULL) {
	if (Settings::enableLogging)
		startLoggingToFile();
}
*/
