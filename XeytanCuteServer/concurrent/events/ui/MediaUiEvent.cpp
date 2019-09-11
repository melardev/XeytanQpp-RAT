#include "MediaUiEvent.h"


MediaUiEvent::MediaUiEvent()
{
}

MediaUiEvent::MediaUiEvent(const MediaType& mediaType, const UiWindow& window, UiEventAction action, qint64 clientId,
                           const QString& selectedDisplay)
	: GuiEvent(window, action, clientId), mediaType(mediaType),
	  selectedDevice(selectedDisplay)
{
}

MediaUiEvent::MediaUiEvent(UiEventAction action, const UiWindow& window, qint64 clientId)
	: GuiEvent(window, action, clientId)
{
}

MediaUiEvent::MediaUiEvent(MediaType media, UiWindow window, UiEventAction action, qint64 clientId)
	: GuiEvent(window, action, clientId), mediaType(media)
{
}


MediaUiEvent::~MediaUiEvent()
{
}
