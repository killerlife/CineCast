#include "tcpclientsocket.h"
#include "crc32.h" 
#include "my_aes.h"   //AES

#include <QtGlobal>
#include "IDManager.h"
#include "baseoperation.h"

char retKL[32];                //ݵȴ,ɹKLṹ        
 

#include <QHostAddress>
TcpClientSocket::TcpClientSocket(int socketDescriptor,QObject *parent) :
    QTcpSocket(parent),socketID(socketDescriptor),v_buf(NULL)

{   
    this->setSocketDescriptor(socketDescriptor);

     printf("TcpClientSocket::TcpClientSocket() QThreadID=%d,QThreadָ=%d\n",QThread::currentThreadId(),QThread::currentThread());
    this->socketID=socketDescriptor;

    connect(this,SIGNAL(readyRead()),this,SLOT(slot_dataReceived()));           //ȡͨԼж, 

    connect(this,SIGNAL(disconnected()),this,SLOT(slot_Disconnected()));   //رʱͶϿź

	connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(Error(QAbstractSocket::SocketError)));


    char* psrc="test.tgz";
    strcpy(this->pathname_updatefile,psrc);   //ʼ·

    flag_update=0;   //̵߳״̬,δʼ0,ִ,1 2,
}

TcpClientSocket::~TcpClientSocket()
{
	if(v_buf != NULL)
		delete[] v_buf;
}


void TcpClientSocket::run()
{

}

void TcpClientSocket::Error(QAbstractSocket::SocketError socketError)
{
}


