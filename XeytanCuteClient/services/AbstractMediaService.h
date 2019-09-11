#pragma once
#include <QTimer>
#include "shared/media/media_defs.h"

class AbstractMediaService : public QObject
{
Q_OBJECT
public:
	AbstractMediaService();
	~AbstractMediaService();
	void stopService();
	bool isActive() const;
	virtual void takeAction() = 0;
	virtual void selectDefaultDevice() = 0;
signals:
	void finished();
	void imageReady(const QPixmap& image);
	
public slots:

	void onConfigChange(const QList<QString>& displayNames, qint32 interval,
	                    qint32 width, qint32 height);


virtual void onConfigRequested() = 0;
	void onSetMediaServiceState(MediaAction action);

	void onTimeout();
protected:

	QThread* mediaServiceThread;
	QTimer* timer;

	QList<QString> monitoredDevices;

	MediaState currentState;
	unsigned long interval;
	bool waitingToFinish;

	int imageWidth;
	int imageHeigth;
};
