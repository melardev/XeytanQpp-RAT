#pragma once
#include "concurrent/events/net/ClientNetEvent.h"
#include "shared/action_defs.h"

class RemoteShellNetClientEvent : public ClientNetEvent
{
public:

	QString data;
	ShellAction action;

	RemoteShellNetClientEvent(const QSharedPointer<Client>& client, const ShellAction action, QString data);
};
