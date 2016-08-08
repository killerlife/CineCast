#include "toc.h"
#include "CTcpServer.h"
#include <QtXml/QDomDocument>
#include <QFileDialog>
#include <QDir>
#include <QFileInfoList>
#include <QList>
#include <Windows.h>

#define  R_SET_DEBUG_CMD 0x500
#pragma pack(1)
typedef struct _KKL_
{
	uint16 m_pkgHead;
	uint16 m_keyID;
	uint16 m_length;
public:
	_KKL_()
	{
		m_pkgHead = 0x7585;
	}
} KKL;

ContentItem* gItem = NULL;
QTcpSocket gDev;

uint32 calc_crc32(uint8 *pData,
				  int32 nLength);

typedef struct _PD_
{
	uint8 tag;
	uint8 len;
	uint32 filmid;
	uint8 ver;
	uint8 type;
	uint8 count;
} PD;

typedef struct _SFD_
{
	uint8 tag;
	uint8 len;
	uint8 Name[9];
} SFD;

typedef struct _FD_
{
	uint8 tag;
	uint8 len;
	uint8 filelen[5];
	uint8 seglen[4];
	uint8 flag:8;
	uint8* pfname;
}FLD;

typedef struct _PID_
{
	uint8 type;
	uint16 res:3;
	uint16 pid:13;
	uint16 res1:4;
	uint16 eil:12;
	struct subcountdesc
	{
		uint8 tag;
		uint8 len;
		uint8 count;
	}scd;
	SFD sfd;
	FLD fd;
	uint8 crc[4];
}PID;

class MyfileInfo: public QFileInfo
{
public:
	int segments;
	int id;
	PID pid;
};

QList<MyfileInfo> myFileInfoList;
#define SEG_SIZE 2000
QFileInfoList myLostInfoList;

struct NotifyTab
{
	unsigned char id;
	unsigned short ssi:1;
	unsigned short pi:1;
	unsigned short res:2;
	unsigned short psl:12;
	struct NotifyDesc
	{
		unsigned char tag;
		unsigned char len;
		uint32 filmid;
		unsigned char ver;
		unsigned char type;
		unsigned char res;
		uint32 count;
	}ND;
	uint32 mid;
	unsigned char res1;
	unsigned char crc[4];
}NT;

struct StartTab
{
	uint8 id;
	uint16 ssi:1;
	uint16 pi:1;
	uint16 res:2;
	uint16 psl:12;
	struct StartDesc
	{
		uint8 tag;
		uint8 len;
		uint32 filmid;
		uint8 ver;
		uint8 type;
		uint64 DCPlen;
		uint64 res;
		uint8 fnlen;
		uint8 fn[13];
		uint8 uuidlen;
		uint8 uuid[36];
		uint8 idlen;
		uint8 idate[10];
		uint8 ilen;
		uint8 issuer[6];
		uint8 clen;
		uint8 create[6];
	}SD;
	struct FileDesc
	{
		uint8 tag;
		uint8 len;
		uint8 filelen[5];
		uint32 seglen:32;
		uint8 flag:8;
		uint8 fname[13];
	}FD;
	uint8 crc[4];
}ST;

struct FinishTab
{
	uint8 id;
	uint16 ssi:1;
	uint16 pi:1;
	uint16 res:2;
	uint16 psl:12;
	struct FinishDesc
	{
		uint8 tag;
		uint8 len;
		uint32 filmid;
		uint8 ver;
		uint8 type;
	}FD;
	uint8 crc[4];
}FT;

struct CancelTab
{
	uint8 id;
	uint16 ssi:1;
	uint16 pi:1;
	uint16 res:2;
	uint16 psl:12;
	struct CancelDesc
	{
		uint8 tag;
		uint8 len;
		uint32 filmid;
		uint8 ver;
		uint8 type;
	}CD;
	uint8 crc[4];
}CT;

struct PATTab
{
	uint8 id;
	uint16 ssi:1;
	uint16 pi:1;
	uint16 res:2;
	uint16 psl:12;
	uint16 tsid;
	uint8 res1:2;
	uint8 ver:5;
	uint8 cni:1;
	uint8 secnum;
	uint8 lsecnum;
	uint16 pn;
	uint16 res2:3;
	uint16 pmtid:13;
	uint8 crc[4];
}PATT;

typedef struct _PMT_
{
	uint8 id;
	uint16 ssi:1;
	uint16 pi:1;
	uint16 res:2;
	uint16 psl:12;
} PMTHead;

uint8 PMTT[4096];

struct FilmTab
{
	uint8 id;
	uint16 ssi:1;
	uint16 pi:1;
	uint16 res:2;
	uint16 psl:12;
	struct FDD
	{
		uint8 tag;
		uint8 len;
		uint32 filmid;
		uint8 ver;
		uint8 type;
		uint8 segnum[4];
	}fdd;
	uint8 buf[4010];
}FilmT;

int cur = -1;
static MyfileInfo fI;
static int segpos = 0;
static FILE *fp = NULL;
uint32 send_count= 0;

unsigned int getBits (const unsigned char *buf,
					  int startbit,
					  int bitlen)
{
	const unsigned char *b;
	unsigned int mask,tmp_long;
	int bitHigh,i;

	b = &buf[startbit / 8];
	startbit %= 8;

	bitHigh = 8;
	tmp_long = b[0];
	for (i = 0; i < ((bitlen-1) >> 3); i++) {
		tmp_long <<= 8;
		tmp_long  |= b[i+1];
		bitHigh   += 8;
	}

	startbit = bitHigh - startbit - bitlen;
	tmp_long = tmp_long >> startbit;
	mask     = (1ULL << bitlen) - 1;
	return tmp_long & mask;
}

void setBits16(uint8 *buf, int startbit, int bitlen, uint16 value)
{
	uint32 tmp, mask;
	mask = (1UL << bitlen) - 1;
	tmp = (value&mask) << ((startbit + bitlen)%8);
	mask = ~((mask) << ((startbit + bitlen)%8));

	uint8 *b, *v, *m;
	b = &buf[startbit / 8];
	v = (uint8 *)&tmp;
	m = (uint8 *)&mask;
	int pos = (bitlen + 7)/8;
	for(int i = 0; i < pos; i++)
	{
		b[i] = (b[i] & m[pos - i - 1]) | v[pos - i - 1];
	}
}

void TOC::on_pushButton_3_clicked()
{
	killTimer(timeid);
	timeid = 0;
	sendCancel();
	bLost = false;
	ui.label_5->setText("NO LOST");
}

void TOC::on_pushButtonFinish_2_clicked()
{
	killTimer(timeid);
	timeid = 0;
	sendFinish();
	bLost = false;
	ui.label_5->setText("NO LOST");
}

void TOC::on_pushButton_5_clicked()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Select LOST"));
	ui.label_5->setText(path);
	myLostInfoList.clear();
	if(!ui.label_5->text().isEmpty())
	{
		path = ui.label_5->text();
		QDir dir = QDir(path);
		myLostInfoList = dir.entryInfoList();
		QList<QFileInfo>::iterator itr;
		for(itr = myLostInfoList.begin(); itr != myLostInfoList.end(); ++itr)
		{
			QFileInfo fi = *itr;
			if(fi.isDir())
				myLostInfoList.erase(itr);
		}
		bLost = true;
	}
}

void TOC::on_pushButton_4_clicked()
{
	ContentItem *Item = (ContentItem*)ui.treeWidget->currentItem();

	if(Item)
	{
		QByteArray ba;
		emit UIcmd_toSocket(Item->tcpClientSocket->socketID, UI_CLOSE_SOCKET, ba);
	}
}

