#include "ClientNetEventExtendedInfo.h"


ClientNetEventExtendedInfo::ClientNetEventExtendedInfo()
{
}

ClientNetEventExtendedInfo::ClientNetEventExtendedInfo(const QSharedPointer<Client>& client,
                                                       const QMap<QString, QString>& environment)
	: ClientNetEvent(NetEventType::PresentationDataExtended, client), environment(environment)
{
}


ClientNetEventExtendedInfo::~ClientNetEventExtendedInfo()
{
}