void TcpClientSocket::slot_dataReceived()
{
	if(isblocking==true)  
	{
		static int NumDataReceived=0;
		printf("Sever:  µReceivedź=%d\n",NumDataReceived++);  //С
		return;
	}

	while (bytesAvailable()<sizeof(KH)) 
	{
		isblocking=true;
		bool ret=waitForReadyRead(30000);
		isblocking=false;
		if(!ret)
		{
			while(bytesAvailable()>0)
			{
				char buf[1024];   
				read(buf,1024); 
			}
			return;
		}
	}

	//ȡ̶ݰ  
	while(bytesAvailable()>=sizeof(KH)) 
	{
		printf("Sever:  bytesAvailable()=%d\n",bytesAvailable());  //С
		int size_head=sizeof(KH);   //ȶȡͷС,bytesAvailable();
		KH *pKH = (KH*)c_buf;
		//int i=this->read(buf,size_head);// read(buf,bytesAvailable());  

		int nread=sizeof(KH);     //ҪȡݴС
		char* tmp=c_buf;             //ַ
		while(nread>0)
		{
			if(bytesAvailable()==0)        //ûЧ
			{
				isblocking=true;
				bool ret=waitForReadyRead(30000);
				isblocking=false;
				if(!ret)
				{
					while(bytesAvailable()>0)
					{
						char buf[1024];   
						read(buf,1024); 
					}
					return;
				}
			}
			int i=read(tmp,nread);   //ȡСķ
			tmp+=i;
			nread-=i;
		}

		if(pKH->pkgHead!=PKG_HEAD) 
		{
			while(bytesAvailable()>0)
			{
				char buf[1024];   
				read(buf,1024); 
			}
			return;  //ǰͷʶ
		}
		else  
		{
			if((pKH->cmd_length + sizeof(KH)) > 490)
			{
				if(v_buf != NULL)
					delete[] v_buf;
				v_buf = new char[pKH->cmd_length + 100];
				m_buf = v_buf;
				memcpy(m_buf, c_buf, sizeof(KH));
			}
			else
			{
				m_buf = c_buf;
			}

			pKH = (KH *)m_buf;
			int cmd_size=pKH->cmd_length;     //õĳȡ

			//ѭȡֱõ
			int nread=pKH->cmd_length;                //ҪȡݴС
			char* tmp=m_buf + sizeof(KH);             //ַ
			while(nread>0)
			{
				if(bytesAvailable()==0)        //ûЧ
				{
					isblocking=true;
					bool ret=waitForReadyRead(30000);
					isblocking=false;
					if(!ret)
					{
						while(bytesAvailable()>0)
						{
							char buf[1024];   
							read(buf, 1024); 
						}
						return;
					}
				}
				int i=read(tmp, nread);   //ȡСķ
				tmp+=i;
				nread-=i;
			}
			SaveProto(m_buf);

			switch(pKH->cmd)
			{
			case CMD_LOGIN_REQUEST:
				{
					LOGINREQUEST* p = (LOGINREQUEST*)m_buf;
					if(!GetIDManager()->GetHardKeyById(p->Machine_ID, this->HardKey))
						return;
					uint8* pos=(uint8*)(m_buf+sizeof(KH)+sizeof(uint32));
					int len=pKH->cmd_length-sizeof(uint32);
					switch(pKH->flag_pwd)
					{
					case KEY_HARD:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->HardKey);
						break;
					case KEY_MEET:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->MeetKey);
						break;
					}
					pos = (uint8*)m_buf + sizeof(KH);
					uint32 CRCsize=pKH->cmd_length - sizeof(uint32);
					uint32 CRC32=Get_CRC32_fromBuffer(0,(uint8*)pos, CRCsize);   //¼CRC

					uint32 CRC32_recv;        //ȡյCRC
					char* pCRC32=m_buf + sizeof(KH) + pKH->cmd_length - sizeof(uint32);
					memcpy(&CRC32_recv, pCRC32, sizeof(uint32));
					if(CRC32!=CRC32_recv)
					{
						printf("Sever:receive CRC32 ERROR\n");
						printf("FILE: %s,LINE: %d \n" ,__FILE__, __LINE__); 
					}
					login_request_process();
				}
				break;
			case CMD_AUTHEN_REQUEST:
				{
					LOGINREQUEST* p = (LOGINREQUEST*)m_buf;
					if(!GetIDManager()->GetHardKeyById(p->Machine_ID, this->HardKey))
						return;
					uint8* pos=(uint8*)(m_buf+sizeof(KH)+sizeof(uint32));
					int len=pKH->cmd_length-sizeof(uint32);
					switch(pKH->flag_pwd)
					{
					case KEY_HARD:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->HardKey);
						break;
					case KEY_MEET:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->MeetKey);
						break;
					}
					pos = (uint8*)m_buf + sizeof(KH);
					uint32 CRCsize=pKH->cmd_length - sizeof(uint32);
					uint32 CRC32=Get_CRC32_fromBuffer(0,(uint8*)pos, CRCsize);   //¼CRC

					uint32 CRC32_recv;        //ȡյCRC
					char* pCRC32=m_buf + sizeof(KH) + pKH->cmd_length - sizeof(uint32);
					memcpy(&CRC32_recv, pCRC32, sizeof(uint32));
					if(CRC32!=CRC32_recv)
					{
						printf("Sever:receive CRC32 ERROR\n");
						printf("FILE: %s,LINE: %d \n" ,__FILE__, __LINE__); 
					}
					authon_request_process();
				}
				break;
			case CMD_HEARTBREAT:
				{
					uint8* pos=(uint8*)(m_buf+sizeof(KH));
					int len=pKH->cmd_length;
					switch(pKH->flag_pwd)
					{
					case KEY_HARD:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->HardKey);
						break;
					case KEY_MEET:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->MeetKey);
						break;
					}
					heart_breat_process();	
				}
				break;
			case CMD_MD5_REQUEST:
				{
					uint8* pos=(uint8*)(m_buf+sizeof(KH));
					int len=pKH->cmd_length;
					switch(pKH->flag_pwd)
					{
					case KEY_HARD:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->HardKey);
						break;
					case KEY_MEET:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->MeetKey);
						break;
					}
					md5_process();
				}
				break;
			case CMD_MD5_RES_UP_REQUEST:
				{
					uint8* pos=(uint8*)(m_buf+sizeof(KH));
					int len=pKH->cmd_length;
					switch(pKH->flag_pwd)
					{
					case KEY_HARD:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->HardKey);
						break;
					case KEY_MEET:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->MeetKey);
						break;
					}
					md5_res_process();
				}
				break;
			case CMD_LOGFILE_BACK:                  //־Ļش,ļܱȽϴ,⴦
				{
					uint8* pos=(uint8*)(m_buf+sizeof(KH));
					int len=pKH->cmd_length;
					switch(pKH->flag_pwd)
					{
					case KEY_HARD:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->HardKey);
						break;
					case KEY_MEET:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->MeetKey);
						break;
					}
					log_back_process();
				}
				break;
			case CMD_LOST_REPORT:
				{
					uint8* pos=(uint8*)(m_buf+sizeof(KH));
					int len=pKH->cmd_length;
					switch(pKH->flag_pwd)
					{
					case KEY_HARD:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->HardKey);
						break;
					case KEY_MEET:
						MyAes_ctr_decrypt(pos,len,(uint8*)this->MeetKey);
						break;
					}
					lost_report_process();
				}
				break;
			}
		}
#if 0
		printf("Sever:  switch(pKH->cmd):=%X\n",pKH->cmd);
		//͵Ӧ���
		switch(pKH->cmd)
		{
		case CMD_UPDATE_BACK:               //ļͷ
			update_file_back_process();               break;
		case CMD_LOGFILE_BACK:
			log_back_process(); break;
		default:	
			printf("Sever: No Case:pKH->cmd=%X\n",pKH->cmd);
			break;
		}
#endif
	}
	//  printf("Sever: BUF_receive:%s\n",buf+sizeof(KH));
}




void TcpClientSocket::slot_Disconnected()
{
		  while(bytesAvailable()>0)
	  {
		  printf("while to read:\n");
		  printf("FILE: %s,LINE: %d \n" ,__FILE__, __LINE__); 
		  char buf[1024];   
		  read(buf,1024); 
	   }



    //ϿʱͶϿź
    emit signal_socketDisConnect(this->socketID,this);    
	//Ͽĸ,ͬʱָ,QTcpserverӦָ
}