void TOC::on_pushButton_2_clicked()
{
	bstart = true;
	sendcount = 0;
	notifycount = 0;
	segpos = 0;
	cur = -1;
	int total = 0;
	for(int i = 0; i < myFileInfoList.size(); i++)
		total += myFileInfoList.at(i).segments;
	ui.progressBar->setMaximum(total);
	ui.label_total->setText(QString::number(total));
	ui.progressBar->setValue(0);
	if(timeid <= 0)
	timeid = startTimer(1);
}

void TOC::on_pushButtonLoadDcp_clicked()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Select DCP"));
	ui.label_DCP->setText(path);
}	

uint32 filmid;

uint32 incid = 0x6d47040c;

void TOC::on_pushButton_clicked()
{
	bool bDcp = ui.radioButton->isChecked();
	if(bDcp)
	{
		incid = ui.lineEdit->text().toInt();
		filmid = ((incid >> 24) & 0xff);
		filmid |= ((incid >> 8) & 0xff00);
		filmid |= ((incid << 8) & 0xff0000);
		filmid |= ((incid << 24) & 0xff000000);
		//filmid = incid;
	}
	else
		filmid = 0xf14a040c;
	if(!ui.label_DCP->text().isEmpty())
	{
		QString path = ui.label_DCP->text();
		QStringList sl = path.split("/");
		QDir dir = QDir(ui.label_DCP->text());
		QFileInfoList list = dir.entryInfoList();
		myFileInfoList.clear();
		uint64 dcplen = 0;
		int id = 0x21;
		for(int i = 0; i < list.size(); i++)
		{
			QFileInfo f = list.at(i);
			if(f.isFile())
			{
				MyfileInfo inf = *((MyfileInfo*)(&f));
				uint64 ss = inf.size();
				int seg = ss/SEG_SIZE;
				seg += ((ss%SEG_SIZE)?1:0);
				inf.segments = seg;
				dcplen += inf.size();
				inf.id = id;
				setBits16((uint8 *)&inf.pid, 11, 13, id++);
//				inf.pid.pid = id++;
				inf.pid.type = 0x5;
				inf.pid.scd.len = 1;
				inf.pid.scd.count = 1;
				inf.pid.sfd.len = 9;
#if 0
				if(bDcp)
				memcpy(inf.pid.sfd.Name, "20160721/", 9);
				else
					memcpy(inf.pid.sfd.Name, "lcupdate/", 9);
#endif
				QString p = sl.at(sl.size() - 1) + "/";
				memcpy(inf.pid.sfd.Name, p.toStdString().c_str(), 9);

				inf.pid.fd.filelen[0] = (inf.size() >> 32) & 0xff;
				inf.pid.fd.filelen[1] = (inf.size() >> 24) & 0xff;
				inf.pid.fd.filelen[2] = (inf.size() >> 16) & 0xff;
				inf.pid.fd.filelen[3] = (inf.size() >> 8) & 0xff;
				inf.pid.fd.filelen[4] = inf.size() & 0xff;
				inf.pid.fd.seglen[0] = (SEG_SIZE >> 24) & 0xff;
				inf.pid.fd.seglen[1] = (SEG_SIZE >> 16) & 0xff;
				inf.pid.fd.seglen[2] = (SEG_SIZE >> 8) & 0xff;
				inf.pid.fd.seglen[3] = (SEG_SIZE) & 0xff;
				inf.pid.fd.pfname = new uint8[inf.fileName().toStdString().size()];
				memcpy(inf.pid.fd.pfname, inf.fileName().toStdString().c_str(), inf.fileName().toStdString().size());
				inf.pid.fd.len = 10 + inf.fileName().toStdString().size();
				int len = inf.pid.fd.len + 2 + inf.pid.sfd.len + 2 + inf.pid.scd.len + 2;
				uint8 *pos = (uint8*)&inf.pid + 3;
				setBits16(pos, 4, 12, len);
//				inf.pid.eil = 0; //real len
				myFileInfoList.push_back(inf);
			}
		}

		//Construct Notify tab
		NT.id = 0x91;
		setBits16((uint8*)&NT, 12, 12, sizeof(NotifyTab) - 3);
// 		NT.psl = sizeof(NotifyTab) - 3;
		NT.ND.filmid = filmid;
		NT.ND.len = sizeof(NotifyTab::NotifyDesc) - 2;
		NT.ND.type = 4;
		NT.ND.count = 1;
		NT.mid = 1;
		uint32 crc = calc_crc32((uint8*)&NT, getBits((uint8*)&NT, 12, 12) - 1) & 0xffffffff;
		NT.crc[0] = crc >> 24;
		NT.crc[1] = (crc >> 16) & 0xff;
		NT.crc[2] = (crc >> 8) & 0xff;
		NT.crc[3] = crc & 0xff;

		//Construct Start tab
		ST.id = 0x92;
		setBits16((uint8*)&ST, 12, 12, sizeof(StartTab) - 3);
// 		ST.psl = sizeof(StartTab) - 3;
		ST.SD.filmid = filmid;
		ST.SD.len = sizeof(StartTab::StartDesc) - 2;
		ST.SD.type = 4;
		ST.SD.DCPlen = dcplen;
		if(bDcp)
		{
			ST.SD.fnlen = 13;
			memcpy(ST.SD.fn, "XuanChuanPian", 13);
		}
		else
		{
			ST.SD.fnlen = 13;
			memcpy(ST.SD.fn, "-Update File-", 13);
		}
		ST.SD.uuidlen = 36;
		memcpy(ST.SD.uuid, "00000000-0000-0000-0000-000000000000", 36);
		ST.SD.idlen = 10;
		memcpy(ST.SD.idate, "2016/07/21", 10);
		ST.SD.ilen = 6;
		memcpy(ST.SD.issuer, "leonis", 6);
		ST.SD.clen = 6;
		memcpy(ST.SD.create, "leonis", 6);
		ST.FD.len = sizeof(StartTab::FileDesc) - 2;
		ST.FD.filelen[0] = (dcplen >> 32) & 0xff;
		ST.FD.filelen[1] = (dcplen >> 24) & 0xff;
		ST.FD.filelen[2] = (dcplen >> 16) & 0xff;
		ST.FD.filelen[3] = (dcplen >> 8) & 0xff;
		ST.FD.filelen[4] = dcplen & 0xff;
		ST.FD.seglen = SEG_SIZE;
		if(bDcp)
			memcpy(ST.FD.fname, "XuanChuanPian", 13);
		else
			memcpy(ST.FD.fname, "-Update File-", 13);
		crc = calc_crc32((uint8*)&ST, getBits((uint8*)&ST, 12, 12) - 1) & 0xffffffff;
		ST.crc[0] = crc >> 24;
		ST.crc[1] = (crc >> 16) & 0xff;
		ST.crc[2] = (crc >> 8) & 0xff;
		ST.crc[3] = crc & 0xff;

		//Construct Finish tab
		FT.id = 0x93;
		setBits16((uint8*)&FT, 12, 12, sizeof(FinishTab) - 3);
// 		FT.psl = sizeof(FinishTab) - 3;
		FT.FD.filmid = filmid;
		FT.FD.len = sizeof(FinishTab::FinishDesc) - 2;
		FT.FD.type = 4;
		crc = calc_crc32((uint8*)&FT, getBits((uint8*)&FT, 12, 12) - 1) & 0xffffffff;
		FT.crc[0] = crc >> 24;
		FT.crc[1] = (crc >> 16) & 0xff;
		FT.crc[2] = (crc >> 8) & 0xff;
		FT.crc[3] = crc & 0xff;

		//Construct Cancel tab
		CT.id = 0x94;
		setBits16((uint8*)&CT, 12, 12, sizeof(CancelTab) - 3);
// 		CT.psl = sizeof(CancelTab) - 3;
		CT.CD.filmid = filmid;
		CT.CD.len = sizeof(CancelTab::CancelDesc) - 2;
		CT.CD.type = 4;
		crc = calc_crc32((uint8*)&CT, getBits((uint8*)&CT, 12, 12) - 1) & 0xffffffff;
		CT.crc[0] = crc >> 24;
		CT.crc[1] = (crc >> 16) & 0xff;
		CT.crc[2] = (crc >> 8) & 0xff;
		CT.crc[3] = crc & 0xff;

		//Construct PAT tab
		PATT.id = 0x00;
		setBits16((uint8*)&PATT, 12, 12, sizeof(PATTab) - 3);
// 		PATT.psl = sizeof(PATTab) - 3;
		PATT.pn = 256;
		uint8 *pbuf = (uint8*)&PATT + 10;
		setBits16((uint8*)pbuf, 3, 13, 0x20);
//		PATT.pmtid = 0x20;
		crc = calc_crc32((uint8*)&PATT, getBits((uint8*)&PATT, 12, 12) - 1) & 0xffffffff;
		PATT.crc[0] = crc >> 24;
		PATT.crc[1] = (crc >> 16) & 0xff;
		PATT.crc[2] = (crc >> 8) & 0xff;
		PATT.crc[3] = crc & 0xff;

		//Construct PMT tab
		//Program descriptor
		uint8 *pos = PMTT + 12;
		PD *pPd = (PD*)pos;
		pPd->filmid = filmid;
		pPd->count = myFileInfoList.size();
		pPd->len = sizeof(PD) - 2;
		pos += sizeof(PD);
		for(int i = 0 ; i < pPd->count; i++)
		{
			PID p = myFileInfoList.at(i).pid;
			memcpy(pos, &p, 8);
			pos += 8;
			memcpy(pos, &p.sfd, 11);
			pos += 11;
			memcpy(pos, &p.fd, 12);
			pos += 12;
			memcpy(pos, p.fd.pfname, p.fd.len - 10);
			pos += p.fd.len - 10;
		}
		PMTHead* pH = (PMTHead*)PMTT;
		pH->id = 0x02;
		setBits16((uint8*)&PMTT, 12, 12, pos - PMTT - 3 + 4);
// 		pH->psl = pos - PMTT - 3 + 4;
		crc = calc_crc32((uint8*)PMTT, getBits((uint8*)&PMTT, 12, 12) - 1) & 0xffffffff;
		pos[0] = crc >> 24;
		pos[1] = (crc >> 16) & 0xff;
		pos[2] = (crc >> 8) & 0xff;
		pos[3] = crc & 0xff;
	}		

	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Can not connect to host!"));
		killTimer(timeid);
		timeid = 0;
		return;
	}

	char buf[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000000;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	if(!dev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	if(!dev.waitForReadyRead(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	int i= dev.read(buf, 1024);
	dev.close();
}				 
		
void TOC::timerEvent(QTimerEvent * te)
{
	if(te->timerId() == timeid)
	{
		if (bstart)
		{
			gItem = (ContentItem*)ui.treeWidget->currentItem();
			if( gItem != NULL)
			{
				gDev.close();
				QHostAddress addr = gItem->tcpClientSocket->getRemote();
				gDev.connectToHost(addr, 10009);
				if(!gDev.waitForConnected(3000))
				{
					QMessageBox::critical(this, tr("ERROR"), tr("Can not connect to host!"));
					killTimer(timeid);
					timeid = 0;
					return;
				}
				bstart = false;
			}
		}
		else
		{
			notifycount++;
			if(notifycount < 50)
			{
				sendNotify();
			}
			else if(notifycount >= 50 && notifycount < 100)
			{
				sendStart();
			}
			else if(notifycount >= 100 && notifycount < 150)
			{
				sendPat();
			}
			else if(notifycount >= 150 && notifycount < 200)
			{
				sendPmt();
			}
			else if(notifycount >=200 && notifycount < 250)
			{
				;
			}
#if 1
			else
			{
				bool bRet;
				switch(sendcount%1000)
				{
				case 0:
					bRet = sendNotify();
					break;
				case 1:
					bRet = sendStart();
					break;
				case 2:
					bRet = sendPat();
					break;
				case 3:
					bRet = sendPmt();
					break;
				default:
					bRet = sendData();
				}
				if(bRet)
				sendcount++;
			}
#endif
		}
	}
}

bool TOC::sendNotify()
{
	char buf[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = 0x1ff;
	pKL->m_length = getBits((uint8*)&NT, 12, 12) + 3;

	int sendsize=sizeof(KKL);
	gDev.write(buf, sendsize);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	gDev.write((char*)&NT, pKL->m_length);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	if(!gDev.waitForReadyRead(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	int i= gDev.read(buf, 1024);
	return true;
}

bool TOC::sendStart()
{
	char buf[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = 0x2ff;
	pKL->m_length = getBits((uint8*)&ST, 12, 12) + 3;

	int sendsize=sizeof(KKL);
	gDev.write(buf, sendsize);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	gDev.write((char*)&ST, pKL->m_length);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	if(!gDev.waitForReadyRead(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	int i= gDev.read(buf, 1024);
	return true;
}

bool TOC::sendCancel()
{
	char buf[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = 0x4ff;
	pKL->m_length = getBits((uint8*)&CT, 12, 12) + 3;

	int sendsize=sizeof(KKL);
	gDev.write(buf, sendsize);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	gDev.write((char*)&CT, pKL->m_length);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	if(!gDev.waitForReadyRead(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	int i= gDev.read(buf, 1024);

	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return false;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected(3000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Can't connect to host!"));
		killTimer(timeid);
		timeid = 0;
		return false;
	}

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000007;

	pKL->m_length = sizeof(uint32);

	sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	if(!dev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	if(!dev.waitForReadyRead(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	i= dev.read(buf, 1024);
	dev.close();
	return true;
}

bool TOC::sendFinish()
{
	char buf[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = 0x3ff;
	pKL->m_length = getBits((uint8*)&FT, 12, 12) + 3;

	int sendsize=sizeof(KKL);
	gDev.write(buf, sendsize);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	gDev.write((char*)&FT, pKL->m_length);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	if(!gDev.waitForReadyRead(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	int i= gDev.read(buf, 1024);
	return true;
}

bool TOC::sendPat()
{
	char buf[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = 0xfe;
	pKL->m_length = getBits((uint8*)&PATT, 12, 12) + 3;

	int sendsize=sizeof(KKL);
	gDev.write(buf, sendsize);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	gDev.write((char*)&PATT, pKL->m_length);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	if(!gDev.waitForReadyRead(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	int i= gDev.read(buf, 1024);
	return true;
}

bool TOC::sendPmt()
{
	char buf[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = 0x20;
	PMTHead*pH = (PMTHead*)PMTT;
	pKL->m_length = getBits((uint8*)pH, 12, 12) + 3;

	int sendsize=sizeof(KKL);
	gDev.write(buf, sendsize);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	gDev.write((char*)&PMTT, pKL->m_length);
	if(!gDev.waitForBytesWritten(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	if(!gDev.waitForReadyRead(2000))
	{
		QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
		killTimer(timeid);
		timeid = 0;
	}
	int i= gDev.read(buf, 1024);
	return true;
}

char *lost_buf = NULL;
uint32 lost_size;

bool TOC::sendData()
{
	if(cur <= 0)
	{
		if(cur < myFileInfoList.size())
			fI = myFileInfoList.at(0);
		segpos = 0;
		fp = fopen(fI.filePath().toStdString().c_str(), "rb");
#if 1
		if(lost_buf != NULL)
		{
			delete[] lost_buf;
			lost_buf = NULL;
		}
		if(bLost)
		{
			for(int i = 0; i < myLostInfoList.size(); i++)
			{
				QFileInfo lI = myLostInfoList.at(i);
				if(lI.fileName().contains(fI.fileName()))
				{
					lost_size = lI.size();
					lost_buf = new char[lost_size];
					FILE *fp = fopen(lI.filePath().toStdString().c_str(), "rb");
					fread(lost_buf, 1, lost_size, fp);
					fclose(fp);
					break;
				}
			}
		}
#endif
		cur = 1;
		send_count = 0;
	}
	if(fp != NULL)
	{
		if(bLost)
		{
			bool bRet = false;
			FilmT.id = 0x80;
			FilmT.fdd.filmid = filmid;
			FilmT.fdd.len = 10;
			while (1)
			{
				if(lost_buf != NULL && !haveSegment(segpos))
				{
					uint64 pos = (uint64) segpos * SEG_SIZE;
					fseek(fp, pos, SEEK_SET);
					int size = fread(FilmT.buf, 1, SEG_SIZE, fp);
					if (size > 0)
					{
						FilmT.fdd.segnum[0] = (segpos >> 24) & 0xff;
						FilmT.fdd.segnum[1] = (segpos >> 16) & 0xff;
						FilmT.fdd.segnum[2] = (segpos >> 8) & 0xff;
						FilmT.fdd.segnum[3] = (segpos) & 0xff;
						setBits16((uint8*)&FilmT, 12, 12, sizeof(FilmTab::FDD) + size + 4);
						//			FilmT.psl = sizeof(FilmTab::FDD) + size + 4;
						uint32 crc = calc_crc32((uint8*)&FilmT, getBits((uint8*)&FilmT, 12, 12) - 1) & 0xffffffff;
						char* pos = (char*)&FilmT + getBits((uint8*)&FilmT, 12, 12) - 1;
						pos[0] = (crc >> 24) & 0xff;
						pos[1] = (crc >> 16) & 0xff;
						pos[2] = (crc >> 8) & 0xff;
						pos[3] = crc & 0xff;

						char buf[1024];
						KKL *pKL = (KKL*)buf;

						pKL->m_pkgHead = 0x7585;
						pKL->m_keyID = fI.id;
						pKL->m_length = getBits((uint8*)&FilmT, 12, 12) + 3;
#if 1
						int sendsize=sizeof(KKL);
						gDev.write(buf, sendsize);
						if(!gDev.waitForBytesWritten(2000))
						{
							QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
							killTimer(timeid);
							timeid = 0;
						}
						gDev.write((char*)&FilmT, pKL->m_length);
// 						while(!gDev.flush());
						if(!gDev.waitForBytesWritten(4000))
						{
							QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
							killTimer(timeid);
							timeid = 0;
						}
						// 			Sleep(40);
						if(!gDev.waitForReadyRead(2000))
						{
							QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
							killTimer(timeid);
							timeid = 0;
						}
						int i= gDev.read(buf, 6);
						send_count++;
						ui.progressBar->setValue(send_count);
						ui.label_count->setText(QString::number(send_count));
#endif
						segpos++;
						bRet = true;
						return bRet;
					}
					else
					{
						fclose(fp);
						if(cur < myFileInfoList.size())
							fI = myFileInfoList.at(cur);
						else
						{
							FilmT.fdd.segnum[0] = (segpos >> 24) & 0xff;
							FilmT.fdd.segnum[1] = (segpos >> 16) & 0xff;
							FilmT.fdd.segnum[2] = (segpos >> 8) & 0xff;
							FilmT.fdd.segnum[3] = (segpos) & 0xff;
							segpos++;
							setBits16((uint8*)&FilmT, 12, 12, sizeof(FilmTab::FDD) + size + 4);
							//			FilmT.psl = sizeof(FilmTab::FDD) + size + 4;
							uint32 crc = calc_crc32((uint8*)&FilmT, getBits((uint8*)&FilmT, 12, 12) - 1) & 0xffffffff;
							char* pos = (char*)&FilmT + getBits((uint8*)&FilmT, 12, 12) - 1;
							pos[0] = (crc >> 24) & 0xff;
							pos[1] = (crc >> 16) & 0xff;
							pos[2] = (crc >> 8) & 0xff;
							pos[3] = crc & 0xff;

							char buf[1024];
							KKL *pKL = (KKL*)buf;

							pKL->m_pkgHead = 0x7585;
							pKL->m_keyID = fI.id;
							pKL->m_length = getBits((uint8*)&FilmT, 12, 12) + 3;
							int sendsize=sizeof(KKL);
							gDev.write(buf, sendsize);
							if(!gDev.waitForBytesWritten(2000))
							{
								QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
								killTimer(timeid);
								timeid = 0;
							}
							gDev.write((char*)&FilmT, pKL->m_length);
// 							while(!gDev.flush());
							if(!gDev.waitForBytesWritten(2000))
							{
								QMessageBox::critical(this, tr("ERROR"), tr("Write to host error!"));
								killTimer(timeid);
								timeid = 0;
							}
							// 			Sleep(40);
							if(!gDev.waitForReadyRead(2000))
							{
								QMessageBox::critical(this, tr("ERROR"), tr("Read from host error!"));
								killTimer(timeid);
								timeid = 0;
							}

							int i= gDev.read(buf, 6);
							killTimer(timeid);
							timeid = 0;
							cur = -6;
							send_count = 0;
							return true;
						}
						segpos = 0;
						fp = fopen(fI.filePath().toStdString().c_str(), "rb");
						if(lost_buf != NULL)
						{
							delete[] lost_buf;
							lost_buf = NULL;
						}
						for(int i = 0; i < myLostInfoList.size(); i++)
						{
							QFileInfo lI = myLostInfoList.at(i);
							if(lI.fileName().contains(fI.fileName()))
							{
								lost_size = lI.size();
								lost_buf = new char[lost_size];
								FILE *fp = fopen(lI.filePath().toStdString().c_str(), "rb");
								fread(lost_buf, 1, lost_size, fp);
								fclose(fp);
								break;
							}
						}
						cur++;
					}
					continue;
				}
				segpos++;
			}
		}
		else
		{
		FilmT.id = 0x80;
			FilmT.fdd.filmid = filmid;
		FilmT.fdd.len = 10;
		int size = fread(FilmT.buf, 1, SEG_SIZE, fp);
		if(size > 0)
		{
			FilmT.fdd.segnum[0] = (segpos >> 24) & 0xff;
			FilmT.fdd.segnum[1] = (segpos >> 16) & 0xff;
			FilmT.fdd.segnum[2] = (segpos >> 8) & 0xff;
			FilmT.fdd.segnum[3] = (segpos) & 0xff;
			segpos++;
			setBits16((uint8*)&FilmT, 12, 12, sizeof(FilmTab::FDD) + size + 4);
//			FilmT.psl = sizeof(FilmTab::FDD) + size + 4;
			uint32 crc = calc_crc32((uint8*)&FilmT, getBits((uint8*)&FilmT, 12, 12) - 1) & 0xffffffff;
			char* pos = (char*)&FilmT + getBits((uint8*)&FilmT, 12, 12) - 1;
			pos[0] = (crc >> 24) & 0xff;
			pos[1] = (crc >> 16) & 0xff;
			pos[2] = (crc >> 8) & 0xff;
			pos[3] = crc & 0xff;

			char buf[1024];
			KKL *pKL = (KKL*)buf;

			pKL->m_pkgHead = 0x7585;
			pKL->m_keyID = fI.id;
			pKL->m_length = getBits((uint8*)&FilmT, 12, 12) + 3;
#if 1
			int sendsize=sizeof(KKL);
			gDev.write(buf, sendsize);
				if(!gDev.waitForBytesWritten(2000))
				{
					QMessageBox::critical(this, tr("ERROR"), tr("DATA: Write to host error!"));
					killTimer(timeid);
					timeid = 0;
				}
			gDev.write((char*)&FilmT, pKL->m_length);
// 				while(!gDev.flush());
				if(!gDev.waitForBytesWritten(4000))
				{
					QMessageBox::critical(this, tr("ERROR"), tr("DATA: Write2 to host error!"));
					killTimer(timeid);
					timeid = 0;
				}
// 			Sleep(40);
				if(!gDev.waitForReadyRead(2000))
				{
					QMessageBox::critical(this, tr("ERROR"), tr("DATA: Read from host error!"));
					killTimer(timeid);
					timeid = 0;
				}
			int i= gDev.read(buf, 6);
				send_count++;
				ui.progressBar->setValue(send_count);
				ui.label_count->setText(QString::number(send_count));
#endif
		}
		else
		{
			fclose(fp);
			if(cur < myFileInfoList.size())
				fI = myFileInfoList.at(cur);
			else
			{
					FilmT.fdd.segnum[0] = (segpos >> 24) & 0xff;
					FilmT.fdd.segnum[1] = (segpos >> 16) & 0xff;
					FilmT.fdd.segnum[2] = (segpos >> 8) & 0xff;
					FilmT.fdd.segnum[3] = (segpos) & 0xff;
					segpos++;
					setBits16((uint8*)&FilmT, 12, 12, sizeof(FilmTab::FDD) + size + 4);
					//			FilmT.psl = sizeof(FilmTab::FDD) + size + 4;
					uint32 crc = calc_crc32((uint8*)&FilmT, getBits((uint8*)&FilmT, 12, 12) - 1) & 0xffffffff;
					char* pos = (char*)&FilmT + getBits((uint8*)&FilmT, 12, 12) - 1;
					pos[0] = (crc >> 24) & 0xff;
					pos[1] = (crc >> 16) & 0xff;
					pos[2] = (crc >> 8) & 0xff;
					pos[3] = crc & 0xff;

					char buf[1024];
					KKL *pKL = (KKL*)buf;

					pKL->m_pkgHead = 0x7585;
					pKL->m_keyID = fI.id;
					pKL->m_length = getBits((uint8*)&FilmT, 12, 12) + 3;
					int sendsize=sizeof(KKL);
					gDev.write(buf, sendsize);
					if(!gDev.waitForBytesWritten(2000))
					{
						QMessageBox::critical(this, tr("ERROR"), tr("DATA: Write to host error!"));
						killTimer(timeid);
						timeid = 0;
					}
					gDev.write((char*)&FilmT, pKL->m_length);
// 					while(!gDev.flush());
					if(!gDev.waitForBytesWritten(4000))
					{
						QMessageBox::critical(this, tr("ERROR"), tr("DATA: Write2 to host error!"));
				killTimer(timeid);
				timeid = 0;
					}
					// 			Sleep(40);
					if(!gDev.waitForReadyRead(2000))
					{
						QMessageBox::critical(this, tr("ERROR"), tr("DATA: Read from host error!"));
						killTimer(timeid);
						timeid = 0;
					}

					int i= gDev.read(buf, 6);
					killTimer(timeid);
					timeid = 0;
					cur = -6;
					send_count = 0;
			}
			segpos = 0;
			fp = fopen(fI.filePath().toStdString().c_str(), "rb");
			cur++;
		}
			return true;
		}
	}
}

bool TOC::haveSegment(uint32 nSegNum)
{
	int p1 = nSegNum/8;
	int p2 = nSegNum%8;
	if(p1 < lost_size)
	{
		if(lost_buf[p1] & (1<<p2))
			return true;
	}
	return false;
}

TOC::TOC(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	bLost = false;
	timeid = 0;
	ui.setupUi(this);
	Init();
}

TOC::~TOC()
{

}

void TOC::Init(void)
{
// 	this->setWindowState(Qt::WindowFullScreen);
	this->showMaximized();
	ui.radioButton->setChecked(true);
	StartServer();
}

void TOC::StartServer(void)
{         
	//server = new CTcpServer(this,port); 
	server_login = new CTcpServer(this,8000);     //½

// 	connect(server_login,SIGNAL(signal_updateServerUI(QString,int)),this,SLOT(slot_updateServerUI(QString,int)));
	//ͻ,Ͽ,źţ֪ͨ,ˢ¿ͻϢб
// 	connect(server_login,SIGNAL(signal_clientChange()),this,SLOT(updateClientList()));


	//new
	server_main = new CTcpServer(this,10099);   //
// 	connect(server_main,SIGNAL(signal_updateServerUI(QString,int)),this,SLOT(slot_updateServerUI(QString,int)));
// 	connect(server_main,SIGNAL(signal_clientChange()),this,SLOT(updateClientList()));



	//źֱӲsockeṭ߳CTcpServerΪת
	connect(this,SIGNAL(UIcmd_toSocket(int,int,QByteArray)),server_login,SIGNAL(UIcmd_toSocket(int,int,QByteArray)));
	connect(this,SIGNAL(UIcmd_toSocket(int,int,QByteArray)),server_main,SIGNAL(UIcmd_toSocket(int,int,QByteArray)));

	//soetck߳ͨCTcpServerתź
	connect(server_login,SIGNAL(signal_CMD_SocketToUI(int,int,int)),this,SLOT(slot_CMD_SocketToUI_process(int,int,int)));
	connect(server_main,SIGNAL(signal_CMD_SocketToUI(int,int,int)),this,SLOT(slot_CMD_SocketToUI_process(int,int,int)));



	connect(server_main,SIGNAL(signal_CMD_TcpServertoUI(int,int)),this,SLOT(slot_CMD_TcpServertoUI_process(int,int)));
	connect(server_login,SIGNAL(signal_CMD_TcpServertoUI(int,int)),this,SLOT(slot_CMD_TcpServertoUI_process(int,int)));
}

void TOC::slot_CMD_SocketToUI_process(int SocketID,int cmdtype,int val)   //socket
{
	switch(cmdtype)
	{
	case SK_UPDATE_TREEWIDGET_LIST:
		do_updateListOfClient();  
		break;
	case SK_UPDATE_TEXTBROWSER:
		{
			char* p = (char*)val;
			ui.textBrowser->setText(QString(p));
		}
		break;
	default:	break;
	}
}

void TOC::slot_CMD_TcpServertoUI_process(int SocketID,int cmdtype)       //UI
{
	switch(cmdtype)
	{
	case SV_SOCKET_DISCONNECT:
		do_updateListOfClient();  
		break;
	default:	break;
	}
}

void TOC::do_updateListOfClient()
{   
	ui.treeWidget->clear();
	for(int i=0;i<server_main->tcpClientSocketList.count();i++)
	{
		TcpClientSocket *pSocket=server_main->tcpClientSocketList.at(i);
		// AuthenREQUEST AuthenRequest;
		ContentItem* item = new ContentItem;
		item->AuthenRequest=pSocket->AuthenRequest;
		item->tcpClientSocket=pSocket;
		item->MakeItem();
		ui.treeWidget->addTopLevelItem((QTreeWidgetItem*)(item));
	}
}

void ContentItem::MakeItem()
{
	pData[0]=QString::number(AuthenRequest.Machine_ID);
	AuthenRequest.SoftVersion[15]='\0';
	//AuthenRequest.Time_Login[15]='\0';
	pData[1]=AuthenRequest.SoftVersion;  //ַ֤
	pData[2]=AuthenRequest.Time_Login;   //ַ֤
	pData[3]=QString::number(AuthenRequest.Model_Log);
	pData[4]=QString::number(AuthenRequest.BeatCycle);
	pData[5]=QString::number(AuthenRequest.Model_Connect);
	pData[6]=QString::number(AuthenRequest.HardDiskNum);
	pData[7]=QString::number(AuthenRequest.AllDiskCapacity);


	setText(0, pData[0].toStdString().c_str());
	setText(1, pData[1].toStdString().c_str());
	setText(2, pData[2].toStdString().c_str());
	setText(3, pData[3].toStdString().c_str());
	setText(4, pData[4].toStdString().c_str());
	setText(5, pData[5].toStdString().c_str());
	setText(6, pData[6].toStdString().c_str());
	setText(7, pData[7].toStdString().c_str());

	QString srtIP=tcpClientSocket->peerAddress().toString();
	QString srtName=tcpClientSocket->peerName();
	int port=tcpClientSocket->peerPort();
	QString srtPort=QString::number(port);

	int flag=tcpClientSocket->flag_update;

	switch(flag)
	{
	case 0: setText(8,"IDLE");    break;  //
	case 1: setText(8,"Waiting"); break;  //
	case 2: setText(8,"OK");    break;  //
	default:	break;
	}
}

void TOC::on_pushButtonMD5_clicked()
{
	QByteArray ba;
	ContentItem* item =(ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	int SockID=item->tcpClientSocket->socketID;   //ܵúñ
	emit UIcmd_toSocket(SockID, UI_GET_MD5RES, ba);
}

void TOC::on_pushButtonLog_clicked()
{
	QByteArray ba;
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	int SockID = item->tcpClientSocket->socketID;
	emit UIcmd_toSocket(SockID, UI_LOG, ba);
}

void TOC::on_pushButtonReboot_clicked()
{
	QByteArray ba;
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	int SockID = item->tcpClientSocket->socketID;
	emit UIcmd_toSocket(SockID, UI_REBOOT, ba);
}

void TOC::on_pushButtonGetFilmList_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 20080);
	if(!dev.waitForConnected())
		return;

	char buf[10240];
	memset(buf, 0, 100);
	buf[0] = 0x55;
	short cmd = 0x11;
	memcpy(buf + 1, &cmd, sizeof(cmd));
	int i = 0;
	memcpy(buf + 3, &i, sizeof(i));

	dev.write(buf, 3+ 4+ i+4);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	memset(buf, 0, 100);
	i = dev.read(buf, 1024);

	memset(buf, 0, 100);
	buf[0] = 0x55;
	cmd = 0x21;
	memcpy(buf + 1, &cmd, sizeof(cmd));
	i = 0;
	memcpy(buf + 3, &i, sizeof(i));

	dev.write(buf, 3+ 4+ i+4);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	i = dev.read(buf, 4+2+1);
	int len = *(uint32*)(buf+3);
	char *str = new char[len+4];
	i = dev.read(str, len+4);
	str[len] = '\0';
	ui.textBrowser->setText(str);
	delete[] str;
	dev.close();
}

void TOC::on_pushButtonDelFilm_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected())
		return;

	char *buf = new char[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000100;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i= dev.read(buf, 1024);
	dev.close();
}

void TOC::on_pushButtonGetFilmInfo_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 20080);
	if(!dev.waitForConnected())
		return;

	char buf[1024];
	buf[0] = 0x55;

	QDomDocument doc;
	QDomProcessingInstruction instruction;  
	instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");  
	doc.appendChild(instruction); 

	QDomElement resp = doc.createElement("response");
	doc.appendChild(resp);

	resp.setAttribute("status", "OK");
	resp.setAttribute("version", "2");
	QDomElement list = doc.createElement("uuid");
	resp.appendChild(list);

	QDomText txt;
	QString fuid = ui.lineEditUUID->text();
	txt = doc.createTextNode(fuid);
	list.appendChild(txt);

	QString s = doc.toString();
	short cmd = 0x24;
	memcpy(buf + 1, &cmd, sizeof(cmd));
	int len = s.toStdString().size();
	memcpy(buf + 1 + 2, &len, sizeof(len));

	char *pos = buf + 1 + 2 + 4;
	memcpy(pos, s.toStdString().c_str(), len);
	memcpy(pos + len, &len, sizeof(len));
	int setsize = len + 7 + sizeof(len);

	dev.write(buf, setsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i = dev.read(buf, 4+2+1);
	len = *(uint32*)(buf+3);
	char *str = new char[len+4];
	i = dev.read(str, len+4);
	str[len] = '\0';
	ui.textBrowser->setText(str);
	dev.close();
}

void TOC::on_pushButtonNext_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected())
		return;

	char *buf = new char[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000000;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i= dev.read(buf, 1024);
	dev.close();
}

void TOC::on_pushButtonNotify_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected())
		return;

	char *buf = new char[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000001;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i= dev.read(buf, 1024);
	dev.close();
}

void TOC::on_pushButtonStart_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected())
		return;

	char *buf = new char[1024];
	KKL *pKL = (KKL*)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000002;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i= dev.read(buf, 1024);
	dev.close();
}

void TOC::on_pushButtonPAT_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected())
		return;

	char *buf = new char[1024];
	KKL *pKL = (KKL *)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000003;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i= dev.read(buf, 1024);
	dev.close();
}

void TOC::on_pushButtonPMT_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected())
		return;

	char *buf = new char[1024];
	KKL *pKL = (KKL *)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000004;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i= dev.read(buf, 1024);
	dev.close();
}

void TOC::on_pushButtonData_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected())
		return;

	char *buf = new char[1024];
	KKL *pKL = (KKL *)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000005;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i= dev.read(buf, 1024);
	dev.close();
}

void TOC::on_pushButtonFinish_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected())
		return;

	char *buf = new char[1024];
	KKL *pKL = (KKL *)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000006;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i= dev.read(buf, 1024);
	dev.close();
}

void TOC::on_pushButtonCancel_clicked()
{
	ContentItem* item = (ContentItem*)ui.treeWidget->currentItem();
	if(item == NULL)
		return;
	QHostAddress addr = item->tcpClientSocket->getRemote();
	QTcpSocket dev;
	dev.connectToHost(addr, 10003);
	if(!dev.waitForConnected())
		return;

	char *buf = new char[1024];
	KKL *pKL = (KKL *)buf;

	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = R_SET_DEBUG_CMD;
	void* pos = buf + sizeof(KKL);
	*((uint32*)pos) = 0x10000007;

	pKL->m_length = sizeof(uint32);

	int sendsize=sizeof(KKL)+sizeof(uint32);
	dev.write(buf, sendsize);
	dev.waitForBytesWritten();
	dev.waitForReadyRead();
	int i= dev.read(buf, 1024);
	dev.close();
}

#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <stdio.h>      /*fprintf(),stderr,BUFSIZ*/
#include <stdlib.h>     /**/
#include <string.h>     /*stderror()*/
#include <fcntl.h>      /*open(),flag*/
#include <errno.h>      /*errno*/
#include <sys/types.h>
#include <sys/stat.h>   /*mode_t*/
#include <io.h>

QString TOC::printfn(QString fn)
{
	QString s;
	s = QString(tr("\nProtocol File: %1\n[ %2 ]---------------------------------------------------------------\n"))
		.arg(fn)
		.arg(tr("Analysis Start"));
	return s;
}

void TOC::on_pushButtonProtocol_clicked()
{
	QStringList flist = QFileDialog::getOpenFileNames(this,
		tr("Choose protocol files"),
		"./protocol",
		tr("Protocols (*.pro)"));

	if(flist.size() > 0)
	{
		QString display;
		for(int i = 0; i < flist.size(); i++)
		{
			struct stat s;
			int fd = _open(flist.at(i).toStdString().c_str(), O_RDONLY);
			fstat(fd, &s);
			_close(fd);

			char *buf = new char[s.st_size];
			FILE *fp = fopen(flist.at(i).toStdString().c_str(), "rb");
			fread(buf, s.st_size, 1, fp);
			fclose(fp);

			display += printfn(flist.at(i));
			decrypt(buf, s.st_size);
			display += print_hex2(buf, s.st_size);
			display += print_kh(buf, s.st_size);
			display += print_crc(buf, s.st_size);

			delete[] buf;
		}
		ui.textBrowser->setText(display);
	}
	else
		return;
}

QString TOC::print_hex2(char* buf, int len)
{
	QString s, ss;
	s = QString(tr("Hexadecimal Dump: \n<ADDR>      <     HEX    >                                   <ASCII>"));
	//Print results:
	for(int i=0;i<len;i++) {
		if(i%16 == 0)
		{
			if(i != 0)
			{
				s += QString("  ");
				for (int j = i - 16; j < i; j++)
				{
					if(buf[j]>=32 && buf[j] <127)
						s += ss.sprintf("%c", (char)buf[j]);
					else
						s += QString(".");
				}
			}
			s += ss.sprintf("\n0x%08X ", i);
		}
		s += ss.sprintf("%02X ",(unsigned char)buf[i]);
	}
	s += ss.sprintf("\n");
	return s;
}

QString TOC::print_kh(char* buf, int len)
{
	QString s;
	KH *pKh = (KH*)buf;
	s = tr("\nParse Message:\n");	s += tr("\n[  HEAD  ]--------------------------------------------------------------------- \n");
	s += QString(tr("%1          PREAMBLE        0x%2\n")).arg(QString::number(sizeof(uint8))).arg(QString::number(pKh->pkgHead, 16));
	s += QString(tr("%1          FLAG            0x%2 "))
		.arg(QString::number(sizeof(uint8)))
		.arg(QString::number(pKh->flag_pwd, 16));
	switch(pKh->flag_pwd)
	{
	case 0:
		s += tr("		- NOT ENCRYPT \n");
		break;
	case 1:
		s += tr("		- ENCRYPTED BY HARD KEY\n");
		break;
	case 2:
		s += tr("		- ENCRYPTED BY SESSION KEY\n");
		break;
	default:
		printf("\n");
	}
	s += QString(tr("%1          RESERVED        0x%2 \n"))
		.arg(QString::number(sizeof(uint32)))
		.arg(QString::number(pKh->rev, 16));
	s += QString(tr("%1          COMMAND         0x%2 "))
		.arg(QString::number(sizeof(uint16)))
		.arg(QString::number(pKh->cmd, 16));

	switch(pKh->cmd)
	{
	case 0x11:
		s += tr("		- Login Request \n");
		break;
	case 0x12:
		s += tr("		- Login Response \n");
		break;
	case 0x13:
		s += tr("		- Auth Request \n");
		break;
	case 0x14:
		s += tr("		- Auth Response \n");
		break;
	case 0x21:
		s += tr("		- Lost Report Request \n");
		break;
	case 0x22:
		s += tr("		- Lost Report Upload \n");
		break;
	case 0x23:
		s += tr("		- Lost Report Confirm \n");
		break;
	case 0x24:
		s += tr("		- MD5 File Request \n");
		break;
	case 0x25:
		s += tr("		- MD5 File Response \n");
		break;
	case 0x26:
		s += tr("		- MD5 File Confirm \n");
		break;
	case 0x27:
		s += tr("		- MD5 Result Request \n");
		break;
	case 0x28:
		s += tr("		- MD5 Result Upload \n");
		break;
	case 0x29:
		s += tr("		- MD5 Result Confirm \n");
		break;
	case 0x31:
		s += tr("		- Heart Beat Request \n");
		break;
	case 0x32:
		s += tr("		- Heart Beat Upload \n");
		break;
	case 0x33:
		s += tr("		- Heart Beat Confirm \n");
		break;
	case 0x34:
		s += tr("		- Log File Request \n");
		break;

	default:
		s += tr("\n");
	}

	s += QString(tr("%1          SUB COMMAND     0x%2 "))
		.arg(QString::number(sizeof(uint16)))
		.arg(QString::number(pKh->cmd_Sub, 16));
	switch(pKh->cmd_Sub)
	{
	case 0:
		s += tr("		- Success \n");
		break;
	case 0x1:
		s += tr("		- System exception \n");
		break;
	case 0x2:
		s += tr("		- Information not exist \n");
		break;
	case 0x3:
		s += tr("		- Wrong default value \n");
		break;
	case 0x4:
		s += tr("		- Reserved \n");
		break;
	case 0x11:
		s += tr("		- Wrong status \n");
		break;
	case 0x12:
		s += tr("		- Wrong param \n");
		break;
	case 0x13:
		s += tr("		- Process failure \n");
		break;
	case 0x14:
		s += tr("		- Reserved \n");
		break;
	case 0x15:
		s += tr("		- Data damage \n");
		break;
	case 0x16:
		s += tr("		- Operation cancel \n");
		break;
	case 0x21:
		s += tr("		- Data already exist \n");
	case 0x22:
		s += tr("		- Data not exist \n");
		break;
	case 0x23:
		s += tr("		- Newest data\n");
		break;
	case 0x24:
		s += tr("		- Error command \n");
		break;
	case 0x25:
		s += tr("		- Unregister hard key \n");
		break;
	case 0x26:
		s += tr("		- Hard key error \n");
		break;
	case 0x27:
		s += tr("		- Invalid ID\n");
		break;
	case 0x50:
		s += tr("		- System error \n");
		break;
	case 0x51:
		s += tr("		- Unknow device \n");
		break;
	case 0x52:
		s += tr("		- Memory error \n");
		break;
	case 0x53:
		s += tr("		- System param error \n");
		break;
	default:
		s += tr("\n");
	}
	s += QString(tr("%1          PROTOCOL LENGTH %2 "))
		.arg(QString::number(sizeof(uint32)))
		.arg(QString::number(pKh->cmd_length));
	if(len == (sizeof(KH) + pKh->cmd_length))
		s += tr("Length correct \n");
	else
		s += tr("Length error \n");
	return s;
}

extern void MyAes_ctr_decrypt(uint8* msg, uint32 fsize,uint8 key[16]);

#include "IDManager.h"
uint32 ID = 0;
void TOC::decrypt(char* buf, int len)
{
	KH *pKh = (KH*)buf;
	char *data;
	int len1;
	char *id = buf + sizeof(KH);
	switch(pKh->cmd)
	{
	case 0x11:
	case 0x13:
		data = buf + sizeof(KH) + sizeof(uint32);
		len1 = pKh->cmd_length - sizeof(uint32);
		ID = *(uint32*)(id);
		break;
	default:
		data = buf + sizeof(KH);
		len1 = pKh->cmd_length;
	}
	int len_out;
	unsigned char key[20] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68};
	switch(pKh->cmd)
	{
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
		GetIDManager()->GetHardKeyById(ID, (char*)key);
		break;
	}
	if(pKh->flag_pwd != 0)
	MyAes_ctr_decrypt((uint8*)data, len1, key);
}

static uint32 m_crc32Table[256] = {
	0x00000000,0x04C11DB7,0x09823B6E,0x0D4326D9,
	0x130476DC,0x17C56B6B,0x1A864DB2,0x1E475005,
	0x2608EDB8,0x22C9F00F,0x2F8AD6D6,0x2B4BCB61,
	0x350C9B64,0x31CD86D3,0x3C8EA00A,0x384FBDBD,
	0x4C11DB70,0x48D0C6C7,0x4593E01E,0x4152FDA9,
	0x5F15ADAC,0x5BD4B01B,0x569796C2,0x52568B75,
	0x6A1936C8,0x6ED82B7F,0x639B0DA6,0x675A1011,
	0x791D4014,0x7DDC5DA3,0x709F7B7A,0x745E66CD,
	0x9823B6E0,0x9CE2AB57,0x91A18D8E,0x95609039,
	0x8B27C03C,0x8FE6DD8B,0x82A5FB52,0x8664E6E5,
	0xBE2B5B58,0xBAEA46EF,0xB7A96036,0xB3687D81,
	0xAD2F2D84,0xA9EE3033,0xA4AD16EA,0xA06C0B5D,
	0xD4326D90,0xD0F37027,0xDDB056FE,0xD9714B49,
	0xC7361B4C,0xC3F706FB,0xCEB42022,0xCA753D95,
	0xF23A8028,0xF6FB9D9F,0xFBB8BB46,0xFF79A6F1,
	0xE13EF6F4,0xE5FFEB43,0xE8BCCD9A,0xEC7DD02D,
	0x34867077,0x30476DC0,0x3D044B19,0x39C556AE,
	0x278206AB,0x23431B1C,0x2E003DC5,0x2AC12072,
	0x128E9DCF,0x164F8078,0x1B0CA6A1,0x1FCDBB16,
	0x018AEB13,0x054BF6A4,0x0808D07D,0x0CC9CDCA,
	0x7897AB07,0x7C56B6B0,0x71159069,0x75D48DDE,
	0x6B93DDDB,0x6F52C06C,0x6211E6B5,0x66D0FB02,
	0x5E9F46BF,0x5A5E5B08,0x571D7DD1,0x53DC6066,
	0x4D9B3063,0x495A2DD4,0x44190B0D,0x40D816BA,
	0xACA5C697,0xA864DB20,0xA527FDF9,0xA1E6E04E,
	0xBFA1B04B,0xBB60ADFC,0xB6238B25,0xB2E29692,
	0x8AAD2B2F,0x8E6C3698,0x832F1041,0x87EE0DF6,
	0x99A95DF3,0x9D684044,0x902B669D,0x94EA7B2A,
	0xE0B41DE7,0xE4750050,0xE9362689,0xEDF73B3E,
	0xF3B06B3B,0xF771768C,0xFA325055,0xFEF34DE2,
	0xC6BCF05F,0xC27DEDE8,0xCF3ECB31,0xCBFFD686,
	0xD5B88683,0xD1799B34,0xDC3ABDED,0xD8FBA05A,
	0x690CE0EE,0x6DCDFD59,0x608EDB80,0x644FC637,
	0x7A089632,0x7EC98B85,0x738AAD5C,0x774BB0EB,
	0x4F040D56,0x4BC510E1,0x46863638,0x42472B8F,
	0x5C007B8A,0x58C1663D,0x558240E4,0x51435D53,
	0x251D3B9E,0x21DC2629,0x2C9F00F0,0x285E1D47,
	0x36194D42,0x32D850F5,0x3F9B762C,0x3B5A6B9B,
	0x0315D626,0x07D4CB91,0x0A97ED48,0x0E56F0FF,
	0x1011A0FA,0x14D0BD4D,0x19939B94,0x1D528623,
	0xF12F560E,0xF5EE4BB9,0xF8AD6D60,0xFC6C70D7,
	0xE22B20D2,0xE6EA3D65,0xEBA91BBC,0xEF68060B,
	0xD727BBB6,0xD3E6A601,0xDEA580D8,0xDA649D6F,
	0xC423CD6A,0xC0E2D0DD,0xCDA1F604,0xC960EBB3,
	0xBD3E8D7E,0xB9FF90C9,0xB4BCB610,0xB07DABA7,
	0xAE3AFBA2,0xAAFBE615,0xA7B8C0CC,0xA379DD7B,
	0x9B3660C6,0x9FF77D71,0x92B45BA8,0x9675461F,
	0x8832161A,0x8CF30BAD,0x81B02D74,0x857130C3,
	0x5D8A9099,0x594B8D2E,0x5408ABF7,0x50C9B640,
	0x4E8EE645,0x4A4FFBF2,0x470CDD2B,0x43CDC09C,
	0x7B827D21,0x7F436096,0x7200464F,0x76C15BF8,
	0x68860BFD,0x6C47164A,0x61043093,0x65C52D24,
	0x119B4BE9,0x155A565E,0x18197087,0x1CD86D30,
	0x029F3D35,0x065E2082,0x0B1D065B,0x0FDC1BEC,
	0x3793A651,0x3352BBE6,0x3E119D3F,0x3AD08088,
	0x2497D08D,0x2056CD3A,0x2D15EBE3,0x29D4F654,
	0xC5A92679,0xC1683BCE,0xCC2B1D17,0xC8EA00A0,
	0xD6AD50A5,0xD26C4D12,0xDF2F6BCB,0xDBEE767C,
	0xE3A1CBC1,0xE760D676,0xEA23F0AF,0xEEE2ED18,
	0xF0A5BD1D,0xF464A0AA,0xF9278673,0xFDE69BC4,
	0x89B8FD09,0x8D79E0BE,0x803AC667,0x84FBDBD0,
	0x9ABC8BD5,0x9E7D9662,0x933EB0BB,0x97FFAD0C,
	0xAFB010B1,0xAB710D06,0xA6322BDF,0xA2F33668,
	0xBCB4666D,0xB8757BDA,0xB5365D03,0xB1F740B4
};

uint32 calc_crc32(uint8 *pData,
				  int32 nLength)
{
	uint32 crc32 = 0xffffffff;
	uint32 tmp1, tmp2, tmp3;
	int32 iIndex;

	for(iIndex = 0; iIndex < nLength; iIndex++)
	{
		tmp1 = (crc32<<8);
		tmp2 = m_crc32Table[((crc32 >> 24) ^ *pData) & 0xff];
		tmp3 = tmp1 ^ tmp2;
		crc32 = (crc32 << 8) ^ m_crc32Table[((crc32>>24) ^ *pData++) & 0xff];
	}
	return crc32;
} 

QString TOC::print_crc(char *buf, int len)
{
	QString s;
	s = tr("\n[ Verify ]-----------------------------------------------------------------------\n");
	KH *pKh = (KH*)buf;
	char *data = buf + sizeof(KH);
	if(pKh->cmd_length <= 0)
	{
		s += tr("\t NO DATA \n");
		return s;
	}

	uint32 crc = calc_crc32((uint8*)data, pKh->cmd_length - 4);
	char *crc2 = buf + sizeof(KH) + pKh->cmd_length - 4;
	if(crc == *(uint32*)(crc2))	{		s += QString(tr("4          CRC32           0x%1 - CRC32 %2"))
			.arg(QString::number(crc, 16))
			.arg(tr("Correct"));
	}	else
	{		s += QString(tr("4          CRC32           0x%1 0x%2- CRC32 %3"))
			.arg(QString::number(crc, 16))
			.arg(QString::number(*(uint32*)crc2, 16))
			.arg(tr("Error"));
	}	return s;}QString TOC::print_proto(char *buf, int len){	QString s;	return s;}







