#pragma once
#include "Packet.h"
#include "PacketMediaRequest.h"

class PacketCameraRequest : public PacketMediaRequest
{
public:
	PacketCameraRequest();
	PacketCameraRequest(MediaAction start);
	~PacketCameraRequest();

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;

	QString selectedCameraDeviceName;
};
