#include "XeytanQppClient.h"
#include "shared/media/media_defs.h"


namespace Test
{
	void test()
	{
	
	}
}

int main(int argc, char* argv[])
{
	// QApplication a(argc, argv);
	Test::test();

	/*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    XeytanQppClient app;
    app.startService();
    return a.exec();
	*/


	XeytanQppClient w;
	qRegisterMetaType<MediaAction>("MediaAction");
	qRegisterMetaType<QList<ScreenInfo>>("QList<ScreenInfo>");
	qRegisterMetaType<QList<CameraInfo>>("QList<CameraInfo>");
	qRegisterMetaType<QList<QString>>("QList<QString>");
	return w.run(argc, argv);
}
