#include "setup.h"
#include <QMessageBox>

Setup::Setup(QTcpSocket* pSocket, QWidget *parent)
	: QWidget(parent), pSocket(NULL),bUpdateNetWork(0)
{
	ui.setupUi(this);
	Init();
	this->pSocket = pSocket;
	m_TMS_timer = -1;       //ȡ־ʱ
	remIndex = 10;
}

Setup::~Setup()
{

}

void Setup::Init()
{
	ui.lineEdit_Frequency->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_symbol_rate->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.comboBox_decimals->setStyleSheet("QComboBox{font-size:18px;font-family:'Book Antiqua';}");
	ui.comboBox_DVB->setStyleSheet("QComboBox{font-size:18px;font-family:'Book Antiqua';}");
	ui.comboBox_SPK->setStyleSheet("QComboBox{font-size:18px;font-family:'Book Antiqua';}");
	ui.comboBox_V->setStyleSheet("QComboBox{font-size:18px;font-family:'Book Antiqua';}");
	ui.comboBox_xy->setStyleSheet("QComboBox{font-size:18px;font-family:'Book Antiqua';}");
	ui.label_ip->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_Gateway->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_Gateway_2->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_Gateway_3->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_Netm_mask->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_DNS->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_DNS_2->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.lineEdit_Ip->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_GateWay->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_Remote_3->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_Net_AsK->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_DNS->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_DNS_2->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");

	ui.label_ip_2->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_Gateway_4->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_Netm_mask_2->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_DNS_3->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_DNS_4->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.lineEdit_Ip_2->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_GateWay_2->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_Net_AsK_2->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_DNS_3->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_DNS_4->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");

// 	ui.checkBox_DHCP_2->setStyleSheet("QCheckBox{font-size:20px;font-family:'Book Antiqua';}");
// 	ui.checkBox_DHCP->setStyleSheet("QCheckBox{font-size:20px;font-family:'Book Antiqua';}");
	ui.lineEdit_Remote_2->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.pushButton_4->setStyleSheet("QPushButton{font-size:18px;font-family:'Book Antiqua';}");
	ui.pushButton_3->setStyleSheet("QPushButton{font-size:18px;font-family:'Book Antiqua';}");
	ui.textBrowser->setStyleSheet("QTextBrowser{font-size:18px;font-family:'Book Antiqua';}");

	ui.radioButtonStatic->setStyleSheet("QRadioButton{font-size:18px;font-family:'Book Antiqua';}");
	ui.radioButtonDhcp->setStyleSheet("QRadioButton{font-size:18px;font-family:'Book Antiqua';}");
	ui.radioButtonStatic_2->setStyleSheet("QRadioButton{font-size:18px;font-family:'Book Antiqua';}");
	ui.radioButtonDhcp_2->setStyleSheet("QRadioButton{font-size:18px;font-family:'Book Antiqua';}");

	ui.labelRemoteStatus->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua'; color:#0000FF;}");

	ui.pushButton_3->setVisible(false);
	ui.pushButton_4->setVisible(false);
// 	ui.comboBox_SPK->setEnabled(false);
#ifndef WIN32
	ui.groupBox_2->setVisible(false);
#endif
#ifndef FULL_VERSION
// 	ui.labe_symbl_rate_8->setVisible(false);
// 	ui.comboBox_SPK->setVisible(false);
	ui.groupBox->setVisible(false);
	ui.label_DNS_2->setVisible(false);
	ui.label_DNS_4->setVisible(false);
	ui.lineEdit_DNS_2->setVisible(false);
	ui.lineEdit_DNS_4->setVisible(false);
#endif
}


extern int m_ConnectStatus;   //״̬01,2ɹ
void Setup::timerEvent(QTimerEvent * te)
{
	if(te->timerId() == m_TMS_timer && m_ConnectStatus == 2)
	{
#ifdef FULL_VERSION
         this->getTMS();      //ȡ־
#endif
		 LoadNetStatus();
		 if(bUpdateNetWork > 0)
		 {
			 bUpdateNetWork++;
			 if(bUpdateNetWork > 2)
			 {
				 LoadNetConfig(true);
				 bUpdateNetWork = 0;
				 ui.radioButtonDhcp_2->setEnabled(true);
				 ui.radioButtonStatic_2->setEnabled(true);
				 ui.radioButtonDhcp->setEnabled(true);
				 ui.radioButtonStatic->setEnabled(true);
				 ui.pushButtonApply1->setEnabled(true);
				 ui.pushButtonApply2->setEnabled(true);
			 }
		 }
//         TMS_stop();
		 if(remIndex < 9)
		 {
			 remIndex++;
			 GetRemoteConnectStatus(2);
		 }
		 else
		 {
			 GetRemoteConnectStatus();
			 ui.lineEdit_Remote_2->setEnabled(true);
			 ui.lineEdit_Remote_3->setEnabled(true);
			 ui.pushButtonApply2_2->setEnabled(true);
			 ui.pushButton_13->setEnabled(true);
		 }
	}
}

void Setup::Interface1Enable(bool bEnable)
{
	ui.lineEdit_Ip->setEnabled(bEnable);
	ui.lineEdit_DNS->setEnabled(bEnable);
	ui.lineEdit_GateWay->setEnabled(bEnable);
	ui.lineEdit_Net_AsK->setEnabled(bEnable);
	ui.lineEdit_DNS_2->setEnabled(bEnable);
}

void Setup::Interface2Enable(bool bEnable)
{
	ui.lineEdit_Ip_2->setEnabled(bEnable);
	ui.lineEdit_DNS_3->setEnabled(bEnable);
	ui.lineEdit_GateWay_2->setEnabled(bEnable);
	ui.lineEdit_Net_AsK_2->setEnabled(bEnable);
	ui.lineEdit_DNS_4->setEnabled(bEnable);
}

#include "../../../netcomm/UiProtocal.h"

void Setup::LoadConfig()
{
	char buf[2048];
	KL *pKL = (KL*)buf;

	int i;
	pKL->m_pkgHead = 0x7585;

	//Load Satellite setting
	pKL->m_keyID = S_GET_CONFIG;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 2048);

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_GET_CONFIG)
	{
		TUNER_CONF tuner;
		int datalen = sizeof (unsigned int) * 5;
		memcpy(&tuner, buf + sizeof(KL), datalen);

		KL *pKL = (KL*) buf;
		int len = pKL->m_length - datalen;
		int pos = sizeof(KL) + datalen;
		while(len > 0)
		{
			SL * sl = (SL*)(buf + pos);
			std::string tmp;
			pos += sizeof(SL);
			len -= sizeof(SL);
			if (sl->m_length > 0)
			{
				char *s = new char[sl->m_length+1];
				memset(s, 0, sl->m_length+1);
				memcpy(s, buf+pos, sl->m_length);
				tmp = s;
				pos += sl->m_length;
				len -= sl->m_length;
				delete[] s;
			}
			else
			{
				tmp = "";
			}

			switch(sl->m_sID)
			{
			case TUNER_DEV_NAME:
				tuner.strDevName = tmp;
				break;
			case  TUNER_DEL_SYS:
				tuner.strDelSys = tmp;
				break;
			case TUNER_FEC:
				tuner.strFec = tmp;
				break;
			case TUNER_MOD:
				tuner.strModulation = tmp;
				break;
			case TUNER_ROLL_OFF:
				tuner.strRollOff = tmp;
				break;
			case TUNER_POL:
				tuner.strPolVert = tmp;
				break;
			}
		}
		ui.lineEdit_Frequency->setText(QString::number(tuner.nFreq));
		ui.lineEdit_symbol_rate->setText(QString::number(tuner.nSR));
		int nIndex = ui.comboBox_DVB->findText(QString(tuner.strDelSys.c_str()));
		if (nIndex != -1)
		{
			ui.comboBox_DVB->setCurrentIndex(nIndex);
		}
		else
		{
			ui.comboBox_DVB->insertItem(0, QString(tuner.strDelSys.c_str()));
			ui.comboBox_DVB->setCurrentIndex(0);
		}

		nIndex = ui.comboBox_xy->findText(QString(tuner.strFec.c_str()));
		if (nIndex != -1)
		{
			ui.comboBox_xy->setCurrentIndex(nIndex);
		}
		else
		{
			ui.comboBox_xy->insertItem(0, QString(tuner.strFec.c_str()));
			ui.comboBox_xy->setCurrentIndex(0);
		}

		nIndex = ui.comboBox_V->findText(QString(tuner.strPolVert.c_str()));
		if (nIndex != -1)
		{
			ui.comboBox_V->setCurrentIndex(nIndex);
		}
		else
		{
			ui.comboBox_V->insertItem(0, QString(tuner.strPolVert.c_str()));
			ui.comboBox_V->setCurrentIndex(0);
		}

		nIndex = ui.comboBox_SPK->findText(QString(tuner.strModulation.c_str()));
		if (nIndex != -1)
		{
			ui.comboBox_SPK->setCurrentIndex(nIndex);
		}
		else
		{
			ui.comboBox_SPK->insertItem(0, QString(tuner.strModulation.c_str()));
			ui.comboBox_SPK->setCurrentIndex(0);
		}

		nIndex = ui.comboBox_decimals->findText(QString(tuner.strRollOff.c_str()));
		if (nIndex != -1)
		{
			ui.comboBox_decimals->setCurrentIndex(nIndex);
		}
		else
		{
			if(tuner.strRollOff == "auto")
			{
				nIndex = ui.comboBox_decimals->findText(tr("auto"));
				if(nIndex != -1)
					ui.comboBox_decimals->setCurrentIndex(nIndex);
				else
				{
			ui.comboBox_decimals->insertItem(0, QString(tuner.strRollOff.c_str()));
			ui.comboBox_decimals->setCurrentIndex(0);
		}
	}
			else
			{
				ui.comboBox_decimals->insertItem(0, QString(tuner.strRollOff.c_str()));
				ui.comboBox_decimals->setCurrentIndex(0);
			}
		}
	}
	LoadNetConfig(true);
	remIndex = 10;	
	LoadRemoteConfig();
}

