#pragma once
#include "AbstractMediaService.h"

class DesktopMediaService : public AbstractMediaService
{
Q_OBJECT
public:
	DesktopMediaService();
	~DesktopMediaService();
	void getScreenShot(QPixmap& pixmap) const;
	virtual void takeAction() override;
	virtual void selectDefaultDevice() override;


signals:
	void screenConfigAvailable(const QList<ScreenInfo>& screens);

public slots:
	virtual void onConfigRequested() override;
};
