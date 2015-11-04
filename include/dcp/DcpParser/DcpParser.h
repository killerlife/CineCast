#ifndef DCPAPP_IDCPPARSE_H
#define DCPAPP_IDCPPARSE_H


#ifdef  _WIN32

#ifdef DCPPARSE_EXPORTS
#define DCPPARSE_API __declspec(dllexport)
#else
#define DCPPARSE_API __declspec(dllimport)
#endif

#else
#define DCPPARSE_API 
#endif

#include <DCPWrapLib/PackingList.h>
#include <DCPWrapLib/CompositionPlayList.h>
#include <string>
#include <vector>


class DCPPARSE_API IDcpParser 
{
public:
	//for get packinglistFile and composition's absolute file name,path must be absolute path!
	virtual bool open(const std::string& path)=0;
	virtual void close()=0;

	virtual unsigned int packingListCount() = 0;
	virtual const CPackingList* packingList(unsigned int index) = 0;
	virtual unsigned int compositionCount(unsigned int index) = 0;
	virtual const CCompositionPlayList* composition(unsigned int pklIndex, unsigned int index) = 0;
    virtual const CCompositionPlayList* find(const std::string& uuid) = 0;
	virtual std::string packingListFile(unsigned int index)=0;
	virtual std::string compositionFile(unsigned int pklIndex, unsigned int index)=0;
	virtual std::string compositionFileByuuid(const std::string& struuid)=0; //(add by qwy)根据uuid得到cpl文件(包含路径)

	virtual int getLastError()=0;
	
};
DCPPARSE_API IDcpParser* CreateDcpParser();
DCPPARSE_API void ReleaseDcpParser(IDcpParser** ppParser);


#endif


