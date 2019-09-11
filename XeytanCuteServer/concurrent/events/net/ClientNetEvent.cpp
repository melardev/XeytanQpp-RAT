#include "ClientNetEvent.h"


ClientNetEvent::ClientNetEvent(NetEventType netEventType, const QSharedPointer<Client>& client):
	NetEvent(netEventType), client(client)
{
}

ClientNetEvent::ClientNetEvent() : NetEvent(NetEventType::TestEvent), cool("Nunca nunca mas"), client(nullptr)
{
}

ClientNetEvent::ClientNetEvent(NetEventType netEventType) : NetEvent(netEventType)
{
}

ClientNetEvent::~ClientNetEvent()
{
}
