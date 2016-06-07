#include "disk.h"
#include <QTcpSocket>


//new
extern  int m_ConnectStatus;    //״̬    =2=1=0
bool bFormat = false;

Disk::Disk(QTcpSocket * socket, QWidget *parent)
	: QWidget(parent), m_Timer(0), p(parent)
{
	ui.setupUi(this);
	Init();
	pSocket = socket;
	m_del = startTimer(1000);
}

Disk::~Disk()
{

}

void Disk::Init()
{
	ui.progressBar->setStyleSheet("QProgressBar{font-size: 14px; font-family:'Book Antiqua';}");
	ui.textBrowser->setStyleSheet("QTextBrowser{font-size: 14px; font-family:'Book Antiqua';}");
	ui.pushButton_format->setStyleSheet("QPushButton{font-size: 14px; font-family:'Book Antiqua';}");
	ui.label->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua'; color:#0000FF;}");
}

#include "../../../netcomm/UiProtocal.h"

void Disk::on_pushButton_format_clicked()
{
	char buf[1024];
#if 1
	if(m_Status != 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Operation Forbidden! You can not format disk while receiving data."));
	}
	else
	{
		switch(QMessageBox::question(this, tr("Question"), tr("Do you want to Format HDD_Disk"),
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
						m_Timer = startTimer(1000);
						ui.pushButton_format->setEnabled(false);
						ui.progressBar->setValue(0);
						bFormat = true;
						hasDisk = false;
						((QTabWidget*)p)->setTabEnabled(0, false);
						((QTabWidget*)p)->setTabEnabled(1, false);
						((QTabWidget*)p)->setTabEnabled(3, false);
						ui.label->setText(tr("Disable another operation while formating."));
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
	}
	return;
#endif
}

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void Disk::timerEvent(QTimerEvent * te)
{
// 	if(te->timerId() == m_del)
// 	{
// 		if(bDel)
// 			ui.pushButton_format->setEnabled(false);
// 		else if(m_Timer == -1)
// 			ui.pushButton_format->setEnabled(true);
// 	}
	if(te->timerId() == m_Timer)
	{
		char buf[4096];
		KL *pKL = (KL*)buf;
REREAD:
		int i;
		pKL->m_pkgHead = 0x7585;
		//Load Format result
		pKL->m_keyID = S_GET_FORMAT_RES;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
#ifdef WIN32
		Sleep(500);
#else
		usleep(500000);
#endif
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
		ss = tr("Disk preparing, please wait...\n");
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_GET_FORMAT_RES)
		{
			char *s = buf + sizeof(KL);
			buf[sizeof(KL) + pKL->m_length] = 0;
			sss = QString::fromUtf8(s);
			int n = sss.lastIndexOf('\n');
			int b = sss.lastIndexOf(0x8);
			if(b > 0)
			{
				for(int i = b; i > 0; i--)
					if(sss.at(i) != 0x8)
					{
						b = i;
						break;
					}
			}
			if(n < b)
				n = b + 1;
			for(int i = 0; i < n; i++)
			{
				if(sss.at(i) != 0x8)
					ss.append(sss.at(i));
				else
					ss.resize(ss.size() - 1);
			}

			if(ss.contains("Writing superblocks and filesystem accounting information: done"))
			{
				hasDisk = true;
				ui.progressBar->setValue(100);
			}
			else if(ss.contains("Writing inode tables: done"))
			{
				hasDisk = true;
				ui.progressBar->setValue(75);
			}
			else if (ss.contains("Allocating group tables: done"))
			{
				hasDisk = true;
				ui.progressBar->setValue(50);
			}
			else if (ss.contains("Discarding device blocks: done"))
			{
				hasDisk = true;
				ui.progressBar->setValue(25);
			}
			else if(ss.contains(QString::fromLocal8Bit("Writing superblocks and filesystem accounting information: ")))
			{
				hasDisk = true;
				ui.progressBar->setValue(100);
			}
			else if(ss.contains(QString::fromLocal8Bit("正在写入inode表: ")))
			{
				hasDisk = true;
				ui.progressBar->setValue(75);
			}
			else if (ss.contains(QString::fromLocal8Bit("Allocating group tables: ")))
			{
				hasDisk = true;
				ui.progressBar->setValue(50);
			}
			else if (ss.contains(QString::fromLocal8Bit("Discarding device blocks: ")))
			{
				hasDisk = true;
				ui.progressBar->setValue(25);
			}
			ui.textBrowser->setText(ss);
			ui.textBrowser->moveCursor(QTextCursor::End);
		}
		//Load Format Status
		pKL->m_keyID = S_GET_FORMAT_STATUS;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
#ifdef WIN32
		Sleep(500);
#else
		usleep(500000);
#endif
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
					goto REREAD;
				}
				ui.pushButton_format->setEnabled(true);
				if(hasDisk)
					QMessageBox::information(this, tr("Information"), tr("Format complete!"));
				else
					QMessageBox::information(this, tr("Information"), tr("No Disk!"));
				ui.textBrowser->setText("");
				ui.progressBar->setValue(0);
				bFormat = false;
				((QTabWidget*)p)->setTabEnabled(0, true);
				((QTabWidget*)p)->setTabEnabled(1, true);
				((QTabWidget*)p)->setTabEnabled(3, true);
				ui.label->setText("");
			}
		}
	}
}