#pragma once
#include "concurrent/events/ui/GuiEvent.h"
#include "shared/media/media_defs.h"

class MediaUiEvent : public GuiEvent
{
public:
	MediaUiEvent();
	MediaUiEvent(const MediaType& mediaType, const UiWindow& window, UiEventAction action, qint64 clientId, const QString& selectedDisplay);
	MediaUiEvent(UiEventAction action, const UiWindow& window, qint64 clientId);
	MediaUiEvent(MediaType media, UiWindow window, UiEventAction pause_action, qint64 client_id);
	~MediaUiEvent();
	
	MediaType mediaType;
	QString selectedDevice;
};
