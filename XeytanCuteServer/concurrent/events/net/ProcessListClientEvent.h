#pragma once
#include "globals.h"
#include "models/Client.h"
#include "concurrent/events/net/ClientNetEvent.h"

class Process;

class ProcessListClientEvent : public ClientNetEvent
{
public:
	
	ProcessListClientEvent(const QSharedPointer<Client>& client, const QList<Process>& list);
	~ProcessListClientEvent();
	QList<Process> processList;
};
