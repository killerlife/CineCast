#ifndef TMSTEST_H
#define TMSTEST_H

#include <QtGui/QMainWindow>
#include "ui_tmstest.h"
#include <QtNetwork>

class tmstest : public QMainWindow
{
	Q_OBJECT

public:
	tmstest(QWidget *parent = 0, Qt::WFlags flags = 0);
	~tmstest();

private:
	Ui::tmstestClass ui;
	QTcpSocket dev;

private slots:
	void on_pushButton_3_clicked();
	void on_pushButton_2_clicked();
	void on_pushButton_clicked();
	void connected();
	void login();
	void end();
	void list();
	void ftp();
};

#endif // TMSTEST_H
