#pragma once
#include "BaseAppDlg.h"
#include "ui_ChatDialog.h"


class GuiMediator;
class ClientNetEventExtendedInfo;

class ChatDialog : public QDialog, public BaseAppDlg
{
Q_OBJECT

public:
	ChatDialog(qint64 clientId, QWidget* parent = Q_NULLPTR);
	~ChatDialog();

	void addMessage(const QString& message);
	signals:
		void sendEventToUiMediator(const GuiEvent& event);
public slots:
	void onSendCommand(bool checked/*=true*/);

private:
	Ui::ChatDialog ui;
};
