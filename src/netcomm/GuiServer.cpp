#include "GuiServer.h"
#include "UiProtocal.h"
#include "BaseOperation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dvb/mytype.h>

#include <log/Log.h>
extern ILog* gLog;

GuiServer gGuiServer;
GuiServer* CreateGuiServer()
{
	return &gGuiServer;
}

void ReleaseGuiServer(GuiServer* pGui)
{

}

CopyThread::CopyThread():m_status(0)
{
}

CopyThread::~CopyThread()
{
	Stop();
}

bool CopyThread::Init()
{
	printf("CopyThread:Init\n");
	Start();
	return true;
}

bool CopyThread::Start()
{
	printf("CopyThread: Start\n");
	Stop();
	m_status = RUN;
	return true;
}

bool CopyThread::Stop()
{
	m_status = STOP;
	printf("CopyThread: Stop\n");
	return true;
}
void CopyThread::doit()
{
   while(1)
	{
		switch(m_status)
		{
		case RUN:
		     printf("CopyThread: Start in loop\n");
			 copy_dir(this->path_dst,this->path_src,this->complete_percent,this->copy_flag);
			 usleep(1000);
			 Stop();    //һξֹͣ
			break;
		case STOP:
			 printf("CopyThread: Stop in loop\n");
		     usleep(1000);
			 return;
		case IDLE:
			usleep(1000);
			break;
		}
   }
}


#include <ftw.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>      /*fprintf(),stderr,BUFSIZ*/
#include <stdlib.h>     /**/
#include <string.h>     /*stderror()*/
#include <fcntl.h>      /*open(),flag*/
#include <errno.h>      /*errno*/
#include <unistd.h>     /*ssize_t*/
#include <sys/types.h>
#include <sys/stat.h>   /*mode_t*/

#define BUFSIZE 1024*512*2   //ļ棬̫ĻΪǵݹݹã洦
#define PERMS 0666
#define DUMMY 0

//long long complete_size=0;       //ѾɿС
//float  complete_percent=0.0;    //Ѿɰٷֱ


//ɨĿ¼ͳƴС,ͳƵֽĿ
long long scan_dir(char *dir, int depth)   // Ŀ¼ɨ? 
{  

    long long totalsize=0;          //Դ·ļܴС

    DIR *dp;                      // Ŀ¼ָ  
    struct dirent *entry;         // direntṹָ뱣Ŀ¼  
    struct stat statbuf;          // statbufṹ? 
    if((dp = opendir(dir)) == NULL) // Ŀ¼ȡĿ¼ָ룬жϲǷ? 
    {  
        puts("can't open scan_dir");   
        return totalsize;  
    }  
    chdir (dir);                     // лǰĿ¼  
    while((entry = readdir(dp)) != NULL)  // ȡһĿ¼Ϣδѭ  
    {  
        lstat(entry->d_name, &statbuf); // ȡһԱ  

       //    totalsize+=statbuf.st_size;   //ͳļС

        if(S_IFDIR &statbuf.st_mode)    // жһԱǷĿ¼  
        {  
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)  
              continue;  
          //  printf("%*s%s/\n", depth, "", entry->d_name);  // Ŀ¼  
            scan_dir(entry->d_name, depth+4);              // ݹ���ɨһĿ? 
          //  totalsize+=statbuf.st_size;   //ͳļС
        }  
        else  
         {
           if( strcmp(entry->d_name,"..")!=0 && strcmp(entry->d_name,".")!=0)
              {
		      //  printf("%*s%s\n", depth, "", entry->d_name);  // ԲĿ¼ĳԱ  
		          totalsize+=statbuf.st_size;   //ͳļС
              }
         }
    }

    chdir("..");       // صϼĿ¼  
    closedir(dp);   	// رĿ¼  
	return  totalsize;
}  


int cpfile(char *path_dst,char *path_src,long long totalsize,long long* complete_size,int* complete_percent,int* copy_flag)
{
  int source,target,nread;
  char iobuf[BUFSIZE];
  
  //static long long complete_size=0;
  if((source=open(path_src,O_RDONLY,DUMMY))==-1)
     {
	    printf("cpfile:Source file open error!\n");
        return 1;
     }
  if((target=open(path_dst,O_WRONLY|O_CREAT,PERMS))==-1)
    {
        printf("cpfile:Target file open error!\n");
        return 2;
     }
   
    int  numprint;    //ƲҪƵӡ


  while((nread=read(source,iobuf,BUFSIZE))>0)
     if(write(target,iobuf,nread)!=nread)
       {
          printf("cpfile:Target file write error!\n");
          return 3;
       }
    else
      {
	   (*complete_size)+=nread;
	   *complete_percent=10000*((float)(*complete_size)/(float)totalsize);   //Ѿɰٷֱ
	   //10000intʹݣͻתΪ0-100
	   // numprint++;
		//if(numprint%100==0)   //ÿ100MӡһϢ
         if(*complete_percent%100==0)   //ÿǰٷ֮һӡһϢ
		{
			printf("complete_size= %lld \n",*complete_size);       
			printf("totalsize= %lld\n",totalsize);                       //
			printf("complete_percent= %d\n",*complete_percent/100);    //ɰٷֱ 
			printf("Copy file to %s ......\n",path_dst);
		}

      }	
/*
   struct stat s,t;
   fstat(source,&s);
   totalsize2+=s.st_size;   //
*/     
   close(source);

   //fflush(FILE* srteam)(stdio.h) : дѻݼʱд,������ļܸоܿ.ǲһʵд?
   //linuxint fsync(int fd);#include <unistd.h>

   fsync(target); //ͬڴ޸ĵļݵ?
   close(target);
   return 0;
}



