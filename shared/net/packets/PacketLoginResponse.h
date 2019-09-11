#pragma once
#include "Packet.h"

class PacketLoginResponse : public Packet
{
public:
	PacketLoginResponse();
	PacketLoginResponse(bool success);

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;


	bool loginWasSuccess() const
	{
		return this->success;
	}

private:
	bool success;
};
