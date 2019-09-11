#include "PacketCameraConfigResponse.h"
#include <shared/media/media_defs.h>


PacketCameraConfigResponse::PacketCameraConfigResponse() : Packet(PacketType::CameraConfig)
{
}

PacketCameraConfigResponse::PacketCameraConfigResponse(const QList<CameraInfo>& list)
	: Packet(PacketType::CameraConfig),
	  availableCameras(list)
{
}

PacketCameraConfigResponse::~PacketCameraConfigResponse()
{
}

QDataStream& PacketCameraConfigResponse::serialize(QDataStream& stream) const
{
	stream << availableCameras;
	return stream;
}

QDataStream& PacketCameraConfigResponse::deserialize(QDataStream& stream)
{
	stream >> availableCameras;
	return stream;
}
