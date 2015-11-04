// XMLManage.h : Declaration of the CXMLCoreManage

#ifndef __XMLMANAGE_H_
#define __XMLMANAGE_H_

//#include "resource.h"       // main symbols
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include "DXMLCore.h"
class CXMLNode;

XERCES_CPP_NAMESPACE_USE

/////////////////////////////////////////////////////////////////////////////
// CXMLCoreManage
class CXMLCoreManage : IXMLManage
{
public:
	CXMLCoreManage();
	virtual ~CXMLCoreManage();

// IXMLManage
public:
	virtual int GetRootNode(IXMLNode** ppNode);
	int SaveXMLFile(const char* szFileName,const IXMLNode* pNode);
	int OpenXMLFile(const char* szFileName,std::string& errinfo);
	int CreateXMLDoc(const char* szName,const char* szNamespace,std::string& errinfo);
	int prase(const char* stringbuf,std::string& errinfo);

public:
	std::map<std::string,std::string>& GetNamespace() {return m_mapNamespace;}
	void AddRef(){m_lRefCount++;}
	void Release(){
		m_lRefCount--;
		if(m_lRefCount == 0)
			delete this;
	}
private:
	CXMLCoreManage(const CXMLCoreManage&);
	CXMLCoreManage& operator =(const CXMLCoreManage&);
	void FreeXMLResource();
	DOMDocument* m_pDoc;
	
	long m_lRefCount;
	XercesDOMParser * m_pParser;
	std::map<std::string,std::string> m_mapNamespace;
	std::string m_strDefaultNamespace;
protected:
//	int InitXMLPlat();
//	void Terminate();
	void parseNamespace(const std::string& szval);
	void parseNamespace(const DOMDocument* pDoc,std::string& strDefault,std::map<std::string,std::string>& mapNamespace);
	void insertmap(const std::string& strdata);
//	const std::string FindNameSpace(const std::string& strval);
};

#endif //__XMLMANAGE_H_
