#include "PacketProcessResponse.h"


PacketProcessResponse::PacketProcessResponse() : Packet(PacketType::ProcessList)
{
}


PacketProcessResponse::~PacketProcessResponse()
{
}

QDataStream& PacketProcessResponse::serialize(QDataStream& stream) const
{
	stream << processes;
	return stream;
}

QDataStream& PacketProcessResponse::deserialize(QDataStream& stream)
{
	stream >> processes;
	return stream;
}
