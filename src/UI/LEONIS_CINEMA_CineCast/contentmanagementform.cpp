#include "contentmanagementform.h"
#include "ui_contentmanagementform.h"
#include <QBitmap>
#include <QDebug>
#include <QMessageBox>

ContentManagementForm::ContentManagementForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContentManagementForm)
{
    ui->setupUi(this);
    this->HDD_name = "/0";
    this->USB_name = "/0";
    ui->label_HDD->setPixmap(QPixmap(":./Image/HDD2.png"));
    ui->label_USB->setPixmap(QPixmap(":./Image/USBd.png"));
    QString QGroupBoxQSS = "QGroupBox {font-size:20px; border: 4px solid red;border-radius: 6px;background-color:#cccccc /*qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde)*/;min-width: 80px; border-color:#666666 }";//333333
    ui->groupBox_HDD_USB->setStyleSheet(QGroupBoxQSS);
    QString QTableViewQSS = "QTableView {border: 2px solid red;border-radius: 6px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde);min-width: 80px; border-color: #666666}";
    ui->tableView_HDD->setStyleSheet(QTableViewQSS);
    ui->tableView_USB->setStyleSheet(QTableViewQSS);
    HDDButtonInit();
    USBButtonInit();
    tableViewInit();
}

ContentManagementForm::~ContentManagementForm()
{
    delete ui;
}

