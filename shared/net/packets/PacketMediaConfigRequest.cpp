#include "PacketMediaConfigRequest.h"
#include "packet_defs.h"


PacketMediaConfigRequest::PacketMediaConfigRequest(PacketType packetType) : Packet(packetType)
{
	Q_ASSERT(packetType == PacketType::DesktopConfig || packetType == PacketType::CameraConfig);
}

PacketMediaConfigRequest::~PacketMediaConfigRequest()
{
}

QDataStream& PacketMediaConfigRequest::serialize(QDataStream& stream) const
{
	return stream;
}

QDataStream& PacketMediaConfigRequest::deserialize(QDataStream& stream)
{
	return stream;
}
