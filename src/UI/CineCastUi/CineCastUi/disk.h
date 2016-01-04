#ifndef DISK_H
#define DISK_H

#include <QWidget>
#include <QMessageBox>


#include "ui_disk.h"
class QTcpSocket;

class Disk : public QWidget
{
	Q_OBJECT

public:
	Disk(QTcpSocket * socket, QWidget *parent = 0);
	~Disk();

private:
	Ui::DiskClass ui;
	QTcpSocket *pSocket;
	int m_Timer;

private:
	void Init();

private slots:
	void on_pushButton_format_clicked();
	void timerEvent(QTimerEvent * te);
};

#endif // DISK_H
