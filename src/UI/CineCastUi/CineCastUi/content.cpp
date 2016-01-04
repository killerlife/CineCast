#include "content.h"
#include <QTcpSocket>

Content::Content(QTcpSocket* pSocket, QWidget *parent)
	: QWidget(parent), pSocket(NULL)
{
	ui.setupUi(this);
	Init();
	this->pSocket = pSocket;

   //бָʼ
   for(int i=0;i<100;i++)
   {
	   item_HDD[i]=NULL;    
	   item_USB[i]=NULL;    
   }
    m_query_timer=-1;                     //ʱѯ
    m_isReady_timer_HDD=-1;              //ʱѯǷ׼
	m_isReady_timer_USB=-1;              //ʱѯǷ׼
}

Content::~Content()
{
	//бָ
	  for(int i=0;i<100;i++)
   {
	  if(item_HDD[i]!=NULL)
	  {
		  delete  item_HDD[i];
	  }
 	  if(item_USB[i]!=NULL)
	  {
		  delete  item_USB[i];
	  }
   }
}

void Content::Init()
{
	ui.pushButton_ExporttoUSB->setStyleSheet("QPushButton{font-size: 18px; font-family:'Book Antiqua';}");
	ui.pushButton_DeleteDir_HDD->setStyleSheet("QPushButton{font-size: 18px; font-family:'Book Antiqua';}");
	ui.pushButton_Refush_Disk->setStyleSheet("QPushButton{font-size: 18px; font-family:'Book Antiqua';}");
	ui.pushButton_USB_UnMount->setStyleSheet("QPushButton{font-size: 18px; font-family:'Book Antiqua';}");
	ui.pushButton_DeleteDir_USB->setStyleSheet("QPushButton{font-size: 18px; font-family:'Book Antiqua';}");
	ui.treeWidget_HDD_info->setStyleSheet("QTreeWidget{font-size: 18px; font-family:'Book Antiqua';}");
	ui.treeWidget_USB_info->setStyleSheet("QTreeWidget{font-size: 18px; font-family:'Book Antiqua';}");
	ui.progressBar_CopyDir->setStyleSheet("QProgressBar{font-size: 18px; font-family:'Book Antiqua';}");
	
	ui.progressBar_CopyDir->setRange(0,10000);   //ýΧ

	QFont font;
	font.setFamily("Book Antiqua");
	font.setPixelSize(18);
	ui.treeWidget_HDD_info->headerItem()->setFont(0, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(1, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(2, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(3, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(4, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(5, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(6, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(7, font);
	ui.treeWidget_USB_info->headerItem()->setFont(0, font);
	ui.treeWidget_USB_info->headerItem()->setFont(1, font);

	ui.groupBox->setStyleSheet("QGroupBox{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_2->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_3->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_4->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_5->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_6->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");

	ui.groupBox_2->setStyleSheet("QGroupBox{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_7->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_8->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_9->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_10->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_11->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_12->setStyleSheet("QLabel{font-size: 18px; font-family:'Book Antiqua';}");
	ui.label_Progress->setVisible(false);
}


#include "../../../netcomm/UiProtocal.h"

//new
extern  int m_ConnectStatus;    //״̬    =2=1=0
extern char buf[2048];   // Ѿŵǰ

void Content::timerEvent(QTimerEvent * te)
{
	if(te->timerId() == m_query_timer)
	{
		switch(m_ConnectStatus)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
              getCopyProgress();   //ʱѯļ
			break;
		}
	}

	if(te->timerId() == m_isReady_timer_HDD&&m_ConnectStatus==2)
	{
         this->ContentisReady_HDD(); 
	}

	if(te->timerId() == m_isReady_timer_USB&&m_ConnectStatus==2)
	{
         this->ContentisReady_USB(); 
	}
}


//ٺϲɴηʽHDD/USB  ,ظ̫

void Content::UpdateContent_HDD()       //б,¼شݣѯ״̬,ȡ3
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
              m_isReady_timer_HDD=startTimer(1000);    //ÿnsҷӲбǷ׼
		}
	 }
		else
	{
		 //ʾʾѾ
		 QMessageBox::information(this,"Information",tr("network error, operation failed"));
         return;
	}
}


void Content::UpdateContent_USB()       //б,¼شݣѯ״̬,ȡ3
{
    if(m_ConnectStatus==2)    //ȷѾӷ
	{
		KL *pKL = (KL*)buf;
		
		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = M_UPDATE_PROGRAM_LIST_USB;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int i =pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_UPDATE_PROGRAM_LIST_USB)
		{
              m_isReady_timer_USB=startTimer(1000);    //ÿnsҷӲбǷ׼
		}
	 }
		else
	{
		 //ʾʾѾ
		 QMessageBox::information(this,"Information",tr("network error, operation failed"));
         return;
	}
}

void Content::ContentisReady_HDD()     //Ƿ׼,׼ֱӵLoadContent_HDD 
{
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
            this->LoadContent_HDD();            //ȡб
		    killTimer(m_isReady_timer_HDD);
            m_isReady_timer_HDD=-1;
		}

	}

}


void Content::ContentisReady_USB()     //Ƿ׼,׼ֱӵLoadContent_USB 
{
    KL *pKL = (KL*)buf;
	pKL->m_pkgHead = 0x7585;
    bool ret=false;
 
	pKL->m_keyID = M_IS_PROGRAM_LIST_READY_USB;
	pKL->m_length = sizeof(bool);
	void* pos = buf + sizeof(KL);
	memcpy(pos,&ret,sizeof(bool));   

	pSocket->write(buf, sizeof(KL) + sizeof(bool));
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	int i = pSocket->read(buf, 1024*1024);

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_IS_PROGRAM_LIST_READY_USB)
	{
		KL *pKL = (KL *)buf;
		int len = pKL->m_length;
		int pos  = sizeof(KL);
		void* pos2 = buf + sizeof(KL);
	    memcpy(&ret,pos2,sizeof(bool));       //ȡص״̬

        if(true==ret)  //Ѿ׼
		{
            this->LoadContent_USB();            //ȡб
		    killTimer(m_isReady_timer_USB);
			m_isReady_timer_USB=-1;
}

	}
}

