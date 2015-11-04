// XMLNode.h : Declaration of the CXMLNode

#ifndef __XMLNODE_H_
#define __XMLNODE_H_

//#include "resource.h"       // main symbols

#include "DXMLCore.h"
#include <xercesc/dom/DOM.hpp>
#include <list>

XERCES_CPP_NAMESPACE_USE

/////////////////////////////////////////////////////////////////////////////
// CXMLNode
class CXMLCoreManage;

class CXMLNode : public IXMLNode
{
public:
	CXMLNode(DOMElement* pEle,CXMLCoreManage* pMng);
	virtual ~CXMLNode();

// IXMLNode
public:
	virtual int GetChildById(int id,IXMLNode** ppNode);
	virtual int SetAttribute(const char* szName,const char* szVal);
	virtual int GetAttribute(const char* szName,std::string& szVal);
	virtual int RemoveAttribute(const char* szName);
	virtual int Clone(IXMLNode** ppNode);
	virtual int CreateChildNode(const char* szName,long lPos,IXMLNode** ppNode);
	virtual int RemoveChildNode(IXMLNode* ppNode);
	virtual int GetChildCount(long* plCount);
	virtual int GetChildCountByName(const char* szName,long* plCount);
	virtual int GetChildByIndex(long lPos,IXMLNode** ppNode);
	virtual int GetChildByNameIndex(const char* szName,long lPos,IXMLNode** ppNode);
	virtual int GetNodeName(std::string& szName);
	virtual int GetPrefixName(std::string& szName);
	virtual int GetLocalName(std::string& szName);
	virtual int GetMaxid(int* pnMaxid);
	virtual int OutputNodeString(std::string& stroutput);
	virtual int SetTextcontent(const char* sztext);
	virtual int GetTextContent(std::string& sztext);
	virtual int InsertChildNode(const IXMLNode* pInputNode,long lPos,IXMLNode** ppNode);
	virtual int InsertChildNode(const void* pDomNode,long lPos,IXMLNode** ppNode);
	virtual int GetDomElement(void** ppDomNode);
	virtual int GetAttributeCount(long* plcount);
	virtual int GetAttributeByNameIndex(int nIndex,std::string& strname,std::string& strval);
	virtual int Save(const char* szFileName);

public:
	virtual void AddRef()
	{
		m_refCount++;
	}
	virtual void Release()
	{
		m_refCount--;
		if(m_refCount == 0)
			delete this;
	}
	
public:
	DOMElement* GetXMLElementpt()const;

private:
	CXMLNode(const CXMLNode&);
	CXMLNode& operator =(const CXMLNode&);
	void initmap();
	CXMLNode* GetChildByIndex(long lPos,const char* szName=NULL);
	DOMElement* m_pElement;

	int m_refCount;
//	std::list<CXMLNode*> m_childList;
	int m_nMaxId;
	CXMLCoreManage* m_pMng;
protected:
//	void Release();
	void Clear();
	int CompareNodeString(const char* szval1,const char* szval2);
	bool QueryNamespace(const XMLCh* xszname,const std::string strname);
	DOMElement* GetChildDOMByIndex(int lpos,const char* szName=NULL);
};

#endif //__XMLNODE_H_