int cpdir(char *target_dir,char *source_dir,long long totalsize,long long* complete_size,int* complete_percent,int* copy_flag)
{
     DIR *source=NULL; 
     DIR *target=NULL;
     struct dirent *ent=NULL;
     char  name1[1024],name2[1024];

     source=opendir(source_dir);    
     mkdir(target_dir,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
     target=opendir(target_dir);  

     if(source!=NULL&&target!=NULL)
        {
          while((ent=readdir(source))!= NULL)
          { 
             if( strcmp(ent->d_name,"..")!=0 && strcmp(ent->d_name,".")!=0)
                {               
                   strcpy(name1,"\0");
                   strcat(name1,source_dir);
                   strcat(name1,"/");
                   strcat(name1,ent->d_name);
                   strcpy(name2,"\0");
                   strcat(name2,target_dir);
                   strcat(name2,"/"); 
                   strcat(name2,ent->d_name);
                   if(ent->d_type==4)
                       cpdir(name2,name1,totalsize,complete_size,complete_percent,copy_flag);
                   if(ent->d_type==8)
                       cpfile(name2,name1,totalsize,complete_size,complete_percent,copy_flag);
                }              
           }
         closedir(source);
         closedir(target);
       }

    return 0;
}
/*
int main(int argc,char *argv[])
{ 
   char path_src[1000],path_dst[1000];
   
	if(argc==1){
	   printf("Please input the sourse direct path and name :");
	   scanf("%s",path_src);
	   printf("Please input the target direct path and name :");
	   scanf("%s",path_dst);
	}
	else if(argc==3){
	strcpy(path_dst,argv[1]);
	strcpy(path_src,argv[2]);
	 printf("argc==3\n");
	}
	else {
	printf("parameters error'\n");
	return -1;
	}

	puts("scan :\n");  
	//scan_dir("/myselfcode", 0);
	scan_dir(path_src, 0);    
	puts("scan over\n");  
	printf("totalsize= %d Mb\n",totalsize/1024/1024);  //
	
    printf("Begin copy ........\n");
    cpdir(path_dst,path_src);
    printf("Copy end !\n");
 
    printf("totalsize= %d \n",totalsize);  //
    printf("totalsize2= %d \n",totalsize2);  //

   return 0;
}
*/
   
//int* complete_percent     ɵİٷֱ
//int* flags                //Ϊ0ʾɣΪ1ʾڿ 


int CopyThread::copy_dir(char *dir_dst,char *dir_src,int* complete_percent,int* copy_flag)
{
	  printf("toCopying PATH_dst=%s PATH_src=%s \n",dir_dst,dir_src);  

	//Ѵݽ·棬ɿ?
    // char Path_dst[1024];
    // char Path_src[1024];

      *complete_percent=0;    //ÿοǰ ɰٷֱ 
	  *copy_flag=1;          //ڿ=1 
    
      long long totalsize=0;

      long long tmp=0;    //Segmetation fault  long long* complete_size;*complete_size=0;
      long long* complete_size=&tmp;

	  totalsize=scan_dir(dir_src,0);

	  printf("toCopying Dir totalsize= %lld\n",totalsize);  
      if(totalsize==0)  return 0;

	  printf("Dir:Begin copy Dir ......\n");
	  cpdir(dir_dst,dir_src,totalsize,complete_size,complete_percent,copy_flag);
	  printf("Dir:Copy end !\n");

	  *copy_flag=0;          //=0 , һɹ
	  return 0;
}





#define GUI_PORT (10003)

GuiServer::GuiServer():m_status(0), m_pThread(0)
{
}

GuiServer::~GuiServer()
{
	Stop();
	if(m_pThread)
	{
		delete m_pThread;
		m_pThread = 0;
	}
}

bool GuiServer::Init()
{
	DPRINTF("[GuiServer] Init\n");
	Start();
	return true;
}

bool GuiServer::Start()
{
	DPRINTF("[GuiServer] Start\n");
	Stop();
	m_status = RUN;
	return true;
}

bool GuiServer::Stop()
{
	m_status = STOP;
	DPRINTF("[GuiServer] Stop\n");
	m_SrvSocket.Destroy();
	std::list<GuiThread*>::iterator itor;
	for (itor = m_listClient.begin(); itor != m_listClient.end(); ++itor)
	{
		GuiThread* client = *itor;
		client->Stop();
		while(!client->isStop()) usleep(100000);
		client->stop();
		delete client;
	}
	m_listClient.clear();
	return true;
}

void GuiServer::doit()
{
	char str[512];
	bool bSuccess = false;
	int i = 0;
	while(m_status != STOP && bSuccess == false)
	{
	try
	{
			if((i % 1000*1000*5) == 0)
			{
				i = 0;
				//DPRINTF("Create socket\n");
		if (m_SrvSocket.Create(AF_INET, SOCK_STREAM, 0))
		{
			struct sockaddr_in addr_in;
			memset(&addr_in, 0, sizeof(sockaddr_in));
			addr_in.sin_family = AF_INET;
			addr_in.sin_port = htons(GUI_PORT);
			addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
			int error = 0;
			do 
			{
				t_timeout tm = 3000;
				if(m_SrvSocket.Bind(&addr_in))
				{
							if (!m_SrvSocket.Listen(20))
						throw -1;
							bSuccess = true;
							DPRINTF("Bind success\n");
				}
				else
					throw -2;

			} while (error);
		}
				else
				{
					//DPRINTF("Create failure\n");
				}
			}
			else
			{
				i++;
				usleep(1000);
			}
	}
	catch (int &a)
	{
		switch(a)
		{
		case -1:
			sprintf(str, "[GuiServer] Server Socket Listen error: %d.", status());
			break;
		case -2:
			sprintf(str, "[GuiServer] Server Socket Bind error: %d.", status());
			break;
		default:
			sprintf(str, "[GuiServer] Server Socket unknow error.", status());
		}
			DPRINTF("%s\n", str);
		if (gLog)
	{
			gLog->Write(LOG_ERROR, str);
		}
			//		return;
		}
	}
	DPRINTF("Run\n");
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			{
				if (m_pThread == NULL)
				{
					m_pThread = new GuiThread;
				}
				t_timeout tm = 100;
				if(m_SrvSocket.Accept(m_pThread->m_socket, NULL, NULL, &tm) == 0)
				{
					m_listClient.push_back(m_pThread);
					m_pThread->Init();
					if (m_pThread->status() == brunt::thread_ready)
					{
						m_pThread->start();
						DPRINTF("Start Gui thread\n");
					}
					m_pThread = NULL;
				}
			}
			break;
		case STOP:
			DPRINTF("[GuiServer] Stop in loop\n");
			return;
		case IDLE:
			usleep(1000);
			break;
		}
	}
}

GuiThread::GuiThread():m_status(0), m_Content(NULL), m_mkfs(NULL)
{
	m_Content = new ContentOperation;
	m_mkfs = new mke2fs;
	copyThread =NULL;   //CopyThread*ļָ߳
}

GuiThread::~GuiThread()
{
	Stop();
	if(m_Content)
	{
		delete m_Content;
		m_Content = NULL;
	}
	if(m_mkfs)
	{
		delete m_mkfs;
		m_mkfs = NULL;
	}
}

bool GuiThread::Init()
{
	DPRINTF("[GuiThread] Init\n");
	Start();
	return true;
}

bool GuiThread::Start()
{
	DPRINTF("[GuiThread] Start\n");
	Stop();
	m_status = RUN;
	bStop = false;
	return true;
}

bool GuiThread::Stop()
{
	m_status = STOP;
	DPRINTF("[GuiThread] Stop\n");

	usleep(100000);
	return true;
}

void GuiThread::doit()
{
	while(1)
	{
		switch(m_status)
		{
		case RUN:
			{
#if 0
				char buf[255];
				memset(buf, 0, 255);
				size_t getsize;
				int error;
				if((error = m_socket.Receive(buf, 255, getsize)) != 0)
				{
					DPRINTF("[GuiThread] Receive error %d\n", error);
					m_status = STOP;
				}
				else
				{
					DPRINTF("%s %d %x\n", buf, error, this);
				}
#else
				if (!UiProcessFilter())
				{
					DPRINTF("[GuiThread] Stop - Receive error\n");
					m_status = STOP;
				}
#endif
			}
			break;
		case STOP:
			DPRINTF("[GuiThread] Stop in loop %d\n", this);
			bStop = true;
			return;
		case IDLE:
			usleep(1000);
			break;
		}
	}
}

bool GuiThread::Read(char* buf, int rSize, int& nRead)
{
	try
	{
		int i = 0;
		int n = sizeof(KL);
		size_t getsize;
		int error = 1;
		KL * pKL;
		t_timeout tm = 1;

		do 
		{
			if(m_socket.Receive(buf+i, n, getsize, &tm) != 0)
				throw -1;
			else
			{
				if (getsize < n)
				{
					n -= getsize;
					i += getsize;
				}else/* if(getsize == n)*/
				{
					pKL = (KL*)buf;
					if (pKL->m_pkgHead == 0x7585)
					{
						error = 0;
					}
					else
					{
						i = 0;
						n = sizeof(KL);
					}
				}
			}
		} while (error && (m_status != STOP));
		n = pKL->m_length;

		i = sizeof(KL);
		do 
		{
			if(m_socket.Receive(buf+i, n, getsize, &tm) != 0)
				throw -1;
			else
			{
				if (getsize < n)
				{
					n -= getsize;
					i += getsize;
				}else
				{
					nRead = pKL->m_length + sizeof(KL);
					return true;
				}
			}
		} while (error && (m_status != STOP));
	}
	catch (int&)
	{
		DPRINTF("[GuiThread] Receive error\n");
		return false;
	}
}

