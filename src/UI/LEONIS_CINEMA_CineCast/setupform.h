#ifndef SETUPFORM_H
#define SETUPFORM_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class SetupForm;
}

class SetupForm : public QWidget
{
    Q_OBJECT

public:
    explicit SetupForm(QWidget *parent = 0);
    ~SetupForm();
    QString getFrequency();
    QString getSymbolrate();
    QString getPolar();
    QString getSignalSystems();
    QString getFEC();
    QString getRolloff();
    QString getModulation();
    QString getECH();
    bool  getDHCP();
    QString getIp();
    QString getNetmAsk();
    QString getGrateway();
    QString getDNS();
    QString getRemote();

private:
    Ui::SetupForm *ui;
    bool checkedfalg;
private:
    QStringList listSpks ;
    QStringList listDvbs ;
    QStringList listrates ;
    QStringList listdecimals ;
    QStringList listSpkECHOS ;
    QStringList listV;
    void init();

};

#endif // SETUPFORM_H
