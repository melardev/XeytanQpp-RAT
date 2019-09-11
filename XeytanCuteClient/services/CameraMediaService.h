#pragma once
#include "AbstractMediaService.h"
#include <QCameraInfo>
#include <QCameraImageCapture>

class CameraMediaService : public AbstractMediaService
{
Q_OBJECT
public:
	CameraMediaService();
	~CameraMediaService();
	void selectCamera(const QCameraInfo& cameraInfo);


	bool init();
	virtual void takeAction() override;
	virtual void selectDefaultDevice() override;
	void refreshAvailableCameras();

	void disconnectAllQtConnections();


signals:
	void cameraError(const CameraErrorDetails& error);
	void cameraConfigReady(const QList<CameraInfo>& availableCameras);

public slots:
	virtual void onConfigRequested() override;
	void onStateChanged(QCamera::State state);

	void onCameraError(QCamera::Error error);
	void displayCaptureError(int id, QCameraImageCapture::Error error, const QString& errorString);
	void updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason reason);
	void readyForCapture(bool ready);
	void processCapturedImage(int requestId, const QImage& img);
	void imageSaved(int id, const QString& fileName);
private:
	QList<QCameraInfo> availableCameras;

	QScopedPointer<QCamera> camera;
	QScopedPointer<QCameraImageCapture> imageCapture;
	QCamera::State expectedState;

	bool hasInit;
	QMetaObject::Connection cameraStateChangedConnection;
	QMetaObject::Connection cameraErrorChangedConnection;
	QMetaObject::Connection cameralockStatusChangedConnection;
	QMetaObject::Connection readyForCameraStatusChangedConnection;
	QMetaObject::Connection imageCapturedConnection;
	QMetaObject::Connection imageSavedConnection;
	QMetaObject::Connection displayCaptureErrorConnection;
};
