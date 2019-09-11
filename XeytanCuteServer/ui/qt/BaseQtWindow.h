#pragma once
#include <QObject>

#include "../concurrent/events/ui/GuiEvent.h"

class BaseQtWindow : public QObject
{
Q_OBJECT
public:
	BaseQtWindow();
	~BaseQtWindow();

signals:
	void sendToUiMediator(const GuiEvent& event);
};
