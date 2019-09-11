#pragma once
#include "shared/net/packets/Packet.h"

enum class PacketType : short;
enum class MediaAction : signed char;

class PacketMediaRequest : public Packet
{
public:

	PacketMediaRequest(const PacketType& packetType);
	PacketMediaRequest(PacketType packet, MediaAction action);
	~PacketMediaRequest();

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;

	qint32 interval = -1;
	qint32 width = -1;
	qint32 height = -1;
	MediaAction action;
};
