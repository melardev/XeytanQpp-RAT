#pragma once
#include "ClientNetEvent.h"
#include <QPixmap>

class MediaClientNetEvent : public ClientNetEvent
{
public:
	MediaClientNetEvent(const NetEventType& eventType);
	MediaClientNetEvent(const NetEventType& eventType, const QSharedPointer<Client>& client, const QPixmap& image);
	~MediaClientNetEvent();
	QPixmap image;
};
