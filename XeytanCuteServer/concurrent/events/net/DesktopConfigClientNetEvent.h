#pragma once
#include "concurrent/events/net/ClientNetEvent.h"

struct ScreenInfo;

class DesktopConfigClientNetEvent : public ClientNetEvent
{
public:

	DesktopConfigClientNetEvent(const QSharedPointer<Client>& client, const QList<ScreenInfo>& screens);
	~DesktopConfigClientNetEvent();
	QList<ScreenInfo> screens;
};
