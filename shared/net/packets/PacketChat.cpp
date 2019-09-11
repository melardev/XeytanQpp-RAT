#include "PacketChat.h"


PacketChat::PacketChat(): Packet(PacketType::Chat)
{
}

PacketChat::PacketChat(ChatAction action) : Packet(PacketType::Chat), action(action)
{
}

QDataStream& PacketChat::serialize(QDataStream& stream) const
{
	stream << qint8(action) << message;
	return stream;
}

QDataStream& PacketChat::deserialize(QDataStream& stream)
{
	qint8 actionInt8;
	stream >> actionInt8 >> message;
	this->action = (ChatAction)actionInt8;

	return stream;
}
