#pragma once
#include <QDataStream>
#include "packet_defs.h"

class Packet
{
public:
	explicit Packet(PacketType type);

	friend QDataStream& operator >>(QDataStream& stream, Packet& packet);
	friend QDataStream& operator <<(QDataStream& stream, const Packet& packet);

	virtual QDataStream& serialize(QDataStream& stream) const = 0;
	virtual QDataStream& deserialize(QDataStream& stream) = 0;
	PacketType type() const
	{
		return packetType;
	};

	virtual ~Packet() = default;

protected:
	PacketType packetType;
};


// QDataStream& operator >>(QDataStream& stream, Packet& packet);
// QDataStream& operator <<(QDataStream& stream, const Packet& packet);
