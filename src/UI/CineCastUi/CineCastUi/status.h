#ifndef STATUS_H
#define STATUS_H

#include <QWidget>
#include <QtNetwork>
#include "ui_status.h"

#include "../../../netcomm/UiProtocal.h"
class Status : public QWidget
{
	Q_OBJECT

public:
	Status(QWidget *parent = 0);
	~Status();

	void UpdateSatellite(TUNER_INFO* tInfo);
	void UpdateRecv(RECEIVE_INFO* tInfo);

private:
	void Init();

private:
	Ui::StatusClass ui;
};

#endif // STATUS_H
