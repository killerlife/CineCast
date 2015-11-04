// Element.cpp: implementation of the CElement class.
//
//////////////////////////////////////////////////////////////////////
//#include "../../util/string/mystring.h"

#include "Element.h"
#include <errordef.h>
#include <DXMLCore/DXMLCore.h>
#include <MyXMLException.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define _ELEMENT_EXCEPTION(p)  if(!p) throw CMyXMLException(ERR_NODE_POINTER_EMPTY,"node pointer is NULL.");

int InitXMLPlat()
{
	InitXMLPlatInfo();
	return 0;
}

void FreeXMLPlat()
{
	Terminate();
}

CElement::CElement(IXMLNode* pNode)
:m_pNode(pNode)
{
	m_nerror = 0;
#if 0
	//Disable by Jaontolt
	if(!m_pNode)
		throw CMyXMLException(ERR_NODE_POINTER_EMPTY,"node pointer is NULL.");
#endif
//	m_pNode->AddRef();
}
//Add by Jaontolt
bool CElement::isNull() const
{
	return(!m_pNode);
}
//End
CElement::CElement(const char* rootName,const char* szInfo,SOURCEFLAG nFlag)
:m_pNode(NULL)
{
	m_nerror = 0;
	int nre = 0;
	IXMLManage* pmng = NULL;
	switch(nFlag)
	{
	case CREATE_FLAG:
		pmng = Createxml(rootName,szInfo);
		break;
	case OPENFILE_FLAG:
		pmng = Parsexml(szInfo,0);
		break;
	case BUFFER_FLAG:
		pmng = Parsexml(szInfo,1);
		break;
	default:
		throw CMyXMLException(ERR_SOURCE_FLAG_ILLEGAL,"source flag is illegal");
		break;
	}
	if(!pmng)
		throw CMyXMLException(ERR_INPUT_INFO_ERROR,"input info is error.");
	nre = pmng->GetRootNode(&m_pNode);
	if(!m_pNode)
		throw CMyXMLException(ERR_NODE_POINTER_EMPTY,"node pointer is NULL.");
	if(nFlag != CREATE_FLAG)
	{
		std::string strnodename;
		m_pNode->GetLocalName(strnodename);
		std::string strparam(rootName);
		int nfind = (int)strparam.find_first_of(':');
		if(nfind > 0)
			strparam = strparam.substr(nfind+1);
		if(strcmp(strnodename.c_str(),strparam.c_str()) != 0)
		{
			m_pNode->Release();
			m_pNode = NULL;
				throw CMyXMLException(ERR_INFO_DIFFER_NODENAME,"info and rootName differ.");
		}
	}

}

CElement::CElement(const CElement& cref)
	: m_pNode(cref.m_pNode)
{
	m_nerror = 0;
	if(!m_pNode)
		throw CMyXMLException(ERR_NODE_POINTER_EMPTY,"node pointer is NULL.");
	m_pNode->AddRef();
}

CElement::~CElement()
{
	if(m_pNode)
		m_pNode->Release();
}

void CElement::AddRef()
{
	m_pNode->AddRef();
}

CElement& CElement::operator =(const CElement& cref)
{
	if(this==&cref)
		return *this;
	EqualNode(cref.m_pNode);
	return *this;
}

CElement CElement::Clone()
{
	_ELEMENT_EXCEPTION(m_pNode);
	IXMLNode* pNewNode=NULL;
	m_nerror = m_pNode->Clone(&pNewNode);
	if(!pNewNode)
		throw CMyXMLException(m_nerror,"construct element failed.");
	return CElement(pNewNode);

}

//save xml file
int CElement::Save(const char* szfile)
{
	return m_pNode->Save(szfile);
}

int CElement::GetAttributeCount()const
{
	_CheckPointer(m_pNode,0);
	long ncount = 0;
	int hr = m_pNode->GetAttributeCount(&ncount);
	return hr>=0?ncount:hr;
//	return (int)ncount;
}

std::string CElement::GetAttributeByNameIndex(std::string& strname,int nIndex)const
{
	_CheckPointer(m_pNode,"");
	std::string strval;
	m_pNode->GetAttributeByNameIndex(nIndex,strname,strval);
	return strval;

}

bool CElement::SetAttribute(const char *szName, const char *szVal)
{
	_CheckPointer(m_pNode,false);
	m_nerror = m_pNode->SetAttribute(szName,szVal);
	return (m_nerror>=0) ? true : false;

}

