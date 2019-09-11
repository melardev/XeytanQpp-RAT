#pragma once
#include "Packet.h"

enum class ConnectionAction : qint8
{
	Close,
	Uninstall,
	Restart,
};
class PacketConnection : public Packet
{
public:
	PacketConnection();
	PacketConnection(ConnectionAction action);
	~PacketConnection();
	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;
	ConnectionAction action;
};
