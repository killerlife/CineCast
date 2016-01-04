#ifndef SETUP_H
#define SETUP_H

#include <QWidget>
#include <QtNetwork>
#include "ui_setup.h"
#include "../../../netcomm/UiProtocal.h"

class Setup : public QWidget
{
	Q_OBJECT

public:
	Setup(QTcpSocket* pSocket, QWidget *parent = 0);
	~Setup();

	void LoadConfig();
	void getTMS();      //ȡ־
	void TMS_start();   //ȡ־ʱ
	void TMS_stop();    //رջȡ־ʱ
	int m_TMS_timer;    //ʱȡ־ 

private:
	void Init();

private:
	Ui::SetupClass ui;
	QTcpSocket* pSocket;
	std::list<NETWORK_CONF> m_list;
	int ethIndex;

private slots:
	void on_comboBox_ECHs_currentIndexChanged(QString);
	void on_pushButton_2_clicked();
	void on_pushButton_clicked();
	void timerEvent(QTimerEvent * te);
};

#endif // SETUP_H
