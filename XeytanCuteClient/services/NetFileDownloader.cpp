#include "NetFileDownloader.h"
#include <utility>

NetFileDownloader::NetFileDownloader(QUrl fileUrl, QObject* parent)
	: QObject(parent), url(std::move(fileUrl))
{
	connect(&netAccessManager, &QNetworkAccessManager::finished, this, &NetFileDownloader::fileDownloaded);
}

void NetFileDownloader::startDownload()
{
	const QNetworkRequest request(url);
	netAccessManager.get(request);
}

void NetFileDownloader::setUrl(QUrl url)
{
	if (this->url != url)
		this->url = url;
}

QByteArray NetFileDownloader::getFileData() const
{
	return downloadedData;
}

void NetFileDownloader::fileDownloaded(QNetworkReply* reply)
{
	downloadedData = reply->readAll();
	reply->deleteLater();
	emit downloadFinished();
}
