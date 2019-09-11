#pragma once
#include "Packet.h"
#include "shared/action_defs.h"

class PacketShell : public Packet
{
public:
	

	PacketShell();
	~PacketShell();

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;


	QString data;
	ShellAction shellAction;
};
