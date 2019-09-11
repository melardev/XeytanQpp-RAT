#pragma once

#include <QMainWindow>
#include "ui_MainGui.h"
#include "BaseQtWindow.h"

class Client;

class MainGui : public QMainWindow
{
Q_OBJECT

public:
	MainGui(QWidget* parent = Q_NULLPTR);
	~MainGui();
	void addClient(const Client& client);
	void addClientPtr(const QSharedPointer<Client>& client);
	void removeClient(const Client& client);
	void removeClient(qint64 clientId);

	void setCountryFlag(QTreeWidgetItem* root, QString& country);
signals:
	void sendEventToUiMediator(const GuiEvent& event);
	void sendEventToUiMediatorPtr(const QSharedPointer<GuiEvent>& event);
public slots:
	void onContextMenuRequested(const QPoint& point);
	void onMenuTriggered(QAction* action);
private:
	Ui::MainGui ui;
	QMap<qint64, QSharedPointer<Client>> clients;
	QTreeWidgetItem* currentSelected;
	QAction* actionShell;
	QAction* actionDownload;
	QAction* actionDownloadAndExecute;
	QAction* actionChat;
	QAction* actionClose;
	QAction* actionUninstall;
	QAction* getInfoExtended;
	QAction* actionCam;
};
