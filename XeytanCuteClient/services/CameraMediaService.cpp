#include "CameraMediaService.h"
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QPixmap>


CameraMediaService::CameraMediaService()
{
	availableCameras = QCameraInfo::availableCameras();

	timer = new QTimer;
	this->interval = 2000;
	imageWidth = 256;
	imageHeigth = 256;
	hasInit = false;
}


CameraMediaService::~CameraMediaService()
{
}

void CameraMediaService::selectCamera(const QCameraInfo& cameraInfo)
{
	if (hasInit) // reset everything if already init
		disconnectAllQtConnections();

	monitoredDevices.clear();
	monitoredDevices.append(cameraInfo.deviceName());

	camera.reset(new QCamera(cameraInfo));

	cameraStateChangedConnection =
		connect(camera.data(), &QCamera::stateChanged, this, &CameraMediaService::onStateChanged);
	cameraErrorChangedConnection =
		connect(camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this,
		        &CameraMediaService::onCameraError);
	cameralockStatusChangedConnection =
		connect(camera.data(),
		        QOverload<QCamera::LockStatus, QCamera::LockChangeReason>::of(
			        &QCamera::lockStatusChanged),
		        this, &CameraMediaService::updateLockStatus);

	imageCapture.reset(new QCameraImageCapture(camera.data()));
	readyForCameraStatusChangedConnection =
		connect(imageCapture.data(), &QCameraImageCapture::readyForCaptureChanged,
		        this, &CameraMediaService::readyForCapture);
	imageCapturedConnection =
		connect(imageCapture.data(), &QCameraImageCapture::imageCaptured, this,
		        &CameraMediaService::processCapturedImage);
	imageSavedConnection =
		connect(imageCapture.data(), &QCameraImageCapture::imageSaved, this,
		        &CameraMediaService::imageSaved);
	displayCaptureErrorConnection =
		connect(imageCapture.data(),
		        QOverload<int, QCameraImageCapture::Error, const QString &>::of(
			        &QCameraImageCapture::error),
		        this, &CameraMediaService::displayCaptureError);


	if (camera->isCaptureModeSupported(QCamera::CaptureStillImage))
		camera->setCaptureMode(QCamera::CaptureStillImage);
	else if (camera->isCaptureModeSupported(QCamera::CaptureVideo))
		camera->setCaptureMode(QCamera::CaptureVideo);
	else
	{
		const CameraErrorDetails error{-1, "Any of the searched Camera capture modes have not been found"};
		emit cameraError(error);
		return;
	}

	camera->start(); // after this call the camera begins taking pictures
}

void CameraMediaService::onCameraError(QCamera::Error error)
{
	QString errorMessage = camera->errorString();

	const CameraErrorDetails cameraErrorStruct{error, QString("onCameraError, error message: %1").arg(errorMessage)};
	emit cameraError(cameraErrorStruct);
}

void CameraMediaService::displayCaptureError(int id, const QCameraImageCapture::Error error, const QString& errorString)
{
	qDebug() << QString("Error %1, QCameraImageCapture::Error %2, Error Message %3")
	            .arg(id).arg(error).arg(errorString);
}


void CameraMediaService::updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason reason)
{
	switch (status)
	{
	case QCamera::Searching:
		qDebug() << "CameraService::updateLockStatus(Searching, " + QString::number(reason) + ")";
		break;
	case QCamera::Locked:
		qDebug() << "CameraService::updateLockStatus(Locked, " + QString::number(reason) + ")";
		break;
	case QCamera::Unlocked:
		qDebug() << "CameraService::updateLockStatus(Unlocked, " + QString::number(reason) + ")";
		break;
	}
}

void CameraMediaService::readyForCapture(bool ready)
{
	qDebug() << QString("CameraService::readyForCapture(%1)").arg(ready);
}

void CameraMediaService::processCapturedImage(int requestId, const QImage& img)
{
	qDebug() << QString("CameraService::processCapturedImage(%1, image.sizeInBytes() = %2)")
	            .arg(requestId)
	            .arg(img.sizeInBytes());
	// QImage scaledImage = img.scaled(QSize(256, 256), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPixmap image = QPixmap::fromImage(img);

	emit imageReady(image);
}

void CameraMediaService::imageSaved(int id, const QString& fileName)
{
	qDebug() << QString("CameraService::imageSaved(%1, image.sizeInBytes() = %2)")
	            .arg(id)
	            .arg(fileName);
}


void CameraMediaService::onStateChanged(QCamera::State state)
{
	// if we programmatically changed the state then if this callback is
	// called just skip it
	if (expectedState == state)
		return;

	switch (state)
	{
	case QCamera::ActiveState:
		qDebug() << "CameraService::onStateChanged(ActiveState)";
		break;
	case QCamera::UnloadedState:
		qDebug() << "CameraService::onStateChanged(UnloadedState)";
		break;
	case QCamera::LoadedState:
		qDebug() << "CameraService::onStateChanged(LoadedState)";
		break;
	}
}

bool CameraMediaService::init()
{
	if (availableCameras.empty())
	{
		refreshAvailableCameras();
		if (availableCameras.empty())
		{
			// const CameraError error{2, "No Available cameras"};
			// emit cameraError(error);
			emit cameraConfigReady(QList<CameraInfo>());
			return false;
		}
	}

	// We have cameras available
	if (monitoredDevices.empty())
		selectDefaultDevice();
	else
	{
		bool found = false;
		for (auto&& cameraInfo : availableCameras)
		{
			if (cameraInfo.deviceName() == monitoredDevices.first())
			{
				found = true;
				selectCamera(cameraInfo);
				break;
			}
		}
		// Select default, for some reason even providing deviceName it nevers find
		// it so this is always executed
		if (!found)
			selectDefaultDevice();
	}
	// we have found at least one camera, proceed
	return true;
}

void CameraMediaService::takeAction()
{
	if (!hasInit)
	{
		if (!init())
			return;
	}

	imageCapture->capture();
}

void CameraMediaService::selectDefaultDevice()
{
	monitoredDevices.clear();
	QCameraInfo defaultCamera = QCameraInfo::defaultCamera();
	monitoredDevices.append(defaultCamera.deviceName());
	selectCamera(defaultCamera);
}

void CameraMediaService::refreshAvailableCameras()
{
	for (const QCameraInfo& cameraInfo : availableCameras)
	{
		if (cameraInfo == QCameraInfo::defaultCamera())
		{
			availableCameras.insert(0, cameraInfo);
		}
		else
		{
			availableCameras.append(cameraInfo);
		}
	}
}

void CameraMediaService::onConfigRequested()
{
	refreshAvailableCameras();
	if (availableCameras.empty())
	{
		emit cameraConfigReady(QList<CameraInfo>());
		return;
	}

	QList<CameraInfo> cameras;
	QDataStream ds;

	Q_FOREACH(auto camera, availableCameras)
	{
		if (camera.isNull()) // would it be possible?
			continue;
		CameraInfo ci{camera.deviceName(), camera.description()};
		cameras.append(ci);
	}

	emit cameraConfigReady(cameras);
}


void CameraMediaService::disconnectAllQtConnections()
{
	hasInit = false;
	disconnect(cameraStateChangedConnection);
	disconnect(cameraErrorChangedConnection);
	disconnect(cameralockStatusChangedConnection);

	disconnect(imageCapturedConnection);
	disconnect(imageSavedConnection);

	disconnect(readyForCameraStatusChangedConnection);

	disconnect(displayCaptureErrorConnection);
}
