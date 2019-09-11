#pragma once
#include <QDialog>
#include "BaseAppDlg.h"
#include "ui_ReverseShellDlg.h"

class ReverseShellDlg : public QDialog, public BaseAppDlg
{
Q_OBJECT
public:
	ReverseShellDlg(qint64 clientId, QWidget* parent = Q_NULLPTR);
	void addOutput(const QString& output);
	void onSendCommand(bool checked = true);
	void disable();
signals:
	void sendEventToUiMediator(const GuiEvent& event);
	void sendEventToUiMediatorPtr(const QSharedPointer<GuiEvent>& event);

private:
	Ui::ReverseShellDlg ui;
	bool emptyOutput;
};