bool GuiThread::Write(char* buf, int wSize, int& nWrite)
{
	try 
	{
		int error = 1;
		int i = 0;
		int n = wSize;
		size_t writesize;
		t_timeout tm = 1;
		do 
		{
			if(m_socket.Send(buf + i, n, writesize, &tm) != 0)
				throw -1;
			else
			{
				if (n > writesize)
				{
					n -= writesize;
					i += writesize;
				}
				else
				{
					nWrite = wSize;
					return true;
				}
			}
		} while (error && (m_status != STOP));

	}
	catch(int &)
	{
		DPRINTF("[GuiThread] Send error\n");
		return false;
	}
}

bool GuiThread::UiProcessFilter()
{
	char buf[2048];
	memset(buf, 0, 2048);
	int getsize;
	if (Read(buf, 2048, getsize))
	{
		KL* pKL = (KL*)buf;
		if (pKL->m_pkgHead == 0x7585)
		{
		switch(pKL->m_keyID)
		{
		case S_GET_SATELLITE:
				return S_GetSatellite(buf);
			case S_GET_RECEIVE:
				return S_GetReceive(buf);
			case S_GET_CONFIG:
				return C_GetTuner(buf);
			case S_SET_CONFIG:
				return C_SetTuner(buf);
			case N_GET_CONFIG:
				return N_GetConfig(buf);
			case N_SET_CONFIG:
				return N_SetConfig(buf);
			case N_GET_REMOTE:
				return N_GetRemote(buf);
			case N_SET_REMOTE:
				return N_SetRemote(buf);

			case M_UPDATE_PROGRAM_LIST_HDD:
               return M_UpdateProgramList_HDD(buf);          //UpdateProgramListˢӲб���Ա? 
			case M_UPDATE_PROGRAM_LIST_USB:
               return M_UpdateProgramList_USB(buf);         //UpdateProgramListˢӲб���Ա?
			
			case M_IS_PROGRAM_LIST_READY_HDD:                //IsProgramListReadyѯǷ׼
			   return M_IsProgramListReady_HDD(buf); 
			case M_IS_PROGRAM_LIST_READY_USB:                    //IsProgramListReadyѯǷ׼
			   return M_IsProgramListReady_USB(buf); 

			case M_GET_HDD_CONTENT_LIST:
				return M_GetContent_HDD(buf);         //new  rename
			case M_GET_HDD_INFO:
				return M_GetDiskInfo_HDD(buf);        //new  rename

			//new
			case M_GET_USB_CONTENT_LIST:
				return M_GetContent_USB(buf);
			case M_GET_USB_INFO:
				return M_GetDiskInfo_USB(buf);

			case M_COPYDIR_HDD_TO_USB:
                 return M_CopyDir_HDD_TO_USB(buf);

			case M_GETCOPYPROCESS:   //ؿ
                return setCopyProgress(buf);

			case S_REBOOT:
				return S_Reboot(buf);
			case S_SHUTDOWN:
				return S_Shutdown(buf);
			case S_SET_FORMATDISK:
				return S_FormatDisk(buf);
			case S_GET_FORMAT_RES:
				return S_FormatResult(buf);
			case S_GET_FORMAT_STATUS:
				return S_FormatStatus(buf);

            //new
			case M_USB_MOUNT:
				return S_USB_Mount(buf);
			case M_USB_UNMOUNT:
				return S_USB_UnMount(buf);

			case M_DELETE_DIR:
               return M_DeleteDir(buf);   //ɾӰƬĿ¼    

			case S_GET_TMS:
               return S_Get_TMS(buf);  
			 
			case R_GET_RAID_INFO:
				return R_GetRaidInfo(buf);

			case M_UPDATE_PROGRAM_LIST_RAID:
				return M_UpdateProgramList_RAID(buf);          //UpdateProgramListˢӲб���Ա? 
			case M_IS_PROGRAM_LIST_READY_RAID:                //IsProgramListReadyѯǷ׼
				return M_IsProgramListReady_RAID(buf); 
			case M_GET_RAID_CONTENT_LIST:
				return M_GetContent_RAID(buf);         //new  rename
			case M_GET_RAID_INFO:
				return M_GetDiskInfo_RAID(buf);        //new  rename

			default:
				return UnknowFunction(buf);
		}
		}
		else
			return ErrorProtocol(buf);
	}
	return false;
}

#include <dvb/SatelliteRecv.h>
bool GuiThread::ErrorProtocol(char* buf)
{
	char str[512];
	KL *pKL = (KL*)buf;
	if (gLog)
	{
		sprintf(str, "[GuiThread] Received error protocol: %04X.", pKL->m_pkgHead);
		gLog->Write(LOG_ERROR, str);
	}
	pKL->m_length = 1;
	buf[sizeof(KL)] = -1;
	int setsize = pKL->m_length + sizeof(KL);
	return Write(buf, setsize, setsize);
}

bool GuiThread::UnknowFunction(char* buf)
{
	char str[512];
	KL *pKL = (KL*)buf;
	if (gLog)
	{
		sprintf(str, "[GuiThread] Received unknow fuction: %04X.", pKL->m_keyID);
		gLog->Write(LOG_ERROR, str);
	}
	pKL->m_length = 1;
	buf[sizeof(KL)] = -1;
	int setsize = pKL->m_length + sizeof(KL);
	return Write(buf, setsize, setsize);
}

bool GuiThread::S_GetSatellite(char* buf)
{
	extern TUNER_INFO gInfo;
	KL *pKL = (KL*)buf;
	pKL->m_length = sizeof(gInfo);
	int setsize = pKL->m_length + sizeof(KL);
	memcpy(buf + sizeof(KL), &gInfo, sizeof(gInfo));
	return Write(buf, setsize, setsize);
}

