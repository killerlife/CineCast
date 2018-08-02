#ifdef WIN32
#include "stdafx.h"
#endif
#include "importThread.h"
#include <program/ProgramError.h>
#include "system.h"
//#include <ProgramParse.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <sleep.h>
#ifdef DEBUGPRINT
#include "brunt/debugprint.h"
#else
#define DP_Trace(arg,...)
#define DPRINT(arg,...) printf(arg)
#define DP_PrintS(arg,...)
#endif
//#include <sys/time.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include "config/config.h"	
using namespace fileiolib;
using namespace std;

#define BREAK_POINT_DELETE_FILE_SZIE  1024 * 1024

namespace fs = boost::filesystem;
using namespace brunt;
CImportThread::CImportThread() : m_progress(0), m_status(IMPORT_STATUS_READY), m_pObserver(NULL),m_adPath(""),m_adId("")
{
	m_nIsEnd = 0;
}

int CImportThread::init(const std::vector<filename>& filelist)
{
	if (filelist.size() <= 0)
	{
		return could_not_find_file_error;
	}

 	m_pfilelist = (std::vector<filename>*)&filelist;

	return no_error;
}

long long int CImportThread::progress()
{
	if(m_status != IMPORT_STATUS_READY)
		return m_progress;
	return -1;
}

void CImportThread::setAdPath(std::string& adPath,std::string& adId)
{
	m_adPath = adPath;
	m_adId    = adId;
}

void CImportThread::setProgress()
{
	//m_progress  = 0;			
}


int mytime(char* str)
{
	time_t timep;
	tm* p;
	time	(&timep);
	p = localtime(&timep);
	char buft[20] = {0};
	sprintf(buft,"%04d%02d%02d %02d:%02d:%02d",
		(1900+p->tm_year),(1+p->tm_mon),(p->tm_mday),
		p->tm_hour,p->tm_min,p->tm_sec);
	
//	DPRINT((DP_Trace, "CImportThread", "%s %s",str,buft));
	DPRINT("CImportThread %s %s\n",str,buft);

	return 0;
	//return atoi(buft);
}

