#include "cinecastui.h"
#include "status.h"
#include "setup.h"
#include "disk.h"
#include "content.h"
#include "live.h"
#include "raid.h"
#include "tkr.h"
#include <QPixmap>

int m_ConnectStatus = 0;    //״̬    =2=1=0

CineCastUi::CineCastUi(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags), statusForm(NULL), statusWid(NULL)
{
	ui.setupUi(this);
	Init();

	//this->showNormal();    //ʱһ㲻Ҫȫʱ?
	//this->showMaximized(); 
}

CineCastUi::~CineCastUi()
{
	if (statusForm)
	{
		delete statusForm;
	}
}

void CineCastUi::Init()
{
#ifndef WIN32
	setWindowFlags(Qt::WindowStaysOnTopHint);
#endif
	this->setWindowState(Qt::WindowFullScreen);
	ui.label_LOGO->setStyleSheet("QLabel{color:white}");
//  	this->setStyleSheet("background-color:#336699");     //#0099CCƫ #3366CCƫ
//   	this->setStyleSheet("background-color:#BBBBBB");     //#0099CCƫ #3366CCƫ
	ui.tabWidget->setTabShape(QTabWidget::Triangular);
	ui.tabWidget->setMovable(false);
// 	ui.tabWidget->setStyleSheet("QTabBar{color:white;border-color:#333333}");

	connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(Error(QAbstractSocket::SocketError)));
	connect(&socket, SIGNAL(connected()), this, SLOT(Connected()));
	m_network_timer = startTimer(1000);  //״̬ʱ //old 500
	m_UpdateSatellite_timer = startTimer(1000);   //ӳɹԲ״̬

	statusForm = new Status(ui.tabWidget);
	statusWid =  ui.mdiArea_statue->addSubWindow(statusForm);
	statusWid->setWindowFlags(Qt::FramelessWindowHint);
	statusWid->showMaximized();
 	statusWid->setStyleSheet("background-color:#d4d0c8");
	statusWid->show();

	setupForm = new Setup(&socket);
	setupWid = ui.mdiArea_Setup->addSubWindow(setupForm);
	setupWid->setWindowFlags(Qt::FramelessWindowHint);
	setupWid->showMaximized();
 	setupWid->setStyleSheet("background-color:#d4d0c8");
	setupWid->show();

	diskForm = new Disk(&socket, ui.tabWidget);
	diskWid = ui.mdiArea_Maintain->addSubWindow(diskForm);
	diskWid->setWindowFlags(Qt::FramelessWindowHint);
	diskWid->showMaximized();
 	diskWid->setStyleSheet("background-color:#d4d0c8");
	diskWid->show();

	contentForm = new Content(&socket, ui.tabWidget);
	contentWid = ui.mdiArea_ContentManagement->addSubWindow(contentForm);
	contentWid->setWindowFlags(Qt::FramelessWindowHint);
	contentWid->showMaximized();
 	contentWid->setStyleSheet("background-color:#d4d0c8");
	contentWid->show();

	raidForm = new Raid(&socket);
	liveForm = new Live(&socket);
	tkrForm = new Tkr(&socket);
	
#ifdef FULL_VERSION
	raidWid = ui.mdiArea_RAID->addSubWindow(raidForm);
	raidWid->setWindowFlags(Qt::FramelessWindowHint);
	raidWid->showMaximized();
	raidWid->setStyleSheet("background-color:#d4d0c8");
	raidWid->show();

	liveWid = ui.mdiArea_Live->addSubWindow(liveForm);
	liveWid->setWindowFlags(Qt::FramelessWindowHint);
	liveWid->showMaximized();
	liveWid->setStyleSheet("background-color:#d4d0c8");
	liveWid->show();

	tkrWid = ui.mdiArea_TKR->addSubWindow(tkrForm);
	tkrWid->setWindowFlags(Qt::FramelessWindowHint);
	tkrWid->showMaximized();
	tkrWid->setStyleSheet("background-color:#d4d0c8");
	tkrWid->show();
#else
	ui.label->setVisible(false);
	ui.tabWidget->removeTab(6);
	ui.tabWidget->removeTab(5);
	ui.tabWidget->removeTab(4);
