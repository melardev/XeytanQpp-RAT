#pragma once
#include <QObject>
#include <QApplication>
#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include <QHostInfo>
#include <QProcess>

enum class ChatAction : signed char;
enum class ShellAction : signed char;
class Packet;
enum class MediaAction : signed char;
struct CameraErrorDetails;
struct ScreenInfo;
struct CameraInfo;
class QDataStreamPacketProcessor;
class CameraMediaService;
class DesktopMediaService;

class NetService : public QObject
{
Q_OBJECT
public:
	NetService(QObject* parent = Q_NULLPTR);
	~NetService();
	void startService();

	void sendShellOutput(const QString& data);
	void sendShellStop();
	void handleChat(QDataStream& data_stream);

	void sendChatMessage(const QString& message);
	void chatClosed();
signals:
	void setDesktopConfig(const QList<QString>& displayNames, qint32 interval,
	                      qint32 width, qint32 height);
	void setDesktopServiceState(const MediaAction& action);
	void requestDesktopConfig();
	void requestCameraConfig();

	void setCameraConfig(const QList<QString>& cameraDeviceName, qint32 interval,
	                     qint32 width, qint32 height);
	void setCameraServiceState(const MediaAction& action);
	void requestShellReady(ShellAction action, const QString& data);
	void chatRequest(ChatAction action, const QString& message);

	void disconnected();

private slots:

	void onSocketConnected();
	void onSocketError(QAbstractSocket::SocketError error);
	void handleProcessList(const QDataStream& dataStream);
	void exitApplication();
	void handleConnection(QDataStream& data_stream);
	void onSocketReadyRead();
	void onSocketDisconnected();

	void handlePresentation(QDataStream& data_stream);

	void onScreenShotAvailable(const QPixmap& pixmap);
	void onScreenConfigAvailable(const QList<ScreenInfo>& screens);

	void onCamerasConfigAvailable(const QList<CameraInfo>& availableCameras);
	void onCameraPictureAvailable(const QPixmap& pixmap);
	void onCameraError(const CameraErrorDetails& error);

private:

	// QSharedPointer<DesktopHandler> initDesktopHandler();
	QScopedPointer<DesktopMediaService>& initDesktopService();
	// Incoming data handling methods
	void handleDesktop(QDataStream& readStream);
	void handleLogin(QDataStream& readStream);
	void handleShell(QDataStream& readStream);

	void handleCamera(QDataStream& readStream);
	QScopedPointer<CameraMediaService>& initCameraService();


	void sendPacket(const Packet& packet);

private:
	QTcpSocket* clientSocket;

	QScopedPointer<DesktopMediaService> desktopMediaService;
	QScopedPointer<QDataStreamPacketProcessor> packetProcessor;
	QScopedPointer<CameraMediaService> cameraMediaService;

	quint64 packetHeaderLength;
	quint64 packetSize;
	bool hasReadHeader;

	QMetaObject::Connection connectedConnection;
	QMetaObject::Connection disconnectionConnection;
	QMetaObject::Connection readConnection;
	QMetaObject::Connection errorConnection;
};
