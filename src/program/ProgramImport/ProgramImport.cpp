// ProgramImport.cpp : 定义 DLL 的初始化例程。
//
#ifdef WIN32
#include "stdafx.h"
#endif
#include "ProgramImport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "ProgramImport.h"
#include <program/ProgramError.h>
#include "importThread.h"
#include "breakPointImport.h"
#include "GetSpace.h"
//#include <ProgramParse.h>
#include <program/ProgramQuery.h>
#include <activeThreadManager_i.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include "config/config.h"
#include "v15common.h"
#ifdef DEBUGPRINT
#include "brunt/debugprint.h"
#else
#ifdef DEBUG
#define DP_PrintS(arg,...) printf(arg,...)
#define DPRINT(arg,...) printf(arg,...)
#else
#define DP_PrintS(arg,...) printf(arg)
#define DPRINT(arg,...) printf(arg)
#endif
#endif
#ifdef WIN32
#include <afx.h>
#endif

#include <iostream>
using namespace std;
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		
//#include <windows.h>
/*
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
*/
#else
#include <fstream>
#endif

using namespace brunt;
using namespace std;
namespace fs = boost::filesystem;

#define DISKROOT	"/storage"
//getConfig().getProgramRootPath()	// "/hdisk/program/"
#define USBROOT		"/media/usb"
//getConfig().getUsbRootPath()		// "/usb/DMS_HDD/"


class CProgramImport : public IProgramImport, public ImportObserver
{
public:
	CProgramImport();
	virtual ~CProgramImport();

	// for ImportObserver
	virtual void notify(int status,int nIsEnd);

	// for IProgramImport 
	virtual int import(const std::string& programPath, const std::string& programId);
	virtual const ImportProgress& progress();;
	virtual int getImportStatus() { return m_status; }
	virtual int stop();
	virtual long long int getProgramSize();
	virtual int getImportSize(long long int& status);	
	virtual void setProgramType(int ProgramType) ;	
	virtual bool isBreakPointImport(string& id,unsigned int& nSize,int& nType);
	virtual bool DelBreakPointImport();
	virtual void DelBreakPointImportFile();
	virtual int getLastError(){return m_error;};

private:
	int checkDestSpace(const vector<string>& childFiles);
	int checkImportProgram(const std::string& programPath, const std::string& programId);
	int checkTargetProgram(const std::string& programPath, const std::string& programId);
	void close();
	void setObserver(CImportThread& importThread) { importThread.setObserver(this); };
	bool checkPath(const string& path);

	// Aavan: 2008-7-3
	string getDstPath(const string& basePath, const string& id);//, const bool isdms, const string& adjustpath);

	// Aavan: 2008-7-3 判断是否为DMS节目
	bool isDmsProgram(vector<string>& childFiles);
	string getDmsDstProgram(vector<string>& childFiles);
	bool DeleteDirectory(char *DirName);

private:
	int m_error;
	std::string m_root;
	std::string m_destDeletePath;
	ImportProgress m_progress;
	brunt::IThreadManager* m_pManager;
	std::vector<CImportThread*> m_importThreadList; 
	int m_thread;
	int m_status;
	CGetSpace getSpace;
private:
	long long int  m_nProgramSize;  // 节目大小
	std::vector<filename> m_oProgramfileList; //节目文件列表 
	CImportThread* m_importThread;
	int m_ProgramType;  //	ta_type_movie = 0, 
	//   ta_type_publicad,
	//   ta_type_businessad 
	int m_nIsEnd;//线程结束

	IProgramQuery*  m_pQueryImport;
	IProgramQuery*  m_pQueryTarget;

	//IBreakPointImport* m_breakPointImport;
	int   m_nSaveTime;
	std::string m_sProgramId; 

};



NEWV8PROGRAMIMPORT_API IProgramImport* createProgramImport()
{
	return new CProgramImport;
}

NEWV8PROGRAMIMPORT_API void releaseProgramImport(IProgramImport* programImport)
{
	delete programImport;
}

