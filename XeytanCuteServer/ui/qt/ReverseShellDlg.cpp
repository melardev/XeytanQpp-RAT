#include "ReverseShellDlg.h"
#include "concurrent/events/ui/GuiEvent.h"

ReverseShellDlg::ReverseShellDlg(qint64 clientId, QWidget* parent)
	: QDialog(parent), BaseAppDlg(clientId, UiWindow::ReverseShellWindow), emptyOutput(true)
{
	ui.setupUi(this);
	setWindowTitle(QString("Reverse Shell [%1]").arg(clientId));
	connect(ui.btnSend, &QPushButton::clicked, this, &ReverseShellDlg::onSendCommand);
}

void ReverseShellDlg::addOutput(const QString& output)
{
	if (emptyOutput)
	{
		emptyOutput = false;
		ui.txtShellOutput->setText(output);
	}
	else
	{
		ui.txtShellOutput->append(output);
	}
}

void ReverseShellDlg::onSendCommand(bool checked/*=true*/)
{
	QString* command = new QString(ui.txtShellInput->toPlainText());
	GuiEvent event(UiWindow::ReverseShellWindow, UiEventAction::UpdateAction, clientId);
	event.setData(command);
	ui.txtShellInput->setText(""); // clear() also works, but would delete all the history, etc
	emit sendEventToUiMediator(event);
}

void ReverseShellDlg::disable()
{
	// User disconnected or closed the reverse shell process
	ui.txtShellOutput->setEnabled(false);
	ui.txtShellInput->setEnabled(false);
	ui.btnSend->setEnabled(false);
	ui.txtShellOutput->append("Shell Closed");
}
