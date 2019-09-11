#pragma once

#include <QDialog>
#include "ui_RemoteDesktopDialog.h"
#include "GuiMediator.h"
#include "concurrent/events/net/MediaClientNetEvent.h"
#include "BaseAppDlg.h"

class DesktopConfigClientNetEvent;

class RemoteDesktopDialog : public QDialog, public BaseAppDlg
{
Q_OBJECT

public:
	RemoteDesktopDialog(qint64 clientId, QWidget* parent = Q_NULLPTR);
	~RemoteDesktopDialog();
	void setClientInfo(const QSharedPointer<MediaClientNetEvent>& event);
	void setDisplayInfo(const QSharedPointer<DesktopConfigClientNetEvent>& event);
signals:
	void windowClosed(qint64 client_id, UiWindow window);
	// void sendEventToMediator(const GuiEvent& event);
	void sendEventToUiMediatorPtr(const QSharedPointer<GuiEvent>& event);

private slots:
	
	void btnStartClicked();
	void btnStopClicked();
private:
	Ui::RemoteDesktopDialog ui;
	// qint64 clientId;
};
