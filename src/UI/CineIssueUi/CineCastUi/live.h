#ifndef LIVE_H
#define LIVE_H

#include <QWidget>
#include <QtNetwork>
#include "ui_live.h"

class Live : public QWidget
{
	Q_OBJECT

public:
	Live(QTcpSocket * socket, QWidget *parent = 0);
	~Live();

private:
	void Init();

private:
	Ui::LiveClass ui;

private slots:
	void on_radioButton_2_clicked();
	void on_radioButton_clicked();
};

#endif // LIVE_H
