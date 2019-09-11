#pragma once
#include "shared/net/packets/Packet.h"

struct ScreenInfo;

class PacketDesktopConfigResponse : public Packet
{
public:
	PacketDesktopConfigResponse();
	PacketDesktopConfigResponse(const QList<ScreenInfo>& screens);
	~PacketDesktopConfigResponse();
	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;

	QList<ScreenInfo> screens;
};
