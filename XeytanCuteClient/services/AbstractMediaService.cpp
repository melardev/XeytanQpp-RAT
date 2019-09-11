#include "AbstractMediaService.h"
#include <QThread>

AbstractMediaService::AbstractMediaService()
{
	this->interval = 2000;
	mediaServiceThread = Q_NULLPTR;
	currentState = Stopped;
}


AbstractMediaService::~AbstractMediaService()
{
}


bool AbstractMediaService::isActive() const
{
	return currentState == Started || currentState == Paused;
}

void AbstractMediaService::onConfigChange(const QList<QString>& displayNames, qint32 interval, qint32 width,
                                          qint32 height)
{
	// If cameraName is set then use it, otherwise check if we are already monitoring the default camera
	// if we we are then that's it, but if we are not then add the default
	if (!displayNames.isEmpty())
		monitoredDevices = displayNames;
	else if (monitoredDevices.isEmpty())
	{
		// If the user did not supply a valid new camera name and we were mot monitoring one already
		// then select default
		selectDefaultDevice();
	} // else just go ahead, cameraName input was empty, but we were already monitoring a camera, keep using it


	// if invalid value preserve the previous value if any, otherwise set default
	if (interval <= 0)
	{
		if (this->interval <= 0)
			this->interval = 500;
	}
	else
	{
		// Supplied input is valid
		this->interval = interval;
	}

	// if invalid value preserve the previous value if any, otherwise set default
	if (width <= 0 || width > 1290)
	{
		if (this->imageWidth <= 0)
			this->imageWidth = 500;
	}
	else
	{
		this->imageWidth = width;
	}

	// if invalid value preserve the previous value if any, otherwise set default
	if (height <= 0)
	{
		if (this->imageHeigth <= 0)
			this->imageHeigth = 256;
	}
	else
	{
		this->imageHeigth = height;
	}
}

void AbstractMediaService::onSetMediaServiceState(MediaAction action)
{
	Q_ASSERT(action == MediaAction::Start || action == MediaAction::Pause || action == MediaAction::Stop);

	this->currentState = mediaActionStateMap[action];

	if (currentState == Started)
	{
		// We should have been configured the camera
		if (monitoredDevices.isEmpty())
		{
			// It may happen if the user selected an invalid camera
			// Or camera failed to start
			return;
		}

		if (mediaServiceThread == Q_NULLPTR)
		{
			mediaServiceThread = new QThread;
			connect(mediaServiceThread, &QThread::started, this, [this]()
			{
				// Create the timer in the background thread, because we can not create it
				// in one thread and start it in another thread
				timer = new QTimer;
				timer->setInterval(interval);
				connect(timer, &QTimer::timeout, this, &AbstractMediaService::onTimeout);
				timer->start();
			});
			// move this CameraService object to a background thread
			this->moveToThread(mediaServiceThread);
			mediaServiceThread->start();
		}
		else
		{
			if (Q_LIKELY(!timer->isActive()))
				timer->start();
		}
	}
	else if (currentState == Paused)
	{
		timer->stop();
	}
	else if (currentState == Stopped)
	{
		this->stopService();
	}
}

void AbstractMediaService::onTimeout()
{
	// Really bad design here, I should refactor this, and indeed if I am not wrong
	// this may have race conditions
	if (currentState != Started)
	{
		if (waitingToFinish)
		{
			emit finished();
		}

		return;
	}

	takeAction();
}

void AbstractMediaService::stopService()
{
	timer->stop();
	currentState = Stopped;
	monitoredDevices.clear();
	emit finished();

	/* It has a race condition issue, if the finished signal is emitted first(from onTimeout) then queue keeps looping
	 * to avoid the mutex overhead just call timer stop and we achieve the same functionality without mutexes
	if (timer->isActive())
	{
		// Do not enter an infinite loop when the timer is not active
		// i.e when the server has requested avaialble cameras, but has not selected any yet
		// and requests a stop the timer is not running, executing a QEventLoop will stuck here
		QEventLoop eventLoop;
		connect(this, &AbstractMediaService::finished, &eventLoop, &QEventLoop::quit);
		waitingToFinish = true;
		eventLoop.exec();
		timer->stop();
	}
	*/
}
