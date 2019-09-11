#pragma once
#include "ClientNetEvent.h"
#include "shared/net/packets/PacketChat.h"

class ChatNetClientEvent : public ClientNetEvent
{
public:

	QString message;
	ChatAction action;

	ChatNetClientEvent(const QSharedPointer<Client>& client, const ChatAction action, const QString& message)
		: ClientNetEvent(NetEventType::Chat, client), message(message), action(action)
	{
	}
};
