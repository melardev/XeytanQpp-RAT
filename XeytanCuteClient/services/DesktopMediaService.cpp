#include "DesktopMediaService.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>


DesktopMediaService::DesktopMediaService()
{
	imageWidth = 256;
	imageHeigth = 256;
	interval = 3 * 1000;
	monitoredDevices.clear();
	monitoredDevices.append(QGuiApplication::primaryScreen()->name());
}

DesktopMediaService::~DesktopMediaService()
{
}


void DesktopMediaService::getScreenShot(QPixmap& pixmap) const
{
	Q_ASSERT(monitoredDevices.size() > 0);
	if (monitoredDevices.contains("All") || monitoredDevices.contains("all"))
	{
	}
	else
	{
	}
	QScreen* screen = QGuiApplication::primaryScreen();
	/*
	QMainWindow w;
	w.setVisible(false);
	if (const QWindow* window = w.windowHandle())
		screen = window->screen();
	*/
	pixmap = screen->grabWindow(0);
}

void DesktopMediaService::takeAction()
{
	QPixmap screenShot;
	getScreenShot(screenShot);
	emit imageReady(screenShot);

	if (waitingToFinish)
	{
		emit finished();
	}
}

void DesktopMediaService::selectDefaultDevice()
{
	monitoredDevices.clear();
	monitoredDevices.append(QGuiApplication::primaryScreen()->name());
}

void DesktopMediaService::onConfigRequested()
{
	QList<ScreenInfo> screenInfoList;
	QList<QScreen*> screens = QGuiApplication::screens();
	foreach(const QScreen* screen, screens)
	{
		ScreenInfo screenInfo;

		screenInfo.width = screen->geometry().width();
		screenInfo.width = screen->geometry().height();
		screenInfo.x = screen->geometry().x();
		screenInfo.y = screen->geometry().y();
		screenInfo.name = screen->name();
		screenInfo.serialNumber = screen->serialNumber();
		screenInfo.manufacturer = screen->manufacturer();
		screenInfo.refreshRate = screen->refreshRate();

		screenInfoList.append(screenInfo);
	}

	emit screenConfigAvailable(screenInfoList);
}
