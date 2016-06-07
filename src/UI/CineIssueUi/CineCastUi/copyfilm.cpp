#include "copyfilm.h"
#include <QTcpSocket>
#include "content.h"
#include "../../../netcomm/UiProtocal.h"

class ListItem : public QListWidgetItem, public InfoData
{
public:
	// 	ListItem(int type = Type):QListWidgetItem(type){};

	void MakeItem();
};

CopyFilm::CopyFilm(QTcpSocket* pSocket, QWidget *parent)
	: QDialog(parent), mTimer(-1), bCopy(false)
{
	this->pSocket = pSocket;
	ui.setupUi(this);
	Init();
}

CopyFilm::~CopyFilm()
{

}

void CopyFilm::Init()
{
	ui.label_3->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_2->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_5->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_4->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.groupBox->setStyleSheet("QGroupBox{font-size: 18px; font-family:'Book Antiqua';}");
	ui.groupBox_2->setStyleSheet("QGroupBox{font-size: 18px; font-family:'Book Antiqua';}");
	ui.progressBar->setStyleSheet("QProgressBar{font-size: 18px; font-family:'Book Antiqua';}");
	ui.progressBar_2->setStyleSheet("QProgressBar{font-size: 18px; font-family:'Book Antiqua';}");
	ui.pushButton->setStyleSheet("QPushButton{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_7->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua'; color:#FF000F;}");
	ui.label_6->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua'; color:#FF000F;}");
	ui.listWidget->setVisible(false);

	ui.progressBar->setRange(0,10000); 
	ui.progressBar_2->setRange(0,100); 
}

void CopyFilm::reject()
{
	if(!bCopy)
		QDialog::reject();
}

void CopyFilm::accept()
{
	if(!bCopy)
		QDialog::accept();
}

void CopyFilm::SetCopyList(QList<ContentItem*> &clist, std::string strDest)
{
	this->strDest = strDest;
	uint64 n = 0;
	for(int i = 0; i < clist.size(); i ++)
	{
		ContentItem* item = clist.at(i);
		ListItem *litem = new ListItem;
		for(int j = 0; j < 16; j ++)
			litem->pData[j] = item->pData[j];
		litem->MakeItem();
		ui.listWidget->addItem((QListWidgetItem*)litem);
#ifdef WIN32
		uint64 len = _atoi64(litem->pData[C_FILM_LENGTH].c_str());
#else
		uint64 len = atoll(litem->pData[C_FILM_LENGTH].c_str());
#endif
		n += len;
	}
	lenTotal = n;
	lenCurrent = 0;
	nCurrent = 0;
	nTotal = ui.listWidget->count();
	QString s;
	s.sprintf("%d/%d", nCurrent, nTotal);
	ui.label_2->setText(s);
	s.sprintf("%.2f/%.2f GB", (float)lenCurrent/1024/1024/1024, (float)lenTotal/1024/1024/1024);
	ui.label_4->setText(s);
}

extern  int m_ConnectStatus;    //״̬    =2=1=0
extern char buf[2048];   // Ѿŵǰ

void CopyFilm::on_pushButton_clicked()
{
	bCopy = true;
	ui.pushButton->setEnabled(false);
	mTimer = startTimer(1000);
	bRunNew = true;
}

void CopyFilm::timerEvent(QTimerEvent * te)
{
	if(te->timerId() == mTimer)
	{
		if (bRunNew)
		{
			CopyNextFilm();
		}
		else
		{
			CopyProgress();
		}
	}
}