#endif
	ui.tabWidget->setCurrentIndex(0);

	ui.label_Version->setStyleSheet("QLabel{color:white}");

	m_time_timer = startTimer(500);
	ui.pushButton_Reboot->setToolTip(tr("Reboot Machine"));
	ui.pushButton_Shutdown->setToolTip(tr("Shutdown Machine"));
}

extern bool bFormat;
void CineCastUi::timerEvent(QTimerEvent * te)
{
	extern QString rIp;
	QPixmap pix;
	if(te->timerId() == m_time_timer)
	{
		QTime t = QTime::currentTime();
		ui.label_time->setText(t.toString("hh:mm:ss"));
		if(bFormat)
		{
			ui.pushButton_Reboot->setEnabled(false);
			ui.pushButton_Shutdown->setEnabled(false);
		}
		else
		{
			ui.pushButton_Reboot->setEnabled(true);
			ui.pushButton_Shutdown->setEnabled(true);
		}
	}
	if(te->timerId() == m_network_timer)
	{
		switch(m_ConnectStatus)
		{
		case 0:
			pix.load(QString::fromUtf8(":/CineCastUi/Resources/recv.png"));
			ui.label->setPixmap(pix);
			socket.close();
			socket.connectToHost(rIp, 10003);
			m_ConnectStatus = 1;
			
			break;
		case 1:
			pix.load(QString::fromUtf8(":/CineCastUi/Resources/recv.png"));
			ui.label->setPixmap(pix);
			//ui.label_NetworkStatus->setText("NetWork Status: Connecting ");  //new
			break;
		case 2:
			pix.load(QString::fromUtf8(":/CineCastUi/Resources/idle.png"));
			ui.label->setPixmap(pix);
		//	UiFilter();
			break;
		}
	}

	if(te->timerId() == m_UpdateSatellite_timer)
	{
		switch(m_ConnectStatus)
		{
// 		case 0:
// 			break;
// 		case 1:
// 			break;
		case 2:
			UiFilter();
			break;
		}
	}

}

void CineCastUi::Error(QAbstractSocket::SocketError socketError)
{
	m_ConnectStatus = 0;
	//ui.label_NetworkStatus->setText("NetWork Status:   Error    ");

//     m_network_timer = startTimer(1000);   //·ʱɹرնʱ

	if(m_UpdateSatellite_timer > 0)
	{
   	killTimer(m_UpdateSatellite_timer);
	m_UpdateSatellite_timer = -1;
}
}

void CineCastUi::Connected()
{
	m_ConnectStatus = 2;
	//ui.label_NetworkStatus->setText("NetWork Status:     OK     ");

	//·ʱɹرնʱ
// 	killTimer(m_network_timer);
// 	m_network_timer = -1;


	if (m_UpdateSatellite_timer < 0 && ui.tabWidget->currentIndex() == 0)
	{
		m_UpdateSatellite_timer = startTimer(1000);   //ӳɹԲ״̬
	}
}

#include "../../../netcomm/UiProtocal.h"

char buf[2048];

void CineCastUi::UiFilter()
{
	KL *pKL = (KL*)buf;
	int i;

	//Get Version 
	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = S_GET_VERSION;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	socket.write(buf, sizeof(KL) + 1);
	socket.waitForBytesWritten(-1);
	socket.waitForReadyRead(-1);
	i = socket.read(buf, 2048);
	if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_GET_VERSION)
	{
		ui.label_Version->setText(buf + sizeof(KL));
	}

	//Get Satellite Status
	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = S_GET_SATELLITE;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	socket.write(buf, sizeof(KL) + 1);
	socket.waitForBytesWritten(-1);
	socket.waitForReadyRead(-1);
	i = socket.read(buf, 2048);
	if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_GET_SATELLITE)
	{
		statusForm->UpdateSatellite((TUNER_INFO*) (buf+sizeof(KL)));
	}

	//Get Receive Status
	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = S_GET_RECEIVE;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	socket.write(buf, sizeof(KL) + 1);
	socket.waitForBytesWritten(-1);
	socket.waitForReadyRead(-1);
	i = socket.read(buf, 2048);
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
		statusForm->UpdateRecv((RECEIVE_INFO*)&recv);
	}
}

