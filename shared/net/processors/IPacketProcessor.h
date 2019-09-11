#pragma once
#include <QByteArray>

class Packet;

class IPacketProcessor
{
public:
	IPacketProcessor();
	virtual ~IPacketProcessor();

	virtual QByteArray serialize(const Packet& packet) = 0;
	
};