void CopyFilm::CopyNextFilm()
{
	if(m_ConnectStatus == 2)
	{
		if(ui.listWidget->count() > 0)
		{
			ListItem * item = (ListItem *)ui.listWidget->item(0);
			std::string src = ((ListItem*)item)->pData[C_LOCATE];

			std::string dest = strDest;
			copy_path path;

			int index;
			for(int i=0;i<src.size();i++)
			{
				if(src.at(i)=='/')
					index = i;
			}
			dest += (src.c_str() + index);
			memcpy(path.path_src, src.c_str(), src.size());
			memcpy(path.path_dst, dest.c_str(), dest.size());
			ui.label->setText(item->pData[C_NAME].c_str());

			KL *pKL = (KL*)buf;
			pKL->m_pkgHead = 0x7585;
			pKL->m_keyID =M_COPYDIR_HDD_TO_USB;  //M_COPYDIR_HDD_TO_USB;

			pKL->m_length = sizeof(path);
			int sendsize = sizeof(path) + sizeof(KL);
			void* pos = buf + sizeof(KL);
			memcpy(pos,&path,sizeof(path));
			memcpy(&path,pos,sizeof(path));

			pSocket->write(buf,sendsize);
			pSocket->waitForBytesWritten(-1);
			pSocket->waitForReadyRead(-1);

			int i = pSocket->read(buf, 2048);
			if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_COPYDIR_HDD_TO_USB)
			{
				bRunNew = false;
				nCurrent++;
				QString s;
				s.sprintf("%d/%d", nCurrent, nTotal);
				ui.label_2->setText(s);
			}
		}
		else
		{
			killTimer(mTimer);
			mTimer = -1;
			QDialog::accept();
		}
	}
}

void CopyFilm::CopyProgress()
{
	if(m_ConnectStatus==2) 
	{
		KL *pKL = (KL*)buf;

		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = M_GETCOPYPROCESS;

		int percent=1; 
		pKL->m_length =sizeof(int)+sizeof(int);  
		int sendsize = sizeof(KL)+sizeof(int)+sizeof(int);
		void* pos = buf + sizeof(KL);
		memcpy(pos,&percent,sizeof(int));

		int copy_flag=1;  
		void* pos2 = buf + sizeof(KL)+sizeof(int);
		memcpy(pos2,&copy_flag,sizeof(int));

		pSocket->write(buf,sendsize);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int i =pSocket->read(buf, 2048);

		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_GETCOPYPROCESS)
		{
			int get_percent=1;
			void* pos=buf+sizeof(KL);
			memcpy(&get_percent, pos,sizeof(int));


			int get_copy_flag=1;
			void* pos2=buf+sizeof(KL)+sizeof(int);
			memcpy(&get_copy_flag, pos2,sizeof(int));

			ListItem * item = (ListItem *)ui.listWidget->item(0);

			if(get_percent>=0 && get_percent<=10000)
			{
				uint64 current = lenCurrent;
#ifdef WIN32
				uint64 len = _atoi64(item->pData[C_FILM_LENGTH].c_str());
#else
				uint64 len = atoll(item->pData[C_FILM_LENGTH].c_str());
#endif
				current += (len * get_percent)/10000;
				ui.progressBar->setValue(get_percent);
				QString s;
				s.sprintf("%.2f/%.2f GB", (float)current/1024/1024/1024, (float)lenTotal/1024/1024/1024);
				ui.label_4->setText(s);
				ui.progressBar_2->setValue(current*100/lenTotal);
			}

			if(get_copy_flag==0) 
			{
				ListItem* item = (ListItem*)ui.listWidget->takeItem(0);
				uint64 current = lenCurrent;
#ifdef WIN32
				uint64 len = _atoi64(item->pData[C_FILM_LENGTH].c_str());
#else
				uint64 len = atoll(item->pData[C_FILM_LENGTH].c_str());
#endif
				current += len;
				lenCurrent = current;
				ui.progressBar->setValue(get_percent);
				QString s;
				s.sprintf("%.2f/%.2f GB", (float)current/1024/1024/1024, (float)lenTotal/1024/1024/1024);
				ui.label_4->setText(s);
				ui.progressBar_2->setValue(current*100/lenTotal);
				delete item;
				bRunNew = true;
			}
		}
	}
}