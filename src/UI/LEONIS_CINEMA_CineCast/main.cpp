#include "leoniscinemacinecast.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LEONISCINEMACineCast w;
    QApplication::setStyle("Fusion");
    w.show();
    return a.exec();
}