void Setup::LoadNetConfig(bool bUpdate /* = false */)
{
	char buf[2048];
	KL *pKL = (KL*)buf;

	int i;
	pKL->m_pkgHead = 0x7585;

	//Load NetDevice setting
	m_list.clear();
	pKL->m_keyID = N_GET_CONFIG;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 2048);

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == N_GET_CONFIG)
	{
		m_list.clear();
		KL *pKL = (KL*) buf;

		int len = pKL->m_length;
		int pos = sizeof(KL);
		NETWORK_CONF nc;
		int dns = 0;
		while(len > 0)
		{
			SL * sl = (SL*)(buf + pos);
			std::string tmp;
			pos += sizeof(SL);
			len -= sizeof(SL);
			if (sl->m_length > 0)
			{
				char *s = new char[sl->m_length+1];
				memset(s, 0, sl->m_length+1);
				memcpy(s, buf+pos, sl->m_length);
				tmp = s;
				pos += sl->m_length;
				len -= sl->m_length;
				delete[] s;
			}
			else
			{
				tmp = "";
			}

			switch(sl->m_sID)
			{
			case NET_DHCP:
				dns = 0;
				memcpy(&nc.nDhcp, buf + pos - sl->m_length, sl->m_length);
				break;
			case  NET_DEV_NAME:
				nc.strDevName = tmp;
				break;
			case NET_IP:
				nc.strIp = tmp;
				break;
			case NET_NETMASK:
				nc.strNetmask = tmp;
				break;
			case NET_GATEWAY:
				nc.strGateway = tmp;
				break;
			case NET_CONNECTED:
				nc.strConnected = tmp;
				break;
			case REMOTE_DNS:
				if (dns == 0)
				{
					nc.strDns1 = tmp;
					dns++;
				}
				else
				{
					nc.strDns2 = tmp;
				m_list.push_back(nc);
				}
				break;
			}
		}
	}
	std::list<NETWORK_CONF>::iterator itor;
	i = 0;
	for(itor = m_list.begin(); itor != m_list.end(); ++itor, i++)
	{
		NETWORK_CONF nc = *itor;
		if(i == 0)
		{
			bool bChange = false;
			if(nc.strConnected == "TRUE")
			{
				if (ui.labelNetwork1->isEnabled() == false)
					bChange = true;
				ui.labelNetwork1->setEnabled(true);
			}
			else
			{
				if(ui.labelNetwork1->isEnabled() == true)
					bChange = true;
				ui.labelNetwork1->setEnabled(false);
			}
			if(bUpdate || bChange == true)
			{
				if(nc.strDevName != "NODEV")
				{
					if(nc.nDhcp)
					{
						Interface1Enable(false);
						ui.radioButtonStatic->setChecked(false);
						ui.radioButtonDhcp->setChecked(true);
// 						ui.checkBox_DHCP->setCheckState(Qt::Checked);
						if(nc.strIp == "" || nc.strIp.empty())
						{
							bUpdateNetWork = 1;
							ui.radioButtonDhcp->setEnabled(false);
							ui.radioButtonStatic->setEnabled(false);
						}
					}
					else
					{
						Interface1Enable(true);
						ui.radioButtonStatic->setChecked(true);
						ui.radioButtonDhcp->setChecked(false);
// 						ui.checkBox_DHCP->setCheckState(Qt::Unchecked);
					}
					if(nc.strIp == "")
						ui.lineEdit_Ip->setText("0.0.0.0");
					else
			ui.lineEdit_Ip->setText(nc.strIp.c_str());
					if(nc.strNetmask == "")
						ui.lineEdit_Net_AsK->setText("0.0.0.0");
					else
			ui.lineEdit_Net_AsK->setText(nc.strNetmask.c_str());
					if(nc.strGateway == "")
						ui.lineEdit_GateWay->setText("0.0.0.0");
					else
			ui.lineEdit_GateWay->setText(nc.strGateway.c_str());
					if(nc.strDns1 == "")
						ui.lineEdit_DNS->setText("0.0.0.0");
					else
						ui.lineEdit_DNS->setText(nc.strDns1.c_str());
					if(nc.strDns2 == "")
						ui.lineEdit_DNS_2->setText("0.0.0.0");
					else
						ui.lineEdit_DNS_2->setText(nc.strDns2.c_str());
				}
				else
				{
// 					ui.checkBox_DHCP->setCheckState(Qt::Unchecked);
					ui.lineEdit_Ip->setText("");
					ui.lineEdit_Net_AsK->setText("");
					ui.lineEdit_GateWay->setText("");
					ui.lineEdit_DNS->setText("");
					ui.lineEdit_DNS_2->setText("");
				}
			}
		}
		if(i == 1)
		{
			bool bChange = false;
			if(nc.strConnected == "TRUE")
			{
				if (ui.labelNetwork1_2->isEnabled() == false)
					bChange = true;
				ui.labelNetwork1_2->setEnabled(true);
			}
			else
			{
				if(ui.labelNetwork1_2->isEnabled() == true)
					bChange = true;
				ui.labelNetwork1_2->setEnabled(false);
			}
			if(bUpdate || bChange == true)
			{
				if(nc.strDevName != "NODEV")
				{
			if (nc.nDhcp)
					{
						Interface2Enable(false);
						ui.radioButtonStatic_2->setChecked(false);
						ui.radioButtonDhcp_2->setChecked(true);
// 						ui.checkBox_DHCP_2->setCheckState(Qt::Checked);
						if(nc.strIp == "" || nc.strIp.empty())
						{
							bUpdateNetWork = 1;
							ui.radioButtonDhcp_2->setEnabled(false);
							ui.radioButtonStatic_2->setEnabled(false);
						}
					}
					else
					{
						Interface2Enable(true);
						ui.radioButtonStatic_2->setChecked(true);
						ui.radioButtonDhcp_2->setChecked(false);
// 						ui.checkBox_DHCP_2->setCheckState(Qt::Unchecked);
					}
					if(nc.strIp == "")
						ui.lineEdit_Ip_2->setText("0.0.0.0");
					else
						ui.lineEdit_Ip_2->setText(nc.strIp.c_str());
					if(nc.strNetmask == "")
						ui.lineEdit_Net_AsK_2->setText("0.0.0.0");
					else
						ui.lineEdit_Net_AsK_2->setText(nc.strNetmask.c_str());
					if(nc.strGateway == "")
						ui.lineEdit_GateWay_2->setText("0.0.0.0");
					else
						ui.lineEdit_GateWay_2->setText(nc.strGateway.c_str());
					if(nc.strDns1 == "")
						ui.lineEdit_DNS_3->setText("0.0.0.0");
					else
						ui.lineEdit_DNS_3->setText(nc.strDns1.c_str());
					if(nc.strDns2 == "")
						ui.lineEdit_DNS_4->setText("0.0.0.0");
					else
						ui.lineEdit_DNS_4->setText(nc.strDns2.c_str());
				}
			else
				{
// 					ui.checkBox_DHCP_2->setCheckState(Qt::Unchecked);
					ui.lineEdit_Ip_2->setText("");
					ui.lineEdit_Net_AsK_2->setText("");
					ui.lineEdit_GateWay_2->setText("");
					ui.lineEdit_DNS_3->setText("");
					ui.lineEdit_DNS_4->setText("");
				}
			}
		}
		}

	}

