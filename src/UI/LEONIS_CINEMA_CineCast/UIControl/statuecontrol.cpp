#include "statuecontrol.h"

StatueControl::StatueControl(QObject *parent) :
    QObject(parent)
{
}

StatueControl::StatueControl(StatueForm *&statueForm)
{
    startTimer(500);
    this->statueform = statueForm;
}

void StatueControl::timerEvent(QTimerEvent *)
{
 this->statueform->setSignalQualityValue(qualityMax,qualityCurrent);
 this->statueform->setSignalStrengthValue(strengthMax,strengthCurrent);
 this->statueform->setFilmRevceiverValue(filmLength,receverLength);
 this->statueform->setFilmName(filmName);
 this->statueform->setReceiverStatue(receiveStatue);
 //测试数据
 setSignalQuality(100,23);
 setSignalStrength(120,33);
 setFilmReceiver(13000000000000000000,10000000000000000000);
 setFilmNane("leonisi cinema line");
 setReceiverStatue("Network start wlan! satellite network receive normality");
 //...
}
//=======================================
//  提供给外部的对状态界面输出数据赋值调用的函数
//=======================================
void  StatueControl::setSignalQuality(int maxvalue,int currentValue)
{
    this->qualityMax = maxvalue;
    this->qualityCurrent = currentValue;
}

void  StatueControl::setSignalStrength(int maxvalue,int currentValue)
{
    this->strengthMax = maxvalue;
    this->strengthCurrent = currentValue;
}

void  StatueControl::setFilmReceiver(quint64 filmlength,quint64 currentReceiver)
{
    this->filmLength = filmlength;
    this->receverLength = currentReceiver;
}

void  StatueControl::setFilmNane(QString filmName)
{
    this->filmName = filmName;
}

void StatueControl::setReceiverStatue(QString receiveStr)
{
    this->receiveStatue = receiveStr;
}
