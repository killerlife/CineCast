#ifndef STATUECONTROL_H
#define STATUECONTROL_H

#include <QObject>

#include "statueform.h"

class StatueControl : public QObject
{
    Q_OBJECT

private:

    StatueForm *statueform;
    int qualityMax;
    int qualityCurrent;
    int strengthMax;
    int strengthCurrent;
    quint64 filmLength;
    quint64 receverLength;
    QString filmName;
    QString receiveStatue;
public:
    explicit StatueControl(QObject *parent = 0);
    StatueControl(StatueForm *&statueForm);
    void timerEvent(QTimerEvent *); //改变格式化参数
    void setSignalQuality(int maxvalue = 100,int currentValue = 0);
    void setSignalStrength(int maxvalue = 100,int currentValue = 0);
    void setFilmReceiver(quint64 filmlength = 1000,quint64 currentReceiver = 0);
    void setFilmNane(QString filmName = "No Film Name");
    void setReceiverStatue(QString receiveSttr = "No Statue");
signals:

public slots:

};

#endif // STATUECONTROL_H
