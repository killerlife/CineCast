#include "disk.h"
#include <QTcpSocket>


//new
extern  int m_ConnectStatus;    //״̬    =2=1=0


Disk::Disk(QTcpSocket * socket, QWidget *parent)
	: QWidget(parent), m_Timer(0)
{
	ui.setupUi(this);
	Init();
	pSocket = socket;
}

Disk::~Disk()
{

}

void Disk::Init()
{
	ui.progressBar->setStyleSheet("QProgressBar{font-size: 18px; font-family:'Book Antiqua';}");
	ui.textBrowser->setStyleSheet("QTextBrowser{font-size: 18px; font-family:'Book Antiqua';}");
}

#include "../../../netcomm/UiProtocal.h"

void Disk::on_pushButton_format_clicked()
{
	char buf[1024];

  switch(QMessageBox::question(this,"Question",tr("Do you want to Format HDD_Disk"),
    		QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
		{
				if(m_ConnectStatus==2)    //ȷѾӷ
				{
	KL *pKL = (KL*)buf;

	int i;
	pKL->m_pkgHead = 0x7585;

	//Load Content list
	pKL->m_keyID = S_SET_FORMATDISK;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 1024);
	if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_SET_FORMATDISK)
	{
		m_Timer = startTimer(100);
					ui.pushButton_format->setEnabled(false);
					ui.progressBar->setValue(0);
                        return;
	}
				}
				else
				{
					 //ʾʾѾ
					 // ʧ
                     return;
				}
			}
			break;
		case QMessageBox::Cancel:
		//	label->setText(" Question button / Cancel ");
			break;
		default:
			break;
		}
		return;
}

void Disk::timerEvent(QTimerEvent * te)
{
	char buf[4096];
	if(te->timerId() == m_Timer)
	{
		//char *buf = new char[1024];
		KL *pKL = (KL*)buf;

		int i;
		pKL->m_pkgHead = 0x7585;

		//Load Format result
		pKL->m_keyID = S_GET_FORMAT_RES;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int nread = sizeof(KL);
		int pos = 0;
		i = 0;
		while (nread > 0)
		{
			i = pSocket->read(buf + pos, nread);
			nread -= i;
			pos += i;
		}
		nread = pKL->m_length;
		pos = 0;
		while (nread > 0)
		{
			i = pSocket->read(buf + sizeof(KL) + pos, nread);
			nread -= i;
			pos += i;
		}

		QString ss, sss;
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_GET_FORMAT_RES)
		{
			char *s = buf + sizeof(KL);
			buf[sizeof(KL) + pKL->m_length] = 0;
			sss = QString::fromUtf8(s);
			for(int i = 0; i < sss.size(); i++)
			{
				if(sss.at(i) != 0x8)
					ss.append(sss.at(i));
				else
					ss.resize(ss.size() - 1);
			}
			
			if(ss.contains("Writing superblocks and filesystem accounting information: done"))
				ui.progressBar->setValue(100);
			else if(ss.contains("Writing inode tables: done"))
				ui.progressBar->setValue(75);
			else if (ss.contains("Allocating group tables: done"))
				ui.progressBar->setValue(50);
			else if (ss.contains("Discarding device blocks: done"))
				ui.progressBar->setValue(25);
			else if(ss.contains(QString::fromLocal8Bit("Writing superblocks and filesystem accounting information: ")))
				ui.progressBar->setValue(100);
			else if(ss.contains(QString::fromLocal8Bit("дinode: ")))
				ui.progressBar->setValue(75);
			else if (ss.contains(QString::fromLocal8Bit("Allocating group tables: ")))
				ui.progressBar->setValue(50);
			else if (ss.contains(QString::fromLocal8Bit("Discarding device blocks: ")))
				ui.progressBar->setValue(25);
			ui.textBrowser->setText(ss);
		}
		
		//Load Format Status
		pKL->m_keyID = S_GET_FORMAT_STATUS;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);
		i = pSocket->read(buf, 1024);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_GET_FORMAT_STATUS)
		{
			char *s = buf + sizeof(KL);
			if(buf[sizeof(KL)] == 0)
			{
				if(m_Timer > 0)
				{
				killTimer(m_Timer);
					m_Timer = -1;
				}
				ui.pushButton_format->setEnabled(true);
			}
		}
	}
}