bool GuiThread::S_GetReceive(char* buf)
{
	extern RECEIVE_INFO gRecv;

	SL FN, Uuid, IssueDate, Issuer, Creator, Extend;
	FN.m_sID = RECEIVE_FILM_NAME;
	FN.m_length = gRecv.strFilmName.size();

	Uuid.m_sID = RECEIVE_FILM_UUID;
	Uuid.m_length = gRecv.strUuid.size();

	IssueDate.m_sID = RECEIVE_FILM_ISSUE_DATE;
	IssueDate.m_length = gRecv.strIssueDate.size();

	Issuer.m_sID = RECEIVE_FILM_ISSUER;
	Issuer.m_length = gRecv.strIssuer.size();

	Creator.m_sID = RECEIVE_FILM_CREATOR;
	Creator.m_length = gRecv.strCreator.size();

	Extend.m_sID = RECEIVE_EXTEND;
	Extend.m_length = gRecv.strExtend.size();

	int datalen = sizeof(gRecv.nFileLength) +
		sizeof(gRecv.nReceiveLength) +
		sizeof(gRecv.nTotalSegment) +
		sizeof(gRecv.nReceiveSegment) +
		sizeof(gRecv.nCrcErrorSegment) +
		sizeof(gRecv.nLostSegment) +
		sizeof(gRecv.nFileID) +
		sizeof(gRecv.nReceiveStatus);

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);
	memcpy(pos, &gRecv, datalen);
	pos += datalen;

	memcpy(pos, &FN, sizeof(FN));
	pos += sizeof(FN);
	memcpy(pos, gRecv.strFilmName.c_str(), FN.m_length);
	pos += FN.m_length;

	memcpy(pos, &Uuid, sizeof(Uuid));
	pos += sizeof(Uuid);
	memcpy(pos, gRecv.strUuid.c_str(), Uuid.m_length);
	pos += Uuid.m_length;

	memcpy(pos, &IssueDate, sizeof(IssueDate));
	pos += sizeof(IssueDate);
	memcpy(pos, gRecv.strIssueDate.c_str(), IssueDate.m_length);
	pos += IssueDate.m_length;

	memcpy(pos, &Issuer, sizeof(Issuer));
	pos += sizeof(Issuer);
	memcpy(pos, gRecv.strIssuer.c_str(), Issuer.m_length);
	pos += Issuer.m_length;

	memcpy(pos, &Creator, sizeof(Creator));
	pos += sizeof(Creator);
	memcpy(pos, gRecv.strCreator.c_str(), Creator.m_length);
	pos += Creator.m_length;

	memcpy(pos, &Extend, sizeof(Extend));
	pos += sizeof(Extend);
	memcpy(pos, gRecv.strExtend.c_str(), Extend.m_length);
	pos += Extend.m_length;

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::C_GetTuner(char* buf)
{
	extern TUNER_CONF gTuner;

	SL DevName, DelSys, Fec, Mod, Roll, Pol;
	
	DevName.m_sID = TUNER_DEV_NAME;
	DevName.m_length = gTuner.strDevName.size();

	DelSys.m_sID = TUNER_DEL_SYS;
	DelSys.m_length = gTuner.strDelSys.size();

	Fec.m_sID = TUNER_FEC;
	Fec.m_length = gTuner.strFec.size();

	Mod.m_sID = TUNER_MOD;
	Mod.m_length = gTuner.strModulation.size();

	Roll.m_sID = TUNER_ROLL_OFF;
	Roll.m_length = gTuner.strRollOff.size();

	Pol.m_sID = TUNER_POL;
	Pol.m_length = gTuner.strPolVert.size();

	int datalen = sizeof(unsigned int) * 5;

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);
	memcpy(pos, &gTuner, datalen);
	pos += datalen;

	memcpy(pos, &DevName, sizeof(DevName));
	pos += sizeof(DevName);
	memcpy(pos, gTuner.strDevName.c_str(), DevName.m_length);
	pos += DevName.m_length;

	memcpy(pos, &DelSys, sizeof(DelSys));
	pos += sizeof(DelSys);
	memcpy(pos, gTuner.strDelSys.c_str(), DelSys.m_length);
	pos += DelSys.m_length;

	memcpy(pos, &Fec, sizeof(Fec));
	pos += sizeof(Fec);
	memcpy(pos, gTuner.strFec.c_str(), Fec.m_length);
	pos += Fec.m_length;

	memcpy(pos, &Mod, sizeof(Mod));
	pos += sizeof(Mod);
	memcpy(pos, gTuner.strModulation.c_str(), Mod.m_length);
	pos += Mod.m_length;

	memcpy(pos, &Roll, sizeof(Roll));
	pos += sizeof(Roll);
	memcpy(pos, gTuner.strRollOff.c_str(), Roll.m_length);
	pos += Roll.m_length;

	memcpy(pos, &Pol, sizeof(Pol));
	pos += sizeof(Pol);
	memcpy(pos, gTuner.strPolVert.c_str(), Pol.m_length);
	pos += Pol.m_length;

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	return Write(buf, sendsize, sendsize);
}


bool GuiThread::C_SetTuner(char* buf)
{
	extern TUNER_CONF gTuner;
	extern char bTunerChange;
	int datalen = sizeof(unsigned int) * 5;
	memcpy(&gTuner, buf + sizeof(KL), datalen);

	KL *pKL = (KL*) buf;
	int len = pKL->m_length - datalen;
	int pos = sizeof(KL) + datalen;
	while(len > 0)
	{
		SL * sl = (SL*)(buf + pos);
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
		case TUNER_DEV_NAME:
			gTuner.strDevName = tmp;
			break;
		case  TUNER_DEL_SYS:
			gTuner.strDelSys = tmp;
			break;
		case TUNER_FEC:
			gTuner.strFec = tmp;
			break;
		case TUNER_MOD:
			gTuner.strModulation = tmp;
			break;
		case TUNER_ROLL_OFF:
			gTuner.strRollOff = tmp;
			break;
		case TUNER_POL:
			gTuner.strPolVert = tmp;
			break;
		}
	}
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	int setsize = sizeof(KL) + 1;

	SatelliteConfOperation sco;
	sco.WriteConfig();
	bTunerChange = 1;
	return Write(buf, setsize, setsize);
}


bool GuiThread::N_GetConfig(char* buf)
{
	NetOperation no;

	std::list<NETWORK_CONF> m_list = no.GetNetConfig();

	SL Dhcp, DevName, Ip, Netmask, Gateway;

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);
	
	std::list<NETWORK_CONF>::iterator itor;
	for (itor = m_list.begin(); itor != m_list.end(); ++itor)
	{
		NETWORK_CONF nc = *itor;

		Dhcp.m_sID = NET_DHCP;
		Dhcp.m_length = sizeof(nc.nDhcp);

		DevName.m_sID = NET_DEV_NAME;
		DevName.m_length = nc.strDevName.size();

		Ip.m_sID = NET_IP;
		Ip.m_length = nc.strIp.size();

		Netmask.m_sID = NET_NETMASK;
		Netmask.m_length = nc.strNetmask.size();

		Gateway.m_sID = NET_GATEWAY;
		Gateway.m_length = nc.strGateway.size();

		memcpy(pos, &Dhcp, sizeof(Dhcp));
		pos += sizeof(Dhcp);
		memcpy(pos, &nc.nDhcp, Dhcp.m_length);
		pos += Dhcp.m_length;

		memcpy(pos, &DevName, sizeof(DevName));
		pos += sizeof(DevName);
		memcpy(pos, nc.strDevName.c_str(), DevName.m_length);
		pos += DevName.m_length;

		memcpy(pos, &Ip, sizeof(Ip));
		pos += sizeof(Ip);
		memcpy(pos, nc.strIp.c_str(), Ip.m_length);
		pos += Ip.m_length;

		memcpy(pos, &Netmask, sizeof(Netmask));
		pos += sizeof(Netmask);
		memcpy(pos, nc.strNetmask.c_str(), Netmask.m_length);
		pos += Netmask.m_length;

		memcpy(pos, &Gateway, sizeof(Gateway));
		pos += sizeof(Gateway);
		memcpy(pos, nc.strGateway.c_str(), Gateway.m_length);
		pos += Gateway.m_length;

	}

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::N_SetConfig(char* buf)
{
	std::list<NETWORK_CONF> m_list;

	KL *pKL = (KL*) buf;

	int len = pKL->m_length;
	int pos = sizeof(KL);
	NETWORK_CONF nc;
	while(len > 0)
	{
		SL * sl = (SL*)(buf + pos);
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
		case NET_DHCP:
			memcpy(&nc.nDhcp, buf + pos - sl->m_length, sl->m_length);
			break;
		case  NET_DEV_NAME:
			nc.strDevName = tmp;
			break;
		case NET_IP:
			nc.strIp = tmp;
			break;
		case NET_NETMASK:
			nc.strNetmask = tmp;
			break;
		case NET_GATEWAY:
			nc.strGateway = tmp;
			m_list.push_back(nc);
			break;
		}
	}

	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	int setsize = sizeof(KL) + 1;

	bool res = Write(buf, setsize, setsize);
	NetOperation no;
	no.SetNetConfig(m_list);
	return res;
}