void CineCastUi::on_tabWidget_currentChanged(int nIndex)
{
	switch(nIndex)
	{
	case 0:
		raidForm->Stop();
		setupForm->TMS_stop();    //????? 
		if (m_UpdateSatellite_timer < 0)
		{
			m_UpdateSatellite_timer = startTimer(1000);   //ӳɹԲ״̬
		}
		break;
	case 1: //Content Page
		if(m_UpdateSatellite_timer > 0)
		{
			killTimer(m_UpdateSatellite_timer);
			m_UpdateSatellite_timer = -1;
		}
		raidForm->Stop();
		setupForm->TMS_stop();    //????? 
		if (m_ConnectStatus == 2)
		{
            contentForm->UpdateContentUI();
		}
		break;
	case 2:
		if(m_UpdateSatellite_timer > 0)
		{
			killTimer(m_UpdateSatellite_timer);
			m_UpdateSatellite_timer = -1;
		}
		raidForm->Stop();
		setupForm->TMS_stop();    //????? 
		diskForm->SetStatus(statusForm->m_Status);
		break;
	case 3: //Setup Page
		if(m_UpdateSatellite_timer > 0)
		{
			killTimer(m_UpdateSatellite_timer);
			m_UpdateSatellite_timer = -1;
		}
		raidForm->Stop();
		if(m_ConnectStatus == 2)
		{
			setupForm->LoadConfig();
			setupForm->TMS_start();   //???
	}
		break;
	case 4:
		if(m_UpdateSatellite_timer > 0)
		{
			killTimer(m_UpdateSatellite_timer);
			m_UpdateSatellite_timer = -1;
		}
		if (m_ConnectStatus == 2)
		{
			raidForm->Start();
		}
		break;
	default:
		if(m_UpdateSatellite_timer > 0)
		{
			killTimer(m_UpdateSatellite_timer);
			m_UpdateSatellite_timer = -1;
		}
		raidForm->Stop();
	}
}

extern QString rIp;
void CineCastUi::on_pushButton_Reboot_clicked()
{
    switch(QMessageBox::question(this,"Question",tr("Do you want to Reboot"),
    		QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
		{
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
					socket.write(buf, sizeof(KL) + 1);
					socket.waitForBytesWritten(-1);
					socket.waitForReadyRead(-1);

					i = socket.read(buf, 2048);
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
					socket.write(buf, sizeof(KL) + 1);
					socket.waitForBytesWritten(-1);
					socket.waitForReadyRead(-1);
				    
					i = socket.read(buf, 2048);
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

void CineCastUi::on_pushButton_Shutdown_clicked()
{
    switch(QMessageBox::question(this,"Question",tr("Do you want to Shutdown"),
    		QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
		{
			if(rIp == "127.0.0.1")
			{
#ifdef WIN32
				if(m_ConnectStatus==2)     
				{
					KL *pKL = (KL*)buf;
					int i;

					pKL->m_pkgHead = 0x7585;
					pKL->m_keyID = S_SHUTDOWN;
					pKL->m_length = 1;
					buf[sizeof(KL)] = 1;
					socket.write(buf, sizeof(KL) + 1);
					socket.waitForBytesWritten(-1);
					socket.waitForReadyRead(-1);

					i = socket.read(buf, 2048);
					if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_SHUTDOWN)
					{
					    QMessageBox::information(this,"Information",tr("Shutdown successful"));
                        return;
					}
				 }
					else
				{
					 QMessageBox::information(this,"Information",tr("Shutdown failed,network errors, operation failed"));
                     return;
				}
#else
				system("/bin/sync");
				system("/sbin/init 0");
#endif
		}
			else
			{
				if(m_ConnectStatus==2)     
				{
					KL *pKL = (KL*)buf;
					int i;

					pKL->m_pkgHead = 0x7585;
					pKL->m_keyID = S_SHUTDOWN;
					pKL->m_length = 1;
					buf[sizeof(KL)] = 1;
					socket.write(buf, sizeof(KL) + 1);
					socket.waitForBytesWritten(-1);
					socket.waitForReadyRead(-1);

					i = socket.read(buf, 2048);
					if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == S_SHUTDOWN)
					{
						QMessageBox::information(this,"Information",tr("Shutdown successful"));
						return;
					}
				}
				else
				{
					QMessageBox::information(this,"Information",tr("Shutdown failed,network errors, operation failed"));
					return;
				}
			}
		}
    	break;
    case QMessageBox::Cancel:
    	break;
    default:
    	break;
    }
    return;
}