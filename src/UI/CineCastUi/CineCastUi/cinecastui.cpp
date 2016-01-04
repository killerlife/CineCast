#include "cinecastui.h"
#include "status.h"
#include "setup.h"
#include "disk.h"
#include "content.h"
#include "live.h"
#include "raid.h"
#include "tkr.h"

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
	this->setWindowState(Qt::WindowFullScreen);
	ui.label_LOGO->setStyleSheet("QLabel{color:white}");
//  	this->setStyleSheet("background-color:#336699");     //#0099CCƫ #3366CCƫ
	ui.tabWidget->setTabShape(QTabWidget::Triangular);
// 	ui.tabWidget->setStyleSheet("QTabBar{color:white;border-color:#333333}");
	ui.tabWidget->setCurrentIndex(0);

	connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(Error(QAbstractSocket::SocketError)));
	connect(&socket, SIGNAL(connected()), this, SLOT(Connected()));
	m_network_timer = startTimer(1000);  //״̬ʱ //old 500

	statusForm = new Status;
	statusWid =  ui.mdiArea_statue->addSubWindow(statusForm);
	statusWid->setWindowFlags(Qt::FramelessWindowHint);
	statusWid->showMaximized();
	statusWid->setStyleSheet("background-color:#cccccc");
	statusWid->show();

	setupForm = new Setup(&socket);
	setupWid = ui.mdiArea_Setup->addSubWindow(setupForm);
	setupWid->setWindowFlags(Qt::FramelessWindowHint);
	setupWid->showMaximized();
	setupWid->setStyleSheet("background-color:#cccccc");
	setupWid->show();

	diskForm = new Disk(&socket);
	diskWid = ui.mdiArea_Maintain->addSubWindow(diskForm);
	diskWid->setWindowFlags(Qt::FramelessWindowHint);
	diskWid->showMaximized();
	diskWid->setStyleSheet("background-color:#cccccc");
	diskWid->show();

	contentForm = new Content(&socket);
	contentWid = ui.mdiArea_ContentManagement->addSubWindow(contentForm);
	contentWid->setWindowFlags(Qt::FramelessWindowHint);
	contentWid->showMaximized();
	contentWid->setStyleSheet("background-color:#cccccc");
	contentWid->show();

	raidForm = new Raid(&socket);
	raidWid = ui.mdiArea_RAID->addSubWindow(raidForm);
	raidWid->setWindowFlags(Qt::FramelessWindowHint);
	raidWid->showMaximized();
	raidWid->setStyleSheet("background-color:#cccccc");
	raidWid->show();

	liveForm = new Live(&socket);
	liveWid = ui.mdiArea_Live->addSubWindow(liveForm);
	liveWid->setWindowFlags(Qt::FramelessWindowHint);
	liveWid->showMaximized();
	liveWid->setStyleSheet("background-color:#cccccc");
	liveWid->show();

	tkrForm = new Tkr(&socket);
	tkrWid = ui.mdiArea_TKR->addSubWindow(tkrForm);
	tkrWid->setWindowFlags(Qt::FramelessWindowHint);
	tkrWid->showMaximized();
	tkrWid->setStyleSheet("background-color:#cccccc");
	tkrWid->show();
}

void CineCastUi::timerEvent(QTimerEvent * te)
{
	extern QString rIp;
	if(te->timerId() == m_network_timer)
	{
		switch(m_ConnectStatus)
		{
		case 0:
			socket.connectToHost(rIp, 10003);
			m_ConnectStatus = 1;
			
			break;
		case 1:
			//ui.label_NetworkStatus->setText("NetWork Status: Connecting ");  //new
			break;
		case 2:
		//	UiFilter();
			break;
		}
	}

	if(te->timerId() == m_UpdateSatellite_timer)
	{
		switch(m_ConnectStatus)
		{
		case 0:
			break;
		case 1:
			break;
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

    m_network_timer = startTimer(1000);   //·ʱɹرնʱ

   	killTimer(m_UpdateSatellite_timer);
	m_UpdateSatellite_timer = -1;
}

void CineCastUi::Connected()
{
	m_ConnectStatus = 2;
	//ui.label_NetworkStatus->setText("NetWork Status:     OK     ");

	//·ʱɹرնʱ
	killTimer(m_network_timer);
	m_network_timer = -1;


	m_UpdateSatellite_timer = startTimer(1000);   //ӳɹԲ״̬
}

#include "../../../netcomm/UiProtocal.h"

char buf[2048];

void CineCastUi::UiFilter()
{
	KL *pKL = (KL*)buf;
	int i;

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
			}
		}
		statusForm->UpdateRecv((RECEIVE_INFO*)&recv);
	}
}

void CineCastUi::on_tabWidget_currentChanged(int nIndex)
{
	if(3==nIndex)                    //лҳ3Żȡ־
	{
		   setupForm->TMS_start();   //ȡ־ʱ
	}
	else
	{
	       setupForm->TMS_stop();    //رջȡ־ʱ 
	}
	switch(nIndex)
	{
	case 0:
		if(m_network_timer == -1)
		//	m_network_timer = startTimer(500);
		break;
	case 1: //Content Page
		if (m_ConnectStatus == 2)
		{
		//	killTimer(m_network_timer);
		//	m_network_timer = -1;
		 //   contentForm->LoadContent_HDD();   //ŵUpdateContentUI
		 //   contentForm->LoadContent_USB();   //new
            contentForm->UpdateContentUI();
		}
		break;
	case 2:
		if(m_ConnectStatus == 2)
		{
		//	killTimer(m_network_timer);
		//	m_network_timer = -1;
		}
		break;
	case 3: //Setup Page
		if(m_ConnectStatus == 2)
		{
		//	killTimer(m_network_timer);
		//	m_network_timer = -1;
			setupForm->LoadConfig();
	}
		break;
	}
}





void CineCastUi::on_pushButton_Reboot_clicked()
{
    switch(QMessageBox::question(this,"Question",tr("Do you want to Reboot"),
    		QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
    case QMessageBox::Ok:
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

    	break;
    case QMessageBox::Cancel:
    	break;
    default:
    	break;
    }
    return;
}