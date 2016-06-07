#ifndef NUMBERIM_H
#define NUMBERIM_H

#include <QDialog>
#include "ui_numberim.h"

class NumberIM : public QDialog
{
	Q_OBJECT

public:
	NumberIM(QWidget *parent = 0);
	~NumberIM();

	void SetTitle(QString s, bool bIp = false);
	QString GetNumber();
	void SetNumber(QString s);

private:
	void AddNumber(char *s);

private:
	Ui::NumberIMClass ui;

private slots:
	void on_pushButtonDot_clicked();
	void on_pushButtonClose_clicked();
	void on_pushButtonOK_clicked();
	void on_pushButtonBack_clicked();
	void on_pushButton9_clicked();
	void on_pushButton8_clicked();
	void on_pushButton7_clicked();
	void on_pushButton6_clicked();
	void on_pushButton5_clicked();
	void on_pushButton4_clicked();
	void on_pushButton3_clicked();
	void on_pushButton2_clicked();
	void on_pushButton1_clicked();
	void on_pushButton0_clicked();
};

#endif // NUMBERIM_H