CProgramImport::CProgramImport() : m_pManager(NULL),m_thread(0), m_status(IMPORT_STATUS_READY),m_nProgramSize(0), m_ProgramType(0),m_nIsEnd(0),m_nSaveTime(0)
{
	m_pManager = createThreadManager();
#ifdef WIN32
	m_root = USBROOT;
#else
	m_root = DISKROOT;
#endif
	m_pQueryImport = NULL;
	m_pQueryTarget = NULL;
}

CProgramImport::~CProgramImport()
{	
	releaseThreadManager(m_pManager);
	if(m_pQueryImport)
		releaseProgramQuery(m_pQueryImport);
	if(m_pQueryTarget)
		releaseProgramQuery(m_pQueryTarget);
}

void CProgramImport::close()
{
	m_destDeletePath = "";
	stop();
	m_progress = ImportProgress();

	if(m_pQueryImport)
	{
		releaseProgramQuery(m_pQueryImport);
		m_pQueryImport = NULL;
	}
	if(m_pQueryTarget)
	{
		releaseProgramQuery(m_pQueryTarget);
		m_pQueryTarget = NULL;
	}

	m_error = 0;
}


void CProgramImport::DelBreakPointImportFile()
{
	//TCHAR szPath[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, szPath);
	string fileName = ".";//getConfig().getBinRootPath();
	fileName += "\\breakpointimport.ini";
	remove(fileName.c_str());
}

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool CProgramImport::DeleteDirectory(char *DirName)
{
#ifdef WIN32
	CFileFind tempFind;
	char tempFileFind[200];
	sprintf(tempFileFind,"%s\\*.*",DirName);
	BOOL IsFinded=(BOOL)tempFind.FindFile(tempFileFind);
	while(IsFinded)
	{
		IsFinded=(BOOL)tempFind.FindNextFile();
		if(!tempFind.IsDots())
		{
			char foundFileName[200];
			strcpy(foundFileName,tempFind.GetFileName().GetBuffer(200));
			if(tempFind.IsDirectory())
			{
				char tempDir[200];
				sprintf(tempDir,"%s\\%s",DirName,foundFileName);
				DeleteDirectory(tempDir);
			}
			else
			{
				char tempFileName[200];
				sprintf(tempFileName,"%s\\%s",DirName,foundFileName);
				DeleteFile(tempFileName);
			}
		}
	}
	tempFind.Close();
	if(!RemoveDirectory(DirName))
	{
		::MessageBox(0,"删除目录失败！","警告信息",MB_OK);
		return false;
	}
	return true;
#else
    DIR *d;
    struct dirent *file;
    struct stat buf;
    if(!(d = opendir(DirName)))
	return false;
    while((file = readdir(d)) != NULL)
    {
	if(strncmp(file->d_name, ".", 1) == 0)
	    continue;
	stat(file->d_name, &buf);
	if(!(buf.st_mode & S_IFDIR))
	{
	    remove(file->d_name);
	}
    }
    if(rmdir(DirName) == 0)
        return true;
    return false;
#endif
} 

bool CProgramImport::DelBreakPointImport()
{
	bool bRet = false;
	CBreakPointImportData  breakPoint;
	IBreakPointImport * bpi = createBreakPointImport();

	if (bpi->open())
	{
		bpi->read(breakPoint);
		string sPath = breakPoint.sFilepath;
		if (!sPath.empty())
		{
			bRet = bpi->del();
			bpi->close();
			releaseBreakPointImport(bpi);

			DeleteDirectory((char*)breakPoint.sFilepath.c_str());
			//TCHAR szPath[MAX_PATH];
			//GetCurrentDirectory(MAX_PATH, szPath);
			string fileName = ".";//getConfig().getBinRootPath();
			fileName += "\\breakpointimport.ini";
			if (remove(fileName.c_str()) == -1)
				return true;
		}
	}
	else 
		releaseBreakPointImport(bpi);

	return bRet;
}