std::string CElement::GetAttribute(const char *szName)const
{
	_CheckPointer(m_pNode,"");
//		throw std::runtime_error("get attribute failed,NULL POINTER");
	std::string strval;
	m_pNode->GetAttribute(szName,strval);
	return strval;
}

//datetime
bool CElement::SetAttribute(const char* szName,const DATETIME& szVal)
{
	return SetAttribute(szName,szVal.GetString().c_str());
}

DATETIME CElement::GetAttributeDatetime(const char* szName)const
{
	return DATETIME(GetAttribute(szName).c_str());
}

//rational
bool CElement::SetAttribute(const char* szName,const RATIONAL& szVal)
{
	return SetAttribute(szName,szVal.GetString().c_str());
}

RATIONAL CElement::GetAttributeRational(const char* szName)const
{
	return RATIONAL(GetAttribute(szName).c_str());
}

//Int64
bool CElement::SetAttribute(const char* szName,const Int64 szVal)
{
	char sztmp[20];
	SprintfInt64(sztmp,szVal);
	return SetAttribute(szName,sztmp);
}

Int64 CElement::GetAttributeInt64(const char* szName)const
{
	return atoInt64(GetAttribute(szName).c_str());
}

//long
bool CElement::SetAttribute(const char* szName,const long szVal)
{
	char sztmp[20];
	sprintf(sztmp,"%ld",szVal);
	return SetAttribute(szName,sztmp);
}

long CElement::GetAttributelong(const char* szName)const
{
	return atol(GetAttribute(szName).c_str());
}

//int
bool CElement::SetAttribute(const char* szName,const int szVal)
{
	char sztmp[20];
	sprintf(sztmp,"%d",szVal);
	return SetAttribute(szName,sztmp);
}

int CElement::GetAttributeint(const char* szName)const
{
	return atoi(GetAttribute(szName).c_str());
}

IXMLNode* CElement::CreateChildNodeRept(const char *szName, long lPos, bool bid)
{
	_ELEMENT_EXCEPTION(m_pNode);
//	if(NULL == m_pNode)
//		throw std::runtime_error("CElement create child failed,NULL POINTER");

	IXMLNode* pChildNode = NULL;
	m_nerror = m_pNode->CreateChildNode(szName,lPos,&pChildNode);
/*	if(NULL == pChildNode){
		char szErr[1024];
		sprintf(szErr,"CElement create child failed, error code:%d",hr);
		throw std::runtime_error(szErr);
	}*/
	if(pChildNode && bid)
	{
		int nMaxid = 0;
		m_pNode->GetMaxid(&nMaxid);
		char szid[14];
		sprintf(szid,"%d",nMaxid);
		pChildNode->SetAttribute("id",szid);
	}

	return pChildNode;

}

IXMLNode* CElement::CreateSingleChildNode(const char* szName, long lPos, bool bid)
{
//	if(NULL == m_pNode)
//		throw std::runtime_error("CElement create child by name failed,NULL POINTER");
	_ELEMENT_EXCEPTION(m_pNode);

	IXMLNode* pChildNode = NULL;
	m_nerror = m_pNode->GetChildByNameIndex(szName,0,&pChildNode);
	if(pChildNode)
		return pChildNode;

	m_nerror = m_pNode->CreateChildNode(szName,lPos,&pChildNode);
/*	if(NULL == pChildNode){
		char szErr[1024];
		sprintf(szErr,"CElement create child failed, error code:%d",hr);
		throw std::runtime_error(szErr);
	}*/
	if(pChildNode && bid)
	{
		int nMaxid = 0;
		m_pNode->GetMaxid(&nMaxid);
		char szid[14];
		sprintf(szid,"%d",nMaxid);
		pChildNode->SetAttribute("id",szid);
	}

	return pChildNode;
}

IXMLNode* CElement::GetChildByIndexRept(long lPos)const
{
//	if(NULL == m_pNode)
//		throw std::runtime_error("CElement get child failed,NULL POINTER");
	_CheckPointer(m_pNode,NULL);
	
	IXMLNode* pChildNode = NULL;
	m_pNode->GetChildByIndex(lPos,&pChildNode);
	return pChildNode;


/*	if(NULL == pChildNode)
	{
		char szErr[1024];
		sprintf(szErr,"CElement get child failed, error code:%d",hr);
		throw std::runtime_error(szErr);
	}
	return pChildNode;
*/
	
}

long CElement::GetChildCount()const
{
	_CheckPointer(m_pNode,ERR_NODE_UNEXIST);
	long lCount = 0;
	int hr = m_pNode->GetChildCount(&lCount);
	return hr >=0 ? lCount:(long)hr;

}