void  ContentManagementForm::HDDButtonInit()
{
    connect(ui->pushButton_HddOutput,SIGNAL(pressed()),this,SLOT(updateOutputImage()));
    ui->pushButton_HddOutput->setIconSize(QSize(140,35));
    ui->pushButton_HddOutput->setGeometry(0,0,140,35);
    ui->pushButton_HddOutput->setIcon(QPixmap(":./Image/output.png"));
    connect(ui->pushButton_HddDelete,SIGNAL(pressed()),this,SLOT(updateDeleteImage()));
    ui->pushButton_HddDelete->setIconSize(QSize(140,35));
    ui->pushButton_HddDelete->setGeometry(0,0,140,35);
    ui->pushButton_HddDelete->setIcon(QPixmap(":./Image/Delete0.png"));
}
void ContentManagementForm::updateOutputImage()
{
    ui->pushButton_HddOutput->setIconSize(QSize(140,35));
    ui->pushButton_HddOutput->setGeometry(0,0,140,35);
    ui->pushButton_HddOutput->setIcon(QPixmap(":./Image/output1.png"));
}
void ContentManagementForm::updateDeleteImage()
{
    ui->pushButton_HddDelete->setIconSize(QSize(140,35));
    ui->pushButton_HddDelete->setGeometry(0,0,140,35);
    ui->pushButton_HddDelete->setIcon(QPixmap(":./Image/delete1.png"));
}
void  ContentManagementForm::USBButtonInit()
{
    connect(ui->pushButton_Mount,SIGNAL(pressed()),this,SLOT(updateMountImage()));
    ui->pushButton_Mount->setIconSize(QSize(140,35));
    ui->pushButton_Mount->setGeometry(0,0,140,35);
    ui->pushButton_Mount->setIcon(QPixmap(":./Image/Mount0.png"));
    connect(ui->pushButton_Uninstall,SIGNAL(pressed()),this,SLOT(updateUninstallImage()));
    ui->pushButton_Uninstall->setIconSize(QSize(140,35));
    ui->pushButton_Uninstall->setGeometry(0,0,140,35);
    ui->pushButton_Uninstall->setIcon(QPixmap(":./Image/Uninstall0.png"));
    connect(ui->pushButton_UsbDelete,SIGNAL(pressed()),this,SLOT(updateUsbDeleteImage()));
    ui->pushButton_UsbDelete->setIconSize(QSize(140,35));
    ui->pushButton_UsbDelete->setGeometry(0,0,140,35);
    ui->pushButton_UsbDelete->setIcon(QPixmap(":./Image/Delete0.png"));
}
void ContentManagementForm::updateMountImage()
{
    ui->pushButton_Mount->setIconSize(QSize(140,35));
    ui->pushButton_Mount->setGeometry(0,0,140,35);
    ui->pushButton_Mount->setIcon(QPixmap(":./Image/Mount1.png"));
}
void ContentManagementForm::updateUninstallImage()
{
    ui->pushButton_Uninstall->setIconSize(QSize(140,35));
    ui->pushButton_Uninstall->setGeometry(0,0,140,35);
    ui->pushButton_Uninstall->setIcon(QPixmap(":./Image/Uninstall1.png"));
}
void ContentManagementForm::updateUsbDeleteImage()
{
    ui->pushButton_UsbDelete->setIconSize(QSize(140,35));
    ui->pushButton_UsbDelete->setGeometry(0,0,140,35);
    ui->pushButton_UsbDelete->setIcon(QPixmap(":./Image/delete1.png"));
}
void ContentManagementForm::tableViewInit()
{
      modelSize = 2;
      HDDModel = new QStandardItemModel;
      USBModel = new QStandardItemModel;
      HDDModel->setColumnCount(2);
      USBModel->setColumnCount(2);
      HDDModel->setHeaderData(0,Qt::Horizontal,"HHD_ID");
      HDDModel->setHeaderData(1,Qt::Horizontal,"HDD_name");
      USBModel->setHeaderData(0,Qt::Horizontal,"USB_ID");
      USBModel->setHeaderData(1,Qt::Horizontal,"USB_name");
      QStringList liststr;
      liststr <<"No data";
      addHDD(liststr);
      addUSB(liststr);
}
void ContentManagementForm::addHDD(QStringList HDDlist)
{
    int row = HDDlist.length();
    HDDModel->setRowCount(row);
    for(int i = 0;i < row;i++)
    {
       QString namestr = HDDlist.at(i);
       QStandardItem *itemID = new QStandardItem(QString::number(i+1));
       HDDModel->setItem(i,0,itemID);
       QStandardItem* itemName = new QStandardItem(namestr);
       HDDModel->setItem(i,1,itemName);
    }
    ui->tableView_HDD->setModel(HDDModel);
    ui->tableView_HDD->verticalHeader()->hide();
    ui->tableView_HDD->setColumnWidth(0,80);
    ui->tableView_HDD->setColumnWidth(1,625);
    ui->tableView_HDD->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_USB->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void ContentManagementForm::addUSB(QStringList USBlist)
{
    int row = USBlist.length();
    USBModel->setRowCount(row);
    for(int j = 0;j < row;j++)
    {
        QString namestr = USBlist.at(j);
        QStandardItem *itemID = new QStandardItem(QString::number(j+1));
        USBModel->setItem(j,0,itemID);
        QStandardItem*itemName = new QStandardItem(namestr);
        USBModel->setItem(j,1,itemName);
    }
    ui->tableView_USB->setModel(USBModel);
    ui->tableView_USB->verticalHeader()->hide();
    ui->tableView_USB->setColumnWidth(0,80);
    ui->tableView_USB->setColumnWidth(1,625);
    ui->tableView_USB->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_HDD->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ContentManagementForm::on_tableView_HDD_clicked(const QModelIndex &index)
{
    QString  hddstr = ui->tableView_HDD->currentIndex().data().toString();
    this->HDD_name = hddstr;
}

void ContentManagementForm::on_tableView_USB_clicked(const QModelIndex &index)
{
    QString  usbstr = ui->tableView_USB->currentIndex().data().toString();
    this->USB_name = usbstr;
}
void ContentManagementForm::setHDD_name()
{
    this->HDD_name = "/0";
}
void ContentManagementForm::setUsb_name()
{
    this->USB_name = "/0";
}
QString ContentManagementForm::getHDD_name()
{
    if(this->HDD_name == "/0")
    {
        QMessageBox::warning(this,"提示","请选择要操作的HDD设备");
        return "/0";
    }
   return this->HDD_name;
}
QString ContentManagementForm::getUsb_name()
{
    if(this->USB_name == "/0")
    {
        QMessageBox::warning(this,"提示","请选择要操作的USB设备");
        return "/0";
    }
    return this->USB_name;
}

void ContentManagementForm::on_pushButton_HddOutput_clicked()
{
    ui->pushButton_HddOutput->setIconSize(QSize(140,35));
    ui->pushButton_HddOutput->setGeometry(0,0,140,35);
    ui->pushButton_HddOutput->setIcon(QPixmap(":./Image/output.png"));
    emit outputHDD();
}

void ContentManagementForm::on_pushButton_HddDelete_clicked()
{
    ui->pushButton_HddDelete->setIconSize(QSize(140,35));
    ui->pushButton_HddDelete->setGeometry(0,0,140,35);
    ui->pushButton_HddDelete->setIcon(QPixmap(":./Image/Delete0.png"));
    emit deleteHDD();
}

void ContentManagementForm::on_pushButton_Mount_clicked()
{
    ui->pushButton_Mount->setIconSize(QSize(140,35));
    ui->pushButton_Mount->setGeometry(0,0,140,35);
    ui->pushButton_Mount->setIcon(QPixmap(":./Image/Mount0.png"));
    emit mountUSB();
}

void ContentManagementForm::on_pushButton_Uninstall_clicked()
{
    ui->pushButton_Uninstall->setIconSize(QSize(140,35));
    ui->pushButton_Uninstall->setGeometry(0,0,140,35);
    ui->pushButton_Uninstall->setIcon(QPixmap(":./Image/Uninstall0.png"));
    emit uninstallUSB();
}

void ContentManagementForm::on_pushButton_UsbDelete_clicked()
{
    ui->pushButton_UsbDelete->setIconSize(QSize(140,35));
    ui->pushButton_UsbDelete->setGeometry(0,0,140,35);
    ui->pushButton_UsbDelete->setIcon(QPixmap(":./Image/Delete0.png"));
    emit deleteUSB();
}
