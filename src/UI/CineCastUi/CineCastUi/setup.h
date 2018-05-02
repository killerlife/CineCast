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
	void getTMS();      //取志
	void TMS_start();   //取志时
	void TMS_stop();    //乇栈取志时
	int m_TMS_timer;    //时取志 

private:
	void Init();
	void LoadNetConfig(bool bUpdate = false);
	void LoadRemoteConfig();
	void LoadNetStatus();
	void Interface1Enable(bool bEnable = true);
	void Interface2Enable(bool bEnable = true);
	void GetRemoteConnectStatus(int i = 0);
	void LoadRecvFinish();

private:
	Ui::SetupClass ui;
	QTcpSocket* pSocket;
	std::list<NETWORK_CONF> m_list;
	int ethIndex;
	int remIndex;
	int bUpdateNetWork;
	bool firsttime;

private slots:
	void on_pushButtonApply2_3_clicked();
	void on_lineEdit_Frequency_editingFinished();
	void on_pushButton_13_clicked();
	void on_pushButtonApply2_2_clicked();
	void on_pushButtonApply2_clicked();
	void on_pushButtonApply1_clicked();
	void on_lineEdit_DNS_4_editingFinished();
	void on_lineEdit_DNS_3_editingFinished();
	void on_lineEdit_GateWay_2_editingFinished();
	void on_lineEdit_Net_AsK_2_editingFinished();
	void on_lineEdit_Ip_2_editingFinished();
	void on_lineEdit_DNS_2_editingFinished();
	void on_lineEdit_DNS_editingFinished();
	void on_lineEdit_GateWay_editingFinished();
	void on_lineEdit_Net_AsK_editingFinished();
	void on_lineEdit_Ip_editingFinished();
	void on_radioButtonDhcp_2_toggled(bool);
	void on_radioButtonStatic_2_toggled(bool);
	void on_radioButtonStatic_toggled(bool);
	void on_radioButtonDhcp_toggled(bool);
	void on_pushButton_12_clicked();
	void on_pushButton_11_clicked();
	void on_pushButton_10_clicked();
	void on_pushButton_9_clicked();
	void on_pushButton_8_clicked();
	void on_pushButton_7_clicked();
	void on_pushButton_6_clicked();
	void on_pushButton_5_clicked();
#if 0
	void on_comboBox_ECHs_currentIndexChanged(QString);
#endif
	void on_pushButton_2_clicked();
	void on_pushButton_clicked();
	void timerEvent(QTimerEvent * te);
};

#endif // SETUP_H
