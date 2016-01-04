#include <QtGui/QApplication>
#include "tmstest.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	tmstest w;
	w.show();
	return a.exec();
}
