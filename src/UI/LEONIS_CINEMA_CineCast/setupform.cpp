#include "setupform.h"
#include "ui_setupform.h"
#include <QDebug>
SetupForm::SetupForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupForm)
{
    ui->setupUi(this);
    checkedfalg = false;
    listDvbs<<"DVB-S"<<"DVB-S2";
    listrates<<"None"<<"1/2"<<"2/3"<<"3/4"<<"4/5"<<"5/6"<<"6/7"<<"7/8"<<"8/9"<<"Auto";
    listdecimals<<"0.20"<<"0.25"<<"0.35";
    listSpks<<"QAM16"<<"QAM32"<<"QAM64"<<"QAM128"<<"QPSK"<<"8PSK"<<"16APSK"<<"VSB8"<<"VSB16"<<"QAMAUTO";
    listSpkECHOS<<"ETH0"<<"ETH1"<<"ETH2"<<"ETH3";
    listV<<"H"<<"V";
    init();
}

SetupForm::~SetupForm()
{
    delete ui;
}
void SetupForm::init()
{
    QString QLabelQSS = "QLabel {border: 2px solid red;border-radius: 6px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde);min-width: 80px; border-color: #9999FF}";
    QString QGroupBoxQSS = "QGroupBox {font-size:20px; border: 4px solid red;border-radius: 6px;background-color:#cccccc /*qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde)*/;min-width: 80px; border-color:#666666}";
    QString lineEditQSS = "QLineEdit {border: 2px solid red; font-size:16px; border-radius: 6px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde);min-width: 80px; border-color: #666666}";
    QString lineEditQSSENGSH = "QLineEdit {border: 2px solid red; font-size:16px;font-family:'Book Antique'; border-radius: 6px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde);min-width: 80px; border-color: #666666}";
    QString QComboBoxQSS = "QComboBox {border: 2px solid red; font-size:16px; border-radius: 6px;background-color: #cccccc/*qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde)*/;min-width: 120px; border-color: #666666}";

    ui->groupBox_staeliite->setStyleSheet(QGroupBoxQSS);
    ui->lineEdit_Frequency->setStyleSheet(lineEditQSS);
    ui->lineEdit_symbol_rate->setStyleSheet(lineEditQSS);
    ui->groupBox_Network->setStyleSheet(QGroupBoxQSS);
    ui->lineEdit_Ip->setStyleSheet(lineEditQSSENGSH);
    ui->lineEdit_GateWay->setStyleSheet(lineEditQSSENGSH);
    ui->lineEdit_Remote->setStyleSheet(lineEditQSSENGSH);
    ui->lineEdit_Net_AsK->setStyleSheet(lineEditQSSENGSH);
    ui->lineEdit_DNS->setStyleSheet(lineEditQSSENGSH);
    QString fontQSSCHINA = "font-size: 20px;";
    QString fontQSSENG = "font-size: 20px;font-family:'Book Antique'";
    ui->label_DNS->setStyleSheet(fontQSSENG);
    ui->label_frequency->setStyleSheet(fontQSSCHINA);
    ui->label_Gateway->setStyleSheet(fontQSSENG);
    ui->label_ip->setStyleSheet(fontQSSENG);
    ui->label_Netm_mask->setStyleSheet(fontQSSENG);
    ui->label_Remote->setStyleSheet(fontQSSENG);
    ui->label_3->setStyleSheet(fontQSSENG);
    ui->label_frequency_2->setStyleSheet(fontQSSCHINA);
    ui->labe_symbl_rate_4->setStyleSheet(fontQSSENG);
    ui->labe_symbl_rate_5->setStyleSheet(fontQSSCHINA);
    ui->labe_symbl_rate_6->setStyleSheet(fontQSSENG);
    ui->labe_symbl_rate_8->setStyleSheet(fontQSSCHINA);
    ui->labe_symbl_rate->setStyleSheet(fontQSSCHINA);
    QString CheckBoxQSS = "QCheckBox {border: 2px solid red;font-family:Book Antique; font-size:16px; border-radius: 6px;min-width: 80px; border-color: #666666}";
    ui->checkBox_DHCP->setStyleSheet(CheckBoxQSS);

    ui->comboBox_decimals->setStyleSheet(QComboBoxQSS);
    ui->comboBox_DVB->setStyleSheet(QComboBoxQSS);
    ui->comboBox_ECHs->setStyleSheet(QComboBoxQSS);
    ui->comboBox_SPK->setStyleSheet(QComboBoxQSS);
    ui->comboBox_V->setStyleSheet(QComboBoxQSS);
    ui->comboBox_xy->setStyleSheet(QComboBoxQSS);

    ui->comboBox_decimals->addItems(listdecimals);
    ui->comboBox_DVB->addItems(listDvbs);
    ui->comboBox_ECHs->addItems(listSpkECHOS);
    ui->comboBox_SPK->addItems(listSpks);
    ui->comboBox_V->addItems(listV);
    ui->comboBox_xy->addItems(listrates);
}
QString SetupForm::getFrequency()
{
    return  ui->lineEdit_Frequency->text().trimmed();
}
QString SetupForm::getSymbolrate()
{
    return ui->lineEdit_symbol_rate->text().trimmed();
}
QString SetupForm::getPolar()
{
    return ui->comboBox_V->currentText().trimmed();
}
QString SetupForm::getSignalSystems()
{
    return ui->comboBox_DVB->currentText().trimmed();
}
QString SetupForm::getFEC()
{
    return ui->comboBox_xy->currentText().trimmed();
}
QString SetupForm::getRolloff()
{
    return ui->comboBox_decimals->currentText().trimmed();
}
QString SetupForm::getModulation()
{
    return ui->comboBox_SPK->currentText().trimmed();
}
QString SetupForm::getECH()
{
    return ui->comboBox_ECHs->currentText().trimmed();
}
bool SetupForm::getDHCP()
{
    if(ui->checkBox_DHCP->isChecked())
    {
        return true;
    }

    if(!ui->checkBox_DHCP->isChecked())
    {
        return false;
    }
}
QString SetupForm::getIp()
{
    return ui->lineEdit_Ip->text().trimmed();
}
QString SetupForm::getNetmAsk()
{
    return ui->lineEdit_Net_AsK->text().trimmed();
}
QString SetupForm::getGrateway()
{
    return ui->lineEdit_GateWay->text().trimmed();
}
QString SetupForm::getDNS()
{
    return ui->lineEdit_DNS->text().trimmed();
}
QString SetupForm::getRemote()
{
    return ui->lineEdit_Remote->text().trimmed();
}
