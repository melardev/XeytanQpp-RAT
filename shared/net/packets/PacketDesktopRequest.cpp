#include "PacketDesktopRequest.h"


PacketDesktopRequest::PacketDesktopRequest(): PacketMediaRequest(PacketType::Desktop)
{
}

PacketDesktopRequest::PacketDesktopRequest(MediaAction mediaAction) : PacketMediaRequest(PacketType::Desktop, mediaAction)
{
}


PacketDesktopRequest::~PacketDesktopRequest()
{
}

QDataStream& PacketDesktopRequest::serialize(QDataStream& stream) const
{
	PacketMediaRequest::serialize(stream);
	stream << displayNames;
	return stream;
}

QDataStream& PacketDesktopRequest::deserialize(QDataStream& stream)
{
	PacketMediaRequest::deserialize(stream);
	stream >> displayNames;
	return stream;
}
