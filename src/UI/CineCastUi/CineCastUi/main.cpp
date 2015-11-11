#include <QtGui/QApplication>
#include "cinecastui.h"

QString rIp;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	rIp = "169.254.1.230";
	CineCastUi w;
	QApplication::setStyle("plastique");
	w.show();
	return a.exec();
}