bool GuiThread::N_GetRemote(char* buf)
{
	NetOperation no;

	REMOTE_CONF rc = no.GetRemoteConfig();

	SL Dns1, Dns2, Remote, Port;

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);

	Dns1.m_sID = REMOTE_DNS;
	Dns1.m_length = rc.strDns1.size();

	Dns2.m_sID = REMOTE_DNS;
	Dns2.m_length = rc.strDns2.size();

	Remote.m_sID = REMOTE_SERVER;
	Remote.m_length = rc.strRemote.size();

	Port.m_sID = REMOTE_PORT;
	Port.m_length = sizeof(rc.nPort);

	memcpy(pos, &Dns1, sizeof(Dns1));
	pos += sizeof(Dns1);
	memcpy(pos, rc.strDns1.c_str(), Dns1.m_length);
	pos += Dns1.m_length;

	memcpy(pos, &Dns2, sizeof(Dns2));
	pos += sizeof(Dns2);
	memcpy(pos, rc.strDns2.c_str(), Dns2.m_length);
	pos += Dns2.m_length;

	memcpy(pos, &Remote, sizeof(Remote));
	pos += sizeof(Remote);
	memcpy(pos, rc.strRemote.c_str(), Remote.m_length);
	pos += Remote.m_length;

	memcpy(pos, &Port, sizeof(Port));
	pos += sizeof(Port);
	memcpy(pos, &rc.nPort, sizeof(rc.nPort));
	pos += Port.m_length;

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::N_SetRemote(char* buf)
{
	REMOTE_CONF rc;

	KL *pKL = (KL*) buf;
	int len = pKL->m_length;
	int pos = sizeof(KL);
	int dns = 0;
	while(len > 0)
	{
		SL * sl = (SL*)(buf + pos);
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
		case REMOTE_DNS:
			if(dns == 0)
			{
				rc.strDns1 = tmp;
				dns++;
			}
			else
				rc.strDns2 = tmp;
			break;
		case  REMOTE_SERVER:
			rc.strRemote = tmp;
			break;
		case REMOTE_PORT:
			memset(&rc.nPort, 0, sizeof(rc.nPort));
			if(tmp.size() < 4)
				memcpy(&rc.nPort, tmp.c_str(), tmp.size());
			else
				memcpy(&rc.nPort, tmp.c_str(), sizeof(rc.nPort));
			break;
		}
	}
	pKL->m_length = 1;
	buf[sizeof(KL)] = 1;
	int setsize = sizeof(KL) + 1;

	bool res = Write(buf, setsize, setsize);
	NetOperation no;
	no.SetRemoteConfig(rc);
	return res;
}

bool GuiThread::M_UpdateProgramList_HDD(char* buf)          //UpdateProgramListˢӲб���Ա? 
{
	KL* pKL = (KL*)buf;
	int sendsize = sizeof(KL) + 1;

	std::vector<int> srcList; 
	srcList.push_back(PST_HDD);
	m_Content->UpdateProgramList(srcList);

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::M_UpdateProgramList_RAID(char* buf)          //UpdateProgramListˢӲб���Ա? 
{
	KL* pKL = (KL*)buf;
	int sendsize = sizeof(KL) + 1;

	std::vector<int> srcList; 
	srcList.push_back(PST_RAID);
	m_Content->UpdateProgramList(srcList);

	return Write(buf, sendsize, sendsize);
}


bool GuiThread::M_UpdateProgramList_USB(char* buf)          //UpdateProgramListˢӲб���Ա? 
{
    KL* pKL = (KL*)buf;
	int sendsize = sizeof(KL) + 1;

	std::vector<int> srcList; 
	srcList.push_back(PST_USB);
	m_Content->UpdateProgramList(srcList);

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::M_IsProgramListReady_HDD(char* buf)         //IsProgramListReadyѯǷ׼
{
    KL* pKL = (KL*)buf;
	int sendsize = sizeof(KL) + sizeof(bool);
    bool ret=m_Content->IsProgramListReady(PST_HDD);
	void* pos = buf + sizeof(KL);
	memcpy(pos,&ret,sizeof(bool));               //Ƿ׼õ״̬,صͻ

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::M_IsProgramListReady_RAID(char* buf)         //IsProgramListReadyѯǷ׼
{
	KL* pKL = (KL*)buf;
	int sendsize = sizeof(KL) + sizeof(bool);
	bool ret=m_Content->IsProgramListReady(PST_RAID);
	void* pos = buf + sizeof(KL);
	memcpy(pos,&ret,sizeof(bool));               //Ƿ׼õ״̬,صͻ

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::M_IsProgramListReady_USB(char* buf)         //IsProgramListReadyѯǷ׼
{
    KL* pKL = (KL*)buf;
	int sendsize = sizeof(KL) + sizeof(bool);
    bool ret=m_Content->IsProgramListReady(PST_USB);
	void* pos = buf + sizeof(KL);
	memcpy(pos,&ret,sizeof(bool));               //Ƿ׼õ״̬,صͻ

	return Write(buf, sendsize, sendsize);
}



bool GuiThread::M_GetContent_HDD(char* buf)
{
	printf("run to M_GetContent_HDD-start()\n");
	SL item;
/*
	vector<int> src; src.push_back(PST_HDD);
	m->update...
		m->isReady
		getcontent
*/

	std::vector<InfoData> m_list = m_Content->GetContentList(PST_HDD);
	uint64 ss = m_list.size() * sizeof(SL) * 16;

	DPRINTF("HDD:m_list.size()=%d\n",m_list.size());

	DPRINTF("HDD:ss---------->%lld\n", ss);
	
	std::vector<InfoData>::iterator itor;
	InfoData info;
	for (itor = m_list.begin(); itor != m_list.end(); ++itor)
	{
		info = *itor;
		for (int i = 0; i < 15; i++)
		{
		//	DPRINTF("HDD:%s\n", info.pData[i].c_str());
			ss += info.pData[i].size();
		}
	}

	ss += sizeof(KL) + 100;

	DPRINTF("HDD:mallocsize---------->%lld\n", ss);
	//char *buff =new char[ss];
	char *buff =NULL;
	buff=new char[ss];             //new
    if(buff==NULL) 
	{   
		printf("malloc error ");
		printf("FILE: %s,LINE: %d DATE:%s %s\n" ,__FILE__, __LINE__,__DATE__,__TIME__);
		return false;
	}


	KL *pKL = (KL*)buff;
	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = M_GET_HDD_CONTENT_LIST;

	void* pos = buff + sizeof(KL);

	for (itor = m_list.begin(); itor != m_list.end(); ++itor)
	{
		info = *itor;

		item.m_sID = CONTENT_ID;
		item.m_length = info.pData[0].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[0].c_str(), item.m_length);
		pos += item.m_length;
		DPRINTF("HDD:ID:%s %lld\n", info.pData[0].c_str(), item.m_length);

		item.m_sID = CONTENT_NAME;
		item.m_length = info.pData[1].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[1].c_str(), item.m_length);
		pos += item.m_length;
		DPRINTF("HDD:NAME:%s %lld\n", info.pData[1].c_str(), item.m_length);

		item.m_sID = CONTENT_PROGRESS;
		item.m_length = info.pData[2].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[2].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[2].c_str(), item.m_length);

		item.m_sID = CONTENT_STATUS;
		item.m_length = info.pData[3].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[3].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[3].c_str(), item.m_length);

		item.m_sID = CONTENT_FORMAT;
		item.m_length = info.pData[4].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[4].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[4].c_str(), item.m_length);

		item.m_sID = CONTENT_FILMLENGTH;
		item.m_length = info.pData[5].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[5].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[5].c_str(), item.m_length);

		item.m_sID = CONTENT_PROGRAMLENGTH;
		item.m_length = info.pData[6].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[6].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[6].c_str(), item.m_length);

		item.m_sID = CONTENT_STEREOSCOPIC;
		item.m_length = info.pData[7].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[7].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[7].c_str(), item.m_length);

		item.m_sID = CONTENT_ISSUER;
		item.m_length = info.pData[8].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[8].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[8].c_str(), item.m_length);

		item.m_sID = CONTENT_ISSUEDATE;
		item.m_length = info.pData[9].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[9].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[9].c_str(), item.m_length);

		item.m_sID = CONTENT_TIMERANGE;
		item.m_length = info.pData[10].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[10].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[10].c_str(), item.m_length);

		item.m_sID = CONTENT_RECVSEGMENT;
		item.m_length = info.pData[11].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[11].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[11].c_str(), item.m_length);

		item.m_sID = CONTENT_TOTALSEGMENT;
		item.m_length = info.pData[12].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[12].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[12].c_str(), item.m_length);

		item.m_sID = CONTENT_RECV_DATETIME;
		item.m_length = info.pData[13].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[13].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[13].c_str(), item.m_length);


		item.m_sID = CONTENT_LOCATE;
		item.m_length = info.pData[14].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[14].c_str(), item.m_length);
		pos += item.m_length;
		DPRINTF("HDD:PATH:%s %lld\n", info.pData[14].c_str(), item.m_length);
		
	}
	DPRINTF("HDD:pos %d ---------->buf %d\n", pos, buff);
	int sendsize = (int)((char*)pos - buff);
	pKL->m_length = sendsize - sizeof(KL);
	DPRINTF("HDD:sendsize---------->%d\n", sendsize);

	bool res = Write(buff, sendsize, sendsize);

	delete[] buff;

	printf("run to M_GetContent_HDD-end()  res=\n%d",res);

	return res;
}