bool CProgramImport::isBreakPointImport(string& id,unsigned int& nSize,int& nType)	
{	
	IBreakPointImport * bpi = createBreakPointImport();
	if (bpi->open())
	{
		cout << "******isBreakPointImport" << endl;
		CBreakPointImportData  breakPoint;
		bpi->read(breakPoint);
		id = breakPoint.sID;
		nType =breakPoint.nType;
		nSize = breakPoint.nProSize;
		bpi->close();
		releaseBreakPointImport(bpi);
		//m_breakPointImport->close();
		if (breakPoint.sID != "")
		{
			return true;
		}

	}
	else
		releaseBreakPointImport(bpi);

	return false;
}

int CProgramImport::checkDestSpace(const vector<string>& childFiles)
{
	unsigned int srcFileSize = getSpace.getFileSize(childFiles);

	try
	{
		unsigned int freeSpace = getSpace.getAvalibleSpace(m_root);
		fs::path root(m_root);
		fs::is_directory(root);

		if(srcFileSize > freeSpace)
			return not_enough_space;
	}
	catch(const fs::filesystem_error&)
	{
		return program_import_root_error;
	}
	return no_error;
}

int CProgramImport::stop()
{
	if(m_importThreadList.size() == 0)
		return no_error;

	m_nProgramSize =0;
	m_nIsEnd = 0;

	m_pManager->terminate();
	std::vector<CImportThread*>::iterator ite;
	for(ite = m_importThreadList.begin(); ite != m_importThreadList.end(); ++ite)
	{
		delete *ite;
	}

	m_importThreadList.clear();

	//	if(!m_progress.isEnd && !m_destDeletePath.empty())
	long long int status;
	if(m_nProgramSize != getImportSize(status)  && !m_destDeletePath.empty())   
	{
		cout << "stop" << endl;
		DelBreakPointImport();
		//m_breakPointImport->del();
		//cout << "command" << endl;
		//string command = "rm -f -r " + m_destDeletePath;
		//system(command.c_str());
	}

	return no_error;
}

int CProgramImport::checkImportProgram(const std::string& programPath, const std::string& programId)
{
	int error = 0;
	/*  old code ->
	IProgramParse* programParse = createProgramParse();

	if(programParse == NULL)
		return get_program_parse_error;

	if(programParse->init(programPath) < 0)
	{
		error = invalid_program_error;
	}
	else
	{
		error = programParse->getProgramId(programId);
		//std::vector<ReelInfo> filePathList;
		//error = programParse->getProgramFilePath(filePathList);
	}
	releaseProgramParse(programParse);
	  old code <- 
	*/

	if(m_pQueryImport)
		releaseProgramQuery(m_pQueryImport);
	m_pQueryImport = createProgramQuery();

	error = m_pQueryImport->open(programPath, true);
	if(error!=0)
	{
		DPRINT("IProgramQuery->open() error: %d", error);
//		DPRINT((DP_Error, "CProgramImport", "IProgramQuery->open() error: %d", error));
		error = invalid_program_error;
		releaseProgramQuery(m_pQueryImport);
		return error;
	}
	if(m_pQueryImport->getProgramIndex(programId)<0)
	{
		DPRINT("cannot find the program (id:%s).", programId.c_str());
//		DPRINT((DP_Error, "CProgramImport", "cannot find the program (id:%s).", programId.c_str()));
		error = invalid_program_error;
	}

	return error;
}

int CProgramImport::checkTargetProgram(const std::string& programPath, const std::string& programId)
{
	int error = 0;

	if(m_pQueryTarget)
		releaseProgramQuery(m_pQueryTarget);
	m_pQueryTarget = createProgramQuery();

	error = m_pQueryTarget->open(programPath, true);
	if(error!=0)
	{
		DPRINT("IProgramQuery->open() error: %d", error);
//		DPRINT((DP_Error, "CProgramImport", "IProgramQuery->open() error: %d", error));
		error = invalid_program_error;
		releaseProgramQuery(m_pQueryTarget);
		return error;
	}
	if(m_pQueryTarget->getProgramIndex(programId)<0)
	{
		DPRINT("cannot find the program (id:%s).", programId.c_str());
//		DPRINT((DP_Error, "CProgramImport", "cannot find the program (id:%s).", programId.c_str()));
		error = invalid_program_error;
	}

	return error;
}