void Setup::LoadRemoteConfig()
{
	char buf[2048];
	KL *pKL = (KL*)buf;

	int i;
	pKL->m_pkgHead = 0x7585;
	//Load Remote setting
	pKL->m_keyID = N_GET_REMOTE;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 2048);
	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == N_GET_REMOTE)
	{
		KL *pKL = (KL*) buf;

		int len = pKL->m_length;
		int pos = sizeof(KL);
		int dns = 0;
		REMOTE_CONF rc;
		while(len > 0)
		{
			SL * sl = (SL*)(buf + pos);
			std::string tmp;
			pos += sizeof(SL);
			len -= sizeof(SL);
			if (sl->m_length > 0)
			{
				char *s = new char[sl->m_length+1];
				memset(s, 0, sl->m_length+1);
				memcpy(s, buf+pos, sl->m_length);
				tmp = s;
				pos += sl->m_length;
				len -= sl->m_length;
				delete[] s;
			}
			else
			{
				tmp = "";
			}

			switch(sl->m_sID)
			{
			case REMOTE_DNS:
				break;
			case  REMOTE_SERVER:
				rc.strRemote = tmp;
				break;
			case REMOTE_PORT:
				memset(&rc.nPort, 0, sizeof(rc.nPort));
				if(tmp.size() < 4)
					memcpy(&rc.nPort, tmp.c_str(), tmp.size());
				else
					memcpy(&rc.nPort, tmp.c_str(), sizeof(rc.nPort));
				break;
			}
		}
		ui.lineEdit_Remote_2->setText(rc.strRemote.c_str());
		ui.lineEdit_Remote_3->setText(QString::number(rc.nPort));
	}
}

void Setup::LoadNetStatus()
{
	char buf[2048];
	KL *pKL = (KL*)buf;

	int i;
	pKL->m_pkgHead = 0x7585;

	//Load NetDevice setting
	m_list.clear();
	pKL->m_keyID = N_GET_STATUS;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 2048);

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == N_GET_STATUS)
	{
		m_list.clear();
		KL *pKL = (KL*) buf;

		int len = pKL->m_length;
		int pos = sizeof(KL);
		NETWORK_CONF nc;
		int dns = 0;
		while(len > 0)
		{
			SL * sl = (SL*)(buf + pos);
			std::string tmp;
			pos += sizeof(SL);
			len -= sizeof(SL);
			if (sl->m_length > 0)
			{
				char *s = new char[sl->m_length+1];
				memset(s, 0, sl->m_length+1);
				memcpy(s, buf+pos, sl->m_length);
				tmp = s;
				pos += sl->m_length;
				len -= sl->m_length;
				delete[] s;
			}
			else
			{
				tmp = "";
			}

			switch(sl->m_sID)
			{
			case  NET_DEV_NAME:
				nc.strDevName = tmp;
				break;
			case NET_CONNECTED:
				nc.strConnected = tmp;
				m_list.push_back(nc);
				break;
			}
		}
	}

	std::list<NETWORK_CONF>::iterator itor;
	i = 0;
	int down = 0;
	for(itor = m_list.begin(); itor != m_list.end(); ++itor, i++)
	{
		NETWORK_CONF nc = *itor;
		if(i == 0)
		{
			bool bChange = false;
			if(nc.strConnected == "TRUE")
			{
				if (ui.labelNetwork1->isEnabled() == false)
				{
					bChange = true;
					bUpdateNetWork = 1;
					ui.radioButtonStatic->setEnabled(false);
					ui.radioButtonDhcp->setEnabled(false);
					firsttime = true;
				}
				ui.groupBox_3->setEnabled(true);
				ui.labelNetwork1->setEnabled(true);
			}
			else if(nc.strConnected == "FALSE")
			{
				if(ui.labelNetwork1->isEnabled() == true)
				{
					bChange = true;
					bUpdateNetWork = 1;
					ui.radioButtonDhcp->setEnabled(false);
					ui.radioButtonStatic->setEnabled(false);
				}
				else
				{
					down++;
				}
				ui.groupBox_3->setEnabled(true);
				ui.labelNetwork1->setEnabled(false);
			}
			else
			{
				ui.groupBox_3->setEnabled(false);
				ui.labelNetwork1->setEnabled(false);
			}
		}
		if(i == 1)
		{
			if(nc.strConnected == "TRUE")
			{
				if (ui.labelNetwork1_2->isEnabled() == false)
				{
					bUpdateNetWork = 1;
					ui.radioButtonStatic_2->setEnabled(false);
					ui.radioButtonDhcp_2->setEnabled(false);
					firsttime = true;
				}
				ui.groupBox_4->setEnabled(true);
				ui.labelNetwork1_2->setEnabled(true);
			}
			else if(nc.strConnected == "FALSE")
			{
				if(ui.labelNetwork1_2->isEnabled() == true)
				{
					bUpdateNetWork = 1;
					ui.radioButtonDhcp_2->setEnabled(false);
					ui.radioButtonStatic_2->setEnabled(false);
				}
				else
				{
					down++;
				}
				ui.groupBox_4->setEnabled(true);
				ui.labelNetwork1_2->setEnabled(false);
			}
			else 
			{
				ui.groupBox_4->setEnabled(false);
				ui.labelNetwork1_2->setEnabled(false);
			}
		}
	}
	if(down >= 2 && firsttime)
	{
		on_pushButtonApply2_2_clicked();
		firsttime = false;
	}
}

