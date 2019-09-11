#include "RemoteClient.h"
#include <utility>
#include "shared/net/packets/PacketType.h"

RemoteClient::RemoteClient()
{
}

RemoteClient::~RemoteClient()
{
	qDebug() << QString("Remote client with Id %1 and socket descriptor %2 is being destroyed")
	            .arg(uuid.toString())
	            .arg(mClient->getId());
}


RemoteClient::RemoteClient(QSharedPointer<Client> client): context(nullptr),
                                                           mClient(std::move(client))
{
}

RemoteClient::RemoteClient(const QUuid& uuid, const QSharedPointer<Client>& client,
                           const QSharedPointer<ClientContext>& context)
	:
	context(context),
	mClient(client),
	uuid(uuid)
{
}

bool RemoteClient::expectsPacketAndPop(const PacketType& packetType)
{
	bool result = expectedPackets.contains(packetType);
	if (result)
		expectedPackets.removeOne(packetType);
	return result;
}

QSharedPointer<Client> RemoteClient::getClient()
{
	return mClient;
}

void RemoteClient::appendExpectedPacket(PacketType packetType)
{
	expectedPackets.append(packetType);
}

QSharedPointer<NetClientService> RemoteClient::getNetClientService()
{
	return context->getNetClientService();
}

const QSharedPointer<ClientContext> RemoteClient::getContext()
{
	return context;
}

bool RemoteClient::expectsPacket(PacketType type)
{
	return expectedPackets.contains(type);;
}

void RemoteClient::removeExpectedPacket(PacketType packet)
{
	expectedPackets.removeOne(packet);
}

void RemoteClient::removeAllExpected()
{
	expectedPackets.clear();
}
