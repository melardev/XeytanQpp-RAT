#include "ChatDialog.h"


ChatDialog::ChatDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowTitle(QString("Chat - Client"));
	connect(ui.btnSend, &QPushButton::clicked, this, &ChatDialog::onSendCommand);
}

ChatDialog::~ChatDialog()
{
}

void ChatDialog::addMessage(const QString& message)
{
	ui.txtChatOutput->append("Server: " + message);
}

void ChatDialog::onSendCommand(bool checked)
{
	emit sendMessageClicked(ui.txtChatInput->toPlainText());
	ui.txtChatOutput->append("Me: " + ui.txtChatInput->toPlainText());
	ui.txtChatInput->setText(""); // clear() also works, but would delete all the history, etc
}
