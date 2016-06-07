#include "delconfirm.h"
#include <QTcpSocket>
#include <QListWidgetItem>
#include "content.h"
#include "../../../netcomm/UiProtocal.h"

extern  int m_ConnectStatus;    //״̬    =2=1=0
extern char buf[2048];   // Ѿŵǰ

class ListItem : public QListWidgetItem, public InfoData
{
public:
// 	ListItem(int type = Type):QListWidgetItem(type){};

	void MakeItem();
};

void ListItem::MakeItem()
{
	setText(pData[C_NAME].c_str());
}

DelConfirm::DelConfirm(QTcpSocket* pSocket, QWidget *parent)
	: QDialog(parent)
{
	this->pSocket = pSocket;
	ui.setupUi(this);
	Init();
}

DelConfirm::~DelConfirm()
{

}

void DelConfirm::Init()
{
	ui.label->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.groupBox->setStyleSheet("QGroupBox{font-size: 18px; font-family:'Book Antiqua';}");
	ui.groupBox_2->setStyleSheet("QGroupBox{font-size: 18px; font-family:'Book Antiqua';}");
	ui.listWidgetRaid->setStyleSheet("QListWidget{font-size: 18px; font-family:'Book Antiqua';}");
	ui.listWidgetRD->setStyleSheet("QListWidget{font-size: 18px; font-family:'Book Antiqua';}");
	ui.pushButtonDelete->setStyleSheet("QPushButton{font-size: 18px; font-family:'Book Antiqua';}");
	ui.pushButtonCancel->setStyleSheet("QPushButton{font-size: 18px; font-family:'Book Antiqua';}");
}

void DelConfirm::on_pushButtonCancel_clicked()
{
	QDialog::reject();
}

void DelConfirm::on_pushButtonDelete_clicked()
{
	for(int i = 0; i < ui.listWidgetRaid->count(); i++)
	{
		ListItem *item = (ListItem*)ui.listWidgetRaid->item(i);
		std::string path = item->pData[C_LOCATE];
	}

	for(int i = 0; i < ui.listWidgetRD->count(); i++)
	{
		ListItem *item = (ListItem*)ui.listWidgetRD->item(i);
		std::string path = item->pData[C_LOCATE];
		DeleteFilm(path);
	}

	QDialog::accept();
}

void DelConfirm::SetDeleteList(QList<ContentItem*> &raidList, QList<ContentItem*> &rdList)
{
	for (int i = 0; i < raidList.size(); i++)
	{
		ContentItem* item = raidList.at(i);
		ListItem *litem = new ListItem;
		for(int j = 0; j < 16; j++)
			litem->pData[j] = item->pData[j];
		litem->MakeItem();
		ui.listWidgetRaid->addItem((QListWidgetItem*)litem);
	}
	for (int i = 0; i < rdList.size(); i++)
	{
		ContentItem* item = rdList.at(i);
		ListItem *litem = new ListItem;
		for(int j = 0; j < 16; j++)
			litem->pData[j] = item->pData[j];
		litem->MakeItem();
		ui.listWidgetRD->addItem((QListWidgetItem*)litem);
	}
}

void DelConfirm::DeleteFilm(std::string path)
{
	if(m_ConnectStatus==2)    
	{
		char path_del[512];
		strcpy(path_del, path.c_str());

		KL *pKL = (KL*)buf;

		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = M_DELETE_DIR;


		pKL->m_length = sizeof(path_del);
		int sendsize = sizeof(path_del) + sizeof(KL);
		void* pos = buf + sizeof(KL);
		memcpy(pos,path_del,sizeof(path_del));

		memcpy(path_del,pos,sizeof(path_del));

		pSocket->write(buf,sendsize);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int i = pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_DELETE_DIR)
		{
			// return;
		}
	}
}