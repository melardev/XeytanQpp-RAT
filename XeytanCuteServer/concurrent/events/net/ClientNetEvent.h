#pragma once
#include "NetEvent.h"
#include "models/Client.h"
#include <QSharedPointer>

class ClientNetEvent : public NetEvent
{
public:
	// ClientNetEvent();
	// ClientNetEvent(const Client& rc);
	// ClientNetEvent(NetEventType netEventType, const Client& rc);

	ClientNetEvent(NetEventType netEventType, const QSharedPointer<Client>& client);
	ClientNetEvent(NetEventType netEventType, const QSharedPointer<Client>& client, void* data);
	ClientNetEvent();
	ClientNetEvent(NetEventType netEventType);
	virtual ~ClientNetEvent();

	// RemoteClient remoteClient;
	QString cool;
	// Client client;

	QSharedPointer<Client> client;
};