void TcpClientSocket::crc_encry_send(char* buf,int size,int val)   //CRCֶ,Ȼܷ posʾƫ
{
	   printf("Sever:crc_encry_send()--START\n");
	   KH *pKH = (KH*)buf;        //Զ屣
       pKH->rev=CMD_REV;          //Զ屣
       pKH->pkgHead=PKG_HEAD;
 
	   //CRCУ
	   if(pKH->cmd_length == 0)
	   {
		   SaveProto(buf);
		   write(buf,size);
		   flush();                 //new ҲԼӿд
		   //socket.bytesToWrite ()ڵȴдݵֽҲĴСҲдѭжֵΪ0,Ѿ
		   waitForBytesWritten(-1);
		   printf("bytesToWrite=%d\n",bytesToWrite());
		   printf("Sever:crc_encry_send()--END\n");
	   }
	   else
	   {
		   char* pbuf=buf+sizeof(KH);
		   uint32 CRCsize=size-sizeof(KH)-sizeof(uint32);
		   uint32 CRC32=Get_CRC32_fromBuffer(0,(uint8*)pbuf,CRCsize);
		   printf("Sever:send CRC32_of_CMD=%X\n",CRC32);

		   char* pCRC32=buf+size-sizeof(uint32);
		   memcpy(pCRC32,&CRC32,sizeof(uint32));
		   //CRCУܷ֮
		   //ݱ͵Ӧļܷ
		   uint8* pos=(uint8*)(buf+sizeof(KH));
		   pKH->flag_pwd=KEY_MEET;
		   int len=pKH->cmd_length;
		   if(pKH->cmd==CMD_LOGIN_BACK||pKH->cmd==CMD_AUTHEN_BACK);  //ǵ¼/֤MachineIDֶβ,
		   {
			   pKH->flag_pwd=KEY_HARD;   //ǵ¼֤Ӳָ,ûԿ
		   }

		   if(size> sizeof(KH))
		   {
			   pKH->flag_pwd=KEY_NO;
			   switch(pKH->flag_pwd)
			   {
			   case KEY_NO:											             break;         
			   case KEY_HARD: MyAes_ctr_encrypt(pos,len,(uint8*)this->HardKey);break;
			   case KEY_MEET: MyAes_ctr_encrypt(pos,len,(uint8*)this->MeetKey);break;
			   default:	break;
			   }
			   printf("Client: receive: ܷʽ=%d Կ:",pKH->flag_pwd);
			   for(int i=0;i<16;i++)
			   {  
				   printf("%02X ",(unsigned char)this->HardKey[i]);
			   }
			   printf("\n");

		   }

		   SaveProto(buf);
		   write(buf,size);
		   flush();                 //new ҲԼӿд
		   //socket.bytesToWrite ()ڵȴдݵֽҲĴСҲдѭжֵΪ0,Ѿ
		   waitForBytesWritten(-1);
		   printf("bytesToWrite=%d\n",bytesToWrite());
		   printf("Sever:crc_encry_send()--END\n");
	   }
}


void TcpClientSocket::heart_breat_process()
{
	printf("login_request_process()_process\n");
	//һֽṹ
	HEARTBREAT_1* p = (HEARTBREAT_1*)m_buf;
	char* pos = m_buf;

	int len = sizeof(HEARTBREAT_1) - sizeof(KH); //һֲKHĳ

	char* pos2 = pos + sizeof(HEARTBREAT_1); //ȡ䳤ļ
	if(p->filmNameLength > 0)
	{
		//ڴȡĿǰûȡ
		//char *dest = new char[p->filmNameLength];
		//memcpy(dest, pos2, p->filmNameLength);
		pos2 += p->filmNameLength;
		len += p->filmNameLength;
	}
	HEARTBREAT_2* p2= (HEARTBREAT_2*)pos2;
	len += (sizeof(HEARTBREAT_2) - sizeof(uint32));

	uint32 crc = Get_CRC32_fromBuffer(crc, (uint8*)pos+sizeof(KH), len);
	if(crc!=p2->crc32)
	{
		printf("Sever:receive CRC32 ERROR\n");
		printf("FILE: %s,LINE: %d \n" ,__FILE__, __LINE__); 
	}
	
	p->kh.cmd = 0x0033; //ȷ
	p->kh.cmd_length = 0;
	crc_encry_send((char*)&p->kh, sizeof(KH), 0);    //У ܣ //ƫΪ0,ȫ
}

void TcpClientSocket::heart_breat_request()
{
	KH* p = (KH*)m_buf;
	p->cmd = 0x0031;
	p->cmd_length = 0;
	crc_encry_send((char*)p, sizeof(KH), 0);
}

