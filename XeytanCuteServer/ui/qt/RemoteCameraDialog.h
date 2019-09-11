#pragma once

#include "ui_RemoteCameraDialog.h"
#include "GuiMediator.h"
#include "concurrent/events/net/MediaClientNetEvent.h"
#include "BaseAppDlg.h"
#include "concurrent/events/net/CameraConfigClientNetEvent.h"
#include "concurrent/events/ui/MediaUiEvent.h"

class DesktopConfigClientNetEvent;

class RemoteCameraDialog : public QDialog, public BaseAppDlg
{
Q_OBJECT

public:
	RemoteCameraDialog(qint64 clientId, QWidget* parent = Q_NULLPTR);
	~RemoteCameraDialog();
	void setClientInfo(const QSharedPointer<MediaClientNetEvent>& event);
	void setDisplayInfo(const QSharedPointer<CameraConfigClientNetEvent>& event) const;
signals:
	void windowClosed(qint64 client_id, UiWindow window);
	// void sendEventToMediator(const GuiEvent& event);
	void sendEventToUiMediatorPtr(const QSharedPointer<GuiEvent>& event);

private slots:

	void btnStartClicked();
	void btnStopClicked();
private:
	Ui::RemoteCameraDialog ui;
	// qint64 clientId;
};
