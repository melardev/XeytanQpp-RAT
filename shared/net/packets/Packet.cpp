#include "Packet.h"

QDataStream& operator<<(QDataStream& stream, const Packet& packet)
{
	return packet.serialize(stream);
}

QDataStream& operator>>(QDataStream& stream, Packet& packet)
{
	return packet.deserialize(stream);
}

Packet::Packet(PacketType type):packetType(type)
{

}
