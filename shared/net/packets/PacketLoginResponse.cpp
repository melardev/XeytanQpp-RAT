#include "PacketLoginResponse.h"


PacketLoginResponse::PacketLoginResponse() : Packet(PacketType::Login), success(false)
{
}

PacketLoginResponse::PacketLoginResponse(bool success): Packet(PacketType::Login), success(success)
{
}

QDataStream& PacketLoginResponse::serialize(QDataStream& stream) const
{
	stream << success;
	return stream;
}

QDataStream& PacketLoginResponse::deserialize(QDataStream& stream)
{
	stream >> success;
	return stream;
}