void TcpClientSocket::login_request_process()    //¼  
{
        LOGINREQUEST* p=(LOGINREQUEST*)m_buf;
        int ID=p->Machine_ID;   //ȡջID,IDûע,
        bool retID=GetIDManager()->GetHardKeyById(ID,this->HardKey);  //ͨ豸IDҶӦӲָ    
		
     //жϽջǷIDѾע,ش kh.cmd_Sub
     //ѾעöӦӲָƽ

	 //if(m_ConnectStatus==2)
	{
        LOGINBACK Loginback;
		memset(&Loginback,0,sizeof(LOGINBACK));   //ʼ֤òֽΪ0
		Loginback.kh.pkgHead=PKG_HEAD;
		Loginback.kh.flag_pwd=KEY_HARD;
		Loginback.kh.cmd=CMD_LOGIN_BACK;
		Loginback.kh.cmd_Sub=CMD_ZERO;
		Loginback.kh.cmd_length=sizeof(LOGINBACK)-sizeof(KH);
        
       QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName()); //һHostInfoĶȻfromNameгʼ
	   QHostAddress address=info.addresses().first();                    //info.addresses() һַ���ȡһ
       QString strIP="192.168.64.122";//address.toString();
	   printf("Sever:IP=%s\n",strIP.toStdString().c_str());

       char *ptmp=&(Loginback.ReSeverIP[0]);
	   strcpy(ptmp,strIP.toStdString().c_str());
// 	   strcpy(ptmp, "169.254.1.110");
       Loginback.ReSeverPort=10099;    

	   QDate Date =QDate::currentDate();//ȡϵͳڵʱ
	   QTime Time =QTime::currentTime();//ȡϵͳڵʱ

       Loginback.Year=Date.year();
       Loginback.Month=Date.month();                
       Loginback.Day=Date.day();                
       Loginback.Hour=Time.hour();               
       Loginback.Minute=Time.minute();          
       Loginback.Seconds=Time.second();  
       Loginback.CRC32=0;

	   printf("Sever:Date:%d %d %d \n",Date.year(),Date.month(),Date.day());
	   printf("Sever:Time:%d %d %d \n",Time.hour(),Time.minute(),Time.second());


        if(retID==true)     //IDѾע
		{
			printf("¼:ע豸ID\n");
		    Loginback.kh.cmd_Sub=CMD_ZERO;              //ɹע
		}
		else
		{
			printf("¼:δע豸ID\n");
		    Loginback.kh.cmd_Sub=0X27;                   //δעIDָ,
		}

	   char* psend=(char*)&Loginback;
	   int   sendsize=sizeof(LOGINBACK);
       crc_encry_send(psend,sendsize,sizeof(uint32));   //У ܣ
	   
	   if(retID==false)     //IDѾδע
	   {
		   //귴źźϿӣ
           disconnectFromHost();           //ȶϿ
           waitForDisconnected(-1);        //ȴر
		   printf("δעIDĵ¼,ԶϿ\n");
	   }
	   
	   //flag_login=true;
	}
       //do_send_file();   //ļ
} 

void TcpClientSocket::authon_request_process()     //֤ 
{   
      AuthenREQUEST* p=(AuthenREQUEST*)m_buf;  //ȷ֤Ϣ
// 	  memset(buf, 0, 10*1024);

      AuthenREQUEST tmp;
      memcpy(&tmp,p,sizeof(AuthenREQUEST));  //
      this->AuthenRequest=tmp;                //䵽ǰ������

      //ź֪ͨtcpserver->ui      //֤Ϣı
      //ϿҲҪ
/*
//֤ authentication feedback
typedef struct _AuthenBACK_
{
	KH kh;
	char    MeetKey[16];          //Կ128λֶΣջԶάϵͳݲϵͳջĵļԿ
    char    RandomCode[16];       //128λֶΣҪЯԹջ֤
    uint32  Model_Log;            //־ģʽ32λ޷ͣջԶάϵͳջȷϱ־ģʽ
    uint32  reserved;             //  
    uint32  BeatCycle;            // 32λ޷ͣջԶάϵͳջȷϱڡ
    uint32  Model_Connect;        //ӷʽ 32λ޷ͣջԶάϵͳջȷϱӷʽ
	uint32  CRC32;                //rpchof ṹֶμǰCRCУֵ
}AuthenBACK;
*/
     AuthenBACK AuthenBack;
	 memset(&AuthenBack,0,sizeof(AuthenBACK));
	 AuthenBack.kh.pkgHead=PKG_HEAD;
	 AuthenBack.kh.flag_pwd=KEY_NO;
	 AuthenBack.kh.cmd=CMD_AUTHEN_BACK;
	 AuthenBack.kh.cmd_Sub=CMD_ZERO;
	 AuthenBack.kh.cmd_length=sizeof(AuthenBACK)-sizeof(KH);

	 char MeetKey_tmp[16] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68};
     memcpy(AuthenBack.MeetKey,MeetKey_tmp,sizeof(MeetKey_tmp));
     
	 //Ҳ浽ǰSocket
     memcpy(this->MeetKey,MeetKey_tmp,sizeof(MeetKey_tmp));   //ΪԺļԿ

     //getRandomCode(char RandoCode[16]);    //Client --ݲ
     getRandomCode(RandomCode_tmp);

     memcpy(AuthenBack.RandomCode,RandomCode_tmp,sizeof(RandomCode_tmp));
     AuthenBack.Model_Log=1;                   //ջ,ϱ־
     AuthenBack.BeatCycle=5;                   //ջ,ϱĵ  //ֶ趨 
     AuthenBack.Model_Connect=1;

	 char* psend=(char*)&AuthenBack;
	 int   sendsize=sizeof(AuthenBACK);
     crc_encry_send(psend,sendsize,0);    //У ܣ //ƫΪ0,ȫ

    //do_send_file(); //ļ
    //źŲ

	// emit signal_authenRequest();         //µ֤,UI 

	 heart_breat_request();
	 //ԶĿͻб
	 emit signal_CMD_SocketToUI(this->socketID,SK_UPDATE_TREEWIDGET_LIST,0); 

}

