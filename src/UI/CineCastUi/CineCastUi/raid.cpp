#include "raid.h"
#include "content.h"
#include <QCheckBox>
#include "delconfirm.h"
#include "copyfilm.h"

Raid::Raid(QTcpSocket * socket, QWidget *parent)
	: QWidget(parent), m_timer(-1), m_content(-1), m_raid(-1)
{
	ui.setupUi(this);
	this->pSocket = socket;
	Init();
}

Raid::~Raid()
{

}

void Raid::Init()
{
	ui.groupBox->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.groupBox_2->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.groupBox_3->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.groupBox_4->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.groupBox_5->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_7->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_6->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_5->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_4->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_3->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_2->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_9->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_8->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_11->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_13->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_14->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_15->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_17->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_16->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_19->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_18->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_20->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_21->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_22->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_23->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_24->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_25->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");

	ui.pushButton->setStyleSheet("QPushButton{font-size: 20px; font-family:'Book Antiqua';}");

	ui.label_4->setVisible(false);
	ui.label_5->setVisible(false);
	ui.label_10->setVisible(false);
	ui.label_11->setVisible(false);
	ui.label_12->setVisible(false);
	ui.label_13->setVisible(false);
	ui.label_14->setVisible(false);
	ui.label_15->setVisible(false);
	ui.label_16->setVisible(false);
	ui.label_17->setVisible(false);
	ui.label_18->setVisible(false);
	ui.label_19->setVisible(false);

	QFont font;
	font.setFamily("Book Antiqua");
	font.setPixelSize(18);
	ui.treeWidget_HDD_info->setStyleSheet("QTreeWidget{font-size: 18px; font-family:'Book Antiqua';}");
	ui.treeWidget_HDD_info_2->setStyleSheet("QTreeWidget{font-size: 18px; font-family:'Book Antiqua';}");
	ui.treeWidget_HDD_info->headerItem()->setFont(0, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(1, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(2, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(3, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(4, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(5, font);
// 	ui.treeWidget_HDD_info->headerItem()->setFont(6, font);
// 	ui.treeWidget_HDD_info->headerItem()->setFont(7, font);
	ui.treeWidget_HDD_info->setColumnWidth(0, 45);
// 	ui.treeWidget_HDD_info->setColumnWidth(1, 50);
	ui.treeWidget_HDD_info->setColumnWidth(1, 500);
// 	ui.treeWidget_HDD_info->setColumnWidth(3, 40);
	ui.treeWidget_HDD_info->setColumnWidth(3, 80);
	ui.treeWidget_HDD_info->setColumnWidth(4, 98);
	ui.treeWidget_HDD_info->setColumnWidth(5, 98);

	ui.treeWidget_HDD_info_2->headerItem()->setFont(0, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(1, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(2, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(3, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(4, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(5, font);
// 	ui.treeWidget_HDD_info_2->headerItem()->setFont(6, font);
// 	ui.treeWidget_HDD_info_2->headerItem()->setFont(7, font);
	ui.treeWidget_HDD_info_2->setColumnWidth(0, 45);
// 	ui.treeWidget_HDD_info_2->setColumnWidth(1, 50);
	ui.treeWidget_HDD_info_2->setColumnWidth(1, 500);
// 	ui.treeWidget_HDD_info_2->setColumnWidth(3, 40);
	ui.treeWidget_HDD_info_2->setColumnWidth(3, 80);
	ui.treeWidget_HDD_info_2->setColumnWidth(4, 98);
	ui.treeWidget_HDD_info_2->setColumnWidth(5, 98);

	ui.groupBox_2->setVisible(false);
#if 0
	ui.groupBox->setEnabled(false);
	ui.groupBox_2->setEnabled(false);
	ui.groupBox_3->setEnabled(false);
	ui.groupBox_4->setEnabled(false);
	ui.groupBox_5->setEnabled(false);
#endif
}

#include "../../../netcomm/UiProtocal.h"

void Raid::Start()
{
	loadState();
	if(m_timer == -1)
		m_timer = startTimer(5000);

	UpdateContent_HDD();
	if(m_content == -1)
		m_content = startTimer(1000);
	UpdateContent_RAID();
	if(m_raid == -1)
		m_raid = startTimer(1000);
}

void Raid::timerEvent(QTimerEvent *te)
{
	if(te->timerId() == m_timer)
	{
		loadState();
	}
	if(te->timerId() == m_content)
	{
		ContentisReady_HDD();
	}
	if(te->timerId() == m_raid)
	{
		ContentisReady_RAID();
	}
}

void Raid::Stop()
{
	if(m_timer != -1)
	{
		killTimer(m_timer);
		m_timer = -1;
	}
	if(m_content != -1)
	{
		killTimer(m_content);
		m_content = -1;
	}
	if(m_raid != -1)
	{
		killTimer(m_raid);
		m_raid = -1;
	}
}

extern  int m_ConnectStatus;    //״̬    =2=1=0
extern char buf[2048];   // Ѿŵǰ

void Raid::loadState()
{
	char buf[2048];
	KL *pKL = (KL*)buf;
	int i;
	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_GET_RAID_INFO;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 2048);
	
	if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == R_GET_RAID_INFO)
	{
		RAID_INFO ri;
		int datalen = sizeof(uint16) * 4 + sizeof(uint64) * 2;
		memcpy(&ri, buf + sizeof(KL), datalen);

		int len = pKL->m_length - datalen;
		int pos = sizeof(KL) + datalen;
		while(len > 0)
		{
			SL *sl = (SL*)(buf + pos);
			std::string tmp;
			pos += sizeof(SL);
			len -= sizeof(SL);
			if(sl->m_length > 0)
			{
				char *s = new char[sl->m_length + 1];
				memset(s, 0, sl->m_length + 1);
				memcpy(s, buf + pos, sl->m_length);
				tmp = s;
				pos += sl->m_length;
				len -= sl->m_length;
				delete[] s;
			}
			else
				tmp = "";
			switch(sl->m_sID)
			{
			case RAID_INFO_LEVEL:
				ri.strLevel = tmp;
				break;
			case RAID_INFO_STATE:
				ri.strState = tmp;
				break;
			case RAID_INFO_DEVSTATE:
				ri.strDevState.push_back(tmp);
				break;
			}
		}
		QString s = ri.strLevel.c_str();
		s = s.toUpper();
		ui.label_6->setText(s);
		ui.label_2->setText(QString::number(ri.nRaidDevices));
		s.sprintf("%.2fTB", (float)ri.nArraySize/1000000000000);
		ui.label_20->setText(s);
		s.sprintf("%.2fTB", (float)(ri.nArraySize - ri.nUsedSize)/1000000000000);
		ui.label_22->setText(s);
		s.sprintf("%.2fTB", (float)ri.nUsedSize/1000000000000);
		ui.label_24->setText(s);
		ui.label_8->setText(ri.strState.c_str());
		if (ri.strState.compare("clean") == 0)
			ui.label_8->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
		else
			ui.label_8->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

		QStringList lst;
		switch(ri.strDevState.size())
		{
		case 0:
			ui.label_10->setVisible(false);
			ui.label_11->setVisible(false);
			ui.label_12->setVisible(false);
			ui.label_13->setVisible(false);
			ui.label_14->setVisible(false);
			ui.label_15->setVisible(false);
			ui.label_16->setVisible(false);
			ui.label_17->setVisible(false);
			ui.label_18->setVisible(false);
			ui.label_19->setVisible(false);
			break;
		case 1:
			ui.label_10->setVisible(true);
			ui.label_11->setVisible(true);
			ui.label_12->setVisible(false);
			ui.label_13->setVisible(false);
			ui.label_14->setVisible(false);
			ui.label_15->setVisible(false);
			ui.label_16->setVisible(false);
			ui.label_17->setVisible(false);
			ui.label_18->setVisible(false);
			ui.label_19->setVisible(false);
			s = ri.strDevState.at(0).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_10->setText(s);
			if (lst.at(0) == "active")
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			break;
		case 2:
			ui.label_10->setVisible(true);
			ui.label_11->setVisible(true);
			ui.label_12->setVisible(true);
			ui.label_13->setVisible(true);
			ui.label_14->setVisible(false);
			ui.label_15->setVisible(false);
			ui.label_16->setVisible(false);
			ui.label_17->setVisible(false);
			ui.label_18->setVisible(false);
			ui.label_19->setVisible(false);
			s = ri.strDevState.at(0).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_10->setText(s);
			if (lst.at(0) == "active")
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(1).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_12->setText(s);
			if (lst.at(0) == "active")
				ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");
			break;
		case 3:
			ui.label_10->setVisible(true);
			ui.label_11->setVisible(true);
			ui.label_12->setVisible(true);
			ui.label_13->setVisible(true);
			ui.label_14->setVisible(true);
			ui.label_15->setVisible(true);
			ui.label_16->setVisible(false);
			ui.label_17->setVisible(false);
			ui.label_18->setVisible(false);
			ui.label_19->setVisible(false);
			s = ri.strDevState.at(0).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_10->setText(s);
			if (lst.at(0) == "active")
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(1).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_12->setText(s);
			if (lst.at(0) == "active")
				ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(2).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_15->setText(s);
			if (lst.at(0) == "active")
				ui.label_15->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_15->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");
			break;
		case 4:
			ui.label_10->setVisible(true);
			ui.label_11->setVisible(true);
			ui.label_12->setVisible(true);
			ui.label_13->setVisible(true);
			ui.label_14->setVisible(true);
			ui.label_15->setVisible(true);
			ui.label_16->setVisible(true);
			ui.label_17->setVisible(true);
			ui.label_18->setVisible(false);
			ui.label_19->setVisible(false);
			s = ri.strDevState.at(0).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_10->setText(s);
			if (lst.at(0) == "active")
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(1).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_12->setText(s);
			if (lst.at(0) == "active")
				ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(2).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_15->setText(s);
			if (lst.at(0) == "active")
				ui.label_15->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_15->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(3).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_16->setText(s);
			if (lst.at(0) == "active")
				ui.label_16->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_16->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");
			break;
		case 5:
			ui.label_10->setVisible(true);
			ui.label_11->setVisible(true);
			ui.label_12->setVisible(true);
			ui.label_13->setVisible(true);
			ui.label_14->setVisible(true);
			ui.label_15->setVisible(true);
			ui.label_16->setVisible(true);
			ui.label_17->setVisible(true);
			ui.label_18->setVisible(true);
			ui.label_19->setVisible(true);
			ui.label_10->setText(lst.at(0));

			s = ri.strDevState.at(0).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_10->setText(s);
			if (lst.at(0) == "active")
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(1).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_12->setText(s);
			if (lst.at(0) == "active")
				ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(2).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_15->setText(s);
			if (lst.at(0) == "active")
				ui.label_15->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_15->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(3).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_16->setText(s);
			if (lst.at(0) == "active")
				ui.label_16->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_16->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");

			s = ri.strDevState.at(4).c_str();
			lst = s.split("|");
			s = lst.at(1);
			lst = s.split(" ");
			ui.label_18->setText(s);
			if (lst.at(0) == "active")
				ui.label_18->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
			else
				ui.label_18->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#ff0000;}");
			break;
		}
	}
}

void Raid::LoadContent_HDD()
{
	if(m_ConnectStatus != 2)    //ȷѾӷ
		return;

	char *buf = new char[1024*1024];
	KL *pKL = (KL*)buf;

	int i;
	pKL->m_pkgHead = 0x7585;

	DISK_INFO di_hdd;
	//Load Disk info
	pKL->m_keyID = M_GET_HDD_INFO;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 50);
	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_GET_HDD_INFO)
	{
		memcpy(&di_hdd, buf + sizeof(KL), sizeof(di_hdd));
	}	

	//Load Content list
	pKL->m_keyID = M_GET_HDD_CONTENT_LIST;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);

	i = pSocket->read(buf, 1024*1024);

	int total = pKL->m_length + sizeof(KL);

	int r = total - i;
	char* aaa = (char*)buf + i;
	ui.treeWidget_HDD_info_2->clear();
	while(r)
	{
		pSocket->waitForReadyRead(-1);
		i = pSocket->read(aaa, r);
		aaa += i;
		r -= i;
	}

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_GET_HDD_CONTENT_LIST)
	{
		KL *pKL = (KL *)buf;
		int len = pKL->m_length;
		int pos  = sizeof(KL);

		ContentItem* item;

		ui.treeWidget_HDD_info_2->clear();
		while(len > 0)
		{
			SL *sl = (SL*)(buf+pos);
			std::string tmp;
			pos += sizeof(SL);
			len -= sizeof(SL);
			if (sl->m_length > 0)
			{
				char *s = new char[sl->m_length+1];
				memset(s, 0, sl->m_length+1);
				memcpy(s, buf + pos, sl->m_length);
				tmp = s;
				pos += sl->m_length;
				len -= sl->m_length;
				delete[] s;
			}
			else
				tmp = "";

			switch(sl->m_sID)
			{
			case CONTENT_ID:
				item = new ContentItem;
				item->pData[C_ID] = tmp;
				break;
			case CONTENT_NAME:
				item->pData[C_NAME] = tmp;
				break;
			case CONTENT_PROGRESS:
				item->pData[C_PROGRESS] = tmp + "%";
				break;
			case CONTENT_STATUS:
				item->pData[C_STATUS] = tmp;
				break;
			case CONTENT_FORMAT:
				item->pData[C_FORMAT] = tmp;
				break;
			case CONTENT_FILMLENGTH:
				item->pData[C_FILM_LENGTH] = tmp;
				break;
			case CONTENT_PROGRAMLENGTH:
				item->pData[C_PROGRAM_LENGTH] = tmp;
				break;
			case CONTENT_STEREOSCOPIC:
				item->pData[C_STEREO] = tmp;
				break;
			case CONTENT_ISSUER:
				item->pData[C_ISSUER] = tmp;
				break;
			case CONTENT_ISSUEDATE:
				item->pData[C_ISSUEDATE] = tmp;
				break;
			case CONTENT_TIMERANGE:
				item->pData[C_TIMERANGE] = tmp;
				break;
			case CONTENT_RECVSEGMENT:
				item->pData[C_RECVSEGMENT] = tmp;
				break;
			case CONTENT_TOTALSEGMENT:
				item->pData[C_TOTAL_SEGMENT] = tmp;
				break;
			case CONTENT_RECV_DATETIME:
				item->pData[C_RECV_DATETIME] = tmp;
				break;
			case CONTENT_LOCATE:
				item->pData[C_LOCATE] = tmp;
				item->MakeItem_HDD();

				if(item->pData[C_PROGRESS] == "100%")
				{
				ui.treeWidget_HDD_info_2->addTopLevelItem((QTreeWidgetItem*)(item));
				QCheckBox *ckBox = new QCheckBox;
				ui.treeWidget_HDD_info_2->setItemWidget((QTreeWidgetItem*)(item), 0, ckBox);
				}
				break;
			}
		}
	}

	delete[] buf;
	static int num1=0;
}

void Raid::LoadContent_RAID()
{
	if(m_ConnectStatus != 2)    //ȷѾӷ
		return;

	char *buf = new char[1024*1024];
	KL *pKL = (KL*)buf;

	int i;
	pKL->m_pkgHead = 0x7585;

	DISK_INFO di_hdd;
	//Load Disk info
	pKL->m_keyID = M_GET_RAID_INFO;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 50);
	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_GET_RAID_INFO)
	{
		memcpy(&di_hdd, buf + sizeof(KL), sizeof(di_hdd));
	}	

	//Load Content list
	pKL->m_keyID = M_GET_RAID_CONTENT_LIST;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);

	i = pSocket->read(buf, 1024*1024);

	int total = pKL->m_length + sizeof(KL);

	int r = total - i;
	char* aaa = (char*)buf + i;

	ui.treeWidget_HDD_info->clear();
	while(r)
	{
		pSocket->waitForReadyRead(-1);
		i = pSocket->read(aaa, r);
		aaa += i;
		r -= i;
	}

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_GET_RAID_CONTENT_LIST)
	{
		KL *pKL = (KL *)buf;
		int len = pKL->m_length;
		int pos  = sizeof(KL);

		ContentItem* item;

		ui.treeWidget_HDD_info->clear();
		while(len > 0)
		{
			SL *sl = (SL*)(buf+pos);
			std::string tmp;
			pos += sizeof(SL);
			len -= sizeof(SL);
			if (sl->m_length > 0)
			{
				char *s = new char[sl->m_length+1];
				memset(s, 0, sl->m_length+1);
				memcpy(s, buf + pos, sl->m_length);
				tmp = s;
				pos += sl->m_length;
				len -= sl->m_length;
				delete[] s;
			}
			else
				tmp = "";

			switch(sl->m_sID)
			{
			case CONTENT_ID:
				item = new ContentItem;
				item->pData[C_ID] = tmp;
				break;
			case CONTENT_NAME:
				item->pData[C_NAME] = tmp;
				break;
			case CONTENT_PROGRESS:
				item->pData[C_PROGRESS] = tmp + "%";
				break;
			case CONTENT_STATUS:
				item->pData[C_STATUS] = tmp;
				break;
			case CONTENT_FORMAT:
				item->pData[C_FORMAT] = tmp;
				break;
			case CONTENT_FILMLENGTH:
				item->pData[C_FILM_LENGTH] = tmp;
				break;
			case CONTENT_PROGRAMLENGTH:
				item->pData[C_PROGRAM_LENGTH] = tmp;
				break;
			case CONTENT_STEREOSCOPIC:
				item->pData[C_STEREO] = tmp;
				break;
			case CONTENT_ISSUER:
				item->pData[C_ISSUER] = tmp;
				break;
			case CONTENT_ISSUEDATE:
				item->pData[C_ISSUEDATE] = tmp;
				break;
			case CONTENT_TIMERANGE:
				item->pData[C_TIMERANGE] = tmp;
				break;
			case CONTENT_RECVSEGMENT:
				item->pData[C_RECVSEGMENT] = tmp;
				break;
			case CONTENT_TOTALSEGMENT:
				item->pData[C_TOTAL_SEGMENT] = tmp;
				break;
			case CONTENT_RECV_DATETIME:
				item->pData[C_RECV_DATETIME] = tmp;
				break;
			case CONTENT_LOCATE:
				item->pData[C_LOCATE] = tmp;
				item->MakeItem_HDD();

				if (item->pData[C_PROGRESS] == "100%")
				{
				ui.treeWidget_HDD_info->addTopLevelItem((QTreeWidgetItem*)(item));
				QCheckBox *ckBox = new QCheckBox;
				ui.treeWidget_HDD_info->setItemWidget((QTreeWidgetItem*)(item), 0, ckBox);
				}
				break;
			}
		}
	}

	delete[] buf;
	static int num1=0;
}

