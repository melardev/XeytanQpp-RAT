#pragma once
#include "IPacketProcessor.h"

class QDataStreamPacketProcessor : public IPacketProcessor
{
public:
	QDataStreamPacketProcessor();
	~QDataStreamPacketProcessor();
	void writeIntoFile(const QByteArray& byteArray);
	QByteArray serialize(const Packet& packet) override;
	void deserialize(const QByteArray& inputStream, Packet& packet) const;
};