int getfilesize(char* name)
{
    FILE *fp;
    if((fp=fopen(name,"r"))==NULL)
        return 0;
    fseek(fp,0,SEEK_END);
    return ftell(fp);    //return NULL;
}

void TcpClientSocket::update_file_back_process()   //ļͺ 
{

/*
	typedef struct _UpdateFileBACK_
{
	KH kh;
    uint32  UpdateCheckResult;     //32λ޷ͣȷϡֵΪ0ʾЧ1ʾЧ
    uint32  reserved;               //ֶ
    char  OldVersion[16];          //128λֶΣǰİ汾Ϣ
    char  UpdateSerialNo[16];      //128λֶΣյкš
    char  reserved2[16];           //ֶ
	uint32  CRC32;                  //rpchof ṹֶμǰCRCУֵ
}UpdateFileBACK;
*/

      //UpdateFileBACK
	  printf("authon_request_process()\n");
      UpdateFileBACK* p=(UpdateFileBACK*)buf;  //ȷ֤Ϣ

      UpdateFileBACK tmp;
      memcpy(&tmp,p,sizeof(UpdateFileBACK));  //
      this->UpdateFileBack=tmp;                //䵽ǰ������

      //ļģЧ,Ϳʼ
      if(p->UpdateCheckResult==0)
	  {
	       printf("Ч\n");
           do_update_start(); //ջ,
	  }
	  else   //=1Ч
	  {
	       printf("Ч\n");
           //źŸUIʾʧ  ʵ
	  }
}


//־Ļش,ļܱȽϴ,⴦
void TcpClientSocket::log_back_process()
{
	KH *pKH = (KH*)m_buf;
	//ڴ˵õ־,־Ϣͷ
	LOGBACKINFO* p=(LOGBACKINFO*)(m_buf + sizeof(KH));

	char* pLog= (char*)p + sizeof(LOGBACKINFO);   //־ݵַ ڴ˴
	//͸UIʾ

	emit signal_CMD_SocketToUI(this->socketID, SK_UPDATE_TEXTBROWSER, (int)pLog); 
}

void TcpClientSocket::log_process()
{
	KH *pKH = (KH*)buf;
	uint32 cmd_size=pKH->cmd_length;     //õĳȡ
#if 0
	char* bufLog=new char[cmd_size];     //ڴ汣־һ㲻̫
	memcpy(bufLog,buf,sizeof(KH));       //ͷ

	//ȡ־ļͷ
	int nread=cmd_size;              //Ҫȡ־
	char* tmp=bufLog+sizeof(KH);   //ַ
	while(nread>0)
	{
		if(bytesAvailable()==0)        //ûЧ
		{
			isblocking=true;
			bool ret=waitForReadyRead(30000);
			isblocking=false;
			if(!ret)
			{
				printf("ȴ,ûµݵ:˳\n");
				printf("FILE: %s,LINE: %d \n" ,__FILE__, __LINE__);     //ļк

				while(bytesAvailable()>0)
				{
					printf("While read to Clear Socket Buf:\n");
					printf("FILE: %s,LINE: %d \n" ,__FILE__, __LINE__); 
					char buf[1024];   
					read(buf,1024); 
				}
				return;
			}
		}
		int i=read(tmp,nread);   //ȡСķ
		tmp+=i;
		nread-=i;
	} 
	//ڴ˵õ־,־Ϣͷ
	LOGBACKINFO* p=(LOGBACKINFO*)bufLog;
	uint32 LogSize=p->LogLengthCurr	;

	char* pCRC=(char*)(bufLog+sizeof(LOGBACKINFO)+LogSize);  //CRCУַ
	uint32  CRCrecv=0x999;//ԭCRC
	memcpy(&CRCrecv,pCRC,sizeof(uint32));    //ȡԭCRCУ


	//CRCУ
	char* pbuf=bufLog+sizeof(KH);           
	uint32 CRCsize=cmd_size-sizeof(KH)-sizeof(uint32);
	uint32 CRC32_info=Get_CRC32_fromBuffer(0,(uint8*)pbuf,CRCsize);

	if(CRC32_info!=CRCrecv)
	{
		printf("Server:LOGrecv CRC32 Error\n");
	}

	char* pLog=bufLog+sizeof(LOGBACKINFO);   //־ݵַ ڴ˴
	//͸UIʾ
#endif

}





