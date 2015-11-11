#ifndef CONTENTMANAGEMENTFORM_H
#define CONTENTMANAGEMENTFORM_H

#include <QWidget>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
namespace Ui {
class ContentManagementForm;
}

class ContentManagementForm : public QWidget
{
    Q_OBJECT

public:
    explicit ContentManagementForm(QWidget *parent = 0);
    ~ContentManagementForm();
private:
    QStandardItemModel *HDDModel;
    QStandardItemModel *USBModel;
    void  HDDButtonInit();
    void  USBButtonInit();
    void  tableViewInit();
    int modelSize;
    QString HDD_name;
    QString USB_name;

public:
    void  addHDD(QStringList HDDlist);
    void  addUSB(QStringList USBlist);
    QString getUsb_name();
    QString getHDD_name();
    void setHDD_name();
    void setUsb_name();

private slots:

    void on_tableView_HDD_clicked(const QModelIndex &index);
    void on_tableView_USB_clicked(const QModelIndex &index);
    void updateOutputImage();
    void updateDeleteImage();
    void updateMountImage();
    void updateUninstallImage();
    void updateUsbDeleteImage();
    void on_pushButton_HddOutput_clicked();

    void on_pushButton_HddDelete_clicked();

    void on_pushButton_Mount_clicked();

    void on_pushButton_Uninstall_clicked();

    void on_pushButton_UsbDelete_clicked();

private:
    Ui::ContentManagementForm *ui;
signals:
    void outputHDD();
    void deleteHDD();
    void mountUSB();
    void uninstallUSB();
    void deleteUSB();
};

#endif // CONTENTMANAGEMENTFORM_H