void Raid::UpdateContent_HDD()
{
	if(m_ConnectStatus==2)    //ȷѾӷ
	{
		KL *pKL = (KL*)buf;

		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = M_UPDATE_PROGRAM_LIST_HDD;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int i =pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_UPDATE_PROGRAM_LIST_HDD)
		{
		}
	}
}

void Raid::UpdateContent_RAID()
{
	if(m_ConnectStatus==2)    //ȷѾӷ
	{
		KL *pKL = (KL*)buf;

		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = M_UPDATE_PROGRAM_LIST_RAID;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int i =pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_UPDATE_PROGRAM_LIST_RAID)
		{
		}
	}
}

void Raid::ContentisReady_HDD()     //Ƿ׼,׼ֱӵLoadContent_HDD 
{
	if(m_ConnectStatus != 2)    //ȷѾӷ
		return;

	KL *pKL = (KL*)buf;
	pKL->m_pkgHead = 0x7585;
	bool ret=false;

	pKL->m_keyID = M_IS_PROGRAM_LIST_READY_HDD;
	pKL->m_length = sizeof(bool);
	void* pos = buf + sizeof(KL);
	memcpy(pos,&ret,sizeof(bool));   

	pSocket->write(buf, sizeof(KL) + sizeof(bool));
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i = pSocket->read(buf, 1024*1024);

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_IS_PROGRAM_LIST_READY_HDD)
	{
		KL *pKL = (KL *)buf;
		int len = pKL->m_length;
		int pos  = sizeof(KL);
		void* pos2 = buf + sizeof(KL);
		memcpy(&ret,pos2,sizeof(bool));       //ȡص״̬

		if(true==ret)  //Ѿ׼
		{
			LoadContent_HDD(); //ȡб
			if (m_content > 0)
			{
				killTimer(m_content);
				m_content = -1;
			}
		}
	}
}

