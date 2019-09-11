#pragma once
#include <QtGlobal>
#include "concurrent/events/event_defs.h"
#include "ui/ui_defs.h"

// enum class UiEventIntent;
// enum class UiEventAction;

class GuiEvent
{
public:

	GuiEvent(UiWindow source, UiEventIntent intent, UiEventAction action, qint64 clientId);
	GuiEvent(UiWindow event);

	GuiEvent();
	GuiEvent(UiWindow window, UiEventAction action, qint64 clientId);
	virtual ~GuiEvent();

	UiWindow getSource() const
	{
		return source;
	}

	void setData(void* data)
	{
		this->data = data;
	}

	void setAction(UiEventAction action)
	{
		this->action = action;
	}

	UiEventAction getAction() const
	{
		return action;
	}

	void* getData() const
	{
		return data;
	}

	const UiEventIntent getIntent() const;
	const qint64 getClientId() const;
	void setSource(UiWindow source);

private:
	UiWindow source;
	UiEventAction action;
	void* data;

	qint64 clientId;
	UiEventIntent intent;
};
