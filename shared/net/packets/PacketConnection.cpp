#include "PacketConnection.h"


PacketConnection::PacketConnection() : Packet(PacketType::Connection)
{
}

PacketConnection::PacketConnection(ConnectionAction action) : Packet(PacketType::Connection), action(action)
{
}


PacketConnection::~PacketConnection()
{
}

QDataStream& PacketConnection::serialize(QDataStream& stream) const
{
	stream << qint8(action);
	return stream;
}

QDataStream& PacketConnection::deserialize(QDataStream& stream)
{
	qint8 actionQInt8;
	stream >> actionQInt8;
	action = static_cast<ConnectionAction>(actionQInt8);
	return stream;
}
