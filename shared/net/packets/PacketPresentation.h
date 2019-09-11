#pragma once
#include "Packet.h"
#include <QMap>

enum Details : qint8
{
	Basic,
	Extended
};

class PacketPresentation : public Packet
{
public:
	PacketPresentation();
	~PacketPresentation();

	QString getOs() const
	{
		return os;
	}

	QString getPcName() const
	{
		return pcName;
	}

	QString get_password() const
	{
		return password;
	}

	void setOs(const QString &os);
	void setPcName(const QString &pcName);
	void setPassword(const QString &password);

	virtual QDataStream& serialize(QDataStream& stream) const override;
	virtual QDataStream& deserialize(QDataStream& stream) override;
	Details getDetails();
	QMap<QString, QString> getEnvironment();
	void addEnv(const QString& key, const QString& value);
	void setDetails(Details details);


private:
	PacketType m_type;
	QString os;
	QString pcName;
	QString password;
	Details details = Basic;
	QMap<QString, QString> environment;
};

