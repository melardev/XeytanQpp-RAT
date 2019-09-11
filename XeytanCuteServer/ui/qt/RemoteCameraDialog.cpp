#include "RemoteCameraDialog.h"
#include "GuiMediator.h"
#include "concurrent/events/net/DesktopConfigClientNetEvent.h"
#include "concurrent/events/ui/MediaUiEvent.h"
#include <shared/media/media_defs.h>
#include "concurrent/events/net/CameraConfigClientNetEvent.h"

RemoteCameraDialog::RemoteCameraDialog(qint64 clientId, QWidget* parent)
	: QDialog(parent), BaseAppDlg(clientId, UiWindow::CameraWindow)
{
	ui.setupUi(this);
}

RemoteCameraDialog::~RemoteCameraDialog()
{
}

void RemoteCameraDialog::setClientInfo(const QSharedPointer<MediaClientNetEvent>& event)
{
	// event->image.save("D:/temp/serverui_screenshot.png");
	clientId = event->client->getId();
	ui.lblPixmapImage->setPixmap(event->image);
	ui.lblPixmapImage->adjustSize();
	ui.btnStart->setEnabled(true);
	ui.btnStart->setText("Pause (not implemented)");
	ui.btnStop->setEnabled(true);
}


void RemoteCameraDialog::setDisplayInfo(const QSharedPointer<CameraConfigClientNetEvent>& event) const
{
	int index = 0;
	if (event->cameras.empty())
	{
		ui.lblPixmapImage->setText("No cameras available on the target");
		ui.comboBoxCameras->setEnabled(false);
		return;
	}
	foreach(auto& camera, event->cameras)
	{
		QString displayText = camera.description;
		ui.comboBoxCameras->addItem(displayText, camera.deviceName);
		index++;
	}
	ui.comboBoxCameras->setEnabled(true);
	ui.comboBoxCameras->setCurrentIndex(0);

	ui.btnStart->setEnabled(true);
}

void RemoteCameraDialog::btnStartClicked()
{
	if (ui.btnStart->text().contains("Start", Qt::CaseInsensitive))
	{
		ui.btnStart->setText("Initializing ...");
		ui.btnStart->setEnabled(false);
		ui.btnStop->setEnabled(false);
		int index = ui.comboBoxCameras->currentIndex();
		QString selectedDisplay = ui.comboBoxCameras->itemData(index).toString();

		const QSharedPointer<GuiEvent> uiEvent = QSharedPointer<GuiEvent>(
			new MediaUiEvent(MediaType::Camera, UiWindow::CameraWindow, UiEventAction::StartAction, clientId,
			                 selectedDisplay));
		emit sendEventToUiMediatorPtr(uiEvent);
	}
	else if (ui.btnStart->text().contains("pause", Qt::CaseInsensitive))
	{
		ui.btnStart->setText("Resume");
		ui.btnStart->setEnabled(true);
		ui.btnStop->setEnabled(true);
		const QSharedPointer<GuiEvent> uiEvent = QSharedPointer<GuiEvent>(
			new MediaUiEvent(MediaType::Camera, UiWindow::CameraWindow, UiEventAction::PauseAction, clientId));
		emit sendEventToUiMediatorPtr(uiEvent);
	}
}

void RemoteCameraDialog::btnStopClicked()
{
	ui.btnStop->setText("Stopping ...");
	ui.btnStart->setEnabled(false);
	ui.btnStop->setEnabled(false);

	const QSharedPointer<GuiEvent> uiEvent = QSharedPointer<GuiEvent>(
		new MediaUiEvent(UiEventAction::StopAction, UiWindow::CameraWindow, clientId));
	emit sendEventToUiMediatorPtr(uiEvent);
}