void Raid::ContentisReady_RAID()     //Ƿ׼,׼ֱӵLoadContent_HDD 
{
	if(m_ConnectStatus != 2)    //ȷѾӷ
		return;
	KL *pKL = (KL*)buf;
	pKL->m_pkgHead = 0x7585;
	bool ret=false;

	pKL->m_keyID = M_IS_PROGRAM_LIST_READY_RAID;
	pKL->m_length = sizeof(bool);
	void* pos = buf + sizeof(KL);
	memcpy(pos,&ret,sizeof(bool));   

	pSocket->write(buf, sizeof(KL) + sizeof(bool));
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i = pSocket->read(buf, 1024*1024);

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_IS_PROGRAM_LIST_READY_RAID)
	{
		KL *pKL = (KL *)buf;
		int len = pKL->m_length;
		int pos  = sizeof(KL);
		void* pos2 = buf + sizeof(KL);
		memcpy(&ret,pos2,sizeof(bool));       //ȡص״̬

		if(true==ret)  //Ѿ׼
		{
			LoadContent_RAID(); //ȡб
			if (m_raid > 0)
			{
				killTimer(m_raid);
				m_raid = -1;
			}
		}
	}
}

void Raid::on_pushButton_4_clicked()
{
	UpdateContent_HDD();
	if(m_content == -1)
		m_content = startTimer(1000);
	UpdateContent_RAID();
	if(m_raid == -1)
		m_raid = startTimer(1000);
}