#if 0
int CImportThread::importFile()
{
	std::vector<filename>::iterator ite;
	filename fn;
	m_nIsEnd = 0;

	for(ite = m_pfilelist->begin(); ite != m_pfilelist->end(); ++ite)
	{
		fn = *ite;
		m_src   = fn.m_src;
		m_dest = fn.m_dest;

		DP_PrintS("file src = %s",m_src.c_str());
		DP_PrintS("file m_dest = %s",m_dest.c_str());

		try
		{
			fs::path destPath(m_dest);
			fs::create_directories(destPath.branch_path());
		}
		catch (const fs::filesystem_error& ex)
		{
			std::string error = ex.what();
			return false;
		}

#ifdef WIN32
		HANDLE hSrcFile;
		hSrcFile = CreateFile(m_src.c_str(), GENERIC_READ, (DWORD)0, NULL, 
			                  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL); 

		if (hSrcFile == NULL)
		{
//			DPRINT((DP_Trace, "CImportThread", "Open source file %s failed!", m_src.c_str()));
			DPRINT("CImportThread Open source file %s failed!\n", m_src.c_str());
			return 0;
		}
//		DPRINT((DP_Trace, "CImportThread", "Open source file %s succeeded!", m_src.c_str()));
		DPRINT("CImportThread Open source file %s succeeded!\n", m_src.c_str());


		HANDLE hDestFile = NULL;
		hDestFile = CreateFile(m_dest.c_str(),  GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
							   OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL); 

		if (hDestFile == NULL)
		{
			DPRINT("CImportThread Open dest file %s failed!\n", m_dest.c_str());
//			DPRINT((DP_Trace, "CImportThread", "Open dest file %s failed!", m_dest.c_str()));
			CloseHandle(hDestFile);
			return 0;
		}
//		DPRINT((DP_Trace, "CImportThread", "Open dest file %s succeeded!", m_dest.c_str()));
		DPRINT("CImportThread Open dest file %s succeeded!\n", m_dest.c_str());

		LARGE_INTEGER   pSize;   
		GetFileSizeEx(hDestFile,&pSize);
		__int64 dwFileSize = pSize.QuadPart;
		__int64  lDistance = dwFileSize;
		if (dwFileSize <= BREAK_POINT_DELETE_FILE_SZIE)
		{
			lDistance = 0;
		}

		if (lDistance != 0)
		{
			DWORD dwPtr = SetFilePointerEx (hSrcFile, pSize, NULL, FILE_BEGIN) ; 
			dwPtr = SetFilePointerEx (hDestFile, pSize, NULL, FILE_BEGIN) ; 
		}
		m_progress	+= dwFileSize;

		DWORD nSize = 1024 * 1024;//64;// * 10;  // * 100;
		char * pBuf = new char[nSize];
		unsigned long nBytesRead,nLongW = 0;
		int n = 1;
//		DPRINT((DP_Trace, "CImportThread", "start copy"));
		DPRINT("start copy");
		mytime("开始");
		BOOL bResult = TRUE;
		while ((status() != thread_stoping) && (status() != thread_stopped))
		{			
			nBytesRead = 0;
			bResult = TRUE;
			bResult = ReadFile(hSrcFile,pBuf,nSize,&nBytesRead,NULL);
			mytime("结束");
			if (nBytesRead == 0) 
			{   // this is the end of the file 
				break;
			}
//			DPRINT((DP_Trace, "CtestfileDlg", "[%d]read size %d", n, nBytesRead));
			DPRINT("CImportThread [%d]read size %d\n", n, nBytesRead);

			BOOL nBytesWrite = WriteFile(hDestFile,pBuf,nBytesRead,&nLongW,NULL);
			if(!nBytesWrite)
			{
				break;
			}
			m_progress += nLongW;
//			DPRINT((DP_Trace, "CtestfileDlg", "[%d]write size %d", n, nLongW));
			DPRINT("CImportThread [%d]write size %d\n", n, nLongW);

			n++;
		}

		if(bResult)
//			DPRINT((DP_Trace, "CtestfileDlg", "end copy succeeded."));
			DPRINT("CImportThread end copy succeeded.\n");
		else
//			DPRINT((DP_Trace, "CtestfileDlg", "end copy error: "));
			DPRINT("CImportThread end copy error: \n");

		delete pBuf;

		CloseHandle(hSrcFile);
		CloseHandle(hDestFile);
#else
		FILE *fpSrc = NULL;
		fpSrc = fopen(m_src.c_str(), "rb");

		if (fpSrc == NULL)
		{
//			DPRINT((DP_Trace, "CImportThread", "Open source file %s failed!", m_src.c_str()));
			DPRINT("CImportThread Open source file %s failed!\n", m_src.c_str());
			return 0;
		}
//		DPRINT((DP_Trace, "CImportThread", "Open source file %s succeeded!", m_src.c_str()));
		DPRINT("CImportThread Open source file %s succeeded!\n", m_src.c_str());


		FILE *fpDest = NULL;
		fpDest = fopen(m_dest.c_str(),  "ab"); 

		if (fpDest == NULL)
		{
//			DPRINT((DP_Trace, "CImportThread", "Open dest file %s failed!", m_dest.c_str()));
			DPRINT("CImportThread Open dest file %s failed!\n", m_dest.c_str());
			fclose(fpSrc);
			return 0;
		}
//		DPRINT((DP_Trace, "CImportThread", "Open dest file %s succeeded!", m_dest.c_str()));
		DPRINT("CImportThread Open dest file %s succeeded!\n", m_dest.c_str());

		uint64_t   pSize;
		struct stat sbuf;
		stat(m_dest.c_str(), &sbuf);
		pSize = sbuf.st_size;
		uint64_t dwFileSize = sbuf.st_size;
		uint64_t lDistance = dwFileSize;
		if (dwFileSize <= BREAK_POINT_DELETE_FILE_SZIE)
		{
			lDistance = 0;
		}

		if (lDistance != 0)
		{
			fseek(fpSrc, pSize, SEEK_SET);
			fseek(fpDest, pSize, SEEK_SET);
		}
		m_progress	+= dwFileSize;

		uint32_t nSize = 1024 * 1024;//64;// * 10;  // * 100;
		char * pBuf = new char[nSize];
		unsigned long nBytesRead,nLongW = 0;
		int n = 1;
//		DPRINT((DP_Trace, "CImportThread", "start copy"));
		DPRINT("CImportThread start copy\n");
		mytime("start");
		BOOL bResult = TRUE;
		while ((status() != thread_stoping) && (status() != thread_stopped))
		{			
			nBytesRead = 0;
			bResult = TRUE;
			nBytesRead = fread(pBuf, nSize, 1, fpSrc);
			mytime("stop");
			if (nBytesRead == 0) 
			{   // this is the end of the file 
				break;
			}
//			DPRINT((DP_Trace, "CtestfileDlg", "[%d]read size %d", n, nBytesRead));
			DPRINT("CImportThread [%d]read size %d\n", n, nBytesRead);

			uint32_t nBytesWrite = fwrite(pBuf, nBytesRead, 1, fpDest); //WriteFile(hDestFile,pBuf,nBytesRead,&nLongW,NULL);
			if(nBytesWrite == 0)
			{
				break;
			}
			m_progress += nBytesWrite;
//			DPRINT((DP_Trace, "CtestfileDlg", "[%d]write size %d", n, nBytesWrite));
			DPRINT("CImportThread [%d]write size %d\n", n, nBytesWrite);

			n++;
		}

		if(bResult)
//			DPRINT((DP_Trace, "CtestfileDlg", "end copy succeeded."));
			DPRINT("CImportThread end copy succeeded.\n");
		else
//			DPRINT((DP_Trace, "CtestfileDlg", "end copy error: "));
			DPRINT("CImportThread end copy error: \n");

		delete pBuf;

		fclose(fpSrc);
		fclose(fpDest);
#endif
	}
	m_nIsEnd = 1;

	return 1;
}
#endif



