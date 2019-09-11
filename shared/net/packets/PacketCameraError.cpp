#include "PacketCameraError.h"


PacketCameraError::PacketCameraError() : Packet(PacketType::CameraError)
{
}

PacketCameraError::PacketCameraError(const CameraErrorDetails& error) : Packet(PacketType::CameraError), cameraError(error)
{
}


PacketCameraError::~PacketCameraError()
{
}

QDataStream& PacketCameraError::serialize(QDataStream& stream) const
{
	stream << cameraError;
	return stream;
}

QDataStream& PacketCameraError::deserialize(QDataStream& stream)
{
	stream >> cameraError;
	return stream;
}
