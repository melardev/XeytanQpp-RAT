#pragma once
#include "Packet.h"
#include <QPixmap>

class PacketMediaResponse : public Packet
{
public:
	PacketMediaResponse();
	PacketMediaResponse(const PacketType& packetType);
	PacketMediaResponse(PacketType packetType, const QPixmap& pixmap);
	~PacketMediaResponse();

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;
	QPixmap image;
};
