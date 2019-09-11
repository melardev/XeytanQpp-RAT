#pragma once

#include <QDialog>
#include "../ui_ClientInfoDlg.h"
#include "BaseAppDlg.h"


class GuiMediator;
class ClientNetEventExtendedInfo;

class ClientInfoDlg : public QDialog, public BaseAppDlg
{
Q_OBJECT

public:
	ClientInfoDlg(qint64 clientId, QWidget* parent = Q_NULLPTR);
	~ClientInfoDlg();
	void setClientInfo(const QSharedPointer<ClientNetEventExtendedInfo>& info);

public slots:
	void onCellSelected(int, int);

private:
	Ui::ClientInfoDlg ui;
};
