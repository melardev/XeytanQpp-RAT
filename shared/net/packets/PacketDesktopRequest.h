#pragma once
#include "PacketMediaRequest.h"
#include <QList>

class PacketDesktopRequest : public PacketMediaRequest
{
public:
	PacketDesktopRequest();
	PacketDesktopRequest(MediaAction mediaAction);
	
	~PacketDesktopRequest();

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;

	QList<QString> displayNames;
};