IXMLNode* CElement::GetChildByNameIndexRept(const char *szName, long lPos)const
{
//	if(NULL == m_pNode)
//		throw std::runtime_error("CElement get child failed,NULL POINTER");
	_CheckPointer(m_pNode,NULL);
	
	IXMLNode* pChildNode = NULL;
	m_pNode->GetChildByNameIndex(szName,lPos,&pChildNode);
/*	if(NULL == pChildNode)
	{
		char szErr[1024];
		sprintf(szErr,"CElement get child by name failed, error code:%d",hr);
		throw std::runtime_error(szErr);
	}*/
	return pChildNode;
	
}

std::string CElement::GetLocalName()const
{
	_ELEMENT_EXCEPTION(m_pNode);

	std::string strval;
	m_pNode->GetLocalName(strval);
	return strval;
}

std::string CElement::GetPrefix()const
{
	_ELEMENT_EXCEPTION(m_pNode);

	std::string strval;
	m_pNode->GetPrefixName(strval);
	return strval;
}

std::string CElement::GetNodeName()const
{
	_ELEMENT_EXCEPTION(m_pNode);

	std::string strval;
	m_pNode->GetNodeName(strval);
	return strval;
	
}

int CElement::Getid()const
{
	_CheckPointer(m_pNode,ERR_NODE_UNEXIST);
	std::string szval;
	m_pNode->GetAttribute("id",szval);
	return atoi(szval.c_str());
}

int CElement::RemoveChildNode(CElement *element)
{
	_CheckPointer(element,ERR_NODE_UNEXIST);
	return RemoveChildNode(element->m_pNode);
}

int CElement::RemoveChildNode(IXMLNode* pNode)
{
	_CheckPointer(m_pNode,ERR_NODE_UNEXIST);
	if(!pNode)
		return ERR_NODE_UNEXIST;
	m_nerror = m_pNode->RemoveChildNode(pNode);
	return m_nerror;
}

int CElement::RemoveChildByIndex(int nIndex)
{
	try{
		IXMLNode* pChild = GetChildByIndexRept(nIndex);
		if(!pChild)
			return 1;
		m_nerror = RemoveChildNode(pChild);
		pChild->Release();
		return m_nerror;
	}
	catch(std::exception& e)
	{
		std::cout<<"remove child node exception:"<<e.what()<<std::endl;
		m_nerror = ERR_REMOVE_CHILD_EXCEPTION;
	}
	catch (...) {
		m_nerror = ERR_REMOVE_CHILD_EXCEPTION;
	}
	return m_nerror;

}

int CElement::RemoveChildByNameIndex(const char* szname, int nIndex)
{
	try{
		IXMLNode* pChild = GetChildByNameIndexRept(szname,nIndex);
		if(!pChild)
			return 1;
		int nre = RemoveChildNode(pChild);
		pChild->Release();
		return nre;
	}
	catch(std::exception& e)
	{
		std::cout<<"remove child node exception:"<<e.what()<<std::endl;
		m_nerror = ERR_REMOVE_CHILD_EXCEPTION;
	}
	catch (...) {
		m_nerror = ERR_REMOVE_CHILD_EXCEPTION;
	}
	return m_nerror;
}

int CElement::RemoveChildById(int id)
{
	try{
		IXMLNode* pChild = GetChildByIdRept(id);
		if(!pChild)
			return 1;
		int nre = RemoveChildNode(pChild);
		pChild->Release();
		return nre;
	}
	catch(std::exception& e)
	{
		std::cout<<"remove child node exception:"<<e.what()<<std::endl;
		m_nerror = ERR_REMOVE_CHILD_EXCEPTION;
	}
	catch (...) {
		m_nerror = ERR_REMOVE_CHILD_EXCEPTION;
	}
	return m_nerror;
}
/*
bool CElement::IsValid()
{
	return (m_pNode!=NULL);

}
*/
std::string CElement::GetDataString()const
{
	_ELEMENT_EXCEPTION(m_pNode);
	std::string stroutput;
//	if(NULL == m_pNode)
//		throw std::runtime_error("get data string failed,NULL POINTER");
	m_pNode->OutputNodeString(stroutput);
	return stroutput;
}

int CElement::SetTextcontent(const char* szval)
{
	_CheckPointer(m_pNode,ERR_NODE_UNEXIST);
	m_nerror = m_pNode->SetTextcontent(szval);
	return m_nerror;
}

