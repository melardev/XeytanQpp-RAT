#include "ProcessListDlg.h"
#include "shared/models/process.h"
#include <QString>
#include <QFile>
#include "BaseQtWindow.h"

ProcessListDlg::ProcessListDlg(qint64 clientId, QWidget* parent)
	: QDialog(parent), BaseAppDlg(clientId, UiWindow::ProcessListWindow)
{
	ui.setupUi(this);
}

ProcessListDlg::~ProcessListDlg()
{
}

void ProcessListDlg::setProcessList(const QSharedPointer<ProcessListClientEvent>& processes)
{
	int row = 0;

	Q_FOREACH(const Process& process, processes->processList)
	{
		ui.tableProcesses->insertRow(0);
		int col = 0;
		QTableWidgetItem* item = new QTableWidgetItem(QString::number(process.pid));
		ui.tableProcesses->setItem(0, col++, item);

		QString procName = process.path;

		procName = procName.replace("\\", "/")
		        .replace("//", "/")
		        .split("/").last();

		item = new QTableWidgetItem(procName);
		item->setData(Qt::DecorationRole, process.icon);

		ui.tableProcesses->setItem(0, col++, item);

		item = new QTableWidgetItem(process.path);
		ui.tableProcesses->setItem(0, col++, item);

		item = new QTableWidgetItem(process.title);
		ui.tableProcesses->setItem(0, col++, item);

		row++;
	}

	ui.tableProcesses->resizeColumnsToContents();
}
