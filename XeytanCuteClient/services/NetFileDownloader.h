#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetFileDownloader : public QObject
{
Q_OBJECT
public:
	NetFileDownloader(QUrl url, QObject* parent = Q_NULLPTR);
	void startDownload();
	void setUrl(QUrl url);
	QByteArray getFileData() const;
signals:
	void downloadFinished();
private slots:
	void fileDownloaded(QNetworkReply* reply);

private:
	QByteArray downloadedData;
	QNetworkAccessManager netAccessManager;
	QUrl url;
};
