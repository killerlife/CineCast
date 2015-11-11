#ifndef SETUPCONTROL_H
#define SETUPCONTROL_H

#include <QObject>

#include "setupform.h"

class SetupControl : public QObject
{
    Q_OBJECT
    
private:

    SetupForm *setupForm;
    QString frequency;
    QString symbolrate;
    QString polar;
    QString SignalSystems;
    QString FEC;
    QString rolloff;
    QString Modulation;
    QString ECH;
    bool DHCP;
    QString ip;
    QString NetmAsk;
    QString Grateway;
    QString DNS;
    QString Remote;
    QString frequency2;
    QString symbolrate2;
    QString polar2;
    QString SignalSystems2;
    QString FEC2;
    QString rolloff2;
    QString Modulation2;
    QString ECH2;
    bool DHCP2;
    QString ip2;
    QString NetmAsk2;
    QString Grateway2;
    QString DNS2;
    QString Remote2;
    bool DHCPChecked;
    bool DHCPNoChecked;
private:

    void timerEvent(QTimerEvent *);
    void setupcontrol();
public:

    explicit SetupControl(QObject *parent = 0);
    SetupControl(SetupForm *&form);
    bool  setfrequency();
    bool  setsymbolrate();
    bool  setpolar();
    bool  setSignalSystems();
    bool  setFEC();
    bool  setrolloff();
    bool  setModulation();
    bool  setECH();
    bool  setDHCP();
    bool  setip();
    bool  setNetmAsk();
    bool  setGrateway();
    bool  setDNS();
    bool  setRemote();

signals:

public slots:

};

#endif // SETUPCONTROL_H