std::string CElement::GetTextcontent()const
{
//	if(NULL == m_pNode)
//		throw std::runtime_error("get textcontent failed,NULL POINTER");
	_CheckPointer(m_pNode,"");
	std::string strval;
	m_pNode->GetTextContent(strval);
	return strval;
}


IXMLNode* CElement::GetChildByIdRept(int id) const
{
	_CheckPointer(m_pNode,NULL);
	
	IXMLNode* pChildNode = NULL;
	m_pNode->GetChildById(id,&pChildNode);
	
	return pChildNode;
	
}

bool CElement::SetChildElementText(const char* szname,const char* szval,long lpos)
{
	IXMLNode* pnode = NULL;
	pnode = GetChildByNameIndexRept(szname);
	if(!pnode)
	{
		try{
		pnode = CreateChildNodeRept(szname,lpos);
		}
		catch(std::exception& e)
		{
			std::cout<<"create child node exception:"<<e.what()<<std::endl;
			m_nerror = ERR_CREATE_CHILD_EXCEPTION;
			return false;
		}
		catch (...) {
			m_nerror = ERR_CREATE_CHILD_EXCEPTION;
			return false;
		}
	}
	if(!pnode)	return false;

//	pnode->AddRef();
	bool breval = pnode->SetTextcontent(szval)>=0 ? true:false;
	pnode->Release();

	return breval;
	
}

std::string CElement::GetChildElementText(const char* szname)const
{
	IXMLNode* pnode = GetChildByNameIndexRept(szname);//NULL;
	if(!pnode)
		return std::string("");
	std::string strval;
	pnode->GetTextContent(strval);
	pnode->Release();
	return strval;
}

//datetime
bool CElement::SetChildElementText(const char* szname,const DATETIME& szval,long lpos)
{
	return SetChildElementText(szname,szval.GetString().c_str(),lpos);
}

DATETIME CElement::GetChildElementTextDatetime(const char* szname)const
{
	return DATETIME(GetChildElementText(szname).c_str());
}

//optional_attr
bool CElement::SetChildElementText(const char* szName,const OPTIONAL_ATTR& szVal,long lpos)
{
	IXMLNode* pnode = GetChildByNameIndexRept(szName);
	if(!pnode)
	{

		try{
			pnode = CreateChildNodeRept(szName,lpos);
		}
		catch(std::exception& e)
		{
			std::cout<<"create child node exception:"<<e.what()<<std::endl;
			m_nerror = ERR_CREATE_CHILD_EXCEPTION;
			return false;
		}
		catch (...) {
			m_nerror = ERR_CREATE_CHILD_EXCEPTION;
			return false;
		}
	}
	if(!pnode)	return false;
	m_nerror = pnode->SetTextcontent(szVal.strValue.c_str());
	bool breval = true;
	if(szVal.bAttribute)
	{
		breval = (pnode->SetAttribute(szVal.strAttrName.c_str(),szVal.strAttrValue.c_str())==0) ? true : false;
	}
	pnode->Release();
	return breval;
}

int CElement::GetChildElementTextOptionalText(const char* szName,OPTIONAL_ATTR& opattrval)const
{
	opattrval.strValue="";
	opattrval.strAttrValue="";
	IXMLNode* pnode = GetChildByNameIndexRept(szName);
	if(!pnode)
		return m_nerror;
	pnode->GetTextContent(opattrval.strValue);
	
	pnode->GetAttribute(opattrval.strAttrName.c_str(),opattrval.strAttrValue);
	if(opattrval.strAttrValue.length() > 0)
		opattrval.bAttribute =true;
	pnode->Release();
	return 0;
}

//usertext
bool CElement::SetChildElementText(const char* szName,const USERTEXT& szVal,long lpos)
{
	return SetChildElementText(szName,(OPTIONAL_ATTR)szVal,lpos);
}

USERTEXT CElement::GetChildElementTextUsertext(const char* szName)const
{
	USERTEXT userval;
	GetChildElementTextOptionalText(szName,userval);
	return userval;
	
}

//scope
bool CElement::SetChildElementText(const char* szName,const SCOPEATTR& szVal,long lpos)
{
	return SetChildElementText(szName,(OPTIONAL_ATTR)szVal,lpos);
}

SCOPEATTR CElement::GetChildElementTextScopetext(const char* szName)const
{
	SCOPEATTR scopeval;
	GetChildElementTextOptionalText(szName,scopeval);
	return scopeval;
	
}

//rational
bool CElement::SetChildElementText(const char* szname,const RATIONAL& szval,long lpos)
{
	return SetChildElementText(szname,szval.GetString().c_str(),lpos);
}

