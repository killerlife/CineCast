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

private:
	void Init();

private:
	Ui::RaidClass ui;
};

#endif // RAID_H
