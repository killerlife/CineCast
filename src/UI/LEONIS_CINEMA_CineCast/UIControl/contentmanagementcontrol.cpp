#include "contentmanagementcontrol.h"
#include <QMessageBox>
ContentmanagementControl::ContentmanagementControl(QObject *parent) :
    QObject(parent)
{

}
ContentmanagementControl::ContentmanagementControl(ContentManagementForm *&form)
{
    this->contentManagementForm = form;
    startTimer(500);
    connect(this->contentManagementForm,SIGNAL(deleteHDD()),this,SLOT(delete_HDD()));
    connect(this->contentManagementForm,SIGNAL(deleteUSB()),this,SLOT(delete_USB()));
    connect(this->contentManagementForm,SIGNAL(outputHDD()),this,SLOT(output_HDD()));
    connect(this->contentManagementForm,SIGNAL(mountUSB()),this,SLOT(mount_USB()));
    connect(this->contentManagementForm,SIGNAL(uninstallUSB()),this,SLOT(uninstall_USB()));
}
void ContentmanagementControl::timerEvent(QTimerEvent *)
{
    showHDD(hddlist);
    showUSB(usblist);
}
void ContentmanagementControl::showHDD(QStringList hddlist)
{
    if(this->HDDlist != hddlist)
    {
      this->contentManagementForm->addHDD(hddlist);
      HDDlist = hddlist;
    }
    return ;
}
void ContentmanagementControl::showUSB(QStringList usblist)
{
   if(this->USBlist != usblist)
   {
     this->contentManagementForm->addUSB(usblist);
     USBlist = usblist;
   }
   return ;
}

//===============================
//      提供给外部的对管理界面的数据输入赋值 调用函数
//===============================
void ContentmanagementControl::setHDD(QStringList hddlist)
{
    this->hddlist = hddlist;
}
void ContentmanagementControl::setUSB(QStringList usblist)
{
    this->usblist =  usblist;
}
//===============================
//      提供给外部的对管理界面的数据操作 调用函数
//===============================
void ContentmanagementControl::output_HDD()
{
    this->Hddname = this->contentManagementForm->getHDD_name();
    if(this->Hddname == "/0"){return;}
    //****底层调用  导出（this->Hddname）
    this->contentManagementForm->setHDD_name();//置空操作变量
}
void ContentmanagementControl::delete_HDD()
{
    this->Hddname = this->contentManagementForm->getHDD_name();
    if(this->Hddname == "/0"){return;}
     //****底层调用  删除（this->Hddname）
    this->contentManagementForm->setHDD_name();//置空操作变量
}
void ContentmanagementControl::mount_USB()
{
    this->usbName = this->contentManagementForm->getUsb_name();
    if(this->usbName == "/0"){return ;}
      //****底层调用
    this->contentManagementForm->setUsb_name();//置空操作变量
}
void ContentmanagementControl::uninstall_USB()
{
    this->usbName = this->contentManagementForm->getUsb_name();
    if(this->usbName == "/0"){return ;}
     //****底层调用
    this->contentManagementForm->setUsb_name();//置空操作变量
}
void ContentmanagementControl::delete_USB()
{
    this->usbName = this->contentManagementForm->getUsb_name();
    if(this->usbName == "/0"){return ;}
     //****底层调用 删除（this->usbName）
    this->contentManagementForm->setUsb_name();//置空操作变量
}
