#pragma once
#include "Packet.h"

class PacketProcessRequest : public Packet
{
public:
	PacketProcessRequest();
	~PacketProcessRequest();
	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;
};
