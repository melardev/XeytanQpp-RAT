#pragma once
#include "shared/net/packets/Packet.h"

class PacketMediaConfigRequest : public Packet
{
public:
	
	PacketMediaConfigRequest(PacketType packetType);
	~PacketMediaConfigRequest();
	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;
};