//Save all setting
void Setup::on_pushButton_clicked()
{
// 	TMS_stop();
	unsigned int frq = ui.lineEdit_Frequency->text().toUInt();
	if(frq < 12250000)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Frequency must above 12250000."));
		ui.lineEdit_Frequency->setText("12250000");
		return;
	}
	if(frq > 13450000)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Frequency must less 13450000."));
		ui.lineEdit_Frequency->setText("13450000");
	}
	//Set Satellite
	TUNER_CONF tuner;
	char buf[2048];
	KL* pKL = (KL*)buf;
	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = S_SET_CONFIG;

	tuner.nFreq = ui.lineEdit_Frequency->text().toUInt();
	tuner.nSR = ui.lineEdit_symbol_rate->text().toUInt();
	if(tuner.nFreq == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Frequencies must over zero."));
		tuner.nFreq = 1;
		ui.lineEdit_Frequency->setText("1");
	}
	if(tuner.nSR == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("symbol Rate must over zero."));
		tuner.nSR = 1;
		ui.lineEdit_symbol_rate->setText("1");
	}
	tuner.strDelSys = ui.comboBox_DVB->currentText().toStdString();
	tuner.strFec = ui.comboBox_xy->currentText().toStdString();
	tuner.strPolVert = ui.comboBox_V->currentText().toStdString();
	tuner.strModulation = ui.comboBox_SPK->currentText().toStdString();
	if(ui.comboBox_decimals->currentText() == tr("auto"))
		tuner.strRollOff = "auto";
	else
	tuner.strRollOff = ui.comboBox_decimals->currentText().toStdString();

	SL DevName, DelSys, Fec, Mod, Roll, Pol;

	DevName.m_sID = TUNER_DEV_NAME;
	DevName.m_length = tuner.strDevName.size();

	DelSys.m_sID = TUNER_DEL_SYS;
	DelSys.m_length = tuner.strDelSys.size();

	Fec.m_sID = TUNER_FEC;
	Fec.m_length = tuner.strFec.size();

	Mod.m_sID = TUNER_MOD;
	Mod.m_length = tuner.strModulation.size();

	Roll.m_sID = TUNER_ROLL_OFF;
	Roll.m_length = tuner.strRollOff.size();

	Pol.m_sID = TUNER_POL;
	Pol.m_length = tuner.strPolVert.size();

	int datalen = sizeof(unsigned int) * 5;

	char* pos = buf + sizeof(KL);
	memcpy(pos, &tuner, datalen);
	pos += datalen;

	memcpy(pos, &DevName, sizeof(DevName));
	pos += sizeof(DevName);
	memcpy(pos, tuner.strDevName.c_str(), DevName.m_length);
	pos += DevName.m_length;

	memcpy(pos, &DelSys, sizeof(DelSys));
	pos += sizeof(DelSys);
	memcpy(pos, tuner.strDelSys.c_str(), DelSys.m_length);
	pos += DelSys.m_length;

	memcpy(pos, &Fec, sizeof(Fec));
	pos += sizeof(Fec);
	memcpy(pos, tuner.strFec.c_str(), Fec.m_length);
	pos += Fec.m_length;

	memcpy(pos, &Mod, sizeof(Mod));
	pos += sizeof(Mod);
	memcpy(pos, tuner.strModulation.c_str(), Mod.m_length);
	pos += Mod.m_length;

	memcpy(pos, &Roll, sizeof(Roll));
	pos += sizeof(Roll);
	memcpy(pos, tuner.strRollOff.c_str(), Roll.m_length);
	pos += Roll.m_length;

	memcpy(pos, &Pol, sizeof(Pol));
	pos += sizeof(Pol);
	memcpy(pos, tuner.strPolVert.c_str(), Pol.m_length);
	pos += Pol.m_length;

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i = pSocket->read(buf, 2048);

#if 0
	//Set network
	on_lineEdit_Ip_editingFinished();
	on_lineEdit_Net_AsK_editingFinished();
	on_lineEdit_GateWay_editingFinished();
// 	on_lineEdit_DNS_editingFinished();
// 	on_lineEdit_DNS_2_editingFinished();
	on_lineEdit_Ip_2_editingFinished();
	on_lineEdit_Net_AsK_2_editingFinished();
	on_lineEdit_GateWay_2_editingFinished();
// 	on_lineEdit_DNS_3_editingFinished();
// 	on_lineEdit_DNS_4_editingFinished();

	pKL->m_keyID = N_SET_CONFIG;
	SL Dns1, Dns2, Dhcp, Ip, Netmask, Gateway;
	pos = buf + sizeof(KL);
	std::list<NETWORK_CONF> newList;
	i = 0;
	std::list<NETWORK_CONF>::iterator itor;
	for (itor = m_list.begin(); itor != m_list.end(); ++itor, i++)
	{
		NETWORK_CONF nc = *itor;
		if(i == 0)
		{
			if(ui.groupBox_3->isEnabled())
			{
				if(ui.radioButtonDhcp->isChecked())
					nc.nDhcp = 1;
				else
					nc.nDhcp = 0;
				nc.strIp = ui.lineEdit_Ip->text().toStdString();
				nc.strGateway = ui.lineEdit_GateWay->text().toStdString();
				nc.strNetmask = ui.lineEdit_Net_AsK->text().toStdString();
				nc.strDns1 = ui.lineEdit_DNS->text().toStdString();
				nc.strDns2 = ui.lineEdit_DNS_2->text().toStdString();
				newList.push_back(nc);
			}
		}
		if(i == 1)
		{
			if(ui.groupBox_4->isEnabled())
			{
				if(ui.radioButtonDhcp_2->isChecked())
					nc.nDhcp = 1;
				else
					nc.nDhcp = 0;
				nc.strIp = ui.lineEdit_Ip_2->text().toStdString();
				nc.strGateway = ui.lineEdit_GateWay_2->text().toStdString();
				nc.strNetmask = ui.lineEdit_Net_AsK_2->text().toStdString();
				nc.strDns1 = ui.lineEdit_DNS_3->text().toStdString();
				nc.strDns2 = ui.lineEdit_DNS_4->text().toStdString();
				newList.push_back(nc);
			}
		}
	}

	for (itor = newList.begin(); itor != newList.end(); ++itor, i++)
		{
			NETWORK_CONF nc = *itor;

			Dhcp.m_sID = NET_DHCP;
			Dhcp.m_length = sizeof(nc.nDhcp);

			DevName.m_sID = NET_DEV_NAME;
			DevName.m_length = nc.strDevName.size();

			Ip.m_sID = NET_IP;
			Ip.m_length = nc.strIp.size();

			Netmask.m_sID = NET_NETMASK;
			Netmask.m_length = nc.strNetmask.size();

			Gateway.m_sID = NET_GATEWAY;
			Gateway.m_length = nc.strGateway.size();

		Dns1.m_sID = REMOTE_DNS;
		Dns1.m_length = nc.strDns1.size();

		Dns2.m_sID = REMOTE_DNS;
		Dns2.m_length = nc.strDns2.size();

			memcpy(pos, &Dhcp, sizeof(Dhcp));
			pos += sizeof(Dhcp);
			memcpy(pos, &nc.nDhcp, Dhcp.m_length);
			pos += Dhcp.m_length;

			memcpy(pos, &DevName, sizeof(DevName));
			pos += sizeof(DevName);
			memcpy(pos, nc.strDevName.c_str(), DevName.m_length);
			pos += DevName.m_length;

			memcpy(pos, &Ip, sizeof(Ip));
			pos += sizeof(Ip);
			memcpy(pos, nc.strIp.c_str(), Ip.m_length);
			pos += Ip.m_length;

			memcpy(pos, &Netmask, sizeof(Netmask));
			pos += sizeof(Netmask);
			memcpy(pos, nc.strNetmask.c_str(), Netmask.m_length);
			pos += Netmask.m_length;

			memcpy(pos, &Gateway, sizeof(Gateway));
			pos += sizeof(Gateway);
			memcpy(pos, nc.strGateway.c_str(), Gateway.m_length);
			pos += Gateway.m_length;

		memcpy(pos, &Dns1, sizeof(Dns1));
		pos += sizeof(Dns1);
		memcpy(pos, nc.strDns1.c_str(), Dns1.m_length);
		pos += Dns1.m_length;

		memcpy(pos, &Dns2, sizeof(Dns2));
		pos += sizeof(Dns2);
		memcpy(pos, nc.strDns2.c_str(), Dns2.m_length);
		pos += Dns2.m_length;
	}

	sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 2048);
	bUpdateNetWork = 1;
	ui.radioButtonDhcp_2->setEnabled(false);
	ui.radioButtonStatic_2->setEnabled(false);
	ui.radioButtonDhcp->setEnabled(false);
	ui.radioButtonStatic->setEnabled(false);
	Interface1Enable(false);
	Interface2Enable(false);
	TMS_start();
#endif
#if 0
	QMessageBox::information(this, tr("Save"), tr("Save success! It will be reboot and make sure the setting works."));

	extern QString rIp;
	if(rIp == "127.0.0.1")
	{
#ifdef WIN32
		if(m_ConnectStatus==2)    
		{
			KL *pKL = (KL*)buf;
			int i;

			pKL->m_pkgHead = 0x7585;
			pKL->m_keyID = S_REBOOT;
			pKL->m_length = 1;
			buf[sizeof(KL)] = 1;
			pSocket->write(buf, sizeof(KL) + 1);
			pSocket->waitForBytesWritten(-1);
			pSocket->waitForReadyRead(-1);

			i = pSocket->read(buf, 2048);
			if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_REBOOT)
			{
				QMessageBox::information(this,"Information",tr("Reboot successful"));
				return;
			}
			}
		else
		{
			//ʾʾѾ
			// ʧ
			QMessageBox::information(this,"Information",tr("Reboot failed,network errors, operation failed"));
			return;
		}
