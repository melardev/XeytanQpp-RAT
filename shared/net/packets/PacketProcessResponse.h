#pragma once
#include "Packet.h"
#include "shared/models/process.h"

class PacketProcessResponse : public Packet
{
public:
	PacketProcessResponse();
	~PacketProcessResponse();

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;
	QList<Process> processes;
};
