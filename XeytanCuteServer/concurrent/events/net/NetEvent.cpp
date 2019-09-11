#include "NetEvent.h"


NetEvent::~NetEvent()
{
}

NetEvent::NetEvent(): eventType(NetEventType::Unknown)
{
}

NetEvent::NetEvent(NetEventType eventType): eventType(eventType)
{
}