//Delete
void Raid::on_pushButton_5_clicked()
{
	QList<ContentItem*> raidList;
	QList<ContentItem*> rdList;
	for(int i = 0; i < ui.treeWidget_HDD_info->topLevelItemCount(); i++)
	{
		QTreeWidgetItem* item = ui.treeWidget_HDD_info->topLevelItem(i);
		QCheckBox *cb = (QCheckBox *)ui.treeWidget_HDD_info->itemWidget(item, 0);
		if(cb->checkState() == Qt::Checked)
			raidList.push_back((ContentItem*)item);
	}
	for(int i = 0; i < ui.treeWidget_HDD_info_2->topLevelItemCount(); i++)
	{
		QTreeWidgetItem* item = ui.treeWidget_HDD_info_2->topLevelItem(i);
		QCheckBox *cb = (QCheckBox *)ui.treeWidget_HDD_info_2->itemWidget(item, 0);
		if(cb->checkState() == Qt::Checked)
			rdList.push_back((ContentItem*)item);
	}

	if((raidList.size() + rdList.size()) > 0)
	{
		DelConfirm dc(pSocket, this);
		dc.SetDeleteList(raidList, rdList);
		if(m_timer > 0)
		{
			killTimer(m_timer);
			m_timer = -1;
		}
		if(dc.exec() == QDialog::Accepted)
			on_pushButton_4_clicked();
		m_timer = startTimer(5000);
	}
}

