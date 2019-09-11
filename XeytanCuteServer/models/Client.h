#pragma once

#include "shared/media/media_defs.h"

class Client
{
public:
	Client();
	Client(qint64 id);
	Client(qint64 id, const QString& OperatingSystem, const QString& pcName);
	
	~Client();
	void setOs(const QString& os);
	void setPcName(const QString& pcName);
	QString getPcName() const;
	QString getOs() const;
	
	void setId(qint64 clientId);
	qint64 getId() const;
	bool isStreamingWebcam() const;
	bool isStreamingDesktop() const;
	bool isClientChatting();
	void setCameraStreamingState(MediaState state);
	void setDesktopStreamingState(MediaState state);

private:
	QString pcName;
	QString os;
	qint64 id;
	MediaState cameraMediaState;
	MediaState desktopMediaState;
};
