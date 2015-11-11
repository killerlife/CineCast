#ifndef CONTENTMANAGEMENTCONTROL_H
#define CONTENTMANAGEMENTCONTROL_H

#include <QObject>
#include <QStringList>
#include "contentmanagementform.h"

class ContentmanagementControl : public QObject
{
    Q_OBJECT
public:
    explicit ContentmanagementControl(QObject *parent = 0);
    ContentmanagementControl(ContentManagementForm *&form);
    void setHDD(QStringList hddlist);
    void setUSB(QStringList usblist);
private:
    void timerEvent(QTimerEvent *);
    void showHDD(QStringList hddlist);
    void showUSB(QStringList usblist);
    ContentManagementForm * contentManagementForm;
    QStringList HDDlist;
    QStringList USBlist;
    QStringList hddlist;
    QStringList usblist;
    QString Hddname;
    QString usbName;
signals:

private slots:
    void output_HDD();
    void delete_HDD();
    void mount_USB();
    void uninstall_USB();
    void delete_USB();

};

#endif // CONTENTMANAGEMENTCONTROL_H
