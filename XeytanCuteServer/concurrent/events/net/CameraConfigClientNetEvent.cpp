#include "CameraConfigClientNetEvent.h"
#include <shared/media/media_defs.h>

CameraConfigClientNetEvent::CameraConfigClientNetEvent()
{
}


CameraConfigClientNetEvent::~CameraConfigClientNetEvent()
{
}

CameraConfigClientNetEvent::CameraConfigClientNetEvent(const QSharedPointer<Client>& client,
                                                       const QList<CameraInfo>& cameras)
	: ClientNetEvent(NetEventType::CameraConfigData, client), cameras(cameras)
{
}
