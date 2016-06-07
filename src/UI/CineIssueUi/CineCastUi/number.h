#ifndef NUMBER_H
#define NUMBER_H

#include <QDialog>
#include "ui_number.h"

class Number : public QDialog
{
	Q_OBJECT

public:
	Number(QWidget *parent = 0);
	~Number();
	void SetTitle(QString s);
	QString GetNumber();
	void SetNumber(QString s);

private:
	Ui::NumberClass ui;
};

#endif // NUMBER_H
