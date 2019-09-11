#include "Client.h"

Client::Client()
{
	cameraMediaState = Stopped;
	desktopMediaState = Stopped;
}

Client::Client(qint64 id): id(id)
{
	cameraMediaState = Stopped;
	desktopMediaState = Stopped;
}

Client::Client(qint64 uuid, const QString& OperatingSystem, const QString& pcName): id(uuid),
                                                                                    os(OperatingSystem),
                                                                                    pcName(pcName)
{
	cameraMediaState = Stopped;
	desktopMediaState = Stopped;
}

Client::~Client()
{
}

void Client::setOs(const QString& os)
{
	this->os = os;
}

void Client::setPcName(const QString& pcName)
{
	this->pcName = pcName;
}

QString Client::getPcName() const
{
	return pcName;
}

QString Client::getOs() const
{
	return os;
}

void Client::setId(qint64 clientId)
{
	this->id = clientId;
}

qint64 Client::getId() const
{
	return id;
}

bool Client::isStreamingWebcam() const
{
	return cameraMediaState != Stopped;
}

bool Client::isStreamingDesktop() const
{
	return desktopMediaState != Stopped;
}

bool Client::isClientChatting()
{
	return false;
}

void Client::setCameraStreamingState(MediaState state)
{
	cameraMediaState = state;
}

void Client::setDesktopStreamingState(MediaState state)
{
	desktopMediaState = state;
}
