#include "FileSystemHandler.h"


FileSystemHandler::FileSystemHandler()
{
}

FileSystemHandler::~FileSystemHandler()
{
}

#include <QDesktopServices>

void roots()
{
	QString writeablePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
	QString downloadPath = QStandardPaths::displayName(QStandardPaths::DownloadLocation);
	//QString a = QDesktopServices::storageLocationImpl(QStandardPaths::AppLocalDataLocation);
}
