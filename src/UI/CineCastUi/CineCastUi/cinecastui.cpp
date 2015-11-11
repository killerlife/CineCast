#include "cinecastui.h"
#include "status.h"
#include "setup.h"

int m_ConnectStatus = 0;

CineCastUi::CineCastUi(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags), statusForm(NULL), statusWid(NULL)
{
	ui.setupUi(this);
	Init();
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
//  	this->setStyleSheet("background-color:#336699");     //#0099CCÆ«°µ #3366CCÆ«ÁÁ
	ui.tabWidget->setTabShape(QTabWidget::Triangular);
// 	ui.tabWidget->setStyleSheet("QTabBar{color:white;border-color:#333333}");
	ui.tabWidget->setCurrentIndex(0);

	connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(Error(QAbstractSocket::SocketError)));
	connect(&socket, SIGNAL(connected()), this, SLOT(Connected()));
	m_timer = startTimer(500);

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
}

void CineCastUi::timerEvent(QTimerEvent * te)
{
	extern QString rIp;
	if(te->timerId() == m_timer)
	{
		switch(m_ConnectStatus)
		{
		case 0:
			socket.connectToHost(rIp, 10003);
			m_ConnectStatus = 1;
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
}

void CineCastUi::Connected()
{
	m_ConnectStatus = 2;
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
			+ sizeof(uint16);
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
	if(nIndex == 3) //Setup page
	{
		if(m_ConnectStatus == 2)
			setupForm->LoadConfig();
	}
}