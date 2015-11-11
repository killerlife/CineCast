#ifndef MAINTAINCONTROL_H
#define MAINTAINCONTROL_H

#include <QObject>

#include "maintainform.h"

class Maintaincontrol : public QObject
{
    Q_OBJECT
public:
    explicit Maintaincontrol(QObject *parent = 0);
    Maintaincontrol(MaintainForm *&maintainForm);
    void control(int maxvalue,int progressvalue);
    void setProgressVlues(int maxvalue,int progressvalue);
private:
    MaintainForm *maintainform;
    bool clickflage;
    int maxvalue;
    int value;
    void timerEvent(QTimerEvent *);
signals:

public slots:
private slots:
   void  formatControl();

};

#endif // MAINTAINCONTROL_H
