#pragma once

#include <QDialog>
#include "ui_ProcessListDlg.h"
#include "BaseAppDlg.h"
#include "concurrent/events/net/ProcessListClientEvent.h"

class ProcessListDlg : public QDialog, public BaseAppDlg
{
Q_OBJECT

public:

	ProcessListDlg(qint64 clientId, QWidget* parent = Q_NULLPTR);
	~ProcessListDlg();
	void setProcessList(const QSharedPointer<ProcessListClientEvent>& processes);
private:
	Ui::ProcessListDlg ui;
};
