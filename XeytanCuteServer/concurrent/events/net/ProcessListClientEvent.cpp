#include "ProcessListClientEvent.h"
#include "shared/models/process.h"


ProcessListClientEvent::ProcessListClientEvent(const QSharedPointer<Client>& client, const QList<Process>& list)
	: ClientNetEvent(NetEventType::ProcessListData, client), processList(list)
{
}


ProcessListClientEvent::~ProcessListClientEvent()
{
}
