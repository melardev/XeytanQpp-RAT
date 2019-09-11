#pragma once
#include <QObject>
#include "ui/qt/GuiMediator.h"

class BaseAppDlg
{
public:
	BaseAppDlg(qint64 clientId, UiWindow window);
	virtual ~BaseAppDlg();
	qint64 getClientId();;
	UiWindow getWindowType();;
protected:
	qint64 clientId;
	UiWindow windowType;
};