void Content::LoadContent_HDD()
{
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
		ui.label_3->setText(QString::number(di_hdd.nTotal/1024/1024));
		ui.label_4->setText(QString::number(di_hdd.nAvali/1024/1024));
		ui.label_6->setText(QString::number((di_hdd.nTotal - di_hdd.nAvali)/1024/1024));
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

		//ContentItem* item = new ContentItem;

		//
		int i=0;
		if(item_HDD[i]!=NULL)  { delete item_HDD[i];}  
        item_HDD[i]= new ContentItem;

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
				item_HDD[i]->pData[C_ID] = tmp;
				break;
			case CONTENT_NAME:
				item_HDD[i]->pData[C_NAME] = tmp;
				break;
			case CONTENT_PROGRESS:
				item_HDD[i]->pData[C_PROGRESS] = tmp + "%";
				break;
			case CONTENT_STATUS:
				item_HDD[i]->pData[C_STATUS] = tmp;
				break;
			case CONTENT_FORMAT:
				item_HDD[i]->pData[C_FORMAT] = tmp;
				break;
			case CONTENT_FILMLENGTH:
				item_HDD[i]->pData[C_FILM_LENGTH] = tmp;
				break;
			case CONTENT_PROGRAMLENGTH:
				item_HDD[i]->pData[C_PROGRAM_LENGTH] = tmp;
				break;
			case CONTENT_STEREOSCOPIC:
				item_HDD[i]->pData[C_STEREO] = tmp;
				break;
			case CONTENT_ISSUER:
				item_HDD[i]->pData[C_ISSUER] = tmp;
				break;
			case CONTENT_ISSUEDATE:
				item_HDD[i]->pData[C_ISSUEDATE] = tmp;
				break;
			case CONTENT_TIMERANGE:
				item_HDD[i]->pData[C_TIMERANGE] = tmp;
				break;
			case CONTENT_RECVSEGMENT:
				item_HDD[i]->pData[C_RECVSEGMENT] = tmp;
				break;
			case CONTENT_TOTALSEGMENT:
				item_HDD[i]->pData[C_TOTAL_SEGMENT] = tmp;
				break;
			case CONTENT_RECV_DATETIME:
				item_HDD[i]->pData[C_RECV_DATETIME] = tmp;
				break;
			case CONTENT_LOCATE:
				item_HDD[i]->pData[C_LOCATE] = tmp;
				item_HDD[i]->MakeItem_HDD();

				ui.treeWidget_HDD_info->addTopLevelItem((QTreeWidgetItem*)(item_HDD[i]));
				//item = new ContentItem;
				//ݵ·ַ
				printf("HDD:Index=%d,LOCATE=%s\n",i,item_HDD[i]->pData[C_LOCATE].c_str());
				i++;
				//if(item_HDD[i]!=NULL)  { delete item_HDD[i];}  
				item_HDD[i]=new ContentItem;
				break;
			}
			
		}

     
	}

    delete[] buf;
	static int num1=0;
	printf("%d:run to:LoadContent_HDD()-end\n",num1++);

}





