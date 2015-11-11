#include "maintaincontrol.h"
#include <QMessageBox>

Maintaincontrol::Maintaincontrol(QObject *parent) :
    QObject(parent)
{
}
Maintaincontrol::Maintaincontrol(MaintainForm *&maintainForm)
{
    clickflage = false;
    maxvalue = 100;
    value = 0;
    startTimer(500);
    this->maintainform = maintainForm;
    connect(this->maintainform,SIGNAL(formatsignal()),this,SLOT(formatControl()));
}
void Maintaincontrol::control(int maxvalue = 100,int progressvalue = 0)//修改
{
    maintainform->setProgressBarValue(maxvalue,progressvalue);
}
void Maintaincontrol::timerEvent(QTimerEvent *) //改变格式化参数
{
    if(clickflage == true)
    {
        control(this->maxvalue,this->value);
    }
    else
    {
        control();
    }
    if((this->maxvalue == this->value )&&(this->maxvalue != 0))
    {
        QMessageBox::warning(maintainform,"温馨提示","格式化完毕");
        clickflage = false;
    }
}
//===========================
//  提供给外部的对维护界面数据输出赋值调用函数
//===========================
void Maintaincontrol::setProgressVlues(int maxvalue,int progressvalue)
{
    this->maxvalue = maxvalue;
    this->value = progressvalue;
}
//===============================
//  操作格式化的函数
//===============================
void Maintaincontrol::formatControl()
{
    clickflage = true;
    QMessageBox message(QMessageBox::NoIcon,"提示！","确定格式化吗？", QMessageBox::Yes | QMessageBox::No, NULL);
    if(message.exec() == QMessageBox::Yes)
    {
      //调用外部格式化
    }
    else
    {
        return ;
    }

}
