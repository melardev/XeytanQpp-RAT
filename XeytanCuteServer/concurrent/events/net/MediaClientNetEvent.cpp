#include "MediaClientNetEvent.h"


MediaClientNetEvent::MediaClientNetEvent(const NetEventType& eventType) : ClientNetEvent(eventType)
{
	Q_ASSERT(eventType == NetEventType::CameraData || eventType == NetEventType::DesktopData);
}


MediaClientNetEvent::MediaClientNetEvent(const NetEventType& eventType, const QSharedPointer<Client>& client,
                                         const QPixmap& image)
	: ClientNetEvent(eventType, client), image(image)
{
	Q_ASSERT(eventType == NetEventType::CameraData || eventType == NetEventType::DesktopData);
}


MediaClientNetEvent::~MediaClientNetEvent()
{
}
