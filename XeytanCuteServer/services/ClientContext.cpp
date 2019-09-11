#include "ClientContext.h"


ClientContext::ClientContext()
{
}

ClientContext::ClientContext(const QSharedPointer<NetServerService>& net_server_service,
                             const QSharedPointer<NetClientService>& client)
	: netServerService(net_server_service),
	  netClientService(client)
{
}


ClientContext::~ClientContext()
{
}

QSharedPointer<NetClientService> ClientContext::getNetClientService()
{
	return netClientService;
}

const QSharedPointer<NetServerService> ClientContext::getNetServerService()
{
	return netServerService;
}



void ClientContext::set_net_client_service(const QSharedPointer<NetClientService>& client)
{
	this->netClientService = client;
}
