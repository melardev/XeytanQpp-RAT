#pragma once
#ifndef MEDIA_DEF_H
#define MEDIA_DEF_H

#include <QDataStream>
#include <QMap>


struct MediaConfig
{
	qint32 interval;
	qint32 width;
	qint32 height;
};

struct CameraMediaConfig : MediaConfig
{
	QList<QString> deviceNames;
};

struct DesktopMediaConfig : MediaConfig
{
	QList<qint32> windowIds;
};

struct ScreenInfo
{
	QString name;
	qint32 x;
	qint32 y;
	qint32 width;
	qint32 height;
	QString serialNumber;
	QString manufacturer;
	qreal refreshRate;
};

struct CameraInfo
{
	QString deviceName;
	QString description;
};

struct CameraErrorDetails
{
	int errorCode;
	QString errorMessage;
};

enum class MediaAction : qint8
{
	Invalid,
	Start,
	Pause,
	Resume,
	Stop,
	MinMediaAction = Start,
	MaxMediaAction = Stop,
};

// Overload operators to enable serialization for ScreenInfo and CameraInfo
inline QDataStream& operator<<(QDataStream& stream, const ScreenInfo& screenInfo)
{
	stream
		<< screenInfo.name << screenInfo.manufacturer << screenInfo.serialNumber
		<< screenInfo.x << screenInfo.y
		<< screenInfo.width << screenInfo.height;
	return stream;
}

inline QDataStream& operator>>(QDataStream& stream, ScreenInfo& screenInfo)
{
	stream
		>> screenInfo.name >> screenInfo.manufacturer >> screenInfo.serialNumber
		>> screenInfo.x >> screenInfo.y
		>> screenInfo.width >> screenInfo.height;
	return stream;
}

inline QDataStream& operator>>(QDataStream& stream, CameraInfo& screenInfo)
{
	stream >> screenInfo.deviceName >> screenInfo.description;
	return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const CameraInfo& cameraInfo)
{
	stream << cameraInfo.deviceName << cameraInfo.description;
	return stream;
}


inline QDataStream& operator>>(QDataStream& stream, CameraErrorDetails& cameraErrorDetails)
{
	stream >> cameraErrorDetails.errorCode >> cameraErrorDetails.errorMessage;
	return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const CameraErrorDetails& cameraErrorDetails)
{
	stream << cameraErrorDetails.errorCode << cameraErrorDetails.errorMessage;
	return stream;
}

enum MediaState
{
	Started,
	Paused,
	Stopped,
};

enum class MediaType
{
	Camera, Desktop
};

static QMap<MediaAction, MediaState> mediaActionStateMap = {
	{MediaAction::Start, Started},
	{MediaAction::Resume, Started},
	{MediaAction::Pause, Paused},
	{MediaAction::Stop, Stopped}
};

#endif
