#include "PacketPresentation.h"


PacketPresentation::PacketPresentation() : Packet(PacketType::Presentation)
{
}


PacketPresentation::~PacketPresentation()
{
}

void PacketPresentation::setOs(const QString& os)
{
	this->os = os;
}

void PacketPresentation::setPcName(const QString& pcName)
{
	this->pcName = pcName;
}

void PacketPresentation::setPassword(const QString& password)
{
	this->password = password;
}

QDataStream& PacketPresentation::serialize(QDataStream& stream) const
{
	stream << qint8(details);
	stream << pcName;
	stream << os;
	stream << password;

	if (details == Extended)
	{
		stream << environment;
	}

	return stream;
}

QDataStream& PacketPresentation::deserialize(QDataStream& stream)
{
	qint8 detailsQint8;
	stream >> detailsQint8;
	this->details = static_cast<Details>(detailsQint8);
	stream >> pcName;
	stream >> os;
	stream >> password;

	if (details == Extended)
	{
		stream >> environment;
	}
	return stream;
}

Details PacketPresentation::getDetails()
{
	return details;
}

QMap<QString, QString> PacketPresentation::getEnvironment()
{
	return environment;
}

void PacketPresentation::addEnv(const QString& key, const QString& value)
{
	environment.insert(key, value);
}

void PacketPresentation::setDetails(Details details)
{
	this->details = details;
}