void Content::LoadContent_USB()
{
	char *buf = new char[1024*1024];
	KL *pKL = (KL*)buf;
	int i;
	pKL->m_pkgHead = 0x7585;


//Load Content list
	pKL->m_keyID = M_GET_USB_CONTENT_LIST;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);
	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 1024*1024);
	int total = pKL->m_length + sizeof(KL);

	int r = total - i;
	char* aaa = (char*)buf + i;
	while(r)
	{
		pSocket->waitForReadyRead(-1);
		i = pSocket->read(aaa, r);
		aaa += i;
		r -= i;
	}

	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_GET_USB_CONTENT_LIST)
	{
		KL *pKL = (KL *)buf;
		int len = pKL->m_length;
		int pos  = sizeof(KL);

		//ContentItem* item = new ContentItem;   // 
		int i=0;
		if(item_USB[i]!=NULL)  { delete item_USB[i];}  

		item_USB[i] = new ContentItem;

		ui.treeWidget_USB_info->clear();
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
				item_USB[i]->pData[C_ID] = tmp;
				break;
			case CONTENT_NAME:
				item_USB[i]->pData[C_NAME] = tmp;
				break;
			case CONTENT_PROGRESS:
				item_USB[i]->pData[C_PROGRESS] = tmp + "%";
				break;
			case CONTENT_STATUS:
				item_USB[i]->pData[C_STATUS] = tmp;
				break;
			case CONTENT_FORMAT:
				item_USB[i]->pData[C_FORMAT] = tmp;
				break;
			case CONTENT_FILMLENGTH:
				item_USB[i]->pData[C_FILM_LENGTH] = tmp;
				break;
			case CONTENT_PROGRAMLENGTH:
				item_USB[i]->pData[C_PROGRAM_LENGTH] = tmp;
				break;
			case CONTENT_STEREOSCOPIC:
				item_USB[i]->pData[C_STEREO] = tmp;
				break;
			case CONTENT_ISSUER:
				item_USB[i]->pData[C_ISSUER] = tmp;
				break;
			case CONTENT_ISSUEDATE:
				item_USB[i]->pData[C_ISSUEDATE] = tmp;
				break;
			case CONTENT_TIMERANGE:
				item_USB[i]->pData[C_TIMERANGE] = tmp;
				break;
			case CONTENT_RECVSEGMENT:
				item_USB[i]->pData[C_RECVSEGMENT] = tmp;
				break;
			case CONTENT_TOTALSEGMENT:
				item_USB[i]->pData[C_TOTAL_SEGMENT] = tmp;
				break;
			case CONTENT_RECV_DATETIME:
				item_USB[i]->pData[C_RECV_DATETIME] = tmp;
				break;
			case CONTENT_LOCATE:
				item_USB[i]->pData[C_LOCATE] = tmp;
				//item->MakeItem();
                item_USB[i]->MakeItem_USB();
				ui.treeWidget_USB_info->addTopLevelItem((QTreeWidgetItem*)(item_USB[i]));
				//ݵ·ַ
				printf("USB:Index=%d,LOCATE=%s\n",i,item_USB[i]->pData[C_LOCATE].c_str());
				//item = new ContentItem;
                i++;
				//if(item_USB[i]!=NULL)  { delete item_USB[i];}  
                item_USB[i] = new ContentItem;
				break;
			}
		}
	}



	//new  ӸU̿ռͳϢ
	DISK_INFO di_usb;
	//Load Disk info
	pKL->m_keyID = M_GET_USB_INFO;
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	pSocket->write(buf, sizeof(KL) + 1);
	pSocket->waitForBytesWritten(-1);

	pSocket->waitForReadyRead(-1);
	i = pSocket->read(buf, 50);
	if (pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_GET_USB_INFO)
	{
		memcpy(&di_usb, buf + sizeof(KL), sizeof(di_usb));
		ui.label_10->setText(QString::number(di_usb.nTotal/1024/1024));
		ui.label_11->setText(QString::number(di_usb.nAvali/1024/1024));
		ui.label_12->setText(QString::number((di_usb.nTotal - di_usb.nAvali)/1024/1024));
	}	
	

	delete[] buf;
	static int num2=0;
	printf("%d:run to:LoadContent_USB()-end\n",num2++);
}





