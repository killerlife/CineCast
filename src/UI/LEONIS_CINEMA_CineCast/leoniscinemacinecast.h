#ifndef LEONISCINEMACINECAST_H
#define LEONISCINEMACINECAST_H

#include "statueform.h"
#include "maintainform.h"
#include "contentmanagementform.h"
#include "setupform.h"

#include <QMdiSubWindow>

#include <QWidget>

namespace Ui {
class LEONISCINEMACineCast;
}

class LEONISCINEMACineCast : public QWidget
{
    Q_OBJECT
private:
    //============================界面对象====================

    StatueForm *statueForm;
    MaintainForm *maintainForm;
    SetupForm *setupForm;
    ContentManagementForm *contentManagementForm;

    //=============================QMdiSubWindow对象==========

     QMdiSubWindow *statueWid;
     QMdiSubWindow *maintainWind;
     QMdiSubWindow *setupWind;
     QMdiSubWindow *contentManagementWind;
private:
     //===========================私有函数=====================
     void init();
     QPalette palette;

public:
    explicit LEONISCINEMACineCast(QWidget *parent = 0);
    ~LEONISCINEMACineCast();

private slots:
    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::LEONISCINEMACineCast *ui;
};

#endif // LEONISCINEMACINECAST_H
