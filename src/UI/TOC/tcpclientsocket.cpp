#include "tcpclientsocket.h"
#include "crc32.h" 
#include "my_aes.h"   //AES加密

#include <QtGlobal>
#include "IDManager.h"
#include "baseoperation.h"

char retKL[32];                //发送数据等待返回,成功就填充KL结构体        
 

#include <QHostAddress>
TcpClientSocket::TcpClientSocket(int socketDescriptor,QObject *parent) :
    QTcpSocket(parent),socketID(socketDescriptor),v_buf(NULL)

{   
    this->setSocketDescriptor(socketDescriptor);

     printf("TcpClientSocket::TcpClientSocket() QThreadID=%d,QThread指针=%d\n",QThread::currentThreadId(),QThread::currentThread());
    this->socketID=socketDescriptor;

    connect(this,SIGNAL(readyRead()),this,SLOT(slot_dataReceived()));           //读取普通数据自己判断, 

    connect(this,SIGNAL(disconnected()),this,SLOT(slot_Disconnected()));   //关闭连接时，发送断开连接信号

	connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(Error(QAbstractSocket::SocketError)));


    char* psrc="test.tgz";
    strcpy(this->pathname_updatefile,psrc);   //初始化升级包路径名

    flag_update=0;   //本线程的升级状态,未开始0,正在执行升级,1 升级完成2,
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
		printf("Sever:  触发新的Received信号=%d\n",NumDataReceived++);  //缓存大小
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

	//读取固定数据包  
	while(bytesAvailable()>=sizeof(KH)) 
	{
		printf("Sever:  bytesAvailable()=%d\n",bytesAvailable());  //缓存大小
		int size_head=sizeof(KH);   //先读取包头解析出包大小,bytesAvailable();
		KH *pKH = (KH*)c_buf;
		//int i=this->read(buf,size_head);// read(buf,bytesAvailable());  

		int nread=sizeof(KH);     //想要读取的数据大小
		char* tmp=c_buf;             //缓存地址
		while(nread>0)
		{
			if(bytesAvailable()==0)        //如果没有有效数据
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
			int i=read(tmp,nread);   //读取完整大小的方法
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
			return;  //如果不是包头标识符
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
			int cmd_size=pKH->cmd_length;     //得到报文长度。

			//循环读取直到得到完整数据
			int nread=pKH->cmd_length;                //想要读取的数据大小
			char* tmp=m_buf + sizeof(KH);             //缓存地址
			while(nread>0)
			{
				if(bytesAvailable()==0)        //如果没有有效数据
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
				int i=read(tmp, nread);   //读取完整大小的方法
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
					uint32 CRC32=Get_CRC32_fromBuffer(0,(uint8*)pos, CRCsize);   //重新计算CRC

					uint32 CRC32_recv;        //取接收到的CRC
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
					uint32 CRC32=Get_CRC32_fromBuffer(0,(uint8*)pos, CRCsize);   //重新计算CRC

					uint32 CRC32_recv;        //取接收到的CRC
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
			case CMD_LOGFILE_BACK:                  //日志报文回传,文件可能比较大,特殊处理
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
		//根据命令类型调用相应处理函数
		switch(pKH->cmd)
		{
		case CMD_UPDATE_BACK:               //升级文件包推送送反馈
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



    //断开连接时，发送断开信号
    emit signal_socketDisConnect(this->socketID,this);    
	//断开哪个连接,同时发送其对象指针,方便在QTcpserver的链表里面对应指针
}



void TcpClientSocket::crc_encry_send(char* buf,int size,int val)   //填充CRC字段,然后加密发送 pos表示加密偏移
{
	   printf("Sever:crc_encry_send()--START\n");
	   KH *pKH = (KH*)buf;        //自定义保留字
       pKH->rev=CMD_REV;          //填充自定义保留字无用
       pKH->pkgHead=PKG_HEAD;
 
	   //做CRC校验
	   if(pKH->cmd_length == 0)
	   {
		   SaveProto(buf);
		   write(buf,size);
		   flush();                 //new 也许可以加快清空写缓存
		   //socket.bytesToWrite ()返回正在等待被写的数据的字节数，也就是输出缓存的大小。也可以写循环判断这个值为0,表明已经发送
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
		   //做完CRC校验之后加密发送
		   //根据报文类型调用相应的加密方法
		   uint8* pos=(uint8*)(buf+sizeof(KH));
		   pKH->flag_pwd=KEY_MEET;
		   int len=pKH->cmd_length;
		   if(pKH->cmd==CMD_LOGIN_BACK||pKH->cmd==CMD_AUTHEN_BACK);  //如果是登录/认证反馈MachineID字段不加密,跳过
		   {
			   pKH->flag_pwd=KEY_HARD;   //如果是登录和认证反馈就用硬件指纹,否则用会晤密钥
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
			   printf("Client: receive: 加密方式=%d 加密密钥:",pKH->flag_pwd);
			   for(int i=0;i<16;i++)
			   {  
				   printf("%02X ",(unsigned char)this->HardKey[i]);
			   }
			   printf("\n");

		   }

		   SaveProto(buf);
		   write(buf,size);
		   flush();                 //new 也许可以加快清空写缓存
		   //socket.bytesToWrite ()返回正在等待被写的数据的字节数，也就是输出缓存的大小。也可以写循环判断这个值为0,表明已经发送
		   waitForBytesWritten(-1);
		   printf("bytesToWrite=%d\n",bytesToWrite());
		   printf("Sever:crc_encry_send()--END\n");
	   }
}


void TcpClientSocket::heart_breat_process()
{
	printf("login_request_process()_process\n");
	//构建心跳第一部分结构件
	HEARTBREAT_1* p = (HEARTBREAT_1*)m_buf;
	char* pos = m_buf;

	int len = sizeof(HEARTBREAT_1) - sizeof(KH); //第一部分不含KH的长度

	char* pos2 = pos + sizeof(HEARTBREAT_1); //取变长的文件名
	if(p->filmNameLength > 0)
	{
		//在此取，目前没取
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
	
	p->kh.cmd = 0x0033; //心跳确认
	p->kh.cmd_length = 0;
	crc_encry_send((char*)&p->kh, sizeof(KH), 0);    //添加校验 加密，发送 //加密偏移为0,报文全部加密
}

void TcpClientSocket::heart_breat_request()
{
	KH* p = (KH*)m_buf;
	p->cmd = 0x0031;
	p->cmd_length = 0;
	crc_encry_send((char*)p, sizeof(KH), 0);
}

void TcpClientSocket::login_request_process()    //登录请求 处理 
{
        LOGINREQUEST* p=(LOGINREQUEST*)m_buf;
        int ID=p->Machine_ID;   //获取接收机ID,如果ID没有注册,
        bool retID=GetIDManager()->GetHardKeyById(ID,this->HardKey);  //通过设备ID查找对应的硬件指纹    
		
     //判断接收机是否ID已经注册,返回错误表 填充kh.cmd_Sub
     //如果已经注册调用对应的硬件指纹解密

	 //if(m_ConnectStatus==2)
	{
        LOGINBACK Loginback;
		memset(&Loginback,0,sizeof(LOGINBACK));   //初始化，保证用不到的字节为0
		Loginback.kh.pkgHead=PKG_HEAD;
		Loginback.kh.flag_pwd=KEY_HARD;
		Loginback.kh.cmd=CMD_LOGIN_BACK;
		Loginback.kh.cmd_Sub=CMD_ZERO;
		Loginback.kh.cmd_length=sizeof(LOGINBACK)-sizeof(KH);
        
       QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName()); //定义一个HostInfo的对象然后对这个对象的fromName函数进行初始化
	   QHostAddress address=info.addresses().first();                    //info.addresses() 返回一个地址表，我们取第一个
       QString strIP="192.168.1.81";//address.toString();
	   printf("Sever:IP=%s\n",strIP.toStdString().c_str());

       char *ptmp=&(Loginback.ReSeverIP[0]);
	   strcpy(ptmp,strIP.toStdString().c_str());
// 	   strcpy(ptmp, "169.254.1.110");
       Loginback.ReSeverPort=10099;    

	   QDate Date =QDate::currentDate();//获取系统现在的时间
	   QTime Time =QTime::currentTime();//获取系统现在的时间

       Loginback.Year=Date.year();
       Loginback.Month=Date.month();                
       Loginback.Day=Date.day();                
       Loginback.Hour=Time.hour();               
       Loginback.Minute=Time.minute();          
       Loginback.Seconds=Time.second();  
       Loginback.CRC32=0;

	   printf("Sever:Date:%d %d %d \n",Date.year(),Date.month(),Date.day());
	   printf("Sever:Time:%d %d %d \n",Time.hour(),Time.minute(),Time.second());


        if(retID==true)     //如果ID已经被注册
		{
			printf("登录请求结果:已注册的设备ID\n");
		    Loginback.kh.cmd_Sub=CMD_ZERO;              //成功注册
		}
		else
		{
			printf("登录请求结果:未注册的设备ID\n");
		    Loginback.kh.cmd_Sub=0X27;                   //未注册ID与指纹,待制作错误表
		}

	   char* psend=(char*)&Loginback;
	   int   sendsize=sizeof(LOGINBACK);
       crc_encry_send(psend,sendsize,sizeof(uint32));   //添加校验 加密，发送
	   
	   if(retID==false)     //如果ID已经未注册
	   {
		   //发送完反馈信号后断开链接；
           disconnectFromHost();           //先断开连接
           waitForDisconnected(-1);        //阻塞等待关闭
		   printf("未注册ID的登录请求,自动断开链接\n");
	   }
	   
	   //flag_login=true;
	}
       //do_send_file();   //发送文件测试
} 

void TcpClientSocket::authon_request_process()     //认证请求处理 
{   
      AuthenREQUEST* p=(AuthenREQUEST*)m_buf;  //先分析认证请求信息
// 	  memset(buf, 0, 10*1024);

      AuthenREQUEST tmp;
      memcpy(&tmp,p,sizeof(AuthenREQUEST));  //填充对象
      this->AuthenRequest=tmp;                //填充到当前对象保存

      //发送信号通知tcpserver->更新ui      //认证信息改变了
      //断开连接也要更新
/*
//认证反馈 authentication feedback
typedef struct _AuthenBACK_
{
	KH kh;
	char    MeetKey[16];          //会唔密钥128位字段，接收机远程维护控制子系统，数据补包子系统与接收机后续报文的加密密钥。
    char    RandomCode[16];       //随机码128位字段，在重要报文中携带，以供接收机验证。
    uint32  Model_Log;            //日志模式32位无符号整型，接收机远程维护控制子系统向接收机确认其上报的日志模式。
    uint32  reserved;             //保留字  
    uint32  BeatCycle;            //心跳周期 32位无符号整型，接收机远程维护控制子系统向接收机确认其上报的心跳周期。
    uint32  Model_Connect;        //连接方式 32位无符号整型，接收机远程维护控制子系统向接收机确认其上报的连接方式。
	uint32  CRC32;                //rpchof 本结构体以上字段加密前的CRC校验值
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
     
	 //也保存到当前Socket对象
     memcpy(this->MeetKey,MeetKey_tmp,sizeof(MeetKey_tmp));   //作为以后的加密密钥

     //getRandomCode(char Rando、Code[16]);    //Client端 计算随机码--暂不
     getRandomCode(RandomCode_tmp);

     memcpy(AuthenBack.RandomCode,RandomCode_tmp,sizeof(RandomCode_tmp));
     AuthenBack.Model_Log=1;                   //命令接收机,上报日志
     AuthenBack.BeatCycle=5;                   //命令接收机,上报心跳报文的周期  //改手动设定 
     AuthenBack.Model_Connect=1;

	 char* psend=(char*)&AuthenBack;
	 int   sendsize=sizeof(AuthenBACK);
     crc_encry_send(psend,sendsize,0);    //添加校验 加密，发送 //加密偏移为0,报文全部加密

    //do_send_file(); //发送文件测试
    //触发信号测试

	// emit signal_authenRequest();         //处理新的认证请求,更新UI 

	 heart_breat_request();
	 //更新远程升级的客户端列表
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

void TcpClientSocket::update_file_back_process()   //升级文件包推送后反馈 处理
{

/*
	typedef struct _UpdateFileBACK_
{
	KH kh;
    uint32  UpdateCheckResult;     //32位无符号整型，升级确认。值为0表示升级包有效，1表示升级包无效。
    uint32  reserved;               //保留字段
    char  OldVersion[16];          //128位字段，接收软件升级前的版本信息。
    char  UpdateSerialNo[16];      //128位字段，接收的软件升级包序列号。
    char  reserved2[16];           //保留字段
	uint32  CRC32;                  //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateFileBACK;
*/

      //UpdateFileBACK
	  printf("authon_request_process()\n");
      UpdateFileBACK* p=(UpdateFileBACK*)buf;  //先分析认证请求信息

      UpdateFileBACK tmp;
      memcpy(&tmp,p,sizeof(UpdateFileBACK));  //填充对象
      this->UpdateFileBack=tmp;                //填充到当前对象保存

      //分析升级文件反馈报文，如果升级包有效,发送开始升级的请求命令
      if(p->UpdateCheckResult==0)
	  {
	       printf("升级包有效\n");
           do_update_start(); //命令接收机,可以升级
	  }
	  else   //=1升级包无效
	  {
	       printf("升级包无效\n");
           //发送信号给UI提示升级失败  待实现
	  }
}


//日志报文回传,文件可能比较大,特殊处理
void TcpClientSocket::log_back_process()
{
	KH *pKH = (KH*)m_buf;
	//在此得到所有日志反馈报文,包括日志信息头
	LOGBACKINFO* p=(LOGBACKINFO*)(m_buf + sizeof(KH));

	char* pLog= (char*)p + sizeof(LOGBACKINFO);   //日志内容地址 在此处理
	//发送给UI显示

	emit signal_CMD_SocketToUI(this->socketID, SK_UPDATE_TEXTBROWSER, (int)pLog); 
}

void TcpClientSocket::log_process()
{
	KH *pKH = (KH*)buf;
	uint32 cmd_size=pKH->cmd_length;     //得到报文长度。
#if 0
	char* bufLog=new char[cmd_size];     //申请内存保存日志一般不会太大
	memcpy(bufLog,buf,sizeof(KH));       //报文头

	//读取日志反馈包文件头
	int nread=cmd_size;              //想要读取日志报文
	char* tmp=bufLog+sizeof(KH);   //缓存地址
	while(nread>0)
	{
		if(bytesAvailable()==0)        //如果没有有效数据
		{
			isblocking=true;
			bool ret=waitForReadyRead(30000);
			isblocking=false;
			if(!ret)
			{
				printf("等待,没有新的数据到来:退出\n");
				printf("FILE: %s,LINE: %d \n" ,__FILE__, __LINE__);     //仅仅文件与行号

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
		int i=read(tmp,nread);   //读取完整大小的方法
		tmp+=i;
		nread-=i;
	} 
	//在此得到所有日志反馈报文,包括日志信息头
	LOGBACKINFO* p=(LOGBACKINFO*)bufLog;
	uint32 LogSize=p->LogLengthCurr	;

	char* pCRC=(char*)(bufLog+sizeof(LOGBACKINFO)+LogSize);  //CRC校验码地址
	uint32  CRCrecv=0x999;//原来CRC
	memcpy(&CRCrecv,pCRC,sizeof(uint32));    //取得原来的CRC校验码


	//重新CRC校验码
	char* pbuf=bufLog+sizeof(KH);           
	uint32 CRCsize=cmd_size-sizeof(KH)-sizeof(uint32);
	uint32 CRC32_info=Get_CRC32_fromBuffer(0,(uint8*)pbuf,CRCsize);

	if(CRC32_info!=CRCrecv)
	{
		printf("Server:LOGrecv CRC32 Error\n");
	}

	char* pLog=bufLog+sizeof(LOGBACKINFO);   //日志内容地址 在此处理
	//发送给UI显示
#endif

}





void TcpClientSocket::do_send_file()   //升级文件包推送
{
/*
//升级包推送报文 (仅包含文件信息,文件内容后续传输)  升级包Upgradepackage
typedef struct _UpdateFileINFO_
{
	KH kh;
    uint64  reserved;               //保留字  
	char    UpdateSerialNo[16];    //128位字段，软件升级包的序列号。
    char    reserved2[16];          //保留字段2 
    uint32  DescriptionLength;     //32位无符号整型，升级软件描述信息长度。
    char    Description[256];      //可变长字段，升级软件描述信息。(暂定256位字符串)
    uint32  FID;                   //32位无符号整型，传送升级软件与配置文件的ID号。
    uint32  UpdateFileNameLength;  //32位无符号整型，升级软件文件名长度。
    char    UpdateFileName[256];   //可变长字段，升级软件文件名。   (暂定256位字符串)
    uint32  UpdateFileLength;      //32位无符号整型，最大可以表示4GB,升级软件文件长度。如升级软件通过互联网传送，需指明升级软件文件长度；如升级软件通过卫星信道传送，该项取值为0。
    //char UpdateFile[1024];       //可变长字段，升级软件文件。如升级软件通过互联网传送，此项为传送的升级软件文件的；如升级软件通过卫星信道传送，此项不存在。
	//uint32  CRC32;               //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateFileINFO;
//传送完成结构体以上后,在传送文件本身,然后传送文件CRC校验码
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
	//char Filename[256]="test.tgz";  //要传送的升级包文件名 
	char* Filename=this->pathname_updatefile;   //包含路径
    printf("升级包文件路径=%s\n",Filename);


    //从路径提取末端文件名---填充到升级包文件结构体
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
    printf("升级包文件名UpdateFileName=%s\n",p1);

    //序列号11位数字
    char* p2=UpFileInfo.UpdateSerialNo;     //用升级包文件名去掉tgz压缩包后缀作为序列号
    strcpy(p2,pname+1);
    char tmp[20];
    strcpy(tmp,p2);

    for(int i=0;i<15;i++)
    {
        if(tmp[i]=='.'&&tmp[i+1]=='t'&&tmp[i+2]=='g'&&tmp[i+3]=='z')
            tmp[i]='\0';   //截断字符串
    }
    tmp[15]='\0';
    strcpy(p2,tmp);

	p2[15]='\0';
	printf("升级包序列号UpdateSerialNo=%s\n",p2);   


    int FileLength=getfilesize(Filename);   //获取某个文件大小
	UpFileInfo.UpdateFileLength=FileLength;
	printf("升级包Filename=%s,FileLength=%d\n",Filename,FileLength);

    //整个命令长度
	UpFileInfo.kh.cmd_length=sizeof(UpdateFileINFO)-sizeof(KH)+FileLength+sizeof(uint32);
    
	  //先计算文件头信息的CRC32
	  char* pbuf=(char*)&UpFileInfo;            
	  uint32 CRCsize=sizeof(UpdateFileINFO);
	  uint32 CRC32_info=Get_CRC32_fromBuffer(0,(uint8*)pbuf,CRCsize);
	  printf("Sever:send CRC32_of_Fileinfo=%X\n",CRC32_info);

      this->UpdateFileInfo=UpFileInfo;   //保存文件信息到当前结构体,



    //依次传送文件信息,文件内容本身,文件CRC校验
	write((char*)&UpFileInfo,sizeof(UpdateFileINFO));
	flush();   
	waitForBytesWritten(-1);
    
	 FILE *fp = fopen(Filename,"rb");
    if(!fp)
    {
		printf("文件读取错误\n");
        return;
    }

	char buffer[1024*40];//1024*50
    uint64 percent=0;    //文件传送百分比
     flag_update=1;   //正在升级

	uint32 sendsize=0;      //记录读取到的文件大小  最大4G
    while(!feof(fp))
    {
        /* read to buffer */
       int count = fread(buffer, sizeof(char),sizeof(buffer),fp);
       CRC32_info=Get_CRC32_fromBuffer(CRC32_info,(uint8*)buffer,count);   //把计算CRC也放到这里

//	    printf("Sever:send count=%d\n",count);
//	    printf("Sever:send CRC32_of_Fileinfo=%X\n",CRC32_info);
//      write buffer to file
 	    write(buffer,count);    //bug在此处  不能write(buffer,sizeof(buffer));
	    flush();
        waitForBytesWritten(-1);

        sendsize+=count; 
        percent=10000*(float)sendsize/(float)FileLength;

            if(percent%100==0)    //不用频繁发送信号
            {
          //      printf("percent=%d\n",percent);
            //发送信号给UI更新进度条
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



void TcpClientSocket::do_update_start() //命令接收机,可以升级
{
/*
//接收机远程维护控制子系统在接收到远程升级包携眷反馈报文后向接收机发出的升级请求。
//命令接收机开始升级
typedef struct _UpdateSTART_
{
	KH kh;
    uint32  UpdateSerialNo;          //128位字段，接收软件升级后的版本信息。
    uint32  reserved;
	uint32  CRC32;                   //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateSTART;
*/
     UpdateSTART UpdateStart;
	 memset(&UpdateStart,0,sizeof(UpdateSTART));
	 UpdateStart.kh.pkgHead=PKG_HEAD;
	 UpdateStart.kh.flag_pwd=KEY_NO;
	 UpdateStart.kh.cmd=CMD_UPDATE_START;
	 UpdateStart.kh.cmd_Sub=CMD_ZERO;
	 UpdateStart.kh.cmd_length=sizeof(UpdateSTART)-sizeof(KH);

	 //getUpdateSerialNo();不用了   //发送文件的时候保存到当前线程对象就行

     memcpy(UpdateStart.UpdateSerialNo,this->UpdateFileInfo.UpdateSerialNo,16);

     
	 flag_update=2;   //到此可以认为升级完成,因为标准里面没有接收机升级操作后的返回值。
    // emit signal_authenRequest(); //更新认证列表UI 

	 //更新远程升级的客户端列表
	 emit signal_CMD_SocketToUI(this->socketID,SK_UPDATE_TREEWIDGET_LIST,0); 

	 char* psend=(char*)&UpdateStart;
	 int   sendsize=sizeof(UpdateSTART);
     crc_encry_send(psend,sendsize,0);    //添加校验 加密，发送 //加密偏移为0,报文全部加密
}

//先用全局变量传递日志请求报文结构体;--改QVariant发送信号
void TcpClientSocket::do_get_Log()   //手动获取日志
{
    //已经在UI里面填充好了 时间 类型等
	LOGREQUEST LOGRequest=this->LOGRequest;
    
	//填充报文头
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

void TcpClientSocket::do_Log()   //手动获取日志
{
	//已经在UI里面填充好了 时间 类型等
	LOGREQUEST LOGRequest=this->LOGRequest;
	LOGRequest.DateStart = 0;
	LOGRequest.DateEnd = 1000;
	LOGRequest.LogType = 0;
	//填充报文头
	LOGRequest.kh.pkgHead=PKG_HEAD;
	LOGRequest.kh.flag_pwd=KEY_MEET;
	LOGRequest.kh.cmd=CMD_LOGFILE_REQUEST;
	LOGRequest.kh.cmd_Sub=CMD_ZERO;
	LOGRequest.kh.cmd_length=sizeof(LOGREQUEST)-sizeof(KH);


	char* psend=(char*)&LOGRequest;
	int   sendsize=sizeof(LOGREQUEST);

	crc_encry_send(psend,sendsize,0);  
}

//加字符串传参？ char* buf,int size
void TcpClientSocket::UIcmd_process(int SocketID,int cmdtype,QByteArray ba)   //	处理UI发过来的命令
{

	 if(SocketID!=this->socketDescriptor())  
//if(SocketID!=this->socketID)  

		return;  //如果不是发给当前socket线程的命令则退出

        printf("TcpClientSocket::UIcmd_process()\n");

        switch(cmdtype)
		{
            case UI_SEND_UPDATE_FILE:       //发送升级文件包
			     do_send_file();   break;
			case UI_GET_LOG:
                 do_get_Log();     break;   //手动获取日志
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
	//构建心跳第一部分结构件
	struct MD5H {
		KH kh;
		uint32 filmID;
		uint32 len;
		uint32 CRC;
	};
	char md5[] = "<EncodeInfo><EncodeFileList ID=\"20160324\" Name=\"Test\"><ChallengeCode>123<ChallengeCode>"
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
	crc_encry_send((char*)&p->kh, sizeof(MD5H) + p->len + 4, 0);    //添加校验 加密，发送 //加密偏移为0,报文全部加密
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