#include "RemoteShellNetClientEvent.h"
#include <utility>

RemoteShellNetClientEvent::RemoteShellNetClientEvent(const QSharedPointer<Client>& client, const ShellAction action,
                                                     QString data):
	ClientNetEvent(NetEventType::ReverseShell, client),
	data(std::move(data)), action(action)
{
}
