#include "DesktopConfigClientNetEvent.h"
#include "shared/media/media_defs.h"

DesktopConfigClientNetEvent::DesktopConfigClientNetEvent(const QSharedPointer<Client>& client,
                                                         const QList<ScreenInfo>& screens)
	: ClientNetEvent(NetEventType::DesktopConfigData, client),
	  screens(screens)
{
}


DesktopConfigClientNetEvent::~DesktopConfigClientNetEvent()
{
}