#if 0
int CImportThread::importFile()
{
	std::vector<filename>::iterator ite;
	filename fn;
	m_nIsEnd = 0;

	for(ite = m_pfilelist->begin(); ite != m_pfilelist->end(); ++ite)
	{
		fn = *ite;
		m_src   = fn.m_src;
		m_dest = fn.m_dest;

		DP_PrintS("file src = %s",m_src.c_str());
		DP_PrintS("file m_dest = %s",m_dest.c_str());

		try
		{
			fs::path destPath(m_dest, fs::native);
			fs::create_directories(destPath.branch_path());
		}
		catch (const fs::filesystem_error& ex)
		{
			std::string error = ex.what();
			return false;
		}

		HANDLE hSrcFile;
		hSrcFile = CreateFile(m_src.c_str(), 
			GENERIC_READ, 
			(DWORD)0, NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE) NULL); 

		if (hSrcFile == NULL)
		{
//			DPRINT((DP_Trace, "CImportThread", "Open source file %s failed!", m_src.c_str()));
			DPRINT("CImportThread Open source file %s failed!\n", m_src.c_str());
			return 0;
		}
//		DPRINT((DP_Trace, "CImportThread", "Open source file %s succeeded!", m_src.c_str()));
		DPRINT("CImportThread Open source file %s succeeded!\n", m_src.c_str());


		HANDLE hDestFile = NULL;
		hDestFile = CreateFile(m_dest.c_str(),  
			GENERIC_WRITE, 
			FILE_SHARE_WRITE, 
			NULL,
			OPEN_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL, 
			(HANDLE) NULL); 

		if (hDestFile == NULL)
		{
			DPRINT((DP_Trace, "CImportThread", "Open dest file %s failed!", m_dest.c_str()));
			CloseHandle(hDestFile);
			return 0;
		}
		DPRINT((DP_Trace, "CImportThread", "Open dest file %s succeeded!", m_dest.c_str()));

		DWORD dwFileSize = GetFileSize(hDestFile,NULL);
		LONG  lDistance = dwFileSize;
		if (dwFileSize >= BREAK_POINT_DELETE_FILE_SZIE)
		{
			lDistance = 0;
		}

		if (lDistance != 0)
		{
			DWORD dwPtr = SetFilePointer (hSrcFile, lDistance, NULL, FILE_BEGIN) ; 
			dwPtr = SetFilePointer (hDestFile, lDistance, NULL, FILE_BEGIN) ; 
		}
		m_progress	+= dwFileSize;

		DWORD nSize = 1024 * 100;//64;// * 10;  // * 100;
		char * pBuf = new char[nSize];
		unsigned long nBytesRead,nLongW = 0;
		int n = 1;
		DPRINT((DP_Trace, "CImportThread", "start copy"));

		BOOL bResult = TRUE;
		while ((status() != thread_stoping) && (status() != thread_stopped))
		{			
			nBytesRead = 0;
			bResult = TRUE;
			bResult = ReadFile(hSrcFile,pBuf,nSize,&nBytesRead,NULL);
			if (nBytesRead == 0) 
			{   // this is the end of the file 
				break;
			}
			DPRINT((DP_Trace, "CtestfileDlg", "[%d]read size %d", n, nBytesRead));

			BOOL nBytesWrite = WriteFile(hDestFile,pBuf,nBytesRead,&nLongW,NULL);
			if(!nBytesWrite)
			{
				break;
			}
			m_progress += nLongW;
			DPRINT((DP_Trace, "CtestfileDlg", "[%d]write size %d", n, nLongW));

			n++;
		}

		if(bResult)
			DPRINT((DP_Trace, "CtestfileDlg", "end copy succeeded."));
		else
			DPRINT((DP_Trace, "CtestfileDlg", "end copy error: "));

		delete pBuf;

		CloseHandle(hSrcFile);
		CloseHandle(hDestFile);
	}
	m_nIsEnd = 1;

	return 1;
}
#endif



