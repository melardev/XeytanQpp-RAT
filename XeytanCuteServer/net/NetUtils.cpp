#include "NetUtils.h"
#include <QString>
#include <QTcpSocket>
#include <qhostaddress.h>


NetUtils::NetUtils()
{
}


QString NetUtils::getIPv4AddrString(QTcpSocket* socket)
{
	QString IPv4IPv6 = socket->peerAddress().toString();
	QString IPv4 = IPv4IPv6.right(IPv4IPv6.size() - 7);
	return IPv4;
}

NetUtils::~NetUtils()
{
}