void TcpClientSocket::do_send_file()   //ļ
{
/*
//ͱ (ļϢ,ļݺ)  Upgradepackage
typedef struct _UpdateFileINFO_
{
	KH kh;
    uint64  reserved;               //  
	char    UpdateSerialNo[16];    //128λֶΣкš
    char    reserved2[16];          //ֶ2 
    uint32  DescriptionLength;     //32λ޷ͣϢȡ
    char    Description[256];      //ɱ䳤ֶΣϢ(ݶ256λַ)
    uint32  FID;                   //32λ޷ͣļIDš
    uint32  UpdateFileNameLength;  //32λ޷ͣļȡ
    char    UpdateFileName[256];   //ɱ䳤ֶΣļ   (ݶ256λַ)
    uint32  UpdateFileLength;      //32λ޷ͣԱʾ4GB,ļȡָͨͣļȣͨŵͣȡֵΪ0
    //char UpdateFile[1024];       //ɱ䳤ֶΣļͨͣΪ͵ļģͨŵͣڡ
	//uint32  CRC32;               //rpchof ṹֶμǰCRCУֵ
}UpdateFileINFO;
//ɽṹϺ,ڴļ,ȻļCRCУ
*/

    UpdateFileINFO UpFileInfo;
	memset(&UpFileInfo,0,sizeof(UpdateFileINFO));
    UpFileInfo.kh.pkgHead=PKG_HEAD;
    UpFileInfo.kh.flag_pwd=KEY_NO;
    UpFileInfo.kh.cmd=CMD_UPDATE_FILE_RECEV;
    UpFileInfo.kh.cmd_Sub=CMD_ZERO;
   // UpFileInfo.kh.cmd_length=sizeof(LOGINBACK)-sizeof(KH);

   // UpFileInfo.UpdateSerialNo;
    UpFileInfo.DescriptionLength=256;
   // UpFileInfo.Description;
   // UpFileInfo.FID=
	UpFileInfo.UpdateFileNameLength=256;

	char* p1=UpFileInfo.UpdateFileName;
	//char Filename[256]="test.tgz";  //Ҫ͵ļ 
	char* Filename=this->pathname_updatefile;   //·
    printf("ļ·=%s\n",Filename);


    //·ȡĩļ---䵽ļṹ
	char* ptmp=Filename; 
	char* pname=Filename;
    for(int i=0;i<256;i++)
	{
		if(*ptmp=='/'||*ptmp=='\\')
		{
			pname=ptmp;
		}
            ptmp++;
	}


    strcpy(p1,pname+1); 
    printf("ļUpdateFileName=%s\n",p1);

    //к11λ
    char* p2=UpFileInfo.UpdateSerialNo;     //ļȥtgzѹ׺Ϊк
    strcpy(p2,pname+1);
    char tmp[20];
    strcpy(tmp,p2);

    for(int i=0;i<15;i++)
    {
        if(tmp[i]=='.'&&tmp[i+1]=='t'&&tmp[i+2]=='g'&&tmp[i+3]=='z')
            tmp[i]='\0';   //ضַ
    }
    tmp[15]='\0';
    strcpy(p2,tmp);

	p2[15]='\0';
	printf("кUpdateSerialNo=%s\n",p2);   


    int FileLength=getfilesize(Filename);   //ȡĳļС
	UpFileInfo.UpdateFileLength=FileLength;
	printf("Filename=%s,FileLength=%d\n",Filename,FileLength);

    //
	UpFileInfo.kh.cmd_length=sizeof(UpdateFileINFO)-sizeof(KH)+FileLength+sizeof(uint32);
    
	  //ȼļͷϢCRC32
	  char* pbuf=(char*)&UpFileInfo;            
	  uint32 CRCsize=sizeof(UpdateFileINFO);
	  uint32 CRC32_info=Get_CRC32_fromBuffer(0,(uint8*)pbuf,CRCsize);
	  printf("Sever:send CRC32_of_Fileinfo=%X\n",CRC32_info);

      this->UpdateFileInfo=UpFileInfo;   //ļϢǰṹ,



    //δļϢ,ļݱ,ļCRCУ
	write((char*)&UpFileInfo,sizeof(UpdateFileINFO));
	flush();   
	waitForBytesWritten(-1);
    
	 FILE *fp = fopen(Filename,"rb");
    if(!fp)
    {
		printf("ļȡ\n");
        return;
    }

	char buffer[1024*40];//1024*50
    uint64 percent=0;    //ļͰٷֱ
     flag_update=1;   //

	uint32 sendsize=0;      //¼ȡļС  4G
    while(!feof(fp))
    {
        /* read to buffer */
       int count = fread(buffer, sizeof(char),sizeof(buffer),fp);
       CRC32_info=Get_CRC32_fromBuffer(CRC32_info,(uint8*)buffer,count);   //ѼCRCҲŵ

//	    printf("Sever:send count=%d\n",count);
//	    printf("Sever:send CRC32_of_Fileinfo=%X\n",CRC32_info);
//      write buffer to file
 	    write(buffer,count);    //bugڴ˴  write(buffer,sizeof(buffer));
	    flush();
        waitForBytesWritten(-1);

        sendsize+=count; 
        percent=10000*(float)sendsize/(float)FileLength;

            if(percent%100==0)    //Ƶź
            {
          //      printf("percent=%d\n",percent);
            //źŸUI½
            emit signal_CMD_SocketToUI(this->socketID,SK_UPDATE_PROGRESSBAR,percent/100);
            }

    }

	  fclose(fp);
      write((char*)(&CRC32_info),sizeof(uint32));
      flush();
	  waitForBytesWritten(-1);



      printf("sendsize=%d\n",sendsize);
	  printf("bytesToWrite=%d\n",bytesToWrite());
	  printf("Sever:send CRC32_of_File_END=%X\n",CRC32_info);
}



