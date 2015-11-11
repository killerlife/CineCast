#include "statueform.h"
#include "ui_statueform.h"
#include <QDebug>
StatueForm::StatueForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatueForm)
{
    ui->setupUi(this);
    QString ProgressBarQSS = "QProgressBar {border: 2px solid red;border-radius: 6px;background-color:#666666/* qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde)*/;min-width: 80px; border-color: #9999FF}";
    ui->progressBar_siganl_Strength->setStyleSheet(ProgressBarQSS);
    ui->progressBar_siganl_Quality->setStyleSheet(ProgressBarQSS);
    ui->progressBar_Revceiver_length->setStyleSheet(ProgressBarQSS);
    QString QGroupBoxQSS = "QGroupBox {font-size:20px;font-family:'宋体'; border: 4px solid red;border-radius:/*borber-background:red;*/ 6px;background-color:#cccccc /*qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde)*/;min-width: 80px; border-color:#666666 }";//333333
    ui->groupBox_satellite->setStyleSheet(QGroupBoxQSS);
    ui->groupBox_Filme->setStyleSheet(QGroupBoxQSS);
    QString fontSStr = "font-size: 20px;font-family:'宋体'";
    ui->label->setStyleSheet(fontSStr);
    ui->label_filmName->setStyleSheet(fontSStr);
    ui->label_5->setStyleSheet(fontSStr);
    ui->label_6->setStyleSheet(fontSStr);
    ui->label_Receiver->setStyleSheet(fontSStr);
    ui->label_9->setStyleSheet(fontSStr);
    ui->label_2->setStyleSheet(fontSStr);
    QString TextBrowserQSS = "QTextBrowser {font-size:24px; border: 2px solid red;border-radius: 6px;background-color:#cccccc /*qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde)*/;min-width: 80px; border-color: #666666}";
    ui->textBrowser->setStyleSheet(TextBrowserQSS);

}

StatueForm::~StatueForm()
{
    delete ui;
}
void StatueForm::setSignalStrengthValue(int strengthMaxValue,int strengthCurrentValue)
{
    ui->progressBar_siganl_Strength->setMaximum(strengthMaxValue);
    ui->progressBar_siganl_Strength->setValue(strengthCurrentValue);
}
void StatueForm::setSignalQualityValue(int qualityMaxValue,int qualityCurrentValue)
{
    ui->progressBar_siganl_Quality->setMaximum(qualityMaxValue);
    ui->progressBar_siganl_Quality->setValue(qualityCurrentValue);
}
void StatueForm::setFilmRevceiverValue(quint64 filmLength,quint64 receiverLength)
{
    QString pointStr = QString::number(receiverLength,10)+"/"+QString::number(filmLength,10);
    ui->label_Receiver->setText(pointStr);
    QString number1 = QString::number(filmLength,10);
    QString number2 = QString::number(receiverLength,10);
    number1 = number1.replace(10,64,"").trimmed();
    number2 = number2.replace(10,64,"").trimmed();
    ui->progressBar_Revceiver_length->setMaximum(number1.toInt());
    ui->progressBar_Revceiver_length->setValue(number2.toInt());
}
void StatueForm::setFilmName(QString filmNamestr)
{
    ui->label_filmName->setText(filmNamestr);
}
void StatueForm:: setReceiverStatue(QString receiverStatueStr)
{
    ui->textBrowser->setText(receiverStatueStr);
}











