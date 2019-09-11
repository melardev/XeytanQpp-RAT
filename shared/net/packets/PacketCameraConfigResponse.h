#pragma once
#include "shared/net/packets/Packet.h"

struct CameraInfo;

class PacketCameraConfigResponse : public Packet
{
public:
	PacketCameraConfigResponse();
	PacketCameraConfigResponse(const QList<CameraInfo>& list);
	~PacketCameraConfigResponse();

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;

	QList<CameraInfo> availableCameras;
};