int CImportThread::importFile()
{
	std::vector<filename>::iterator ite;
	filename fn;
// 	mytime("100K  开始导入节目");
// 
// 	cout << "2进入线程" <<endl;

	m_nIsEnd = 0;

	for(ite = m_pfilelist->begin(); ite != m_pfilelist->end(); ++ite)
	{
		fn = *ite;
		m_src   = fn.m_src;
		m_dest = fn.m_dest;

		cout << "file src = " << m_src << endl;
		cout << "file m_dest = " << m_dest << endl;
		DP_PrintS("file src = %s",m_src.c_str());
		DP_PrintS("file m_dest = %s",m_dest.c_str());

		try
		{
			fs::path destPath(m_dest);
			fs::create_directories(destPath.branch_path());
		}
		catch (const fs::filesystem_error& ex)
		{
			cout << "文件路径已经存在" <<endl;
			std::string error = ex.what();
			return false;
		}
		cout << "file 11111111 = " << m_dest << endl;


		FileHandle srcFile = FileOpenRead(m_src.c_str()); 
		DP_PrintS("CImportThread FileOpenRead = %s",m_src.c_str());


		//FileHandle srcFile = fopen64(m_src.c_str() ,"rb"); 		
		if(!FileValid(srcFile)){
			cout << " 打开 文件错误m_src  = " << m_src  << endl;
			DP_PrintS("打开 文件错误");
			return false;
		}
		cout << "file 22222222 = " << m_dest << endl;
		
		
		FileHandle destFile = FileOpen(m_dest.c_str());	
		if(!FileValid(destFile)){
			destFile = FileOpenNew(m_dest.c_str());
//			destFile = fopen64(m_dest.c_str() ,"w+b"); 
			cout << " 创建目标文件m_dest   = "<< m_dest  << endl;						
			DP_PrintS("创建目标文件");
		}

		
		if(!FileValid(destFile))
		{
			cout << " 创建目标文件错误m_dest   = "<< m_dest  << endl;									
			FileClose(srcFile);
			return false;
		}
		cout << "file 33333333 = " << m_dest << endl;
		DP_PrintS("file");

		fileiolib::UInt64 srcSize = 0, writeSize = 0, dstSize = 0 , readAddSize = 0;
		int nErrNum = 10;
		
		FileSeekEnd(srcFile);
		srcSize = FileTell(srcFile);
		
		FileSeekEnd(destFile);
		dstSize = FileTell(destFile);
		
		FileSeek(srcFile, dstSize);

		// 如果断点的文件小于1M ,不做断点续传
		if(dstSize > srcSize && dstSize < BREAK_POINT_DELETE_FILE_SZIE)
		{
			cout << "dstSize > srcSize  " << endl;					
			cout << "dstSize = " << dstSize << endl;					
			cout << "srcSize = " << srcSize << endl;					

			FileClose(destFile);
			destFile = FileOpenNew(m_dest.c_str());
			dstSize = 0;
		}

		if (dstSize == srcSize)
		{
			cout << "dstSize == srcSize  " << endl;					
			cout << "dstSize = " << dstSize << endl;					
			cout << "srcSize = " << srcSize << endl;					

			m_progress += dstSize;
			cout << "m_progress  = " << m_progress  << endl;					
			continue;
		}
		//else 
		DP_PrintS("dstSize = %d,   srcSize = %d",dstSize,srcSize);

		if (dstSize < srcSize)
		{
			DP_PrintS("dstSize < srcSize       dstSize = %d,   srcSize = %d",dstSize,srcSize);

			cout << "dstSize < srcSize  " << endl;					
			cout << "dstSize = " << dstSize << endl;					
			cout << "srcSize = " << srcSize << endl;	

			if (dstSize > BREAK_POINT_DELETE_FILE_SZIE)
			{
				FileSeekEnd(destFile);
				dstSize = FileTell(destFile) - BREAK_POINT_DELETE_FILE_SZIE;

				FileSeek(destFile, dstSize);	
				FileSeek(srcFile, dstSize);	
			}

			writeSize = dstSize;
			if (dstSize != 0)
			{			
				m_progress += dstSize;
				readAddSize = dstSize;
				cout << "m_progress  = " << m_progress  << endl;									
			}
			cout << "dstSize = " << dstSize << endl;					
			cout << "srcSize = " << srcSize << endl;					
			
			unsigned char buf[102400];
			while((status() != thread_stoping) && (status() != thread_stopped)) 
			{
				DP_PrintS("while in");
				fileiolib::UInt64 readBytes = FileRead(srcFile, buf, sizeof(buf));
				DP_PrintS("while FileRead");
				//cout << "***readBytes = " << readBytes<< endl;
				readAddSize += readBytes;  
				cout << ". ";
				DP_PrintS(".");

				if(readBytes == 0)
				{
					cout << "**readBytes == 0 进入" <<endl;
					if (srcSize == readAddSize){
						brunt::sleep(1000);
						cout << "srcSize = " << srcSize << endl;					
						cout << "readAddSize = " << readAddSize << endl;					
						break;
					}
							
					nErrNum--;
					cout << "1nErrNum =  " << nErrNum <<endl;

					if(nErrNum == 0) {
						cout << "2nErrNum = " << nErrNum << endl;					
						return false;
					}
					else{
						cout << "3nErrNum = " << nErrNum << endl;
						brunt::sleep(100);
						continue;
					}		
				}
				fileiolib::UInt64 writeBytes = FileWrite(destFile, buf, readBytes);
				writeSize += writeBytes;
				m_progress += writeBytes;				
			}
		}
		DP_PrintS("while");

		if (status() != thread_stoping)
		{
			cout << "srcSize = " << srcSize <<endl;
			cout << "readAddSize = " << readAddSize <<endl;
			
			if (srcSize != readAddSize)
			{
				cout << "**err 目标文件与copy的文件大小不等" <<endl;
				m_nIsEnd = 1;
				FileClose(srcFile);
				FileClose(destFile);
				return false;
			}		
		}
		
		if(status() != thread_stoping || status() != thread_stopped)
		{
			//m_progress = 100;
			cout <<"**********status()" << status() <<endl; 
		}
		FileClose(srcFile);
		FileClose(destFile);

		// cui win 不用加这个
		//		system("sync");
	}

	/*  
	// 代码的作用是，广告修改ID
	if (m_adId != "" && m_adPath != "")
	{
		IProgramParse* programParse = createProgramParse();
		if(programParse == NULL)
			return get_program_parse_error;

		int error = 0;
		if(programParse->init(m_adPath) < 0)
		{
			error = invalid_program_error;
		}
		else
		{
			error = programParse->flushProgramId(m_adId);
		}
		releaseProgramParse(programParse);
	}
	*/
	m_nIsEnd = 1;
	cout << "CImportThread  exit m_nIsEnd = " << m_nIsEnd<<endl;
	DP_PrintS("thread end");

	//m_progress = 0;

	return 1;
}

void CImportThread::doit()
{
	m_status = IMPORT_STATUS_WROKING;
	if(importFile() == 0)
		m_status = IMPORT_STATUS_ERROR;
	else
		m_status = IMPORT_STATUS_END;
	m_pObserver->notify(m_status,m_nIsEnd);
	mytime("完成导入节目");

	cout <<" ***doit = " << m_status << endl;
}