#else
		system("/bin/sync");
		system("/sbin/reboot");
#endif
	}
	else
	{
		if(m_ConnectStatus==2)    
		{
			KL *pKL = (KL*)buf;
			int i;

			pKL->m_pkgHead = 0x7585;
			pKL->m_keyID = S_REBOOT;
			pKL->m_length = 1;
			buf[sizeof(KL)] = 1;
			pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);

	i = pSocket->read(buf, 2048);
			if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_REBOOT)
			{
				QMessageBox::information(this,"Information",tr("Reboot successful"));
				return;
			}
		}
		else
		{
			//ʾʾѾ
			// ʧ
			QMessageBox::information(this,"Information",tr("Reboot failed,network errors, operation failed"));
			return;
		}
	}
#endif
}

//Restore satellite setting
void Setup::on_pushButton_2_clicked()
{
	TUNER_CONF tuner;
	ui.lineEdit_Frequency->setText(QString::number(tuner.nFreq));
	ui.lineEdit_symbol_rate->setText(QString::number(tuner.nSR));
	int nIndex = ui.comboBox_DVB->findText(QString(tuner.strDelSys.c_str()));
	if (nIndex != -1)
	{
		ui.comboBox_DVB->setCurrentIndex(nIndex);
	}
	else
	{
		ui.comboBox_DVB->insertItem(0, QString(tuner.strDelSys.c_str()));
		ui.comboBox_DVB->setCurrentIndex(0);
	}

	nIndex = ui.comboBox_xy->findText(QString(tuner.strFec.c_str()));
	if (nIndex != -1)
	{
		ui.comboBox_xy->setCurrentIndex(nIndex);
	}
	else
	{
		ui.comboBox_xy->insertItem(0, QString(tuner.strFec.c_str()));
		ui.comboBox_xy->setCurrentIndex(0);
	}

	nIndex = ui.comboBox_V->findText(QString(tuner.strPolVert.c_str()));
	if (nIndex != -1)
	{
		ui.comboBox_V->setCurrentIndex(nIndex);
	}
	else
	{
		ui.comboBox_V->insertItem(0, QString(tuner.strPolVert.c_str()));
		ui.comboBox_V->setCurrentIndex(0);
	}

	nIndex = ui.comboBox_SPK->findText(QString(tuner.strModulation.c_str()));
	if (nIndex != -1)
	{
		ui.comboBox_SPK->setCurrentIndex(nIndex);
	}
	else
	{
		ui.comboBox_SPK->insertItem(0, QString(tuner.strModulation.c_str()));
		ui.comboBox_SPK->setCurrentIndex(0);
	}

	nIndex = ui.comboBox_decimals->findText(QString(tuner.strRollOff.c_str()));
	if (nIndex != -1)
	{
		ui.comboBox_decimals->setCurrentIndex(nIndex);
	}
	else
	{
		ui.comboBox_decimals->insertItem(0, QString(tuner.strRollOff.c_str()));
		ui.comboBox_decimals->setCurrentIndex(0);
	}
}

//Switch netconfig setting with difference netdev
#if 0
void Setup::on_comboBox_ECHs_currentIndexChanged(QString name)
{
	std::list<NETWORK_CONF>::iterator itor;

	NETWORK_CONF nc;
	//Save config into list
	int i = 0;
	for (itor = m_list.begin(); itor != m_list.end(); ++itor, i++)
	{
		nc = *itor;
		if(ethIndex == i)
		{
			if(ui.radioButtonDhcp->isChecked())
				(*itor).nDhcp = 1;
			else
				(*itor).nDhcp = 0;
			(*itor).strIp = ui.lineEdit_Ip->text().toStdString();
			(*itor).strNetmask = ui.lineEdit_Net_AsK->text().toStdString();
			(*itor).strGateway = ui.lineEdit_GateWay->text().toStdString();
		}
	}

	i = 0;
	for (itor = m_list.begin(); itor != m_list.end(); ++itor, i++)
	{
		nc = *itor;
		if(nc.strDevName == name.toStdString())
		{
			ethIndex = i;
			ui.lineEdit_Ip->setText(nc.strIp.c_str());
			ui.lineEdit_Net_AsK->setText(nc.strNetmask.c_str());
			ui.lineEdit_GateWay->setText(nc.strGateway.c_str());
			if (nc.nDhcp)
				ui.checkBox_DHCP->setCheckState(Qt::Checked);
			else
				ui.checkBox_DHCP->setCheckState(Qt::Unchecked);
		}
	}
}
#endif

#include <qdatetime.h>
void Setup::getTMS()      //ȡ־
{
    	char *buf = new char[1024*1024];
	KL *pKL = (KL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = S_GET_TMS;

  //  QDate Date = QDate::currentDate();//ȡ
    QDateTime time = QDateTime::currentDateTime();//ȡϵͳڵʱ
	//QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //ʾʽ
	QDate Date=time.date();

	QDate DateAfter=Date.addDays(-3);   //ȡǰ-޸Ҫȡǰ־

    LOGDATE LogData;  //

    LogData.after_year=DateAfter.year();
	LogData.after_month=DateAfter.month();
	LogData.after_day=DateAfter.day();
	LogData.before_year=Date.year();
	LogData.before_month=Date.month();
	LogData.before_day=Date.day();

/*
    LogData.after_year=Date.year();
	LogData.after_month=Date.month();
	LogData.after_day=Date.day()-1;
	LogData.before_year=Date.year();
	LogData.before_month=Date.month();
	LogData.before_day=Date.day();
*/
	printf("Date After: %d %d %d\n",DateAfter.year(),DateAfter.month(),DateAfter.day());
	printf("Date Before: %d %d %d\n",Date.year(),Date.month(),Date.day());
/*
    LogData.after_year=2015;
	LogData.after_month=11;
	LogData.after_day=30;
	LogData.before_year=2015;
	LogData.before_month=11;
	LogData.before_day=30;
*/	

	void* pos = buf + sizeof(KL);
    memcpy(pos,&LogData,sizeof(LOGDATE));   //uiݹ

	pKL->m_length = sizeof(LOGDATE);

    int sendsize=sizeof(KL)+sizeof(LOGDATE);
	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i= pSocket->read(buf, 1024*1024);

	//֤ݽ
	int total = pKL->m_length + sizeof(KL);
	int r = total - i;
	char* aaa = (char*)buf + i;
	while(r)
	{
		pSocket->waitForReadyRead(-1);
		i = pSocket->read(aaa, r);
		aaa += i;
		r -= i;
	}

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_GET_TMS)
	{
		QString Data;//="Current Date:"+QString::number(Date.year())+"-"+QString::number(Date.month())+"-"+QString::number(Date.day())+" ";  
		int log_size = pKL->m_length;
        if(log_size<=1) 
		{
	        ui.textBrowser->setText(Data);   
			delete[] buf;
			return;
		}
		void* pos  =buf+sizeof(KL);
		char* cstr_log=new char[log_size+1];   //ȡlog־
        if(NULL==cstr_log) 
		{
			delete[] buf;
			return;
		}
        memcpy(cstr_log,pos,log_size);
	    QString txt_Log=QString(QLatin1String(cstr_log));    // char * תQString
	    
		ui.textBrowser->setText(txt_Log);    
		delete[] cstr_log;
		delete[] buf;
	   }
}

void Setup::TMS_start()   //ȡ־ʱ
{

// 	this->getTMS();  //״λȡ 
	m_TMS_timer = startTimer(1*1000);   //ܿʼһʱ0
//	printf("m_TMS_timer=%d\n",m_TMS_timer);
}

void Setup::TMS_stop()    //رջȡ־ʱ
{
    if(m_TMS_timer>0)
	{
       killTimer(m_TMS_timer);  
	   m_TMS_timer = -1; 
	}
}


