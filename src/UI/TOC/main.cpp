#include <QtGui/QApplication>
#include <QTranslator>
#include "toc.h"

int main(int argc, char *argv[])
{
	QTranslator translator;
	QApplication a(argc, argv);
	translator.load("toc_zh.qm");
	a.installTranslator(&translator);
	TOC w;
	w.show();
	return a.exec();
}
