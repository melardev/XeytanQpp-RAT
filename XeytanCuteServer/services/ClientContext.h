#pragma once
#include "NetClientService.h"

class NetServerService;

class ClientContext
{
public:
	ClientContext();
	ClientContext(const QSharedPointer<NetServerService>& net_server_service,
	              const QSharedPointer<NetClientService>& client);
	~ClientContext();


	QSharedPointer<NetClientService> getNetClientService();
	const QSharedPointer<NetServerService> getNetServerService();


	void set_net_client_service(const QSharedPointer<NetClientService>& client);

private:
	// The owner
	const QSharedPointer<NetServerService> netServerService;
	// The handler for any successive connections: desktop, camera, keylog, etc.
	QSharedPointer<NetClientService> netClientService;
};