bool GuiThread::M_GetContent_RAID(char* buf)
{
	SL item;

	std::vector<InfoData> m_list = m_Content->GetContentList(PST_RAID);
	uint64 ss = m_list.size() * sizeof(SL) * 16;

	std::vector<InfoData>::iterator itor;
	InfoData info;
	for (itor = m_list.begin(); itor != m_list.end(); ++itor)
	{
		info = *itor;
		for (int i = 0; i < 15; i++)
		{
			ss += info.pData[i].size();
		}
	}

	ss += sizeof(KL) + 100;

	//char *buff =new char[ss];
	char *buff =NULL;
	buff=new char[ss];             //new
    if(buff==NULL) 
	{   
		if(gLog)
			gLog->Write(LOG_ERROR, "[GuiTHread] M_GetContent_HDD: alloc memory error.");
		return false;
	}


	KL *pKL = (KL*)buff;
	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = M_GET_RAID_CONTENT_LIST;

	void* pos = buff + sizeof(KL);

	for (itor = m_list.begin(); itor != m_list.end(); ++itor)
	{
		info = *itor;

		item.m_sID = CONTENT_ID;
		item.m_length = info.pData[0].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[0].c_str(), item.m_length);
		pos += item.m_length;
		DPRINTF("HDD:ID:%s %lld\n", info.pData[0].c_str(), item.m_length);

		item.m_sID = CONTENT_NAME;
		item.m_length = info.pData[1].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[1].c_str(), item.m_length);
		pos += item.m_length;
		DPRINTF("HDD:NAME:%s %lld\n", info.pData[1].c_str(), item.m_length);

		item.m_sID = CONTENT_PROGRESS;
		item.m_length = info.pData[2].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[2].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_STATUS;
		item.m_length = info.pData[3].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[3].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_FORMAT;
		item.m_length = info.pData[4].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[4].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_FILMLENGTH;
		item.m_length = info.pData[5].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[5].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_PROGRAMLENGTH;
		item.m_length = info.pData[6].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[6].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_STEREOSCOPIC;
		item.m_length = info.pData[7].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[7].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_ISSUER;
		item.m_length = info.pData[8].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[8].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_ISSUEDATE;
		item.m_length = info.pData[9].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[9].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_TIMERANGE;
		item.m_length = info.pData[10].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[10].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_RECVSEGMENT;
		item.m_length = info.pData[11].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[11].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_TOTALSEGMENT;
		item.m_length = info.pData[12].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[12].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_RECV_DATETIME;
		item.m_length = info.pData[13].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[13].c_str(), item.m_length);
		pos += item.m_length;

		item.m_sID = CONTENT_LOCATE;
		item.m_length = info.pData[14].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[14].c_str(), item.m_length);
		pos += item.m_length;
		
	}
	int sendsize = (int)((char*)pos - buff);
	pKL->m_length = sendsize - sizeof(KL);

	bool res = Write(buff, sendsize, sendsize);

	delete[] buff;

	return res;
}

bool GuiThread::M_GetContent_USB(char* buf)
{
	printf("run to M_GetContent_USB-start()\n");

	SL item;
	std::vector<InfoData> m_list = m_Content->GetContentList(PST_USB);
	//int ss = m_list.size() * sizeof(SL);

	DPRINTF("USB:m_list.size()=%d\n",m_list.size());

	uint64  ss = m_list.size() * sizeof(SL)*16;
	DPRINTF("USB:ss---------->%lld\n", ss);
	
	std::vector<InfoData>::iterator itor;
	InfoData info;
	for (itor = m_list.begin(); itor != m_list.end(); ++itor)
	{
		info = *itor;
		for (int i = 0; i < 15; i++)
		{
		//	DPRINTF("USB:%s\n", info.pData[i].c_str());
			ss += info.pData[i].size();
		}
	}


	ss += sizeof(KL) + 100;

	DPRINTF("USB:mallocsize---------->%lld\n", ss);
	//char *buff = new char[ss];
	char *buff =NULL;
	buff=new char[ss];             //new
    if(buff==NULL) 
	{   
		printf("malloc error ");
		printf("FILE: %s,LINE: %d DATE:%s %s\n" ,__FILE__, __LINE__,__DATE__,__TIME__);
		return false;
	}

	KL *pKL = (KL*)buff;
	pKL->m_pkgHead = 0x7585;
	pKL->m_keyID = M_GET_USB_CONTENT_LIST;

	void* pos = buff + sizeof(KL);

	for (itor = m_list.begin(); itor != m_list.end(); ++itor)
	{
		info = *itor;

		item.m_sID = CONTENT_ID;
		item.m_length = info.pData[0].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[0].c_str(), item.m_length);
		pos += item.m_length;
		DPRINTF("USB:ID:%s %lld\n", info.pData[0].c_str(), item.m_length);



		item.m_sID = CONTENT_NAME;
		item.m_length = info.pData[1].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[1].c_str(), item.m_length);
		pos += item.m_length;
		DPRINTF("USB:NAME:%s %lld\n", info.pData[1].c_str(), item.m_length);

		item.m_sID = CONTENT_PROGRESS;
		item.m_length = info.pData[2].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[2].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[2].c_str(), item.m_length);

		item.m_sID = CONTENT_STATUS;
		item.m_length = info.pData[3].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[3].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[3].c_str(), item.m_length);

		item.m_sID = CONTENT_FORMAT;
		item.m_length = info.pData[4].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[4].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[4].c_str(), item.m_length);



		item.m_sID = CONTENT_FILMLENGTH;
		item.m_length = info.pData[5].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[5].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[5].c_str(), item.m_length);

		item.m_sID = CONTENT_PROGRAMLENGTH;
		item.m_length = info.pData[6].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[6].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[6].c_str(), item.m_length);

		item.m_sID = CONTENT_STEREOSCOPIC;
		item.m_length = info.pData[7].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[7].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[7].c_str(), item.m_length);

		item.m_sID = CONTENT_ISSUER;
		item.m_length = info.pData[8].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[8].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[8].c_str(), item.m_length);

		item.m_sID = CONTENT_ISSUEDATE;
		item.m_length = info.pData[9].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[9].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[9].c_str(), item.m_length);

		item.m_sID = CONTENT_TIMERANGE;
		item.m_length = info.pData[10].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[10].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[10].c_str(), item.m_length);

		item.m_sID = CONTENT_RECVSEGMENT;
		item.m_length = info.pData[11].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[11].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[11].c_str(), item.m_length);

		item.m_sID = CONTENT_TOTALSEGMENT;
		item.m_length = info.pData[12].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[12].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[12].c_str(), item.m_length);

		item.m_sID = CONTENT_RECV_DATETIME;
		item.m_length = info.pData[13].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[13].c_str(), item.m_length);
		pos += item.m_length;
	//	DPRINTF("%s %d\n", info.pData[13].c_str(), item.m_length);


		item.m_sID = CONTENT_LOCATE;
		item.m_length = info.pData[14].size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		memcpy(pos, info.pData[14].c_str(), item.m_length);
		pos += item.m_length;
		DPRINTF("USB:PATH:%s %lld\n", info.pData[14].c_str(), item.m_length);

	}


	DPRINTF("USB:pos %d ---------->buf %d\n", pos, buff);

	int sendsize = (int)((char*)pos - buff);
	pKL->m_length = sendsize - sizeof(KL);

	DPRINTF("USB:sendsize---------->%d\n", sendsize);

	bool res = Write(buff, sendsize, sendsize);
	delete[] buff;

	printf("run to M_GetContent_USB-end()  res=\n%d",res);

	return res;
	
}




