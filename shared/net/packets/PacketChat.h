#pragma once
#include "Packet.h"

enum class ChatAction : qint8
{
	Start,
	AddMessage,
	Close,
};

class PacketChat : public Packet
{
public:
	PacketChat();
	PacketChat(ChatAction action);

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;

	QString message;
	ChatAction action;
};
