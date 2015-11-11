#ifndef STATUEFORM_H
#define STATUEFORM_H

#include <QWidget>

namespace Ui {
class StatueForm;
}

class StatueForm : public QWidget
{
    Q_OBJECT

public:
    explicit StatueForm(QWidget *parent = 0);
    ~StatueForm();
    void setSignalStrengthValue(int strengthMaxValue = 100,int strengthCurrentValue = 20);
    void setSignalQualityValue(int qualityMaxValue = 100,int qualityCurrentValue = 20);
    void setFilmRevceiverValue(quint64 filmLength = 100,quint64 receiverLength = 20);
    void setFilmName(QString filmNamestr = "No name");
    void setReceiverStatue(QString receiverStatueStr = "NO statue");
private:
    Ui::StatueForm *ui;
};

#endif // STATUEFORM_H
