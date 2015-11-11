#include "setup.h"

Setup::Setup(QTcpSocket* pSocket, QWidget *parent)
	: QWidget(parent), pSocket(NULL)
{
	ui.setupUi(this);
	Init();
	this->pSocket = pSocket;
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
	ui.label_Netm_mask->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_DNS->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.label_DNS_2->setStyleSheet("QLabel{font-size:20px;font-family:'Book Antiqua';}");
	ui.lineEdit_Ip->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_GateWay->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_Net_AsK->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_DNS->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_DNS_2->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");

	ui.comboBox_ECHs->setStyleSheet("QComboBox{font-size:18px;font-family:'Book Antiqua';}");
	ui.checkBox_DHCP->setStyleSheet("QCheckBox{font-size:20px;font-family:'Book Antiqua';}");
	ui.lineEdit_Remote_2->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
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
			ui.comboBox_decimals->insertItem(0, QString(tuner.strRollOff.c_str()));
			ui.comboBox_decimals->setCurrentIndex(0);
		}
	}

	//Load NetDevice setting
	m_list.clear();
	ui.comboBox_ECHs->clear();
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
				m_list.push_back(nc);
				break;
			}
		}
	}
	std::list<NETWORK_CONF>::iterator itor;
	i = 0;
	for(itor = m_list.begin(); itor != m_list.end(); ++itor, i++)
	{
		NETWORK_CONF nc = *itor;
		ui.comboBox_ECHs->insertItem(i, QString(nc.strDevName.c_str()));
		if(i == 0)
		{
			ui.lineEdit_Ip->setText(nc.strIp.c_str());
			ui.lineEdit_Net_AsK->setText(nc.strNetmask.c_str());
			ui.lineEdit_GateWay->setText(nc.strGateway.c_str());
			if (nc.nDhcp)
				ui.checkBox_DHCP->setCheckState(Qt::Checked);
			else
				ui.checkBox_DHCP->setCheckState(Qt::Unchecked);
			ethIndex = 0;
		}
	}

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
				if (dns == 0)
				{
					rc.strDns1 = tmp;
					dns++;
				}
				else
					rc.strDns2 = tmp;
				break;
			case  REMOTE_SERVER:
				rc.strRemote = tmp;
				break;
			}
		}
		ui.lineEdit_DNS->setText(rc.strDns1.c_str());
		ui.lineEdit_DNS_2->setText(rc.strDns2.c_str());
		ui.lineEdit_Remote_2->setText(rc.strRemote.c_str());
	}
}

//Save all setting
void Setup::on_pushButton_clicked()
{
	//Set Satellite
	TUNER_CONF tuner;
	char buf[2048];
	KL* pKL = (KL*)buf;
	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = S_SET_CONFIG;

	tuner.nFreq = ui.lineEdit_Frequency->text().toUInt();
	tuner.nSR = ui.lineEdit_symbol_rate->text().toUInt();
	tuner.strDelSys = ui.comboBox_DVB->currentText().toStdString();
	tuner.strFec = ui.comboBox_xy->currentText().toStdString();
	tuner.strPolVert = ui.comboBox_V->currentText().toStdString();
	tuner.strModulation = ui.comboBox_SPK->currentText().toStdString();
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

	//Set remote
	pKL->m_keyID = N_SET_REMOTE;
	REMOTE_CONF rc;
	rc.strDns1 = ui.lineEdit_DNS->text().toStdString();
	rc.strDns2 = ui.lineEdit_DNS_2->text().toStdString();
	rc.strRemote = ui.lineEdit_Remote_2->text().toStdString();

	SL Dns1, Dns2, Remote;
	Dns1.m_sID = REMOTE_DNS;
	Dns1.m_length = rc.strDns1.size();

	Dns2.m_sID = REMOTE_DNS;
	Dns2.m_length = rc.strDns2.size();

	Remote.m_sID = REMOTE_SERVER;
	Remote.m_length = rc.strRemote.size();

	pos = buf + sizeof(KL);

	memcpy(pos, &Dns1, sizeof(Dns1));
	pos += sizeof(Dns1);
	memcpy(pos, rc.strDns1.c_str(), Dns1.m_length);
	pos += Dns1.m_length;

	memcpy(pos, &Dns2, sizeof(Dns2));
	pos += sizeof(Dns2);
	memcpy(pos, rc.strDns2.c_str(), Dns2.m_length);
	pos += Dns2.m_length;

	memcpy(pos, &Remote, sizeof(Remote));
	pos += sizeof(Remote);
	memcpy(pos, rc.strRemote.c_str(), Remote.m_length);
	pos += Remote.m_length;

	sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 2048);

	//Set network
	pKL->m_keyID = N_SET_CONFIG;
	SL Dhcp, Ip, Netmask, Gateway;
	pos = buf + sizeof(KL);

	int currentIndex = ui.comboBox_ECHs->currentIndex();
	std::list<NETWORK_CONF>::iterator itor;
	i = 0;
	for (itor = m_list.begin(); itor != m_list.end(); ++itor, i++)
	{
		if(i != currentIndex)
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
		}
		else
		{
			NETWORK_CONF nc = *itor;
			if(ui.checkBox_DHCP->checkState() == Qt::Checked)
				nc.nDhcp = 1;
			else
				nc.nDhcp = 0;
			nc.strIp = ui.lineEdit_Ip->text().toStdString();
			nc.strNetmask = ui.lineEdit_Net_AsK->text().toStdString();
			nc.strGateway = ui.lineEdit_GateWay->text().toStdString();

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
			if (!nc.nDhcp)
			{
				extern QString rIp;
				rIp = nc.strIp.c_str();
			}
		}
	}

	sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	pSocket->write(buf, sendsize);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 2048);

// 	extern int m_ConnectStatus;
// 	m_ConnectStatus = 0;
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
			if(ui.checkBox_DHCP->checkState() == Qt::Checked)
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