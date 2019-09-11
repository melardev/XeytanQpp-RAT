#include "PacketLoginRequest.h"


PacketLoginRequest::PacketLoginRequest(): Packet(PacketType::Login)
{
}

PacketLoginRequest::PacketLoginRequest(const QString& password, const QString& os, const QString& pc_name)
	: Packet(PacketType::Login), password(password), os(os), pc_name(pc_name)
{
}

void PacketLoginRequest::setOs(const QString& os)
{
	this->os = os;
}

void PacketLoginRequest::setPcName(const QString& pcName)
{
	this->pc_name = pcName;
}

void PacketLoginRequest::setPassword(const QString& password)
{
	this->password = password;
}

void PacketLoginRequest::setType(const PacketType type)
{
	this->m_type = type;
}


QDataStream& PacketLoginRequest::serialize(QDataStream& stream) const
{
	stream << os << pc_name << password;
	return stream;
}

QDataStream& PacketLoginRequest::deserialize(QDataStream& stream)
{
	stream >> os >> pc_name >> password;
	return stream;
}

