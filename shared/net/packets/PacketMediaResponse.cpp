#include "PacketMediaResponse.h"

PacketMediaResponse::PacketMediaResponse() : Packet(PacketType::Invalid)
{
}

PacketMediaResponse::PacketMediaResponse(const PacketType& packetType) : Packet(packetType)
{
	Q_ASSERT(packetType == PacketType::Camera || packetType == PacketType::Desktop);
}

PacketMediaResponse::PacketMediaResponse(PacketType packetType, const QPixmap& pixmap)
	: Packet(packetType), image(pixmap)
{
}

PacketMediaResponse::~PacketMediaResponse()
{
}

QDataStream& PacketMediaResponse::serialize(QDataStream& stream) const
{
	stream << image;
	return stream;
}

QDataStream& PacketMediaResponse::deserialize(QDataStream& stream)
{
	stream >> image;
	return stream;
}