long long int CProgramImport::getProgramSize()
{
	cout << "CProgramImport  ProgramSize= " << m_nProgramSize<<endl;	

	return m_nProgramSize;
}

int CProgramImport::getImportSize(long long int& nSize)
{
	nSize = m_importThread->progress();
	int status;

	if (nSize == -1)
		status = IMPORT_STATUS_ERROR;

	status = m_status;

	if (m_status == IMPORT_STATUS_ERROR)
	{
		//num = m_nProgramSize;
		cout << "*Import  Thread ERROR = " << m_status<<endl;	
	}

	cout << "0904CProgramImport  ImportSize= " << nSize<<endl;	

	int tm = time((time_t*)NULL);

	if (m_nSaveTime == 0)
	{
		m_nSaveTime  = tm;
	}
	else if (tm - m_nSaveTime > 3 )//* 60)
	{
		IBreakPointImport * bpi = createBreakPointImport();
		if (bpi->open())
		{
			cout << "isBreakPointImport" << endl;
			CBreakPointImportData breakPoint;
			breakPoint.sID = m_sProgramId;
			breakPoint.nType = m_ProgramType;
			breakPoint.nProSize = nSize;
			breakPoint.sFilepath = m_destDeletePath;
			bpi->write(breakPoint);
			bpi->close();
			releaseBreakPointImport(bpi);
		}
		else
			releaseBreakPointImport(bpi);	
	}


	if (m_nIsEnd)
	{
		cout << "Import  Thread end   ProgramSize  = "  << m_nProgramSize <<endl;			
		cout << "Import  Thread end   ImportSize     = "  << nSize   <<endl;

		if (nSize < m_nProgramSize && m_nProgramSize != 0)
		{
			status = IMPORT_STATUS_STOP;
			nSize = m_nProgramSize;
		}

	}

	return status;
}

void CProgramImport::setProgramType(int ProgramType) 
{	
	m_ProgramType = ProgramType;

	if (m_ProgramType == 0)
		m_root = DISKROOT ;//+ PATHSEPARATOR + "movie";
	else if(m_ProgramType == 1)
	{
		string t = DISKROOT;
		m_root = t + PATHSEPARATOR + "publicad";
	}
	else if(m_ProgramType == 2)
	{
		string t = DISKROOT;
		m_root = t + PATHSEPARATOR + "businessad";
	}
}

