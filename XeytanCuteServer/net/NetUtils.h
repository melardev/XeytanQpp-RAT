#pragma once
#include <qtcpsocket.h>

class NetUtils
{
public:
	NetUtils();
	static QString getIPv4AddrString(QTcpSocket* socket);
	~NetUtils();
};

