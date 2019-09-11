#include "PacketCameraRequest.h"


PacketCameraRequest::PacketCameraRequest() : PacketMediaRequest(PacketType::Camera)
{
}

PacketCameraRequest::PacketCameraRequest(MediaAction start) : PacketMediaRequest(PacketType::Camera, start)
{
}

PacketCameraRequest::~PacketCameraRequest()
{
}

QDataStream& PacketCameraRequest::serialize(QDataStream& stream) const
{
	PacketMediaRequest::serialize(stream);
	stream << selectedCameraDeviceName;
	return stream;
}

QDataStream& PacketCameraRequest::deserialize(QDataStream& stream)
{
	PacketMediaRequest::deserialize(stream);
	stream >> selectedCameraDeviceName;
	return stream;
}
