#include "baseoperation.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
#include <QSettings>
#include <QString>
#include <QtGlobal>

//自定义打印16进制函数,调试常用
void print_hex(char* buf, int len) 
{
    //Print results:
    for(int i=0;i<len;i++) 
	{
        printf("%02X ",(unsigned char)buf[i]);
        if(15 == i%16)
            printf("\n");
    }
    printf("\n");
}




int getMachine_ID()                           
{
    #ifdef Q_OS_WIN32
           QSettings settings("ID", QSettings::IniFormat);
    #endif
    #ifdef Q_OS_LINUX
           QSettings settings("/etc/CineCast/ID", QSettings::IniFormat);
    #endif


    QString strGroupName="ID_HardKey";
   // settings.beginGroup("ServerIP1");
    settings.beginGroup(strGroupName);
    settings.setValue("ID","1000000000");     //
    //settings.setValue("Port",8080);   //
    settings.setValue("HardKey","KeyNO.1000000END");
    settings.endGroup();
    settings.sync();


   QString strID=settings.value("ID_HardKey/ID","").toString();
   bool ok;
   unsigned int ID= strID.toInt(&ok,10);    
  /*
   QString strHardKey=settings.value("ID_HardKey/HardKey","").toString();
   char *pIPAddr=strHardKey.toLocal8Bit().data();;   
  //printf("IPAddr=%s\n",pIPAddr);
   strcpy(IPAddr,pIPAddr);
   */
   printf("getMachine_ID()=%d\n",ID);
   return ID;
}


void getHardKey(char HardKey[16])              //获取当前接收机的硬件密钥
{

    #ifdef Q_OS_WIN32
           QSettings settings("ID", QSettings::IniFormat);
    #endif
    #ifdef Q_OS_LINUX
           QSettings settings("/etc/CineCast/ID", QSettings::IniFormat);
    #endif


    QString strGroupName="ID_HardKey";
    // settings.beginGroup("ServerIP1");
    settings.beginGroup(strGroupName);
    settings.setValue("ID","1000000000");     //
    //settings.setValue("Port",8080);   //
    settings.setValue("HardKey","KeyNO.1000000END");
    settings.endGroup();
    settings.sync();


    QString strHardKey=settings.value("ID_HardKey/HardKey","").toString();
   // char *pHardKey=strHardKey.toLocal8Bit().data();
	QByteArray ba = strHardKey.toLatin1();  

    //printf("IPAddr=%s\n",pIPAddr);
    //strcpy(HardKey,pHardKey);
	memcpy(HardKey,ba.data(),16);

    printf("getHardKey()=");
	for(int i=0;i<16;i++)
	{  
		printf("%02X ",(unsigned char)HardKey[i]);
	}
		printf("\n");


}



//
int getLoginCounter()                           
{
    return 999;
}


void getSoftVersion(char SoftVersion[16])     //
{
        #ifdef Q_OS_WIN32
               QSettings settings("Version", QSettings::IniFormat);
        #endif
        #ifdef Q_OS_LINUX
               QSettings settings("/etc/CineCast/Version", QSettings::IniFormat);
        #endif

       // 鍐欏叆ini閰嶇疆
       // QString strGroupName="Version";
       // settings.beginGroup("ServerIP1");
       // settings.beginGroup(strGroupName);
        settings.setValue("Version","20150225DD900");  //
        //settings.setValue("Port",8080);   //
        //settings.endGroup();
        settings.sync();


        QString strVersion=settings.value("Version","").toString();

        //char *ptrVersion=strVersion.toLocal8Bit().data();;    //QString杞琧har瀛楃涓?
        //printf("IPAddr=%s\n",pIPAddr);
        //strcpy(IPAddr,pIPAddr);
        //Qstring  str;
       //char*  ch;
        QByteArray ba = strVersion.toLatin1();
       // ch=ba.data();
        int length=ba.size();
        if(length>16)
        {
            printf("Version too long\n");
            return;
        }
       strcpy(SoftVersion,ba.data());
       // SoftVersion[15]='\0';
       //memcpy(SoftVersion,ba.data(),16);

       printf("getSoftVersion=%s\n",SoftVersion);
}


