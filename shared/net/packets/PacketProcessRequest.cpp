#include "PacketProcessRequest.h"


PacketProcessRequest::PacketProcessRequest() : Packet(PacketType::ProcessList)
{
}


PacketProcessRequest::~PacketProcessRequest()
{
}

QDataStream& PacketProcessRequest::serialize(QDataStream& stream) const
{
	return stream;
}

QDataStream& PacketProcessRequest::deserialize(QDataStream& stream)
{
	return stream;
}
