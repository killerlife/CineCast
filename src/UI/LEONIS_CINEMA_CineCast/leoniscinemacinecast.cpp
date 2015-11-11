#include "leoniscinemacinecast.h"
#include "ui_leoniscinemacinecast.h"
#include "UIControl/statuecontrol.h"
#include "UIControl/maintaincontrol.h"
#include "UIControl/contentmanagementcontrol.h"
#include "UIControl/setupcontrol.h"

#include <QDebug>

LEONISCINEMACineCast::LEONISCINEMACineCast(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LEONISCINEMACineCast)
{
    ui->setupUi(this);
    init();
}
void LEONISCINEMACineCast::init()
{
    this->setWindowState(Qt::WindowFullScreen);
    statueForm = NULL;
    maintainForm = NULL;
    setupForm = NULL;
    contentManagementForm = NULL;
    statueWid = NULL;
    maintainWind = NULL;
    setupWind = NULL;
    contentManagementWind = NULL;
    for(int i = 4;i > 0;i--)
    {
       on_tabWidget_tabBarClicked(i);
    }
    on_tabWidget_tabBarClicked(0);
    ui->label_logoBig->setPixmap(QPixmap("I:\\CINEMA_CineCast\\LEONIS_CINEMA_CineCast\\Image\\Leionis_logo.png"));
    ui->label_LOGO->setStyleSheet("QLabel{font-size: 26px; font-family:'Book Antique';color:white}");
    this->setStyleSheet("background-color:#336699");     //#0099CC偏暗 #3366CC偏亮
    ui->tabWidget->setTabShape(QTabWidget::Triangular);
    ui->tabWidget->tabBar()->setStyleSheet("QTabBar{font-size: 28px; font-family:'黑体';color:white;border-color:#333333;margin:32px}");
    ui->tabWidget->setCurrentIndex(0);

}
LEONISCINEMACineCast::~LEONISCINEMACineCast()
{
    delete ui;
    if(statueForm != NULL)
    {
        delete statueForm;
    }
    if(maintainForm != NULL)
    {
        delete maintainForm;
    }
    if(setupForm != NULL)
    {
        delete setupForm;
    }
    if(contentManagementForm != NULL)
    {
        delete contentManagementForm;
    }
}

void LEONISCINEMACineCast::on_tabWidget_tabBarClicked(int index)
{
    switch (index) {
    case 0:
        if(statueForm == NULL)
        {
            statueForm = new StatueForm();
            StatueControl *control = new StatueControl(statueForm);
        }
        if(statueWid  == NULL)
        {

            statueWid =  ui->mdiArea_statue->addSubWindow(statueForm);
            statueWid->setWindowFlags(Qt::FramelessWindowHint);
        }
        statueWid->showMaximized();
        statueWid->setStyleSheet("background-color:#cccccc");
        statueWid->show();
        break;
    case 1:
        if(maintainForm == NULL)
        {
            maintainForm = new MaintainForm();
            Maintaincontrol *control = new Maintaincontrol(maintainForm);
        }
        if(maintainWind == NULL)
        {
            maintainWind = ui->mdiArea_Maintain->addSubWindow(maintainForm);
            maintainWind->setWindowFlags(Qt::FramelessWindowHint);
        }
        maintainWind->showMaximized();
        maintainWind->setStyleSheet("background-color:#CCCCCC");
        maintainWind->show();
        break;
    case 2:
        if(contentManagementForm == NULL)
        {
            contentManagementForm = new ContentManagementForm();
            ContentmanagementControl *control = new ContentmanagementControl(contentManagementForm);
        }
        if(contentManagementWind == NULL)
        {
            contentManagementWind = ui->mdiArea_ContentManagement->addSubWindow(contentManagementForm);
            contentManagementWind->setWindowFlags(Qt::FramelessWindowHint);
        }
        contentManagementWind->showMaximized();
        contentManagementWind->setAutoFillBackground(true);
        contentManagementWind->setPalette(palette);//***添加子界面的界面背景
        contentManagementWind->show();

        contentManagementWind->setStyleSheet("background-color:#CCCCCC");
        break;

    case 3:
        if(setupForm == NULL)
        {
            setupForm = new SetupForm();
            SetupControl *control = new SetupControl(setupForm);
        }
        if(setupWind == NULL)
        {
            setupWind = ui->mdiArea_Setup->addSubWindow(setupForm);
            setupWind->setWindowFlags(Qt::FramelessWindowHint);
        }
        setupWind->showMaximized();
        setupWind->setStyleSheet("background-color:#CCCCCC");
        setupWind->show();
        break;
    }
}
