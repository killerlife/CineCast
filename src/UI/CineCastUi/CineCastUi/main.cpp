#include <QtGui/QApplication>
#include <QTranslator>
#include <QDesktopWidget>
#include "cinecastui.h"

QString rIp;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator translator;
	switch(argc)
	{
	case 1:
		rIp = "127.0.0.1";
		translator.load("cinecastui_zh.qm");
		break;
	case 2:
		rIp = argv[1];
		translator.load("cinecastui_zh.qm");
		break;
	case 3:
		rIp = argv[1];
		translator.load(argv[2]);
		break;
	}
	a.installTranslator(&translator);
	CineCastUi w;
	QApplication::setStyle("plastique");
	w.show();
	return a.exec();
}
