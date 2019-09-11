#include "MediaUiEvent.h"


MediaUiEvent::MediaUiEvent()
{
}

MediaUiEvent::MediaUiEvent(UiEventAction action, qint64 clientId, const QString& selectedDisplay)
	: GuiEvent(DesktopWindow, action, clientId),
	  display(selectedDisplay)
{
}

MediaUiEvent::MediaUiEvent(UiEventAction action, qint64 clientId) : GuiEvent(DesktopWindow, action, clientId)
{
}


MediaUiEvent::~MediaUiEvent()
{
}
