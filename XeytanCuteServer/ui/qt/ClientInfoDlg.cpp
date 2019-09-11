#include "ClientInfoDlg.h"
#include "concurrent/events/net/ClientNetEventExtendedInfo.h"
#include "BaseQtWindow.h"
#include <QTableWidget>

ClientInfoDlg::ClientInfoDlg(qint64 clientId, QWidget* parent)
	: QDialog(parent), BaseAppDlg(clientId, UiWindow::InfoWindow)
{
	ui.setupUi(this);
	ui.tableWidget->clear();
	ui.tableWidget->horizontalHeader()->hide();
	ui.tableWidget->setColumnCount(2);

	ui.tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Property Name"));
	ui.tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Property Value"));

	// This 3 ways to set visibility
	ui.tableWidget->verticalHeader()->setVisible(true);
	// ui.tableWidget->horizontalHeader()->hide();
	ui.tableWidget->horizontalHeader()->show();

	// Adjust column size to the contents
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

ClientInfoDlg::~ClientInfoDlg()
{
}

void ClientInfoDlg::setClientInfo(const QSharedPointer<ClientNetEventExtendedInfo>& info)
{
	Q_ASSERT(info->client->getId() == clientId);
	int row = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row);


	QTableWidgetItem* pcName = new QTableWidgetItem("Pc Name");
	QTableWidgetItem* pcNameValue = new QTableWidgetItem(info->client->getPcName());

	ui.tableWidget->setItem(row, 0, pcName);
	ui.tableWidget->setItem(row, 1, pcNameValue);


	row = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row);


	QTableWidgetItem* os = new QTableWidgetItem("OS Name");
	QTableWidgetItem* osValue = new QTableWidgetItem(info->client->getOs());

	ui.tableWidget->setItem(row, 0, os);
	ui.tableWidget->setItem(row, 1, osValue);

	for (auto keyValuePair = info->environment.begin(); keyValuePair != info->environment.end() - 1; ++keyValuePair)
	{
		row = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(row);
		QTableWidgetItem* key = new QTableWidgetItem(keyValuePair.key());
		QTableWidgetItem* value = new QTableWidgetItem(keyValuePair.value());

		ui.tableWidget->setItem(row, 0, key);
		ui.tableWidget->setItem(row, 1, value);
	}

	// Adjust column size to the contents, not needed since we used setSectionResizeMode in the constructor above
	// ui.tableWidget->resizeColumnsToContents();
}

void ClientInfoDlg::onCellSelected(int row, int col)
{
	// itemAt(xPos, yPos) do not confuse them
	QTableWidgetItem* item = ui.tableWidget->item(row, col);
	auto text = item->text();
}