RATIONAL CElement::GetChildElementTextRational(const char* szname)const
{
	return RATIONAL(GetChildElementText(szname).c_str());
}

//Int64
bool CElement::SetChildElementText(const char* szname,const Int64 szval,long lpos)
{
	char sztmp[20];
	SprintfInt64(sztmp,szval);
	return SetChildElementText(szname,sztmp,lpos);
}

Int64 CElement::GetChildElementTextInt64(const char* szname)const
{
	return atoInt64(GetChildElementText(szname).c_str());
}

//long
bool CElement::SetChildElementText(const char* szname,const long szval,long lpos)
{
	char sztmp[20];
	sprintf(sztmp,"%ld",szval);
	return SetChildElementText(szname,sztmp,lpos);
}

long CElement::GetChildElementTextlong(const char* szname)const
{
	return atol(GetChildElementText(szname).c_str());
}

//int
bool CElement::SetChildElementText(const char* szname,const int szval,long lpos)
{
	char sztmp[20];
	sprintf(sztmp,"%d",szval);
	return SetChildElementText(szname,sztmp,lpos);
}

int CElement::GetChildElementTextint(const char* szname)const
{
	return atoi(GetChildElementText(szname).c_str());
}

void CElement::EqualNode(const IXMLNode *pOriNode)
{
	if(m_pNode == pOriNode)
		return;
	if(m_pNode)
		m_pNode->Release();
	if(!pOriNode)
		throw CMyXMLException(ERR_NODE_POINTER_EMPTY,"node pointer is NULL.");
	m_pNode = (IXMLNode*)pOriNode;
	m_pNode->AddRef();

}


long CElement::GetChildCountByName(const char *szName) const
{
	_CheckPointer(m_pNode,ERR_NODE_UNEXIST);
	long lCount = 0;
	int hr = m_pNode->GetChildCountByName(szName,&lCount);
	if(hr < 0)
		return (long)hr;
	return lCount;
	
}


IXMLNode* CElement::InsertChildElementRept(const CElement& element,long lpos)
{
	_ELEMENT_EXCEPTION(m_pNode);

	IXMLNode* pChildNode=NULL;
	m_nerror = m_pNode->InsertChildNode(element.m_pNode,lpos,&pChildNode);

	return pChildNode;
}

IXMLNode* CElement::InsertChildElementRept(const void* pDomNode,long lpos)
{
	_ELEMENT_EXCEPTION(m_pNode);

	IXMLNode* pChildNode = NULL;
	m_nerror = m_pNode->InsertChildNode(pDomNode,lpos,&pChildNode);

	return pChildNode;
}

void* CElement::GetDomElement()
{
	_CheckPointer(m_pNode,NULL);
	void* pNode = NULL;
	m_nerror = m_pNode->GetDomElement(&pNode);
	return pNode;
}

CElement CElement::InsertChildElement(const CElement& element,long lpos)
{
	IXMLNode* pNode = InsertChildElementRept(element,lpos);
	if(!pNode)
		throw CMyXMLException(m_nerror,"construct element failed.");
	return CElement(pNode);

}

CElement CElement::InsertChildElement(const void* pDomNode,long lpos)
{
	IXMLNode* pNode = InsertChildElementRept(pDomNode,lpos);
	if(!pNode)
		throw CMyXMLException(m_nerror,"construct element failed.");
	return CElement(pNode);
}

CElement CElement::CreateChildNode(const char* szName,long lPos,bool bid)
{
	IXMLNode* pNode = CreateChildNodeRept(szName,lPos,bid);
	if(!pNode)
		throw CMyXMLException(m_nerror,"construct element failed.");
	return CElement(pNode);
}

CElement CElement::GetChildByNameIndex(const char* szName,long lPos)const
{
	IXMLNode* pNode = GetChildByNameIndexRept(szName,lPos);
	if(!pNode)
		throw CMyXMLException(m_nerror,"construct element failed.");
	return CElement(pNode);
}

CElement CElement::GetChildByIndex(long lPos)const
{
	IXMLNode* pNode = GetChildByIndexRept(lPos);
	if(!pNode)
		throw CMyXMLException(m_nerror,"construct element failed.");
	return CElement(pNode);
}

CElement CElement::GetChildById(int id)const
{
	IXMLNode* pNode = GetChildByIdRept(id);
	if(!pNode)
		throw CMyXMLException(m_nerror,"construct element failed.");
	return CElement(pNode);
}

