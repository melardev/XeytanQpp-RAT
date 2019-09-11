#pragma once

#include "ClientContext.h"
#include "concurrent/events/net/ClientNetEvent.h"
#include "shared/net/packets/PacketType.h"

class RemoteClient
{
public:

	RemoteClient();

	RemoteClient(QSharedPointer<Client> client);

	RemoteClient(const QUuid& uuid, const QSharedPointer<Client>& client,
	             const QSharedPointer<ClientContext>& client_context);;
	bool expectsPacketAndPop(const PacketType& packetType);

	~RemoteClient();
	//	Client& getClient();

	QSharedPointer<Client> getClient();
	void appendExpectedPacket(PacketType packetType);
	QSharedPointer<NetClientService> getNetClientService();
	const QSharedPointer<ClientContext> getContext();
	bool expectsPacket(PacketType type);
	void removeExpectedPacket(PacketType packet);
	void removeAllExpected();

private:
	// Client mClient;
	QSharedPointer<Client> mClient;
	QUuid uuid;
	QList<PacketType> expectedPackets;
	const QSharedPointer<ClientContext> context;
};
