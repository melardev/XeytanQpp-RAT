#pragma once
#include <QMetaType>
#include "concurrent/events/event_defs.h"

// enum class NetEventType;

class NetEvent
{
public:
	// to be able to register a class as metatype for signals and slots it must have a default constructor
	NetEvent();
	NetEvent(NetEventType eventType);
	~NetEvent();

	NetEventType eventType;
	
};

Q_DECLARE_METATYPE(NetEvent);