#ifdef Q_OS_LINUX
#include <stdio.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <errno.h>
#endif

void getTime_Startup(char Time_Startup[20])
{

#ifdef Q_OS_LINUX


#include <stdio.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <errno.h>

    struct sysinfo info;
    time_t cur_time = 0;
    time_t boot_time = 0;
    struct tm *ptm = NULL;
    if (sysinfo(&info)) {
    fprintf(stderr, "Failed to get sysinfo, errno:%u, reason:%s\n",
        errno, strerror(errno));
    return;
    }
    time(&cur_time);
    if (cur_time > info.uptime) {
    boot_time = cur_time - info.uptime;
    }
    else {
    boot_time = info.uptime - cur_time;
    }
    ptm = gmtime(&boot_time);

    printf("System boot time: %d-%-d-%d %d:%d:%d\n", ptm->tm_year + 1900,
        ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
  // return 0;

    QString stryear=QString::number(ptm->tm_year + 1900);

    QString strmon;
    if(ptm->tm_mon<9)
         strmon="0"+QString::number(ptm->tm_mon + 1);
    else
         strmon=QString::number(ptm->tm_mon + 1);

    QString strmday;
    if(ptm->tm_mday<10)
         strmday="0"+QString::number(ptm->tm_mday);
    else
         strmday=QString::number(ptm->tm_mday);

    QString strhour;
    if(ptm->tm_hour<10)
         strhour="0"+QString::number(ptm->tm_hour);
    else
         strhour=QString::number(ptm->tm_hour);

    QString strmin;
    if(ptm->tm_min<10)
         strmin="0"+QString::number(ptm->tm_min);
    else
         strmin=QString::number(ptm->tm_min);

    QString strsec;
    if(ptm->tm_sec<10)
         strsec="0"+QString::number(ptm->tm_sec);
    else
         strsec=QString::number(ptm->tm_sec);



   QString strBootTime=stryear+"-"+strmon+"-"+strmday+" "+strhour+":"+strmin+":"+strsec;

   char* ch=strBootTime.toLocal8Bit().data();
   printf("BootTime:%s\n",ch);//

   memcpy(Time_Startup,ch,20);   


#endif

#ifdef Q_OS_WIN32
   char tmp[20]="2016-11-11 12:12:12";   
   strcpy(Time_Startup,tmp);
#endif

   Time_Startup[19]='\0';
   printf("Time_Startup:%s\n",Time_Startup);
}

int getHardDiskNum()                          
{
    int nDiskCount = 0;
    
#ifdef Q_OS_LINUX

FILE* f=fopen("/proc/partitions","r");

if(!f)
{
//fprintf(stderr,"fopen /proc/partitions failed!errno=b%d\n",errno);
return 0;
}

char line[100], ptname[100], devname[120], *s;
int ma,mi,sz;

while(fgets(line,sizeof(line),f))
{
if(sscanf(line," %u %u %u %[^\n ]",&ma,&mi,&sz,ptname)!=4)
continue;
for (s = ptname; *s; s++)
continue;
/* note: excluding '0': e.g. mmcblk0 is not a partition name! */
if (s[-1] >= '1' && s[-1] <= '9')
continue;
nDiskCount++;
}
fclose(f);
#endif
    printf("DiskCount=%d\n",nDiskCount);
    return nDiskCount;
}


#ifdef Q_OS_LINUX
#include <sys/vfs.h>
#endif


unsigned long long getAllDiskCapacity()       
{
#ifdef Q_OS_LINUX

    struct statfs diskInfo;
    char *path="/storage";      
    statfs(path,&diskInfo);     

    unsigned long long blocksize = diskInfo.f_bsize; 
    unsigned long long totalsize = blocksize*diskInfo.f_blocks; 
    printf("HDD_Disk: Total_size = %llu B = %llu KB = %llu MB = %llu GB\n",
        totalsize, totalsize>>10, totalsize>>20, totalsize>>30);

    unsigned long long freeDisk = diskInfo.f_bfree * blocksize; 
    unsigned long long availableDisk = diskInfo.f_bavail * blocksize; 
    printf("HDD_Disk: Disk_free = %llu MB = %llu GB\nDisk_available = %llu MB = %llu GB\n",
        freeDisk>>20, freeDisk>>30, availableDisk>>20, availableDisk>>30);

#endif

#ifdef Q_OS_WIN32
    unsigned long long totalsize=0;
#endif
    printf("totalsize=%d\n",totalsize);
    return totalsize;
}


int compareVersion(char OldVersion[16],char UpdateSerialNo[16])                           //姣旇緝鐜板湪鐗堟湰鍙峰拰鎺ユ敹鍒扮殑鐗堟湰鍙?
{
    int ret=1;
    for(int i=0;i<15;i++)
    {
        if(UpdateSerialNo[i]=='\0'||OldVersion[i]=='\0')
         {
            break;
         }

        if(OldVersion[i]<UpdateSerialNo[i])
        {
            printf("%d ",i);
            ret=0;
            break;
        }
    }

    if(0==ret)
    {
        printf("升级包有效\n");
    }
    else
    {
        printf("升级包无效\n");
    }

     return ret;
}



void getSerialNo(char SerialNo[16])           
{

}


void getUpdateFileName(char name[256])    
{

}


void getinstall_sh(char path[256])      
{

}

bool run_once = false;
void getMeetKey(char MeetKey[16])    
{
	static char key[16];
    //srand(time(0));
	if(!run_once)
	{
		srand(time((unsigned)NULL));
		char* pbuf=key;

		printf("sizeof(MeetKey)/sizeof(int)=%d\n",sizeof(MeetKey)/sizeof(int));

		for(int i=0;i<4;i++)
		{
			int tmp=rand();  
			memcpy(pbuf,&tmp,sizeof(int));
			pbuf=pbuf+sizeof(tmp);
		}
		printf("sizeof(MeetKey)=%d\n",sizeof(MeetKey));

		printf("MeetKey:");
		for(int i=0;i<16;i++)  
		{  
			printf("%02X ",(unsigned char)MeetKey[i]);
		}
		printf("\n");
		run_once = true;
	}
	memcpy(MeetKey, key, 16);
}


void getRandomCode(char RandomCode[16])    
{
    //srand(time(0));
    srand(time((unsigned)NULL));
    char* pbuf=RandomCode;
    for(int i=0;i<16;i++)
	{
        unsigned char tmp=rand()/256+i;
        memcpy(pbuf,&tmp,sizeof(unsigned char));
        pbuf=pbuf+sizeof(unsigned char);
	}

	printf("RandomCode:");
    for(int i=0;i<16;i++)
	{  
        printf("%02X ",(unsigned char)RandomCode[i]);
	}
	    printf("\n");
}












bool getHardKeyuseID(int ID,char HardKey[16])      //通过设备ID查找对应的硬件指纹
{
    static  QList<RegisterIDHARDKEY> ListRegisterIDHardKey;
    static int Numget=0;
	if(Numget==0)
	{
		getRegisterIDAndHardKey(ListRegisterIDHardKey);   //第一次使用初始化注册列表
	    Numget++;
	}
    
	    bool ret=false;
		for(int i=0;i<ListRegisterIDHardKey.count();i++)
	{
		RegisterIDHARDKEY RegisterIDHardKey=ListRegisterIDHardKey.at(i);
        if(RegisterIDHardKey.ID==ID)   //查找到对应ID
		{
		  //获取对应的硬件指纹
		   memcpy(HardKey,RegisterIDHardKey.HardKey,16); 
		   ret=true;     //成功标志
		   break;  
		}
	}
	return ret;

}


void getRegisterIDAndHardKey(QList<RegisterIDHARDKEY>& ListRegisterIDHardKey)
{
    printf("getRegisterIDAndHardKey\n");


    QSettings settings("ListRegisterIDHardKey.ini", QSettings::IniFormat);

#if 0
    int Num=10;
    settings.setValue("RegisterSum",Num);     


//第一次使用写入id和key
    for(int i=0;i<Num;i++)
    {
        QString strGroupName="ListRegisterIDHardKey"+QString::number(i,10);
        // settings.beginGroup("ServerIP1");
        settings.beginGroup(strGroupName);   
        //settings.setValue("ID","1000");             //
        //settings.setValue("Port",8080);         //
        //settings.setValue("HardKey","12345678");

        QString strID=QString::number(1000000000+i,10);
        QString strHardKey="KeyNO."+QString::number(1000000+i*i,10)+"END";    //保证是16位的
        settings.setValue("ID",strID);             //
        settings.setValue("HardKey",strHardKey);
        settings.endGroup();
        settings.sync();
    }
#endif // 0

   QString strSumNum=settings.value("RegisterSum","").toString();
   bool ok;
   unsigned int SumNum= strSumNum.toInt(&ok,10);    //得到注册用户数量

    for(int i=0;i<SumNum;i++)
    {
        QString strGroupName="ListRegisterIDHardKey"+QString::number(i,10);
        QString strchildNameID=strGroupName+"/ID";
        QString strchildNameHardKey=strGroupName+"/HardKey";
        
           QString strID=settings.value(strchildNameID,"").toString();
		   bool ok;
		   unsigned int ID= strID.toInt(&ok,10); 

		   QString strHardKey=settings.value(strchildNameHardKey,"").toString();
		   
		   if(strID==""||strHardKey=="")    //如果获取到的值为空
			   continue;

             
		   
		   char HardKey[32];
		   //char *ptmp=strHardKey.toLocal8Bit().data(); 
		   QByteArray ba = strHardKey.toLatin1();  
		   printf("strHardKey=%s\n",ba.data());

		   int length=strHardKey.size();
		   if(length<16)
		   {
			    printf("注册列表加载错误:HardKey.size()<16\n");
				return;
		   }
		   sscanf(strHardKey.toStdString().c_str(), "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", 
			   HardKey+0, HardKey+1, HardKey+2, HardKey+3, HardKey+4, HardKey+5, HardKey+6, HardKey+7,
			   HardKey+8, HardKey+9, HardKey+10, HardKey+11, HardKey+12, HardKey+13, HardKey+14, HardKey+15);

		   //以上已经得到ID和HardKey
           RegisterIDHARDKEY RegisterIDHardKey;
           RegisterIDHardKey.ID=ID;
		   memcpy(RegisterIDHardKey.HardKey,HardKey,16);
           
           //QList<RegisterIDHARDKEY>& ListRegisterIDHardKey
            ListRegisterIDHardKey.append(RegisterIDHardKey);   //添加进容器
		   //printf("IPAddr=%s\n",pIPAddr);
	}

    //获取完成后依次打印容器内容
	for(int i=0;i<ListRegisterIDHardKey.count();i++)
	{
		RegisterIDHARDKEY RegisterIDHardKey=ListRegisterIDHardKey.at(i);
        printf("ID=%d\n",RegisterIDHardKey.ID);
		printf("HardKey: ");
		for(int j=0;j<16;j++)
		{
			 printf("%02X ",(unsigned char)RegisterIDHardKey.HardKey[j]);
		}
		 printf("\n");
	}

	//因为可能ID号不连续,获取到之后重新生成新的ListRegisterIDHardKey.ini配置文件


}
