#include "XeytanQpp.h"
#include <QtWidgets/QApplication>
#include "concurrent/events/ui/MainGuiEvent.h"
#include "concurrent/events/net/ClientNetEvent.h"
#include "shared/net/packets/Packet.h"
#include "services/RemoteClient.h"

QSharedPointer<XeytanQpp> appInstance;

Q_DECLARE_METATYPE(QSharedPointer<Client>);

int main(int argc, char* argv[])
{
	QApplication application(argc, argv);
	qRegisterMetaType<GuiEvent>("GuiEvent");
	qRegisterMetaType<QSharedPointer<GuiEvent>>("QSharedPointer<GuiEvent>");
	qRegisterMetaType<MainGuiEvent>("MainGuiEvent");
	qRegisterMetaType<QSharedPointer<RemoteClient>>("QSharedPointer<RemoteClient>");
	qRegisterMetaType<QSharedPointer<Packet>>("QSharedPointer<Packet>");
	qRegisterMetaType<NetEvent>("NetEvent");
	// We must register it as well even though it is a subclass of NetEvent which was already registered
	qRegisterMetaType<ClientNetEvent>("ClientNetEvent");
	qRegisterMetaType<QSharedPointer<ClientNetEvent>>("QSharedPointer<ClientNetEvent>");
	//qRegisterMetaType<NetEvent>("NetEvent");
	//	qRegisterMetaType<ClientNetEvent>("ClientNetEvent");
	qRegisterMetaType<QSharedPointer<Client>>("QSharedPointer<Client>");

	// QSharedPointer<XeytanQpp> appInstance = XeytanQpp::getInstance();
	// auto appInstance = new XeytanQpp;
	// appInstance->launch();
	appInstance = XeytanQpp::launch();

	return application.exec();
}
