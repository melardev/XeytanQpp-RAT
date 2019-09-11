#include "RemoteDesktopDialog.h"
#include "GuiMediator.h"
#include "concurrent/events/net/DesktopConfigClientNetEvent.h"
#include "concurrent/events/ui/MediaUiEvent.h"
#include <shared/media/media_defs.h>

RemoteDesktopDialog::RemoteDesktopDialog(qint64 clientId, QWidget* parent)
	: QDialog(parent), BaseAppDlg(clientId, UiWindow::DesktopWindow)
{
	ui.setupUi(this);
}

RemoteDesktopDialog::~RemoteDesktopDialog()
{
}

void RemoteDesktopDialog::setClientInfo(const QSharedPointer<MediaClientNetEvent>& event)
{
	event->image.save("D:/temp/serverui_screenshot.png");
	clientId = event->client->getId();
	ui.lblPixmapImage->setPixmap(event->image);
	ui.lblPixmapImage->adjustSize();
}


void RemoteDesktopDialog::setDisplayInfo(const QSharedPointer<DesktopConfigClientNetEvent>& event)
{
	int index = 0;
	if (event->screens.size() == 0)
	{
		ui.comboBoxDisplays->setEnabled(false);
		ui.lblPixmapImage->setText("Could not retrieve any display device");
		return;
	}
	else
	{
		ui.comboBoxDisplays->setEnabled(true);
		ui.btnStart->setEnabled(true);
		ui.comboBoxDisplays->addItem("All", "All");
	}

	foreach(auto& screen, event->screens)
	{
		QString displayText = screen.name + " (" + QString::number(screen.x)
			+ ", " + QString::number(screen.y) + ")";
		ui.comboBoxDisplays->addItem(displayText, screen.name);
		index++;
	}
}

void RemoteDesktopDialog::btnStartClicked()
{
	ui.btnStart->setText("Initializing ...");
	ui.btnStart->setEnabled(false);
	ui.btnStop->setEnabled(false);
	int index = ui.comboBoxDisplays->currentIndex();
	const QString selectedDisplay = ui.comboBoxDisplays->itemData(index).toString();

	const QSharedPointer<GuiEvent> uiEvent = QSharedPointer<GuiEvent>(
		new MediaUiEvent(MediaType::Desktop, UiWindow::DesktopWindow, UiEventAction::StartAction, clientId,
		                 selectedDisplay));
	emit sendEventToUiMediatorPtr(uiEvent);
}

void RemoteDesktopDialog::btnStopClicked()
{
	ui.btnStop->setText("Stopping ...");
	ui.btnStart->setEnabled(false);
	ui.btnStop->setEnabled(false);

	const QSharedPointer<GuiEvent> uiEvent = QSharedPointer<GuiEvent>(
		new MediaUiEvent(UiEventAction::StopAction, UiWindow::DesktopWindow, clientId));
	emit sendEventToUiMediatorPtr(uiEvent);
}
