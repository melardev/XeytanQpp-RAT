#include "PacketDesktopConfigResponse.h"
#include "shared/media/media_defs.h"

PacketDesktopConfigResponse::PacketDesktopConfigResponse()
	: Packet(PacketType::DesktopConfig)
{
}

PacketDesktopConfigResponse::PacketDesktopConfigResponse(const QList<ScreenInfo>& screens)
	: Packet(PacketType::DesktopConfig),
	  screens(screens)
{
}


PacketDesktopConfigResponse::~PacketDesktopConfigResponse()
{
}

QDataStream& PacketDesktopConfigResponse::serialize(QDataStream& stream) const
{
	stream << screens;
	return stream;
}

QDataStream& PacketDesktopConfigResponse::deserialize(QDataStream& stream)
{
	stream >> screens;
	return stream;
}