int CProgramImport::import(const std::string& programPath, const std::string& programId)
{
	close();
	m_nIsEnd = 0;
	m_status = IMPORT_STATUS_READY;
	try
	{
		std::string tempId,adPath;
		m_destDeletePath = getDstPath(m_root, programId);

		// check if the program has been exist
		m_error = checkTargetProgram(m_destDeletePath, programId);
		if(m_error==0)
		{
			m_error = exist_same_program_name;
			throw m_error;
		}

		/*         
		// 电影和广告分类
		if (m_ProgramType != 0)
		programId = lid;
		else 
		m_error = checkProgram(programPath, programId);
		*/
		m_error = checkImportProgram(programPath, programId);
		if(m_error < 0)
		{
			throw m_error;
		}

		std::vector<std::string> childFiles;
		/*
		// Aavan: 2008-7-3
		bool isdms;
		if(!getSpace.copy_find_file(programPath, childFiles, isdms))
		{
			return could_not_find_file_error;
		}
		*/

		//	bool isdms = isDmsProgram(childFiles);

		// get import files' path
		assert(m_pQueryImport);
		int index = m_pQueryImport->getProgramIndex(programId);
		if(index<0)
			throw -1;
		m_pQueryImport->getProgramFiles(index, childFiles);
		std::string strProgramPath;
		m_pQueryImport->getProgramPath(index, strProgramPath);
		/*
		string adjustpath;
		if (isdms) 
		{
			cout << "########## is DMS program ###########" << endl;
			adjustpath = getDmsDstProgram(childFiles);
			cout << "### adjustpath:" << adjustpath << endl;
		}
		*/

//		checkProgram(m_destDeletePath, tempId);

		m_sProgramId = programId;

		/*
		if(tempId == programId)
		{
			m_error = exist_same_program_name;
			return m_error;
		}*/

		if( (m_error = checkDestSpace(childFiles)) != 0)    //get the valid destination path!
		{
			throw m_error;
		}

		/*
		string sIdFile = getSpace.getSaveFile();
		if (sIdFile != "")
		{
			cout << "关键文件 id  | describe.ini   ||| sIdFile = " << sIdFile << endl;
			childFiles.push_back(sIdFile);
		}
		*/
        

		fs::path srcpath(programPath);
		fs::path destPath(m_destDeletePath);

//		int pos = srcpath.native_file_string().length() + 1;

		vector<string>::iterator ite_child = childFiles.begin();
		m_oProgramfileList.clear();

		for(int i = 0; ite_child != childFiles.end(); ++ite_child, ++i)
		{
			string srcfilename(*ite_child);
			string m_destfilename(destPath.native() + PATHSEPARATOR);			

			int pos = strProgramPath.size();
			m_destfilename += srcfilename.substr(pos+1); 


			fs::path filePath(srcfilename.c_str());
			long long filesize = fs::file_size(filePath);
			m_nProgramSize += filesize;
			DPRINT("import files(%d/%d) : %s : %I64d", 
											i, childFiles.size(), srcfilename.c_str(), filesize);
//			DPRINT((DP_Trace, "CProgramImport", "import files(%d/%d) : %s : %I64d", 
//											i, childFiles.size(), srcfilename.c_str(), filesize));

			filename fn;
			fn.m_src   = srcfilename;
			fn.m_dest = m_destfilename;
			m_oProgramfileList.push_back(fn);
		}


		m_importThread= new CImportThread;
//		if (m_ProgramType != 0)
//		{
//			m_importThread->setAdPath(adPath,lid);
//		}
		m_importThreadList.push_back(m_importThread);
		m_thread = (int)m_importThreadList.size();

		m_importThread->init(m_oProgramfileList);
		setObserver(*m_importThread);

		ThreadRunPolicy policy;
		policy.order = 0;
		policy.policy = POLICY_ERROR_EXIT;

		CThreadRunInfo threadInfo(m_importThread, policy);
		m_pManager->addThread(threadInfo);

	}
	catch(const fs::filesystem_error&)
	{
		m_error = program_import_error;
		return m_error;
	}
	catch(int& e)
	{
		close();
		m_error = e;
		return m_error;
	}

	m_pManager->setParallelNum(0, 1);
	if(m_pManager->run() < 0)
	{
		m_destDeletePath = "";
		return run_thread_manager_error;
	}

	return m_error;
}

const ImportProgress& CProgramImport::progress()
{
	boost::intmax_t numerator = 0, denominator = 0;
	std::vector<CImportThread*>::iterator ite = m_importThreadList.begin();
	for(unsigned int i= 0; i < m_progress.progs.size(), ite != m_importThreadList.end(); ++i, ++ite)
	{
		m_progress.progs[i].progress = (*ite)->progress();
		numerator += (boost::intmax_t)m_progress.progs[i].progress * (boost::intmax_t)m_progress.progs[i].filesize;
		denominator += m_progress.progs[i].filesize;
	}
	if(denominator > 0 && !m_progress.isEnd)
	{
		m_progress.totalProgress = (int)( numerator / denominator);
		if(m_progress.totalProgress > 100)
			--m_progress.totalProgress;
	}

	return m_progress;
}

