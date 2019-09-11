#pragma once
#include "../ui_ChatDialog.h"

class GuiMediator;
class ClientNetEventExtendedInfo;

class ChatDialog : public QDialog
{
Q_OBJECT

public:
	ChatDialog(QWidget* parent = Q_NULLPTR);
	~ChatDialog();

	void addMessage(const QString& message);
signals:
	void sendMessageClicked(const QString& message);

public slots:
	void onSendCommand(bool checked/*=true*/);

private:
	Ui::ChatDialog ui;
};
