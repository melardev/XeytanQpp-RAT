#pragma once
#include "concurrent/events/net/ClientNetEvent.h"
#include <QMap>

class ClientNetEventExtendedInfo : public ClientNetEvent
{
public:
	ClientNetEventExtendedInfo();
	ClientNetEventExtendedInfo(const QSharedPointer<Client>& client, const QMap<QString, QString>& environment);
	~ClientNetEventExtendedInfo();

	QMap<QString, QString> environment;
};
