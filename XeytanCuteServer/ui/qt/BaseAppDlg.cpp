#include "BaseAppDlg.h"


BaseAppDlg::~BaseAppDlg()
{
}

qint64 BaseAppDlg::getClientId()
{
	return clientId;
}

UiWindow BaseAppDlg::getWindowType()
{
	return windowType;
}

BaseAppDlg::BaseAppDlg(qint64 clientId, UiWindow window) : clientId(clientId), windowType(window)
{
}
