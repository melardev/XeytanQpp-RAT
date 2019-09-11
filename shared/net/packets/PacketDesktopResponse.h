#pragma once
#include "Packet.h"
#include <QPixmap>

class PacketDesktopResponse : public Packet
{
public:
	PacketDesktopResponse();
	PacketDesktopResponse(const QPixmap& pixmap);
	~PacketDesktopResponse();


	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;

	QPixmap image;
};
