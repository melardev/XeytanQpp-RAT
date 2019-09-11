#include "PacketDesktopResponse.h"


PacketDesktopResponse::PacketDesktopResponse(): Packet(PacketType::Desktop)
{
}

PacketDesktopResponse::PacketDesktopResponse(const QPixmap& pixmap): Packet(PacketType::Desktop), image(pixmap)
{
}


PacketDesktopResponse::~PacketDesktopResponse()
{
}

QDataStream& PacketDesktopResponse::serialize(QDataStream& stream) const
{
	stream << image;
	return stream;
}

QDataStream& PacketDesktopResponse::deserialize(QDataStream& stream)
{
	stream >> image;
	return stream;
}