void CProgramImport::notify(int status,int nIsEnd)
{
	//FIX ME: not thread safe;
	cout << "*notify  status = " << status << endl;

	m_status = status;
	m_nIsEnd = nIsEnd;
	if(status == IMPORT_STATUS_ERROR)
	{
		m_progress.totalProgress = 100;
		m_progress.isEnd = false;
	}
	else if(status == IMPORT_STATUS_END)
	{
		//cout << "del  breakpointimport"<<endl; 
		//string fileName("/hdisk/data/bp/breakpointimport.dat");
		//string cmd = "rm -f -r " + fileName;
		//system(cmd.c_str());
		//cout << "cmd" << cmd << endl;

		if(m_thread > 1)
			--m_thread;
		else
		{
			m_progress.totalProgress = 100;
			m_progress.isEnd = true;
		}
	}
}

//check if have input/output error
bool CProgramImport::checkPath(const string& path)
{
	try
	{
		if(!fs::exists(path))
			return true;
	}
	catch(const fs::filesystem_error& e)    
	{
		return false;
	}

	try
	{	
		fs::directory_iterator end_itr;
		for(fs::directory_iterator itr(path); itr != end_itr; ++itr)
		{
			try
			{
				if(fs::is_directory(*itr))
				{
					return checkPath(itr->path().native());
				}
			}
			catch(const fs::filesystem_error& e)
			{

				cout << "skip input/output error, CProgramQuery::getProgramList, except:" << e.what() << endl;
				return false;
			}
		}
	}
	catch(const fs::filesystem_error& e)
	{
		//run on 8634, when last ++itr, always throw exception, so omit this exception.
		//return get_program_list_catch_exception_error;
		//cout << "CProgramQuery::getProgramList, except:" << e.what() << endl;
	}
	return true;
}

// Aavan: 2008-7-3
string CProgramImport::getDstPath(const string& basePath, const string& id)//, const bool isdms, const string& adjustpath)
{
	string dst;
//	if (!isdms)
		dst =  basePath  + PATHSEPARATOR + id;
//	else
//		dst = basePath + PATHSEPARATOR + adjustpath;

	// cui 2008-12-17
	return dst;

	/* liang deleted
	fs::path dstPath(dst, fs::native);
	//check this path is valid.
	do
	{
		try
		{       
			//check no input/output error;
			if(checkPath(dst))
				break;
			//should del this path;
			try
			{
				fs::path thePath(dst, fs::native);
				fs::remove_all(thePath);
			}
			catch(const fs::filesystem_error& e)
			{
			}	
		}
		catch(const fs::filesystem_error& e)
		{
			//dstPath exist, but error
			cout << "CProgramImport::getDstPath: " << e.what()<< endl;		    
		}
		dst += "-b";
	}while(1);
	return dst;		
	*/ 
}

// Aavan: 2008-7-3 判断是否为DMS节目
bool CProgramImport::isDmsProgram(vector<string>& childFiles)
{
	vector<string>::iterator ite_child = childFiles.begin();

	for(int i = 0; ite_child != childFiles.end(); ++ite_child, ++i)
	{
		string srcfilename(*ite_child);
		int pos = srcfilename.rfind('/');
		string keyname = srcfilename.substr(pos + 1, srcfilename.length() - pos - 1);
		cout << "@@ srcfilename:" << srcfilename << ", keyname:" << keyname << endl;

		if (keyname == "id")
		{
			// Found DMS program id file.
			return true;
		}
	}

	return false;
}

// Aavan: 2008-7-3 调整目标路径
string CProgramImport::getDmsDstProgram(vector<string>& childFiles)
{
	vector<string>::iterator ite_child = childFiles.begin();
	string srcfilename(*ite_child);
	int pos1 = srcfilename.rfind('/');
	int pos2 = srcfilename.rfind('/', pos1 - 1);
	string dstpath = srcfilename.substr(pos2 + 1, pos1 - pos2 - 1);
	return dstpath;
}















//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CProgramImportApp
/*
BEGIN_MESSAGE_MAP(CProgramImportApp, CWinApp)
END_MESSAGE_MAP()


// CProgramImportApp 构造

CProgramImportApp::CProgramImportApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CProgramImportApp 对象

CProgramImportApp theApp;


// CProgramImportApp 初始化

BOOL CProgramImportApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
*/