//copy to removeable disk
void Raid::on_pushButton_2_clicked()
{
	QList<ContentItem*> raidList;
	for(int i = 0; i < ui.treeWidget_HDD_info->topLevelItemCount(); i++)
	{
		QTreeWidgetItem* item = ui.treeWidget_HDD_info->topLevelItem(i);
		QCheckBox *cb = (QCheckBox *)ui.treeWidget_HDD_info->itemWidget(item, 0);
		if(cb->checkState() == Qt::Checked)
			raidList.push_back((ContentItem*)item);
	}
	if(raidList.size() > 0)
	{
		CopyFilm cf(pSocket, this);
// 		cf.setParent(this);
// 		cf.setModal(true);
		std::string dest = "/storage/ftp";
		cf.SetCopyList(raidList, dest);
		if(m_timer > 0)
		{
			killTimer(m_timer);
			m_timer = 0;
		}
		if(cf.exec() == QDialog::Accepted)
			on_pushButton_4_clicked();
		m_timer = startTimer(5000);

		UpdateContent_HDD();
		if(m_content == -1)
			m_content = startTimer(1000);
		UpdateContent_RAID();
		if(m_raid == -1)
			m_raid = startTimer(1000);
	}
}

//copy to raid disk
void Raid::on_pushButton_3_clicked()
{
	QList<ContentItem*> rdList;
	for(int i = 0; i < ui.treeWidget_HDD_info_2->topLevelItemCount(); i++)
	{
		QTreeWidgetItem* item = ui.treeWidget_HDD_info_2->topLevelItem(i);
		QCheckBox *cb = (QCheckBox *)ui.treeWidget_HDD_info_2->itemWidget(item, 0);
		if(cb->checkState() == Qt::Checked)
			rdList.push_back((ContentItem*)item);
	}
	if(rdList.size() > 0)
	{
		CopyFilm cf(pSocket, this);
		std::string dest = "/raid";
		cf.SetCopyList(rdList, dest);
// 		cf.setParent(this);
// 		cf.setModal(true);
		if(m_timer > 0)
		{
			killTimer(m_timer);
			m_timer = 0;
		}
		if(cf.exec() == QDialog::Accepted)
			on_pushButton_4_clicked();
		m_timer = startTimer(5000);

		UpdateContent_HDD();
		if(m_content == -1)
			m_content = startTimer(1000);
		UpdateContent_RAID();
		if(m_raid == -1)
			m_raid = startTimer(1000);
	}
}