#ifdef WIN32
#pragma warning( disable : 4786 )   // disable warning debug symbol > 255...
#endif // _MSC_VER > 1000


#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <exception>
#include <iostream>
using namespace std;
namespace fs = boost::filesystem;

#include "errordefine.h"
#include "assetMap.h"
#include "DcpParser.h"
#ifdef DEBUGPRINT
#include "brunt/debugprint.h"
#else
#ifdef DEBUG
#define DP_PrintS(arg...) printf(arg...)
#else
#define DP_PrintS(arg...)
#endif
#endif
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
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
#endif


//////////////////////////////////////////////////////////////////////////
struct CPklInfo{
	const string pklFile;
	const CPackingList* pkl;
	vector<string> cplFiles;
	vector<CCompositionPlayList*> cpls;

	CPklInfo(CPackingList* p, const string& file):pkl(p), pklFile(file){};
	~CPklInfo(){
		for(int i=0; i<cpls.size(); i++)
			if(cpls[i]) delete cpls[i];
		if(pkl) delete pkl;
		cpls.clear();
		cplFiles.clear();
	};
};

class CDcpParse: public IDcpParser 
{
public:
	CDcpParse(){
		//m_pPackingList = NULL;
	}
	virtual ~CDcpParse()
	{
		close();
	}
	virtual bool open(const std::string& path);
	virtual void close();
	
	virtual unsigned int packingListCount();
	virtual const CPackingList* packingList(unsigned int index);
	virtual unsigned int  compositionCount(unsigned int index);
	virtual const CCompositionPlayList* composition(unsigned int pklIndex, unsigned int index);
    virtual const CCompositionPlayList* find(const std::string& uuid);

	virtual std::string packingListFile(unsigned int index);
	virtual std::string compositionFile(unsigned int pklIndex, unsigned int index);

//	virtual const CCompositionPlayList* compositionByuuid(const std::string& struuid); //(add by qwy)????uuid????cpl????????
	virtual std::string compositionFileByuuid(const std::string& struuid); //(add by qwy)????uuid????cpl????(????????)
	
	virtual int getLastError()
	{
		return m_error;
	}
protected:
	void findPKLFiles(const std::string& path, std::vector<std::string>& pklFiles);
	std::string findCPLFile(const std::string& path, const std::string& cpluuid);
	void getBasePath(std::string& basePath);
		
private:
	int  m_error;
//	string  m_basePath;
	vector<CPklInfo*> m_pklInfoList;
//	vector<CCompositionPlayList*> m_compositions;
//	vector<std::string>       m_compositionFiles;
//	CPackingList*	m_pPackingList;
//	string  m_packingListFile;
};

CDcpParse g_Parser;

IDcpParser* CreateDcpParser()
{
#if 0
	CDcpParse* pParser = new CDcpParse;
	return pParser;
#else
	return &g_Parser;
#endif
}

void ReleaseDcpParser(IDcpParser** ppParser)
{
#if 0
	delete (CDcpParse*)*ppParser;
	*ppParser = NULL;
#endif
}


