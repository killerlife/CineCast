#include <QtGui/QApplication>
#include "batchid.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BatchID w;
	w.show();
	return a.exec();
}
