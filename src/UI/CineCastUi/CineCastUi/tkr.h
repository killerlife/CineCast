#ifndef TKR_H
#define TKR_H

#include <QWidget>
#include <QtNetwork>
#include "ui_tkr.h"

class Tkr : public QWidget
{
	Q_OBJECT

public:
	Tkr(QTcpSocket * socket, QWidget *parent = 0);
	~Tkr();

private:
	void Init();

private:
	Ui::TkrClass ui;

private slots:
	void on_groupBox_2_toggled(bool);
	void on_treeWidget_clicked(const QModelIndex &);
};

#endif // TKR_H
