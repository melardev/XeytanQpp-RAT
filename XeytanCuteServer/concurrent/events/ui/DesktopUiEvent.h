#pragma once
#include "concurrent/events/ui/GuiEvent.h"

class MediaUiEvent : public GuiEvent
{
public:
	MediaUiEvent();
	MediaUiEvent(UiEventAction action, qint64 clientId, const QString& selectedDisplay);
	MediaUiEvent(UiEventAction stop_action, qint64 clientId);
	~MediaUiEvent();
	QString display;
};
