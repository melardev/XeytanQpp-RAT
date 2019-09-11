#pragma once
#include "Packet.h"
#include "shared/media/media_defs.h"
class PacketCameraError : public Packet
{
public:
	PacketCameraError();
	PacketCameraError(const CameraErrorDetails& error);
	~PacketCameraError();

	QDataStream& serialize(QDataStream& stream) const override;
	QDataStream& deserialize(QDataStream& stream) override;
	CameraErrorDetails cameraError;
};
