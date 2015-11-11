#include "maintainform.h"
#include "ui_maintainform.h"
//#include "UiFile/nypushbutton.h"
#include <QDebug>

MaintainForm::MaintainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaintainForm)
{
    ui->setupUi(this);
    QString ProgressBarQSS = "QProgressBar {border: 2px solid red;border-radius: 6px;background-color: #666666 /*qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde);min-width: 80px*/; border-color: #9999FF}";
    ui->progressBar->setStyleSheet(ProgressBarQSS);
    QString QGroupBoxQSS = "QGroupBox {font-size:20px;font-family:'宋体'; border: 4px solid red;border-radius: 6px;background-color:#cccccc /*qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde)*/;min-width: 80px; border-color: #666666}";
    ui->groupBox->setStyleSheet(QGroupBoxQSS);
    ButtonInit();

}

MaintainForm::~MaintainForm()
{
    delete ui;
}
void MaintainForm::ButtonInit()
{
    connect(ui->pushButton_format,SIGNAL(pressed()),this,SLOT(formatsignalupdateImage()));
    ui->pushButton_format->setIconSize(QSize(140,35));
    ui->pushButton_format->setGeometry(0,0,140,35);
    ui->pushButton_format->setIcon(QPixmap(":./Image/format0.png"));

}

void MaintainForm::setProgressBarValue(int maxvalue,int value)
{

    ui->progressBar->setMaximum(maxvalue);
    ui->progressBar->setValue(value);
}


void MaintainForm::on_pushButton_format_clicked()
{
    ui->pushButton_format->setIconSize(QSize(140,35));
    ui->pushButton_format->setGeometry(0,0,140,35);
    ui->pushButton_format->setIcon(QPixmap(":./Image/format0.png"));
    emit formatsignal();
}
void MaintainForm::formatsignalupdateImage()
{
    ui->pushButton_format->setIconSize(QSize(140,35));
    ui->pushButton_format->setGeometry(0,0,140,35);
    ui->pushButton_format->setIcon(QPixmap(":./Image/format1.png"));
}
