#include "PacketMediaRequest.h"
#include "packet_defs.h"


PacketMediaRequest::PacketMediaRequest(const PacketType& packetType) : Packet(packetType)
{
	Q_ASSERT(packetType == PacketType::Camera || packetType == PacketType::Desktop);
}

PacketMediaRequest::PacketMediaRequest(PacketType packetType, MediaAction action) : Packet(packetType), action(action)
{
	Q_ASSERT(packetType == PacketType::Camera || packetType == PacketType::Desktop);
}

PacketMediaRequest::~PacketMediaRequest()
{
}

QDataStream& PacketMediaRequest::serialize(QDataStream& stream) const
{
	stream << qint8(action) << width << height << interval;
	return stream;
}

QDataStream& PacketMediaRequest::deserialize(QDataStream& stream)
{
	qint8 actionInt8;
	stream >> actionInt8 >> width >> height >> interval;
	action = static_cast<MediaAction>(actionInt8);
	return stream;
}