void TcpClientSocket::do_update_start() //ջ,
{
/*
//ջԶάϵͳڽյԶЯ췴ĺջ
//ջʼ
typedef struct _UpdateSTART_
{
	KH kh;
    uint32  UpdateSerialNo;          //128λֶΣİ汾Ϣ
    uint32  reserved;
	uint32  CRC32;                   //rpchof ṹֶμǰCRCУֵ
}UpdateSTART;
*/
     UpdateSTART UpdateStart;
	 memset(&UpdateStart,0,sizeof(UpdateSTART));
	 UpdateStart.kh.pkgHead=PKG_HEAD;
	 UpdateStart.kh.flag_pwd=KEY_NO;
	 UpdateStart.kh.cmd=CMD_UPDATE_START;
	 UpdateStart.kh.cmd_Sub=CMD_ZERO;
	 UpdateStart.kh.cmd_length=sizeof(UpdateSTART)-sizeof(KH);

	 //getUpdateSerialNo();   //ļʱ������浽ǰ̶߳

     memcpy(UpdateStart.UpdateSerialNo,this->UpdateFileInfo.UpdateSerialNo,16);

     
	 flag_update=2;   //˿Ϊ,Ϊ׼ûнջķֵ
    // emit signal_authenRequest(); //֤бUI 

	 //ԶĿͻб
	 emit signal_CMD_SocketToUI(this->socketID,SK_UPDATE_TREEWIDGET_LIST,0); 

	 char* psend=(char*)&UpdateStart;
	 int   sendsize=sizeof(UpdateSTART);
     crc_encry_send(psend,sendsize,0);    //У ܣ //ƫΪ0,ȫ
}

//ȫֱ־Ľṹ;--QVariantź
void TcpClientSocket::do_get_Log()   //ֶȡ־
{
    //ѾUI ʱ ͵
	LOGREQUEST LOGRequest=this->LOGRequest;
    
	//䱨ͷ
	LOGRequest.kh.pkgHead=PKG_HEAD;
	LOGRequest.kh.flag_pwd=KEY_MEET;
	LOGRequest.kh.cmd=CMD_LOG_REQUEST;
	LOGRequest.kh.cmd_Sub=CMD_ZERO;
	LOGRequest.kh.cmd_length=sizeof(LOGREQUEST)-sizeof(KH);


	char* psend=(char*)&LOGRequest;
	int   sendsize=sizeof(LOGREQUEST);

    crc_encry_send(psend,sendsize,0);  
}

void TcpClientSocket::do_get_md5res()
{
	struct 
	{
		KH kh;
		uint32 filmID;
		uint64 res;
		uint32 crc;
	}MD5Req;
	MD5Req.kh.pkgHead=PKG_HEAD;
	MD5Req.kh.flag_pwd=KEY_MEET;
	MD5Req.kh.cmd = 0x27;
	MD5Req.kh.cmd_Sub = CMD_ZERO;
	MD5Req.kh.cmd_length = sizeof(MD5Req) - sizeof(KH);
	MD5Req.filmID = 0;
	int sendsize = sizeof(MD5Req);
	crc_encry_send((char*)&MD5Req, sendsize, 0);
}

void TcpClientSocket::do_reboot()
{
	struct 
	{
		KH kh;
		uint8 random[16];
		uint32 crc;
	} RebootReq;
	RebootReq.kh.pkgHead = PKG_HEAD;
	RebootReq.kh.flag_pwd = KEY_MEET;
	RebootReq.kh.cmd = 0x81;
	RebootReq.kh.cmd_Sub = CMD_ZERO;
	RebootReq.kh.cmd_length = sizeof(RebootReq) - sizeof(KH);
	memcpy(RebootReq.random, RandomCode_tmp, 16);
	int sendsize = sizeof(RebootReq);
	crc_encry_send((char*)&RebootReq, sendsize, 0);
}

