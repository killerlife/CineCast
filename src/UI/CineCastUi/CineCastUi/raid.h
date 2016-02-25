#ifndef RAID_H
#define RAID_H

#include <QWidget>
#include <QtNetwork>
#include "ui_raid.h"

class Raid : public QWidget
{
	Q_OBJECT

public:
	Raid(QTcpSocket * socket, QWidget *parent = 0);
	~Raid();
	void Start();
	void Stop();

private:
	void Init();
	void loadState();
	void LoadContent_HDD();
	void UpdateContent_HDD();
	void ContentisReady_HDD();

	void LoadContent_RAID();
	void UpdateContent_RAID();
	void ContentisReady_RAID();

private:
	Ui::RaidClass ui;
	QTcpSocket* pSocket;
	int m_timer;
	int m_content;
	int m_raid;

private slots:
	void on_pushButton_3_clicked();
	void on_pushButton_2_clicked();
	void on_pushButton_5_clicked();
	void on_pushButton_4_clicked();
	void timerEvent(QTimerEvent *te);
};

#endif // RAID_H