bool GuiThread::M_GetDiskInfo_HDD(char* buf)
{
	DISK_INFO di;
	di.nAvali = m_Content->GetAvalibleSpace(PST_HDD);
	di.nTotal = m_Content->GetTotalSpace(PST_HDD);

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);

	memcpy(pos, &di, sizeof(DISK_INFO));
	pKL->m_length = sizeof(DISK_INFO);

	int sendsize = sizeof(KL) + pKL->m_length;

	printf("run to M_GetDiskInfo_HDD()\n");

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::M_GetDiskInfo_RAID(char* buf)
{
	DISK_INFO di;
	di.nAvali = m_Content->GetAvalibleSpace(PST_RAID);
	di.nTotal = m_Content->GetTotalSpace(PST_RAID);

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);

	memcpy(pos, &di, sizeof(DISK_INFO));
	pKL->m_length = sizeof(DISK_INFO);

	int sendsize = sizeof(KL) + pKL->m_length;

	printf("run to M_GetDiskInfo_HDD()\n");

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::M_GetDiskInfo_USB(char* buf)
{
	printf("run to M_GetDiskInfo_USB()-start\n");

    DISK_INFO di;
	di.nAvali = m_Content->GetAvalibleSpace(PST_USB);
	di.nTotal = m_Content->GetTotalSpace(PST_USB);
	//di.nAvali = 1024*1024*40;    //ΪֵݵͨԳɹ
	//di.nTotal = 1024*1024*100;

	printf("USB:di.nTotal=%d GB,di.nAvali=%d GB\n",di.nTotal/1024/1024/1024,di.nAvali/1024/1024/1024);

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);
	memcpy(pos, &di, sizeof(DISK_INFO));
	pKL->m_length = sizeof(DISK_INFO);

	int sendsize = sizeof(KL) + pKL->m_length;

	printf("run to M_GetDiskInfo_USB()-end\n");

	return Write(buf, sendsize, sendsize);
	

}



bool GuiThread::S_Reboot(char* buf)
{
	System s;
	KL *pKL = (KL*)buf;
	int sendsize = sizeof(KL) + 1;
	bool res = Write(buf, sendsize, sendsize);
	char str[512];
	sprintf(str, "[GuiThread] Receive Shutdown from UI");
	if (gLog)
	{
		gLog->Write(LOG_SYSTEM, str);
	}
	s.Reboot();
	return res;
}

bool GuiThread::S_Shutdown(char* buf)
{
	System s;
	KL *pKL = (KL*)buf;
	int sendsize = sizeof(KL) + 1;
	bool res = Write(buf, sendsize, sendsize);
	char str[512];
	sprintf(str, "[GuiThread] Receive Shutdown from UI");
	if (gLog)
	{
		gLog->Write(LOG_SYSTEM, str);
	}
	s.Shutdown();
	return res;
}

bool GuiThread::S_FormatDisk(char* buf)
{
	KL* pKL = (KL*)buf;
	int sendsize = sizeof(KL) + 1;
	m_mkfs->FormatDisk(DISK_REMOVEABLE);
	return Write(buf, sendsize, sendsize);
}

bool GuiThread::S_FormatResult(char* buf)
{
	KL* pKL = (KL*)buf;

	std::string s = m_mkfs->GetSOutput();
	pKL->m_length = s.size();
	void* pos = buf + sizeof(KL);
	memcpy(pos, s.c_str(), pKL->m_length);
	int sendsize = sizeof(KL) + pKL->m_length;

	return Write(buf, sendsize, sendsize);
}

bool GuiThread::S_FormatStatus(char* buf)
{
	KL* pKL = (KL*)buf;

	uint8 c = m_mkfs->CheckStatus();
	pKL->m_length = sizeof(uint8);
	buf[sizeof(KL)] = c;
	int sendsize = sizeof(KL) + pKL->m_length;

	return Write(buf, sendsize, sendsize);
}


//new
bool GuiThread::S_USB_Mount(char* buf)
{
    USB usb;

	KL* pKL = (KL*)buf;
	int sendsize = sizeof(KL) + 1;
	usb.USB_Mount();
	return Write(buf, sendsize, sendsize);

}

bool GuiThread::S_USB_UnMount(char* buf)
{
	USB usb;

	KL* pKL = (KL*)buf;
	int sendsize = sizeof(KL) + 1;

	usb.USB_UnMount();

	return Write(buf, sendsize, sendsize);
}



bool GuiThread::M_DeleteDir(char* buf)
{
	KL* pKL = (KL*)buf;
    char path_del[1024];                     //ɾ·
	int sizepath =pKL->m_length;
	int sendsize = sizeof(KL) + sizepath;   //sizeof(path_del)Ҳ
	void* pos = buf + sizeof(KL);
    memcpy(path_del,pos,sizeof(path_del));
	printf("path_del:=%s \n",path_del);   //ȡͻ˴Ĵɾ·
	
    //²ԷŵBaseOperation
    char str_cmd[256]="rm -rf ";    //ǿɾ
    strcat(str_cmd,path_del);       //ƴӳϵͳ
    system(str_cmd);                 //system("umount /media/usb"); 

    printf("delete path:=%s \n",path_del);   

	return Write(buf, sendsize, sendsize);
}



//#include "ThreadCopyDir.h"   //ļ߳
int complete_percent=-1;   //ɰٷֱ
int copy_flag=0;          //״̬