void TcpClientSocket::do_Log()   //ֶȡ־
{
	//ѾUI ʱ ͵
	LOGREQUEST LOGRequest=this->LOGRequest;
	LOGRequest.DateStart = 0;
	LOGRequest.DateEnd = 1000;
	LOGRequest.LogType = 0;
	//䱨ͷ
	LOGRequest.kh.pkgHead=PKG_HEAD;
	LOGRequest.kh.flag_pwd=KEY_MEET;
	LOGRequest.kh.cmd=CMD_LOGFILE_REQUEST;
	LOGRequest.kh.cmd_Sub=CMD_ZERO;
	LOGRequest.kh.cmd_length=sizeof(LOGREQUEST)-sizeof(KH);


	char* psend=(char*)&LOGRequest;
	int   sendsize=sizeof(LOGREQUEST);

	crc_encry_send(psend,sendsize,0);  
}

//ַΣ char* buf,int size
void TcpClientSocket::UIcmd_process(int SocketID,int cmdtype,QByteArray ba)   //	UI
{

	 if(SocketID!=this->socketDescriptor())  
//if(SocketID!=this->socketID)  

		return;  //Ƿǰsocket̵߳˳

        printf("TcpClientSocket::UIcmd_process()\n");

        switch(cmdtype)
		{
            case UI_SEND_UPDATE_FILE:       //ļ
			     do_send_file();   break;
			case UI_GET_LOG:
                 do_get_Log();     break;   //ֶȡ־
			case UI_GET_MD5RES:
				do_get_md5res(); break;
			case UI_REBOOT:
				do_reboot(); break;
			case UI_LOG:
				do_Log(); break;
			default:	break;
		}

		/*
        char* tmp=ba.data();
		 printf("\n");
        int size=ba.size();
		for(int i=0;i<size;i++)
		{
            printf("%02X ",tmp[i]);
			if(i%16==0)
			{
                printf("\n");
			}
		}
        printf("\n");
		*/

}

void TcpClientSocket::md5_process()
{
	//һֽṹ
	struct MD5H {
		KH kh;
		uint32 filmID;
		uint32 len;
		uint32 CRC;
	};
	char md5[] = "<EncodeInfo><EncodeFileList ID=\"20160324\" Name=\"Test\"><ChallengeCode>123</ChallengeCode>"
		"</EncodeFileList></EncodeInfo>";
	struct MD5H* p = (MD5H*)m_buf;
	char* pos = m_buf + sizeof(MD5H);
	p->filmID = 20160324;
	p->len = strlen(md5);
	p->CRC = 0;
	memcpy(pos, md5, p->len);
	pos += p->len;

	p->kh.flag_pwd = 2;
	p->kh.cmd = 0x0025;
	p->kh.cmd_length = sizeof(MD5H) - sizeof(KH) + p->len + 4;
	crc_encry_send((char*)&p->kh, sizeof(MD5H) + p->len + 4, 0);    //У ܣ //ƫΪ0,ȫ
}

void TcpClientSocket::lost_report_process()
{
	struct res{
		KH kh;
		uint32 id;
		uint64 rev;
		uint32 crc;
	};
	char* pos = m_buf;
	struct res* p = (res*)m_buf;
	p->id = 20160324;
	p->rev = 0;

	p->kh.cmd = 0x23;
	p->kh.flag_pwd = 2;
	p->kh.cmd_length = sizeof(res) - sizeof(KH);
	crc_encry_send((char*)&p->kh, sizeof(res), 0);
}

void TcpClientSocket::md5_res_process()
{
	struct res{
		KH kh;
		uint32 id;
		uint32 md5;
		uint64 reserved;
		uint64 rollback;
		uint32 crc;
	};
	struct res *p = (struct res*)m_buf;
	struct {
		KH kh;
		uint32 id;
		uint64 reserved;
		uint32 crc;
	} result;
	result.kh.cmd = 0x29;
	result.kh.flag_pwd = 2;
	result.kh.cmd_length = sizeof(result) - sizeof(KH);
	result.id = p->id;
	crc_encry_send((char*)&result.kh, sizeof(result), 0);
}

#ifdef WIN32
#include <windows.h>
#endif

void TcpClientSocket::SaveProto(char* buf)
{
	time_t t;
	time(&t);
	struct tm *pTm;
	pTm = localtime(&t);
	KH* pKh = (KH*)buf;
#ifdef WIN32
	CreateDirectoryA("protocol", NULL);
#else
	mkdir("protocol");
#endif
	char tt[256];
	sprintf(tt,
		"protocol/%.4d_%.2d_%.2d_%.2d_%.2d_%.2d_%d.pro",
		pTm->tm_year + 1900,
		pTm->tm_mon + 1,
		pTm->tm_mday,
		pTm->tm_hour,
		pTm->tm_min,
		pTm->tm_sec,
		pKh->cmd);
	FILE* fp=fopen(tt, "wb");
	if(fp)
	{
		fwrite(buf, pKh->cmd_length + sizeof(KH), 1, fp);
		fclose(fp);
	}
}

QHostAddress TcpClientSocket::getRemote()
{
	return this->peerAddress();
}