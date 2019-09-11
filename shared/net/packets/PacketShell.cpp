#include "PacketShell.h"


PacketShell::PacketShell(): Packet(PacketType::Shell)
{
}


PacketShell::~PacketShell()
{
}

QDataStream& PacketShell::serialize(QDataStream& stream) const
{
	stream << qint8(shellAction);
	stream << data;

	return stream;
}

QDataStream& PacketShell::deserialize(QDataStream& stream)
{
	qint8 shellActionInt8;
	stream >> shellActionInt8;
	shellAction = (ShellAction)shellActionInt8;
	stream >> data;
	return stream;
}