void Setup::on_pushButton_5_clicked()
{
	char *buf = new char[1024];
	KL *pKL = (KL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KL);
	*((uint32*)pos) = 0x10000002;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KL)+sizeof(uint32);
	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i= pSocket->read(buf, 1024);
}

void Setup::on_pushButton_6_clicked()
{
	char *buf = new char[1024];
	KL *pKL = (KL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KL);
	*((uint32*)pos) = 0x10000001;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KL)+sizeof(uint32);
	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i= pSocket->read(buf, 1024);
}

void Setup::on_pushButton_7_clicked()
{
	char *buf = new char[1024];
	KL *pKL = (KL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KL);
	*((uint32*)pos) = 0x10000003;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KL)+sizeof(uint32);
	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i= pSocket->read(buf, 1024);
}

void Setup::on_pushButton_8_clicked()
{
	char *buf = new char[1024];
	KL *pKL = (KL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KL);
	*((uint32*)pos) = 0x10000004;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KL)+sizeof(uint32);
	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i= pSocket->read(buf, 1024);
}

void Setup::on_pushButton_9_clicked()
{
	char *buf = new char[1024];
	KL *pKL = (KL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KL);
	*((uint32*)pos) = 0x10000005;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KL)+sizeof(uint32);
	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i= pSocket->read(buf, 1024);
}

void Setup::on_pushButton_10_clicked()
{
	char *buf = new char[1024];
	KL *pKL = (KL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KL);
	*((uint32*)pos) = 0x10000006;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KL)+sizeof(uint32);
	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i= pSocket->read(buf, 1024);
}

void Setup::on_pushButton_11_clicked()
{
	char *buf = new char[1024];
	KL *pKL = (KL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KL);
	*((uint32*)pos) = 0x10000007;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KL)+sizeof(uint32);
	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i= pSocket->read(buf, 1024);
}

void Setup::on_pushButton_12_clicked()
{
	char *buf = new char[1024];
	KL *pKL = (KL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KL);
	*((uint32*)pos) = 0;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KL)+sizeof(uint32);
	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i= pSocket->read(buf, 1024);
}

void Setup::on_radioButtonDhcp_toggled(bool)
{
	Interface1Enable(false);
}

void Setup::on_radioButtonStatic_toggled(bool)
{
	Interface1Enable(true);
}

void Setup::on_radioButtonStatic_2_toggled(bool)
{
	Interface2Enable(true);
}

void Setup::on_radioButtonDhcp_2_toggled(bool)
{
	Interface2Enable(false);
}

void Setup::on_lineEdit_Ip_editingFinished()
{
	QString s = ui.lineEdit_Ip->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		bool bWarn = false;
		if(l.at(0).toUInt() == 0)
		{
			bWarn = true;
			s = "1";
		}
		else if(l.at(0).toUInt()> 223)
		{
			bWarn = true;
			s = "223";
		}
		else
			s = l.at(0);
		if(bWarn)
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of IP address \"AAA.BBB.CCC.CCC\" is \"1-223\""));
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of IP address \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of IP address \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of IP address \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_Ip->setText(s);
}

void Setup::on_lineEdit_Net_AsK_editingFinished()
{
	QString s = ui.lineEdit_Net_AsK->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		if(l.at(0).toUInt() > 255)
		{
			s = "255";
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of Netmask \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s = l.at(0);
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of Netmask \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of Netmask \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of Netmask \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_Net_AsK->setText(s);
}

void Setup::on_lineEdit_GateWay_editingFinished()
{
	QString s = ui.lineEdit_GateWay->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		bool bWarn = false;
		if(l.at(0).toUInt() == 0)
		{
			bWarn = true;
			s = "1";
		}
		else if(l.at(0).toUInt()> 223)
		{
			bWarn = true;
			s = "223";
		}
		else
			s = l.at(0);
		if(bWarn)
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of Gateway \"AAA.BBB.CCC.CCC\" is \"1-223\""));
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of Gateway \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of Gateway \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of Gateway \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_GateWay->setText(s);
}

void Setup::on_lineEdit_DNS_editingFinished()
{
	QString s = ui.lineEdit_DNS->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		bool bWarn = false;
		if(l.at(0).toUInt() == 0)
		{
			bWarn = true;
			s = "1";
		}
		else if(l.at(0).toUInt()> 223)
		{
			bWarn = true;
			s = "223";
		}
		else
			s = l.at(0);
		if(bWarn)
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of DNS \"AAA.BBB.CCC.CCC\" is \"1-223\""));
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_DNS->setText(s);
}

void Setup::on_lineEdit_DNS_2_editingFinished()
{
	QString s = ui.lineEdit_DNS_2->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		bool bWarn = false;
		if(l.at(0).toUInt() == 0)
		{
			bWarn = true;
			s = "1";
		}
		else if(l.at(0).toUInt()> 223)
		{
			bWarn = true;
			s = "223";
		}
		else
			s = l.at(0);
		if(bWarn)
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of DNS \"AAA.BBB.CCC.CCC\" is \"1-223\""));
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_DNS_2->setText(s);
}

void Setup::on_lineEdit_Ip_2_editingFinished()
{
	QString s = ui.lineEdit_Ip_2->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		bool bWarn = false;
		if(l.at(0).toUInt() == 0)
		{
			bWarn = true;
			s = "1";
		}
		else if(l.at(0).toUInt()> 223)
		{
			bWarn = true;
			s = "223";
		}
		else
			s = l.at(0);
		if(bWarn)
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of IP address \"AAA.BBB.CCC.CCC\" is \"1-223\""));
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of IP address \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of IP address \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of IP address \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_Ip_2->setText(s);
}

void Setup::on_lineEdit_Net_AsK_2_editingFinished()
{
	QString s = ui.lineEdit_Net_AsK_2->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		if(l.at(0).toUInt() > 255)
		{
			s = "255";
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of Netmask \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s = l.at(0);
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of Netmask \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of Netmask \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of Netmask \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_Net_AsK_2->setText(s);
}

void Setup::on_lineEdit_GateWay_2_editingFinished()
{
	QString s = ui.lineEdit_GateWay_2->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		bool bWarn = false;
		if(l.at(0).toUInt() == 0)
		{
			bWarn = true;
			s = "1";
		}
		else if(l.at(0).toUInt()> 223)
		{
			bWarn = true;
			s = "223";
		}
		else
			s = l.at(0);
		if(bWarn)
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of Gateway \"AAA.BBB.CCC.CCC\" is \"1-223\""));
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of Gateway \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of Gateway \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of Gateway \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_GateWay_2->setText(s);
}

void Setup::on_lineEdit_DNS_3_editingFinished()
{
	QString s = ui.lineEdit_DNS_3->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		bool bWarn = false;
		if(l.at(0).toUInt() == 0)
		{
			bWarn = true;
			s = "1";
		}
		else if(l.at(0).toUInt()> 223)
		{
			bWarn = true;
			s = "223";
		}
		else
			s = l.at(0);
		if(bWarn)
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of DNS \"AAA.BBB.CCC.CCC\" is \"1-223\""));
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_DNS_3->setText(s);
}

