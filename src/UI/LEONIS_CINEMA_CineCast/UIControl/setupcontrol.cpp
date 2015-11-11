#include "setupcontrol.h"
#include <QDebug>
SetupControl::SetupControl(QObject *parent) :
    QObject(parent)
{
}

SetupControl::SetupControl(SetupForm *&form)
{
    this->setupForm = form;
    DHCPChecked = false;
    DHCPNoChecked = false;
    this->frequency = "/0";
    this->symbolrate = "/0";
    this->polar = "/0";
    this->SignalSystems = "/0";
    this->FEC = "/0";
    this->rolloff = "/0";
    this->Modulation = "/0";
    this->ECH = "/0";
    this->DHCP = false;
    this->ip = "/0";
    this->NetmAsk = "/0";
    this->Grateway = "/0";
    this->DNS = "/0";
    this->Remote = "/0";

    this->frequency2 = "/0";
    this->symbolrate2 = "/0";
    this->polar2 = "/0";
    this->SignalSystems2 = "/0";
    this->FEC2 = "/0";
    this->rolloff2 = "/0";
    this->Modulation2 = "/0";
    this->ECH2 = "/0";
    this->DHCP2 = false;
    this->ip2 = "/0";
    this->NetmAsk2 = "/0";
    this->Grateway2 = "/0";
    this->DNS2 = "/0";
    this->Remote2 = "/0";
    startTimer(500);
}

void SetupControl::timerEvent(QTimerEvent *)
{
    this->frequency = setupForm->getFrequency();
    this->symbolrate = setupForm->getSymbolrate();
    this->polar = setupForm->getPolar();
    this->SignalSystems = setupForm->getSignalSystems();
    this->FEC = setupForm->getFEC();
    this->rolloff = setupForm->getRolloff();
    this->Modulation = setupForm->getModulation();
    this->ECH = setupForm->getECH();
    this->DHCP = setupForm->getDHCP();
    this->ip = setupForm->getIp();
    this->NetmAsk = setupForm->getNetmAsk();
    this->Grateway = setupForm->getGrateway();
    this->DNS = setupForm->getDNS();
    this->Remote = setupForm->getRemote();
    setupcontrol();
}

void SetupControl::setupcontrol()
{

 if(frequency != frequency2)
 {
     bool falg = setfrequency();
     if(falg == true)
     {
         frequency2 = frequency;
     }
 }
 if(symbolrate != symbolrate2)
 {
     bool falg = setsymbolrate();
     if(falg == true)
     {
         symbolrate2 = symbolrate;
     }
 }
 if(polar != polar2)
 {
     bool falg = setpolar();
     if(falg == true)
     {
         polar2 = polar;
     }
 }
 if(SignalSystems != SignalSystems2)
 {
     bool falg = setSignalSystems();
     if(falg == true)
     {
         SignalSystems2 = SignalSystems;
     }
 }
 if(FEC != FEC2)
 {
     bool falg = setFEC();
     if(falg == true)
     {
         FEC2 = FEC;
     }
 }
 if(rolloff != rolloff2)
 {
     bool falg = setrolloff();
     if(falg == true)
     {
         rolloff2 = rolloff;
     }
 }
 if(Modulation != Modulation2)
 {
     bool falg = setModulation();
     if(falg == true)
     {
         Modulation2 = Modulation;
     }
 }
 if(ECH != ECH)
 {
     bool falg = setECH();
     if(falg == true)
     {
         ECH2 = ECH;
     }
 }
 if(DHCP != DHCP2)
 {
    bool falg = setDHCP();
     if(falg == true)
     {
         DHCP2 = DHCP;
     }
 }

 if(ip != ip2)
 {
   bool falg = setip();
   if(falg == true)
   {
       ip2 = ip;
   }
 }
 if(NetmAsk != NetmAsk2)
 {
   bool falg = setNetmAsk();
   if(falg == true)
   {
       NetmAsk2 = NetmAsk;
   }
 }
 if(Grateway != Grateway2)
 {
     bool falg = setGrateway();
     if(falg == true)
     {
         Grateway2 = Grateway;
     }
 }
 if(DNS != DNS2)
 {
   bool falg = setDNS();
   if(falg == true)
   {
      DNS2 = DNS;
   }
 }
 if(Remote != Remote2)
 {
    bool falg = setRemote();
    if(falg == true)
    {
        Remote2 = Remote;
    }
 }
}

//==================
//   调用外部函数部分
//==================
bool SetupControl::setfrequency()
{
    //调用外部函数示例
 //return  外部函数名 (this->frequency);
	return true;
}
bool SetupControl::setsymbolrate()
{
    //调用外部函数示例
   //return  外部函数名 (this->symbolrate);
	return true;
}
bool SetupControl::setpolar()
{
    //调用外部函数示例
   //return  外部函数名 (this->polar);
	return true;
}
bool SetupControl::setSignalSystems()
{
    //调用外部函数示例
   //return  外部函数名 (this->SignalSystems);
	return true;
}
bool SetupControl::setFEC()
{
    //调用外部函数示例
   //return  外部函数名 (this->FEC);
	return true;
}
bool SetupControl::setrolloff()
{
    //调用外部函数示例
   //return  外部函数名 (this->rolloff);
	return true;
}
bool SetupControl::setModulation()
{
    //调用外部函数示例
   //return  外部函数名 (this->Modulation);
	return true;
}
bool SetupControl::setECH()
{
    //调用外部函数示例
   //return  外部函数名 (this->ECH);
	return true;
}

bool SetupControl::setDHCP()
{

     if(DHCP == true)//打开DHCP
        {
            //调用外部函数
           //return  外部函数名 (this->DHCP); DHCP = 1 为选择DHCP ;DHCP =0 为不选择DHCP
        }
        else          //关闭DHCP
        {
         //return  外部函数名 (this->DHCP); DHCP = 1 为选择DHCP ;DHCP =0 为不选择DHCP
        }
    return true;
}
bool SetupControl::setip()
{
    //调用外部函数示例
   //return  外部函数名 (this->ip);
	return true;
}
bool SetupControl::setNetmAsk()
{
    //调用外部函数示例
   //return  外部函数名 (this->NetmAsk);
	return true;
}

bool SetupControl::setGrateway()
{
    //调用外部函数示例
   //return  外部函数名 (this->Grateway);
    qDebug()<<this->Grateway;
    return false;
}
bool SetupControl::setDNS()
{
    //调用外部函数示例
   //return  外部函数名 (this->DNS);
    qDebug()<<this->DNS;
    return false;
}
bool  SetupControl::setRemote()
{
    //调用外部函数示例
   //return  外部函数名 (this->Remote);
    qDebug()<<this->Remote;
    return false;
}
