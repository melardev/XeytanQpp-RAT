#include "GuiEvent.h"


GuiEvent::GuiEvent(UiWindow source, UiEventIntent intent, UiEventAction action, qint64 clientId)
	: source(source), intent(intent), action(action), clientId(clientId)
{
}

GuiEvent::GuiEvent(UiWindow event): source(event)
{
}


GuiEvent::GuiEvent()
{
}

GuiEvent::GuiEvent(UiWindow uiWindow, UiEventAction action, qint64 clientId)
	: source(uiWindow), action(action),
	  clientId(clientId)
{
}


GuiEvent::~GuiEvent()
{
}

const UiEventIntent GuiEvent::getIntent() const
{
	return intent;
}

const qint64 GuiEvent::getClientId() const
{
	return clientId;
}

void GuiEvent::setSource(UiWindow source)
{
	this->source = source;
}