void ContentItem::MakeItem_HDD()
{
	setText(0, pData[C_STATUS].c_str());
	setText(1, pData[C_PROGRESS].c_str());
	setText(2, pData[C_NAME].c_str());
	setText(3, pData[C_RECV_DATETIME].c_str());
	setText(4, pData[C_STEREO].c_str());
	setText(5, pData[C_FILM_LENGTH].c_str());
	setText(6, pData[C_RECVSEGMENT].c_str());
	setText(7, pData[C_TOTAL_SEGMENT].c_str());
}


//new޸USBļʾб
void ContentItem::MakeItem_USB()
{
	setText(0, pData[C_NAME].c_str());
    setText(1, pData[C_FILM_LENGTH].c_str());
	//setText(2, pData[C_RECV_DATETIME].c_str());
}



void Content::on_treeWidget_HDD_info_clicked(const QModelIndex &)
{

}

void Content::on_treeWidget_USB_info_clicked(const QModelIndex &)
{

}



//extern char buf[2048];   // Ѿŵǰ
/*
void Content::	on_pushButton_USB_Mount_clicked()
{
	//new Shutdown
	if(m_ConnectStatus==2)    //ȷѾӷ
	{
		KL *pKL = (KL*)buf;
		int i;

		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = M_USB_MOUNT;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		i = pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_USB_MOUNT)
		{
		    QMessageBox::information(this,"Information",tr("operation successful"));
           //   return;
		}
	 }
		else
	{
	
		 //ʾʾѾ
		 QMessageBox::information(this,"Information",tr("network error, operation failed"));
         //  return;
	}

  // this->LoadContent_HDD();   //Ӳ

}
*/


void Content::on_pushButton_USB_UnMount_clicked()
{
  //  this->LoadContent_USB();   //Uʣ

    //new Shutdown
	if(m_ConnectStatus==2)    //ȷѾӷ
	{
		KL *pKL = (KL*)buf;
		int i;

		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = M_USB_UNMOUNT;
		pKL->m_length = 1;
		buf[sizeof(KL)] = 1;
		pSocket->write(buf, sizeof(KL) + 1);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		i =pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_USB_UNMOUNT)
		{
		    QMessageBox::information(this,"Information",tr("operation successful"));
            return;
		}
	 }
		else
	{
		 //ʾʾѾ
		 QMessageBox::information(this,"Information",tr("network error, operation failed"));
         return;
	}



}




