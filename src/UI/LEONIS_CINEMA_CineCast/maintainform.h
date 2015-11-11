#ifndef MAINTAINFORM_H
#define MAINTAINFORM_H

#include <QWidget>

namespace Ui {
class MaintainForm;
}

class MaintainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MaintainForm(QWidget *parent = 0);
    ~MaintainForm();
    void setProgressBarValue(int maxvalue = 100,int value = 0);
private slots:

    void on_pushButton_format_clicked();
    void formatsignalupdateImage();

private:
    void ButtonInit();
private:
    Ui::MaintainForm *ui;
signals:
    void formatsignal();
//private slots:
//    void format();

};

#endif // MAINTAINFORM_H
