#pragma once
#include "ClientNetEvent.h"
struct CameraInfo;

class CameraConfigClientNetEvent : public ClientNetEvent
{
public:
	CameraConfigClientNetEvent();
	CameraConfigClientNetEvent(const QSharedPointer<Client>& client, const QList<CameraInfo>& cameras);
	~CameraConfigClientNetEvent();
	QList<CameraInfo> cameras;
};