void Setup::on_lineEdit_DNS_4_editingFinished()
{
	QString s = ui.lineEdit_DNS_4->text();
	QStringList l = s.split(".");
	if(l.at(0) == "")
		s = "0";
	else
	{
		bool bWarn = false;
		if(l.at(0).toUInt() == 0)
		{
			bWarn = true;
			s = "1";
		}
		else if(l.at(0).toUInt()> 223)
		{
			bWarn = true;
			s = "223";
		}
		else
			s = l.at(0);
		if(bWarn)
			QMessageBox::warning(this, tr("Warning"), tr("The AAA range of DNS \"AAA.BBB.CCC.CCC\" is \"1-223\""));
	}
	s += ".";
	if(l.at(1) == "")
		s += "0";
	else
	{
		if(l.at(1).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The BBB range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(1);
	}
	s += ".";
	if(l.at(2) == "")
		s += "0";
	else
	{
		if(l.at(2).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The CCC range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(2);
	}
	s += ".";
	if(l.at(3) == "")
		s += "0";
	else
	{
		if(l.at(3).toUInt() > 255)
		{
			s += "255";
			QMessageBox::warning(this, tr("Warning"), tr("The DDD range of DNS \"AAA.BBB.CCC.CCC\" is \"0-255\""));
		}
		else
			s += l.at(3);
	}
	ui.lineEdit_DNS_4->setText(s);
}

void Setup::on_pushButtonApply1_clicked()
{
	TMS_stop();
	char buf[2048];
	KL* pKL = (KL*)buf;
	pKL->m_pkgHead = 0x7585;

	//Set network
	on_lineEdit_Ip_editingFinished();
	on_lineEdit_Net_AsK_editingFinished();
	on_lineEdit_GateWay_editingFinished();
	// 	on_lineEdit_DNS_editingFinished();
	// 	on_lineEdit_DNS_2_editingFinished();

	pKL->m_keyID = N_SET_CONFIG;
	SL DevName, Dns1, Dns2, Dhcp, Ip, Netmask, Gateway;
	char *pos = buf + sizeof(KL);
	if(m_list.size() > 0)
	{
		std::list<NETWORK_CONF>::iterator itor = m_list.begin();

		NETWORK_CONF nc = *itor;

		if(ui.groupBox_3->isEnabled())
		{
			if(ui.radioButtonDhcp->isChecked())
				nc.nDhcp = 1;
			else
				nc.nDhcp = 0;
			nc.strIp = ui.lineEdit_Ip->text().toStdString();
			nc.strGateway = ui.lineEdit_GateWay->text().toStdString();
			nc.strNetmask = ui.lineEdit_Net_AsK->text().toStdString();
			nc.strDns1 = ui.lineEdit_DNS->text().toStdString();
			nc.strDns2 = ui.lineEdit_DNS_2->text().toStdString();
		}
		Dhcp.m_sID = NET_DHCP;
		Dhcp.m_length = sizeof(nc.nDhcp);

		DevName.m_sID = NET_DEV_NAME;
		DevName.m_length = nc.strDevName.size();

		Ip.m_sID = NET_IP;
		Ip.m_length = nc.strIp.size();

		Netmask.m_sID = NET_NETMASK;
		Netmask.m_length = nc.strNetmask.size();

		Gateway.m_sID = NET_GATEWAY;
		Gateway.m_length = nc.strGateway.size();

		Dns1.m_sID = REMOTE_DNS;
		Dns1.m_length = nc.strDns1.size();

		Dns2.m_sID = REMOTE_DNS;
		Dns2.m_length = nc.strDns2.size();

		memcpy(pos, &Dhcp, sizeof(Dhcp));
		pos += sizeof(Dhcp);
		memcpy(pos, &nc.nDhcp, Dhcp.m_length);
		pos += Dhcp.m_length;

		memcpy(pos, &DevName, sizeof(DevName));
		pos += sizeof(DevName);
		memcpy(pos, nc.strDevName.c_str(), DevName.m_length);
		pos += DevName.m_length;

		memcpy(pos, &Ip, sizeof(Ip));
		pos += sizeof(Ip);
		memcpy(pos, nc.strIp.c_str(), Ip.m_length);
		pos += Ip.m_length;

		memcpy(pos, &Netmask, sizeof(Netmask));
		pos += sizeof(Netmask);
		memcpy(pos, nc.strNetmask.c_str(), Netmask.m_length);
		pos += Netmask.m_length;

		memcpy(pos, &Gateway, sizeof(Gateway));
		pos += sizeof(Gateway);
		memcpy(pos, nc.strGateway.c_str(), Gateway.m_length);
		pos += Gateway.m_length;

		memcpy(pos, &Dns1, sizeof(Dns1));
		pos += sizeof(Dns1);
		memcpy(pos, nc.strDns1.c_str(), Dns1.m_length);
		pos += Dns1.m_length;

		memcpy(pos, &Dns2, sizeof(Dns2));
		pos += sizeof(Dns2);
		memcpy(pos, nc.strDns2.c_str(), Dns2.m_length);
		pos += Dns2.m_length;

		int sendsize = (int)((char*)pos - buf);
		pKL->m_length = sendsize - sizeof(KL);

		pSocket->write(buf, sendsize);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);
		int i = pSocket->read(buf, 2048);
	}

	bUpdateNetWork = 1;
	ui.radioButtonDhcp->setEnabled(false);
	ui.radioButtonStatic->setEnabled(false);
	ui.pushButtonApply1->setEnabled(false);
	Interface1Enable(false);
	TMS_start();
}

void Setup::on_pushButtonApply2_clicked()
{
	TMS_stop();
	//Set Satellite
	char buf[2048];
	KL* pKL = (KL*)buf;
	pKL->m_pkgHead = 0x7585;


	SL DevName;

#if 1
	//Set network
	on_lineEdit_Ip_2_editingFinished();
	on_lineEdit_Net_AsK_2_editingFinished();
	on_lineEdit_GateWay_2_editingFinished();

	pKL->m_keyID = N_SET_CONFIG;
	SL Dns1, Dns2, Dhcp, Ip, Netmask, Gateway;
	char *pos = buf + sizeof(KL);
	if(m_list.size() > 1)
	{
		std::list<NETWORK_CONF>::iterator itor = m_list.begin();
		itor++;
		NETWORK_CONF nc = *itor;

		if(ui.groupBox_4->isEnabled())
		{
			if(ui.radioButtonDhcp_2->isChecked())
				nc.nDhcp = 1;
			else
				nc.nDhcp = 0;
			nc.strIp = ui.lineEdit_Ip_2->text().toStdString();
			nc.strGateway = ui.lineEdit_GateWay_2->text().toStdString();
			nc.strNetmask = ui.lineEdit_Net_AsK_2->text().toStdString();
			nc.strDns1 = ui.lineEdit_DNS_3->text().toStdString();
			nc.strDns2 = ui.lineEdit_DNS_4->text().toStdString();
		}
		Dhcp.m_sID = NET_DHCP;
		Dhcp.m_length = sizeof(nc.nDhcp);

		DevName.m_sID = NET_DEV_NAME;
		DevName.m_length = nc.strDevName.size();

		Ip.m_sID = NET_IP;
		Ip.m_length = nc.strIp.size();

		Netmask.m_sID = NET_NETMASK;
		Netmask.m_length = nc.strNetmask.size();

		Gateway.m_sID = NET_GATEWAY;
		Gateway.m_length = nc.strGateway.size();

		Dns1.m_sID = REMOTE_DNS;
		Dns1.m_length = nc.strDns1.size();

		Dns2.m_sID = REMOTE_DNS;
		Dns2.m_length = nc.strDns2.size();

		memcpy(pos, &Dhcp, sizeof(Dhcp));
		pos += sizeof(Dhcp);
		memcpy(pos, &nc.nDhcp, Dhcp.m_length);
		pos += Dhcp.m_length;

		memcpy(pos, &DevName, sizeof(DevName));
		pos += sizeof(DevName);
		memcpy(pos, nc.strDevName.c_str(), DevName.m_length);
		pos += DevName.m_length;

		memcpy(pos, &Ip, sizeof(Ip));
		pos += sizeof(Ip);
		memcpy(pos, nc.strIp.c_str(), Ip.m_length);
		pos += Ip.m_length;

		memcpy(pos, &Netmask, sizeof(Netmask));
		pos += sizeof(Netmask);
		memcpy(pos, nc.strNetmask.c_str(), Netmask.m_length);
		pos += Netmask.m_length;

		memcpy(pos, &Gateway, sizeof(Gateway));
		pos += sizeof(Gateway);
		memcpy(pos, nc.strGateway.c_str(), Gateway.m_length);
		pos += Gateway.m_length;

		memcpy(pos, &Dns1, sizeof(Dns1));
		pos += sizeof(Dns1);
		memcpy(pos, nc.strDns1.c_str(), Dns1.m_length);
		pos += Dns1.m_length;

		memcpy(pos, &Dns2, sizeof(Dns2));
		pos += sizeof(Dns2);
		memcpy(pos, nc.strDns2.c_str(), Dns2.m_length);
		pos += Dns2.m_length;
		
		int sendsize = (int)((char*)pos - buf);
		pKL->m_length = sendsize - sizeof(KL);

		pSocket->write(buf, sendsize);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);
		int i = pSocket->read(buf, 2048);
	}
#endif

	bUpdateNetWork = 1;
	ui.radioButtonDhcp_2->setEnabled(false);
	ui.radioButtonStatic_2->setEnabled(false);
	ui.pushButtonApply2->setEnabled(false);
	Interface2Enable(false);
	TMS_start();
}