bool GuiThread::M_CopyDir_HDD_TO_USB(char* buf)
{

	printf("run to M_CopyDir_HDD_TO_USB\n");
  //  tocopy_path path;     //·  
    copy_path path;

	KL* pKL = (KL*)buf;
	int sizepath =pKL->m_length;


	int sendsize = sizeof(KL) + sizepath;
	void* pos = buf + sizeof(KL);

    memcpy(&path,pos,sizepath);
   // printf("path_dst=%s  path_src=%s\n",path.path_dst.c_str(),path.path_src.c_str());
	printf("path_dst:=%s  path_src:=%s\n",&(path.path_dst[0]),&(path.path_src[0]));
	
   //ѾȡԴĿ·ַ
    char* path_src=&(path.path_src[0]);
	char* path_dst=&(path.path_dst[0]);

    //ÿ߳
   // copy_dir(path_dst,path_src,&Complete_percent,&Copy_flags);
   // CopyThread*  
	if(copyThread!=NULL) {delete copyThread;}
	copyThread=new CopyThread;
    
     copyThread->Init();   //

    //ݲ߳
    //copyThread->path_src=path_src;
    //copyThread->path_dst=path_dst;
	strcpy(copyThread->path_src,path_src);
	strcpy(copyThread->path_dst,path_dst);
    
	copyThread->complete_percent=&complete_percent;
    copyThread->copy_flag=&copy_flag;

	if (copyThread->status() == brunt::thread_ready)
	{
		copyThread->start();
		printf("Start CopyThread in GuiThread\n");
	}
	//delete copyThread;
	//copyThread = NULL;

	return Write(buf, sendsize, sendsize);
}


bool GuiThread::setCopyProgress(char* buf)    //ؿ
{
	KL* pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);
	memcpy(pos,&complete_percent,sizeof(int));   //ȫֱΪ

    //pos=pos+sizeof(int);
	void* pos2= buf + sizeof(KL)+sizeof(int);
    memcpy(pos2,&copy_flag,sizeof(int));   //俽״̬?

	int sendsize = sizeof(KL)+ sizeof(int)+sizeof(int);
	return Write(buf, sendsize, sendsize);
}




//#include <log/Log.h>
//#include <log/LogStruct.h>

#include "../../include/log/Log.h"
#include "../../include/log/LogStruct.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h> 
#include <unistd.h>


//TMS  log־
bool GuiThread::S_Get_TMS(char* buf)
{
#if 0
    ILog * plog = CreateLog();
	printf("plog=%d\n",plog);

	LOGDATETIME timeAfter, timeBefore;
	timeAfter.year =2015;
	timeAfter.month =11;
	timeAfter.day =26;

	timeBefore.year =2015;
	timeBefore.month =11;
	timeBefore.day =26;

	TLogQueryResultArray ra;

	int ret;
	ret=plog->Query(LOG_TMS, &timeAfter, &timeBefore,ra);

	printf("ret=%d\n",ret);
	printf("ra.size()=%d\n",ra.size());


    extern std::string str_LOG;//NEW
	printf("Service:str_LOG:%s",str_LOG.c_str());   


	/*
	for(int i = 0; i < ra.size(); i++)
	{
		printf("ra:%s %s", ra[i].time.c_str(), ra[i].text.c_str());   
	    printf("ra.size()=%d\n",ra.size());
	}
	*/

#else
    extern std::string str_LOG;   //ȫַ
	KL* pKL = (KL*)buf;
    LOGDATE LogData;       
	void* pos = buf + sizeof(KL);
    memcpy(&LogData,pos,sizeof(LOGDATE));   //uiݹ

    ILog * plog = CreateLog();
	LOGDATETIME timeAfter, timeBefore;

	timeAfter.year = LogData.after_year;
	timeAfter.month = LogData.after_month;
	timeAfter.day = LogData.after_day;

	timeBefore.year = LogData.before_year;
	timeBefore.month = LogData.before_month;
	timeBefore.day = LogData.before_day;

/*
	timeAfter.year = 2015;
	timeAfter.month = 11;
	timeAfter.day = 25;

	timeBefore.year = 2015;
	timeBefore.month = 11;
	timeBefore.day = 27;
*/

	TLogQueryResultArray ra;
 // std::string  str_log="";

//     printf("After.year=%d After.month=%d After.day=%d\n",timeAfter.year,timeAfter.month,timeAfter.day);
//     printf("Before.year=%d Before.month=%d Before.day=%d\n",timeBefore.year,timeBefore.month,timeBefore.day);
	plog->Query(LOG_TMS, &timeAfter, &timeBefore, ra);
	for(int i = 0; i < ra.size(); i++)
	{
	 //	  printf("%s %s", ra[i].time.c_str(), ra[i].text.c_str());
	 //   str_log+=ra[i].time;
     //   str_log+=ra[i].text;
	}

  //  std::string  str_log="";
  //  str_log+="log1234567890";

	int str_size=str_LOG.size()+1;
	int sendsize = sizeof(KL) + str_size;
	void* pos2 = buf + sizeof(KL);
    memcpy(pos2,str_LOG.c_str(),str_size);
    pKL->m_length =str_size;           //䷵ص־?
// 	printf("str_LOG:\n%s",str_LOG.c_str());   

	ReleaseLog(plog);
#endif

	return Write(buf, sendsize, sendsize);

}

bool GuiThread::R_GetRaidInfo(char* buf)
{
	RaidDetailParser rdp;
	rdp.RunRaidManager();
	
	RAID_INFO ri;
	
	ri.nRaidDevices = rdp.GetRaidDevices();
	ri.nActiveDevices = rdp.GetActiveDevices();
	ri.nWorkingDevices = rdp.GetWorkingDevices();
	ri.nFailedDevices = rdp.GetFailedDevices();
	ri.nArraySize = rdp.GetArraySize();
	ri.nUsedSize = rdp.GetUsedSize();
	ri.strLevel = rdp.GetRaidLevel();
	ri.strState = rdp.GetState();
	ri.strDevState = rdp.GetDevicesState();

	KL *pKL = (KL*)buf;

	void* pos = buf + sizeof(KL);

	memcpy(pos, &ri, sizeof(uint16)*4 + sizeof(uint64)*2);
	pos += sizeof(uint16)*4 + sizeof(uint64)*2;

// 	int sendsize = sizeof(KL) + pKL->m_length;

	SL item;

	item.m_sID = RAID_INFO_LEVEL;
	item.m_length = ri.strLevel.size();
	memcpy(pos, &item, sizeof(SL));
	pos += sizeof(SL);
	if(item.m_length > 0)
	{
		memcpy(pos, ri.strLevel.c_str(), item.m_length);
		pos += item.m_length;
	}
	
	item.m_sID = RAID_INFO_STATE;
	item.m_length = ri.strState.size();
	memcpy(pos, &item, sizeof(SL));
	pos += sizeof(SL);
	if (item.m_length > 0)
	{
		memcpy(pos, ri.strState.c_str(), item.m_length);
		pos += item.m_length;
	}

	for(int i = 0; i < ri.strDevState.size(); i++)
	{
		item.m_sID = RAID_INFO_DEVSTATE;
		item.m_length = ri.strDevState.at(i).size();
		memcpy(pos, &item, sizeof(SL));
		pos += sizeof(SL);
		if (item.m_length > 0)
		{
			memcpy(pos, ri.strDevState.at(i).c_str(), item.m_length);
			pos += item.m_length;
		}
	}

	int sendsize = (int)((char*)pos - buf);
	pKL->m_length = sendsize - sizeof(KL);

	return Write(buf, sendsize, sendsize);
}