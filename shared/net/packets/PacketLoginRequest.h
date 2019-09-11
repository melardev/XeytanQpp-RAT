#pragma once
#include "Packet.h"

class PacketLoginRequest : public Packet
{
public:
	PacketLoginRequest();
	PacketLoginRequest(const QString& password, const QString& os, const QString& pc_name);

	QString get_os() const
	{
		return os;
	}

	QString get_pcName() const
	{
		return pc_name;
	}

	QString get_password() const
	{
		return password;
	}

	void setOs(const QString &os);
	void setPcName(const QString &pcName);
	void setPassword(const QString &password);
	void setType(const PacketType type);

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;
	

private:
	PacketType m_type;
	QString os;
	QString pc_name;
	QString password;
};