//////////////////////////////////////////////////////////////////////////
bool my_find_file( const fs::path & dir_path,std::vector<std::string>& childs,bool dir=false) 
{
	if ( !fs::exists( dir_path ) )
	{
//		cout << "my_find_file error." << endl;
		return false;
	}
	try
	{
		fs::directory_iterator end_itr; // default construction yields past-the-end
		for ( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
		{
//			cout << itr->native_file_string() << " dir:" << fs::is_directory( *itr ) << endl;    // liang test
			
			if (dir && fs::is_directory( *itr ) )
			{
			
	            string str=itr->path().native();//native_file_string();
				childs.push_back(str);
				
			}
			else if (!dir&&!fs::is_directory( *itr ) ) 
			{
				string str=itr->path().native();
				childs.push_back(str);
				
			}
		}
	}
	catch(const fs::filesystem_error&)
	{
		//cout <<"catch exception:" << e.what() << "," << childs.size() << endl;
	}
	return true;
}

std::string CDcpParse::findCPLFile(const std::string& path, const std::string& cpluuid)
{
	fs::path thepath(path);
	vector<string> childFiles;		
	
	if(!my_find_file(thepath,childFiles) )
	{
		m_error= ERR_DIRECTORY_UNEXIST;
		return ""; 
	}

//	DP_PrintS("CDcpParse::findCPLFile %d", childFiles.size());
	DP_PrintS("CDcpParse::findCPLFile %d", childFiles.size());

	for(unsigned int i=0;i<childFiles.size();++i)
	{

		try
		{
			if(1)
			{
				int nLen = strlen(childFiles[i].c_str());
				if (nLen != 0)
				{
					char* p = (char*)strstr(childFiles[i].c_str(),".mxf");
					if(p != NULL)
					{
						continue;
					}
				}
				//Add by Jaontolt
				fs::path path(childFiles[i]);
				if(fs::file_size(path) > 1000000000)
					continue;
				//End
			}

			DP_PrintS("CCompositionPlayList childFiles[%d] = %s",i,childFiles[i].c_str());
			CCompositionPlayList cpl(childFiles[i].c_str());
			DP_PrintS("CCompositionPlayList 2");
			if(cpl.GetUUIDElement() == cpluuid){
				DP_PrintS("CCompositionPlayList cpl.GetUUIDElement() == cpluuid = %s",cpluuid.c_str());
				return childFiles[i];			
			}
		}
		catch (...)
		{
			DP_PrintS("CCompositionPlayList continue");
			continue;
		}

	}
	return "";
}

void CDcpParse::findPKLFiles(const std::string& path, std::vector<std::string>& pklFiles)
{
	fs::path thepath(path);
	vector<string> childFiles;

//	cout << "findPKLFiles(" <<path << ")" << endl;

	if(!my_find_file(thepath,childFiles) )
	{
		m_error= ERR_DIRECTORY_UNEXIST;
		return; 
	}
       
//	cout << "child size: " << childFil.es.size() << endl;
	
	for(unsigned int i=0; i<childFiles.size(); ++i)
	{
//		cout << "looking:" << childFiles[i] << endl;

//Add by Jaontolt
		//Check XML file first!
		if (childFiles[i].find(".mxf") != std::string::npos)
		{
			continue;
		}
		fs::path path(childFiles[i]);
		if(fs::file_size(path) > 1000000000)
			continue;
//End
		try
		{
			CPackingList packingList(childFiles[i].c_str());
			pklFiles.push_back(childFiles[i]);
		//	return childFiles[i];
		}
		catch (...)
		{
			continue;
		}
	}
}

void CDcpParse::getBasePath(std::string& basePath)
{
	string::size_type pos=basePath.find_last_of("/\\");
	if(pos !=string::npos)
		basePath=basePath.substr(0, pos+1);
	for(unsigned int i=0; i<basePath.length();++i)
	{
		if( '\\' ==basePath[i])
			basePath[i]='/';
	}
}

bool CDcpParse::open(const std::string& path)
{
	close();	// to be look at more safety

	DP_PrintS("CDcpParse open %s", path.c_str());

	std::vector<std::string> filesPKL;
	findPKLFiles(path, filesPKL);
	if(filesPKL.size()==0)
	{
		DP_PrintS("CDcpParse error: PackFileList cannot be finded.");
		m_error=ERR_PKL_FILE_UNEXIST;
		return false;
	}

	if( filesPKL.empty() )
	{
		m_error=ERR_PKL_FILE_UNEXIST;
		return false;
	}

	for(int k=0; k<filesPKL.size(); k++)
	{
		CPackingList* pkl = NULL;
		
		DP_PrintS("CDcpParse parse the PKL file '%s' ... ", filesPKL[k].c_str());

		try
		{
			pkl = new CPackingList(filesPKL[k].c_str());
		} catch (std::exception& e) {
			m_error = ERR_PKL_OPEN_FAILED;
		} catch (...) {
			m_error = ERR_PKL_OPEN_FAILED;
		}

		if(!pkl)
		{
			m_error = ERR_PKL_OPEN_FAILED;
			DP_PrintS("CDcpParse parse the PKL file '%s' failed. ", filesPKL[k].c_str());
// 			delete pkl;
			return false;
		}

		CPklInfo* pklInfo = new CPklInfo(pkl, filesPKL[k]);
		try
		{
			CPack_AssetList assetList = pklInfo->pkl->GetAssetList();
			int count = assetList.GetChildCountByName("Asset");
			for(int i=0; i< count;++i)
			{
				CPack_Asset assert =assetList.GetAssetByIndex(i);
				if(assert.GetTypeElement() != ASSETTYPE_COMPOSITIONPLAYLIST)
					continue;

				string fullname = findCPLFile(path,assert.GetUUIDElement());

				if(fullname.empty())
				{
					//baiqian add, if cpl isn't in path? so the best way is to find by assetmap.xml
					string basePath(path);
					getBasePath(basePath);

					CTaAssetMap assetMap;
					if(assetMap.open(basePath))
					{
						string filename = assetMap.findFile(assert.GetUUIDElement());
						fullname = basePath + filename;
						assetMap.close();
					}
					else
					{
						continue;
					}
				}

				CCompositionPlayList* pComList =new CCompositionPlayList(fullname.c_str());
				if(pComList)
				{
					pklInfo->cpls.push_back(pComList);
					pklInfo->cplFiles.push_back(fullname);
				}
			}
		}
		catch (std::exception& e) 
		{
			m_error = ERR_PKL_OPEN_FAILED;
		}
		catch (...) 
		{
			m_error = ERR_PKL_OPEN_FAILED;
		}

		if(m_error)
		{
			delete pklInfo;
			return false;
		}

		m_pklInfoList.push_back(pklInfo);
	}

	return true;
}

void CDcpParse::close()
{
	DP_PrintS("CDcpParse close");
	
	unsigned int i=0;

	for(i=0; i<m_pklInfoList.size(); ++i)
	{
		delete m_pklInfoList[i];
	}
	m_pklInfoList.clear();

	m_error = 0;
}

unsigned int CDcpParse::packingListCount()
{
	return m_pklInfoList.size();
}

const CPackingList* CDcpParse::packingList(unsigned int index)
{
	if(index>=0 && index<m_pklInfoList.size())
		return m_pklInfoList[index]->pkl;
	return NULL;
}

unsigned int CDcpParse::compositionCount(unsigned int index)
{
	if(index>=0 && index<m_pklInfoList.size())
		return m_pklInfoList[index]->cpls.size();
	return 0;
}

const CCompositionPlayList* CDcpParse::composition(unsigned int pklIndex, unsigned int index)
{
//	if( !(index>=0 && index<m_compositions.size()) )
//		throw std::exception("error composition index");
    if(pklIndex<0 && pklIndex>=m_pklInfoList.size())
		return NULL;

	if( index>=0 && index<m_pklInfoList[pklIndex]->cpls.size() )
		return m_pklInfoList[pklIndex]->cpls[index];
	return NULL;
}

const CCompositionPlayList* CDcpParse::find(const std::string& uuid)
{
	for(unsigned int k=0; k<m_pklInfoList.size(); k++)
	{
		for(unsigned int i = 0; i < m_pklInfoList[k]->cpls.size(); ++i )
		{
			if(m_pklInfoList[k]->cpls[i]->GetUUIDElement() == uuid)
				return m_pklInfoList[k]->cpls[i];
		}
	}
	return NULL;
}

std::string CDcpParse::packingListFile(unsigned int index)
{
	if( index>=0 && index< m_pklInfoList.size() )
		return m_pklInfoList[index]->pklFile;
	return "";
}

std::string CDcpParse::compositionFile(unsigned int pklIndex, unsigned int index)
{
    if(pklIndex<0 && pklIndex>=m_pklInfoList.size())
		return NULL;

	if( index>=0 && index<m_pklInfoList[pklIndex]->cplFiles.size() )
		return m_pklInfoList[pklIndex]->cplFiles[index];

	return "";
}

std::string CDcpParse::compositionFileByuuid(const std::string& struuid)
{
	for(unsigned int k = 0; k < m_pklInfoList.size(); k++)
	{
		for(unsigned int i = 0; i < m_pklInfoList[k]->cpls.size(); ++i )
		{
			if(m_pklInfoList[k]->cpls[i]->GetUUIDElement() == struuid)
				return m_pklInfoList[k]->cplFiles[i];
		}
	}
	return "";
}


