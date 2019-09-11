#include "ChatDialog.h"
#include "ui/ui_defs.h"
#include "concurrent/events/ui/GuiEvent.h"

ChatDialog::ChatDialog(qint64 clientId, QWidget* parent)
	: QDialog(parent), BaseAppDlg(clientId, UiWindow::ChatWindow)
{
	ui.setupUi(this);

	setWindowTitle(QString("Chat - Master - [%1]").arg(clientId));
	connect(ui.btnSend, &QPushButton::clicked, this, &ChatDialog::onSendCommand);
}

ChatDialog::~ChatDialog()
{
}

void ChatDialog::addMessage(const QString& message)
{
	ui.txtChatOutput->append("Client: " + message);
}

void ChatDialog::onSendCommand(bool checked)
{
	QString* command = new QString(ui.txtChatInput->toPlainText());
	GuiEvent event(UiWindow::ChatWindow, UiEventAction::UpdateAction, clientId);
	event.setData(command);

	ui.txtChatOutput->append("Me: " + ui.txtChatInput->toPlainText());
	ui.txtChatInput->setText(""); // clear() also works, but would delete all the history, etc
	emit sendEventToUiMediator(event);
}
