//////////////////////////////////////////////////////////////////////////
//
#ifndef __DXMLCORE_H_
#define __DXMLCORE_H_

#include <string>
#include <map>

#ifdef _WIN32

#ifdef DXMLCORE_EXPORTS
#define DXMLCORE_API __declspec(dllexport)
#else
#define DXMLCORE_API __declspec(dllimport)
#endif

#else
#define DXMLCORE_API
#endif


struct DXMLCORE_API IXMLNode
{
	virtual int GetAttributeCount(/*[out]*/long* plcount)=0;
	virtual int GetAttributeByNameIndex(/*[in]*/int nIndex,/*[in,out]*/std::string& strname,/*[out]*/std::string& strval)=0;
	virtual int SetAttribute(/*[in]*/const char* szName,/*[in]*/const char* szVal)=0;
	virtual int GetAttribute(/*[in]*/const char* szName,/*[out]*/std::string& szVal)=0;
	virtual int RemoveAttribute(/*[in]*/const char* szName)=0;

	virtual int Clone(/*[out]*/IXMLNode** ppNode)=0;
	virtual int CreateChildNode(/*[in]*/const char* szName,/*[in]*/long lPos,/*[out]*/IXMLNode** ppNode)=0;
	virtual int RemoveChildNode(/*[out]*/IXMLNode* ppNode)=0;
	virtual int GetChildCount(/*[out]*/long* plCount)=0;
	virtual int GetChildCountByName(/*[in]*/const char* szName,/*[out]*/long* plCount)=0;
	virtual int GetChildByIndex(/*[in]*/long lPos,/*[out]*/IXMLNode** ppNode)=0;
	virtual int GetChildById(/*[in]*/int id,/*[out]*/IXMLNode** ppNode)=0;
	virtual int GetChildByNameIndex(/*[in]*/const char* szName,/*[in]*/long lPos,/*[out]*/IXMLNode** ppNode)=0;
	virtual int GetNodeName(std::string& szName)=0;
	virtual int GetPrefixName(std::string& szName)=0;
	virtual int GetLocalName(std::string& szName)=0;
	virtual int GetMaxid(/*[out]*/int* pnMaxid)=0;
	virtual int OutputNodeString(/*[out]*/std::string& stroutput)=0;
	virtual int SetTextcontent(/*[in]*/const char* sztext)=0;
	virtual int GetTextContent(std::string& sztext)=0;
	virtual int InsertChildNode(/*[in]*/const IXMLNode* pInputNode,/*[in]*/long lPos,/*[out]*/IXMLNode** ppNode)=0;
	virtual int InsertChildNode(/*[in]*/const void* pDomNode,/*[in]*/long lPos,/*[out]*/IXMLNode** ppNode)=0;
	virtual void AddRef()=0;
	virtual void Release()=0;
	virtual int GetDomElement(/*[out]*/void** ppDomNode)=0;
	virtual int Save(/*[in]*/const char* szFileName)=0;

};


struct DXMLCORE_API IXMLManage
{
public:
	virtual int GetRootNode(/*[out]*/IXMLNode** ppNode)=0;
//	virtual int SaveXMLFile(/*[in]*/const char* szFileName,/*[in]*/const IXMLNode* pNode)=0;
//	virtual int OpenXMLFile(/*[in]*/const char* szFileName)=0;
//	virtual int CreateXMLDoc(/*[in]*/const char* szName,/*[in]*/const char* szNamespace=NULL)=0;
//	virtual int prase(/*[in]*/const char* stringbuf,/*[in]*/int nFlag)=0;
};

DXMLCORE_API void InitXMLPlatInfo();
DXMLCORE_API void Terminate();
DXMLCORE_API IXMLManage* Createxml(const char* szname,const char* sznamespace=NULL);
DXMLCORE_API IXMLManage* Parsexml(const char* szdata,int nFlag=0);
//DXMLCORE_API IXMLManage*  createXMLManage();
//DXMLCORE_API  void releaseXMLManage(IXMLManage* pManage);

#endif
