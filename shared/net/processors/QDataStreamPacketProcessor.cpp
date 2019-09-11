#include "QDataStreamPacketProcessor.h"
#include "../packets/Packet.h"
#include <QDataStream>
#include <QIODevice>

QDataStreamPacketProcessor::QDataStreamPacketProcessor()
{
}


QDataStreamPacketProcessor::~QDataStreamPacketProcessor()
{
}

QByteArray QDataStreamPacketProcessor::serialize(const Packet& packet)
{
	QByteArray serializedPacket;

	QDataStream outPacketBufferStream(&serializedPacket, QIODevice::ReadWrite);
	outPacketBufferStream.setVersion(QDataStream::Qt_4_0);
	// 4 bytes will be for the size, for now we don't know the size, so write 0, we will come back to this later
	outPacketBufferStream << quint64(0);
	// 4 bytes for the packet Type
	const int packetHeaderLength = serializedPacket.size();

	outPacketBufferStream << (qint16)(packet.type());

	// serialize the packet
	outPacketBufferStream << packet;

	// seek to the beginning
	outPacketBufferStream.device()->seek(0);
	// now we know the packet size, write it
	const quint64 totalPacketSize = quint64(serializedPacket.size());
	outPacketBufferStream << quint64(totalPacketSize - packetHeaderLength);
	return serializedPacket;
}

void QDataStreamPacketProcessor::deserialize(const QByteArray& inputStream, Packet& packet) const
{
	QDataStream stream(inputStream);
	stream >> packet;
}


#include <QFile>

void QDataStreamPacketProcessor::writeIntoFile(const QByteArray& byteArray)
{
	QFile file("D:/temp/dump.bin");
	file.open(QIODevice::WriteOnly);
	file.write(byteArray);
	file.close();
}