#ifdef WIN32
#include <Windows.h>
#endif
void Setup::on_pushButtonApply2_2_clicked()
{
	char buf[2048];
	KL* pKL = (KL*)buf;
	pKL->m_pkgHead = 0x7585;

	//Set remote
	pKL->m_keyID = N_SET_REMOTE;
	REMOTE_CONF rc;
/*
	rc.strDns1 = ui.lineEdit_DNS->text().toStdString();
	rc.strDns2 = ui.lineEdit_DNS_2->text().toStdString();
*/
	QString src = ui.lineEdit_Remote_2->text();
	src = src.remove(" ");
	if(src.isEmpty() || src == "")
	{
		QMessageBox::warning(this, tr("Warning"), tr("Remote can not empty."));
		return;
	}
	for(int i = 0; i < src.size(); i++)
	{
		if(src.at(i) > 0x127)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Doesn't support non ASCII."));
			return;
		}
	}
	ui.lineEdit_Remote_2->setEnabled(false);
	ui.lineEdit_Remote_3->setEnabled(false);
	ui.pushButtonApply2_2->setEnabled(false);
	ui.pushButton_13->setEnabled(false);

	rc.strRemote = src.toStdString();//ui.lineEdit_Remote_2->text().toStdString();
	rc.nPort = ui.lineEdit_Remote_3->text().toUInt(); //You need to change for get value from UI
	if(rc.nPort > 65535)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Port range is \"1-65535\""));
		rc.nPort = 65535;
		ui.lineEdit_Remote_3->setText("65535");
	}
	if(rc.nPort == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Port range is \"1-65535\""));
		rc.nPort = 1;
		ui.lineEdit_Remote_3->setText("1");
	}

	SL /*Dns1, Dns2, */Remote, Port;
/*
	Dns1.m_sID = REMOTE_DNS;
	Dns1.m_length = rc.strDns1.size();

	Dns2.m_sID = REMOTE_DNS;
	Dns2.m_length = rc.strDns2.size();
*/

	Remote.m_sID = REMOTE_SERVER;
	Remote.m_length = rc.strRemote.size();

	Port.m_sID = REMOTE_PORT;
	Port.m_length = sizeof(rc.nPort);

	char* pos = buf + sizeof(KL);

/*
	memcpy(pos, &Dns1, sizeof(Dns1));
	pos += sizeof(Dns1);
	memcpy(pos, rc.strDns1.c_str(), Dns1.m_length);
	pos += Dns1.m_length;

	memcpy(pos, &Dns2, sizeof(Dns2));
	pos += sizeof(Dns2);
	memcpy(pos, rc.strDns2.c_str(), Dns2.m_length);
	pos += Dns2.m_length;
*/

	memcpy(pos, &Remote, sizeof(Remote));
	pos += sizeof(Remote);
	memcpy(pos, rc.strRemote.c_str(), Remote.m_length);
	pos += Remote.m_length;

	memcpy(pos, &Port, sizeof(Port));
	pos += sizeof(Port);
	memcpy(pos, &rc.nPort, sizeof(rc.nPort));
	DPRINTF("port %d\n", rc.nPort);
	pos += Port.m_length;

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i = pSocket->read(buf, 2048);

	LoadRemoteConfig();

	GetRemoteConnectStatus(1);
	remIndex = 0;
}

void Setup::on_pushButton_13_clicked()
{
	ui.lineEdit_Remote_2->setText("www.chinafilmdsd.com");
	ui.lineEdit_Remote_3->setText("8000");
}

void Setup::GetRemoteConnectStatus(int i /* = 0 */)
{
	if(i == 0)
	{
		char buf[1024];
		KL *pKL = (KL*)buf;
		int i;

		//Get Receive Status
		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = S_GET_RECEIVE;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);
		i = pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_GET_RECEIVE)
		{
			RECEIVE_INFO recv;
			int datalen = sizeof(uint64) * 6
				+ sizeof(uint32)
				+ sizeof(uint32);
			memcpy(&recv, buf + sizeof(KL), datalen);

			int len = pKL->m_length - datalen;
			int pos = sizeof(KL) + datalen;
			while (len > 0)
			{
				SL *sl = (SL*)(buf+pos);
				std::string tmp;
				pos += sizeof(SL);
				len -= sizeof(SL);
				if (sl->m_length > 0)
				{
					char *s = new char[sl->m_length+1];
					memset(s, 0, sl->m_length+1);
					memcpy(s, buf+pos, sl->m_length);
					tmp = s;
					pos += sl->m_length;
					len -= sl->m_length;
					delete[] s;
				}
				else
				{
					tmp = "";
				}

				switch(sl->m_sID)
				{
				case RECEIVE_FILM_NAME:
					recv.strFilmName = tmp;
					break;
				case  RECEIVE_FILM_UUID:
					recv.strUuid = tmp;
					break;
				case RECEIVE_FILM_ISSUE_DATE:
					recv.strIssueDate = tmp;
					break;
				case RECEIVE_FILM_ISSUER:
					recv.strIssuer = tmp;
					break;
				case RECEIVE_FILM_CREATOR:
					recv.strCreator = tmp;
					break;
				case RECEIVE_EXTEND:
					recv.strExtend = tmp;
					break;
				}
			}
			if (recv.strExtend.find("REMOTE:1") != std::string::npos)
				ui.labelRemoteStatus->setText(tr("Connected."));
			else
				ui.labelRemoteStatus->setText(tr("Connecting..."));
		}
	}
	else if(i == 1)
	{
		ui.labelRemoteStatus->setText(tr("Connecting..."));
	}
	else if(i == 2)
	{
		char buf[1024];
		KL *pKL = (KL*)buf;
		int i;

		//Get Receive Status
		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = S_GET_RECEIVE;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);
		i = pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_GET_RECEIVE)
		{
			RECEIVE_INFO recv;
			int datalen = sizeof(uint64) * 6
				+ sizeof(uint32)
				+ sizeof(uint32);
			memcpy(&recv, buf + sizeof(KL), datalen);

			int len = pKL->m_length - datalen;
			int pos = sizeof(KL) + datalen;
			while (len > 0)
			{
				SL *sl = (SL*)(buf+pos);
				std::string tmp;
				pos += sizeof(SL);
				len -= sizeof(SL);
				if (sl->m_length > 0)
				{
					char *s = new char[sl->m_length+1];
					memset(s, 0, sl->m_length+1);
					memcpy(s, buf+pos, sl->m_length);
					tmp = s;
					pos += sl->m_length;
					len -= sl->m_length;
					delete[] s;
				}
				else
				{
					tmp = "";
				}

				switch(sl->m_sID)
				{
				case RECEIVE_FILM_NAME:
					recv.strFilmName = tmp;
					break;
				case  RECEIVE_FILM_UUID:
					recv.strUuid = tmp;
					break;
				case RECEIVE_FILM_ISSUE_DATE:
					recv.strIssueDate = tmp;
					break;
				case RECEIVE_FILM_ISSUER:
					recv.strIssuer = tmp;
					break;
				case RECEIVE_FILM_CREATOR:
					recv.strCreator = tmp;
					break;
				case RECEIVE_EXTEND:
					recv.strExtend = tmp;
					break;
				}
			}
		}
	}
}

void Setup::on_lineEdit_Frequency_editingFinished()
{
#if 0
	unsigned int frq = ui.lineEdit_Frequency->text().toUInt();
	if(frq < 12250000)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Frequency must above 12250000."));
		ui.lineEdit_Frequency->setText("12250000");
		return;
	}
	if(frq > 13450000)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Frequency must less 13450000."));
		ui.lineEdit_Frequency->setText("13450000");
	}
#endif
}