void Content::on_pushButton_ExporttoUSB_clicked()
{
// int currentItem=(int)(ui.treeWidget_HDD_info->currentItem());

	//ȡ˵ǰѡʱĬΪ-1
	int currentIndex_row=(int)(ui.treeWidget_HDD_info->currentIndex().row());
    printf("treeWidget_HDD_info->currentIndex_row=%d\n",currentIndex_row);
    if(currentIndex_row==-1)   return;  //ûѡ������


    //һµҪӲ̿U̵·UI͵
	//new Shutdown
	if(m_ConnectStatus==2)    //ȷѾӷ
	{
		//tocopy_path path;     //·  
		//path.path_src=item_HDD[currentIndex_row]->pData[C_LOCATE];
		//path.path_dst="/media/usb";
		//printf("path_dst=%s  path_src=%s\n",path.path_dst.c_str(),path.path_src.c_str());
        
        char tmp_dst[256]="/media/usb";//"/root/copytest";//  "/media/usb";
      //  char tmp_dst[256]="/root/copytest";
		//char tmp1[256]="/root/copytest";
		char tmp_src[256]="/root/copytest2";
		copy_path path;

		std::string stmp=item_HDD[currentIndex_row]->pData[C_LOCATE];
		strcpy(tmp_src,stmp.c_str());


       //ȡԴ·ĩ·?
		char tmp3[256];  
        char* ptmp;
		for(int i=0;i<sizeof(tmp_src);i++)
		{
              if(tmp_src[i]=='/')
                  ptmp=&tmp_src[i];
		}
        strcpy(tmp3,ptmp);
        strcat(tmp_dst,tmp3);    

		strcpy(&(path.path_src[0]),tmp_src);
		strcpy(&(path.path_dst[0]),tmp_dst);


		KL *pKL = (KL*)buf;
		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID =M_COPYDIR_HDD_TO_USB;  //M_COPYDIR_HDD_TO_USB;
		
		//pKL->m_length = 1;
		//buf[sizeof(KL)] = 1;
		//pSocket->write(buf, sizeof(KL) + 1);

	    pKL->m_length = sizeof(path);
	    int sendsize = sizeof(path) + sizeof(KL);
	    void* pos = buf + sizeof(KL);
	    memcpy(pos,&path,sizeof(path));

		//֤ȷ
	    memcpy(&path,pos,sizeof(path));
	    //printf("path_dst=%s  path_src=%s\n",path.path_dst.c_str(),path.path_src.c_str());
	    printf("path_dst=%s  path_src=%s\n",&(path.path_dst[0]),&(path.path_src[0]));
	

        pSocket->write(buf,sendsize);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int i = pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_COPYDIR_HDD_TO_USB)
		{
		     QMessageBox::information(this,"Information",tr("operation start"));
           //   return;

			ui.pushButton_ExporttoUSB->setEnabled(false);  //ťʹܣļɺڻָֻ֤һļڿ
            m_query_timer = startTimer(1000);   //ʱ,ʱѯļ
		}

	 }
		else
	{
	
		 //ʾʾѾ
		 QMessageBox::information(this,"Information",tr("network error, operation failed"));
         //  return;
	}

    //this->UpdateContent_HDD();
}

int Content::getCopyProgress()   //ʱѯļ
{
	printf("run to getCopyProgress()\n");


#if 1

   //new Shutdown
	if(m_ConnectStatus==2)    //ȷѾӷ
	{
		KL *pKL = (KL*)buf;
		
		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID = M_GETCOPYPROCESS;
        
		int percent=1;          //ȡĽ      //ֱΪ
		pKL->m_length =sizeof(int)+sizeof(int);        //ݳһҪȷ
	    int sendsize = sizeof(KL)+sizeof(int)+sizeof(int);
	    void* pos = buf + sizeof(KL);
	    memcpy(pos,&percent,sizeof(int));

		int copy_flag=1;  
        void* pos2 = buf + sizeof(KL)+sizeof(int);
	    memcpy(pos2,&copy_flag,sizeof(int));
 
		//֤ȷ
	    //memcpy(&percent,pos,sizeof(int));  
		//printf("percent:=%d\n",percent);

		pSocket->write(buf,sendsize);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int i =pSocket->read(buf, 2048);

		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_GETCOPYPROCESS)
		{
		    int get_percent=1;                           //ֱΪ
	        void* pos=buf+sizeof(KL);
	        memcpy(&get_percent,pos,sizeof(int));
		    
			
			int get_copy_flag=1;                         //ȡĿ״̬   
            void* pos2=buf+sizeof(KL)+sizeof(int);
	        memcpy(&get_copy_flag,pos2,sizeof(int));    //ɣ۳ɹ������0
           

		    printf("ȡĽpercent:=%d\n",get_percent);
			printf("ȡ״̬percent:=%d\n",get_copy_flag);


			if(get_percent>=0&&get_percent<=10000)
            ui.progressBar_CopyDir->setValue(get_percent);     //
			//QString s=QString::number(get_percent);
			float f=(float)get_percent/100;
            QString s = QString("%1").arg(f);
            //ui.label_Progress->setText(s); 

			ui.pushButton_ExporttoUSB->setEnabled(false);      //ťʹܣļɺڻָ
			
			if(get_copy_flag==0) 
			{
				killTimer(m_query_timer);       //ɣʧ,ɱѯȵĶʱ 
		        m_query_timer=-1;
				ui.pushButton_ExporttoUSB->setEnabled(true);   //ťʹܣļɺڻָֻ֤һļڿ
				this->UpdateContent_USB();
			}


		   // QMessageBox::information(this,"Information",tr("operation successful"));
           // return;
		}
	 }
		else
	{
		 //ʾʾѾ
		// QMessageBox::information(this,"Information",tr("network error, operation failed"));
       //  return 0;
	}

	

#endif
	return 0;
}

void Content::on_pushButton_DeleteDir_HDD_clicked()
{
    //ȡ˵ǰѡʱĬΪ-1
	int currentIndex_row=(ui.treeWidget_HDD_info->currentIndex().row());
    printf("treeWidget_HDD_info->currentIndex_row=%d\n",currentIndex_row);
    if(currentIndex_row==-1)   return;  //ûѡ������

	if(m_ConnectStatus==2)    
	{
        char path_del[256];   //ɾ·
		std::string stmp=item_HDD[currentIndex_row]->pData[C_LOCATE];
		strcpy(path_del,stmp.c_str());

		KL *pKL = (KL*)buf;

		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID =M_DELETE_DIR;  //M_COPYDIR_HDD_TO_USB;
		

	    pKL->m_length = sizeof(path_del);
	    int sendsize = sizeof(path_del) + sizeof(KL);
	    void* pos = buf + sizeof(KL);
	    memcpy(pos,path_del,sizeof(path_del));

		//֤ȷ
	    memcpy(path_del,pos,sizeof(path_del));
		printf("ɾĿ¼:path_del=%s ",path_del);
	
        pSocket->write(buf,sendsize);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int i = pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_DELETE_DIR)
		{
		     QMessageBox::information(this,"Information",tr("operation successful"));
          // return;
		}

	 }
		else
	{
	
		 //ʾʾѾ
		 QMessageBox::information(this,"Information",tr("network error, operation failed"));
         //  return;
	}

	this->UpdateContent_HDD();
}

void Content::on_pushButton_DeleteDir_USB_clicked()
{
    //ȡ˵ǰѡʱĬΪ-1
	int currentIndex_row=(ui.treeWidget_USB_info->currentIndex().row());
    printf("treeWidget_USB_info->currentIndex_row=%d\n",currentIndex_row);
    if(currentIndex_row==-1)   return;  //ûѡ������

	if(m_ConnectStatus==2)    
	{
        char path_del[256];   //ɾ·
		std::string stmp=item_USB[currentIndex_row]->pData[C_LOCATE];
		strcpy(path_del,stmp.c_str());

		KL *pKL = (KL*)buf;

		pKL->m_pkgHead = 0x7585;
		pKL->m_keyID =M_DELETE_DIR;  //M_COPYDIR_HDD_TO_USB;
		

	    pKL->m_length = sizeof(path_del);
	    int sendsize = sizeof(path_del) + sizeof(KL);
	    void* pos = buf + sizeof(KL);
	    memcpy(pos,path_del,sizeof(path_del));

		//֤ȷ
	    memcpy(path_del,pos,sizeof(path_del));
		printf("ɾĿ¼:path_del=%s ",path_del);
	
        pSocket->write(buf,sendsize);
		pSocket->waitForBytesWritten(-1);
		pSocket->waitForReadyRead(-1);

		int i = pSocket->read(buf, 2048);
		if(pKL->m_pkgHead == 0x7585 && pKL->m_keyID == M_DELETE_DIR)
		{
		     QMessageBox::information(this,"Information",tr("operation successful"));
          // return;
		}

	 }
		else
	{
	
		 //ʾʾѾ
		 QMessageBox::information(this,"Information",tr("network error, operation failed"));
         //  return;
	}


	this->UpdateContent_USB();
}


void Content::on_progressBar_CopyDir_valueChanged(int)
{

}

void Content::UpdateContentUI()     //ͻ,ڿ,Ҫ½
{
  //  getCopyProgress();
  // LoadContent_HDD();
  // LoadContent_USB();
	this->UpdateContent_USB();
	this->UpdateContent_HDD();      //б,¼شݣѯ״̬,ȡ3


   if(m_query_timer==-1)
   {
		m_query_timer = startTimer(1000);   //ʱ,ʱѯļ
		printf("run to UpdateContentUI()\n");
   }
   

}

void Content::on_pushButton_Refush_Disk_clicked()
{

	this->UpdateContent_USB();
	this->UpdateContent_HDD();      //б,¼شݣѯ״̬,ȡ3

}