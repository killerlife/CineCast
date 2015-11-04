// XMLNode.cpp : Implementation of CXMLNode

//#include "XMLCore.h"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>

#include "XMLNode.h"
#include <string/mystring.h>
#include <errordef.h>
#include <stdexcept>
#include <iostream>
#include "XMLManage.h"
//using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CXMLNode
void PRINTF_EXCEPTION(const char* szmsg,const XMLCh* szdes=NULL)
{
#ifdef _DEBUG
	if(szmsg)
		XERCES_STD_QUALIFIER cerr<<szmsg;
	if(szdes)
		XERCES_STD_QUALIFIER cerr<<szdes;
	XERCES_STD_QUALIFIER cerr<<XERCES_STD_QUALIFIER endl;

#endif
};

extern const std::string FindNameSpace(const std::string& strval);

#ifdef _DEBUG
static int s_nodecreatecount = 0;
static int s_nodereleasecount = 0;
#endif

CXMLNode::CXMLNode(DOMElement* pEle,CXMLCoreManage* pMng)
: m_pElement(pEle),m_pMng(pMng)
{
	if(!m_pElement || !m_pMng)
		throw std::runtime_error("construct xml node failed: NULL POINTER.");
	m_refCount = 1;
	m_nMaxId = 1;
	m_pMng->AddRef();
	initmap();
#ifdef _DEBUG
	s_nodecreatecount++;
#endif
}

CXMLNode::~CXMLNode()
{
#ifdef _DEBUG
	s_nodereleasecount++;
//	std::cout<<"xml node count ++:"<<s_nodecreatecount<<std::endl;
//	std::cout<<"xml node count --:"<<s_nodereleasecount<<std::endl;
#endif
	m_pMng->Release();
}


int CXMLNode::CompareNodeString(const char* szval1,const char* szval2)
{
#ifdef _WIN32
	return stricmp(szval1,szval2);
#else
	return strcasecmp(szval1,szval2);
#endif
}

void CXMLNode::initmap()
{
	//////////////////////////////////////////////////////////////////////////
	//get max id
	Clear();
	try{
		if(!m_pElement->hasChildNodes())
			return;
		DOMNodeList* pList = reinterpret_cast<DOMNode*>(m_pElement)->getChildNodes();
		long lCount = pList->getLength();

		for(long i=0;i<lCount;i++)
		{
			DOMElement* pNode = reinterpret_cast<DOMElement*>(pList->item(i));
			if(pNode)
			{
				MyStr szstr(pNode->getNodeName());
				const char* szname = szstr.unicodeForm();
//				char* szname = XMLString::transcode(pNode->getNodeName());
				//////////////////////////////////////////////////////////////////////////
				//if node include textcontent,it add a child node whose node name is "#text"
				int cmpResult = CompareNodeString(szname,"#text");
				if( cmpResult == 0)
				{
//					XMLString::release(&szname);
					continue;
				}
				cmpResult = CompareNodeString(szname,"#comment");
//				XMLString::release(&szname);
				if( cmpResult == 0)
					continue;

				MyWStr szid("id");
				if(pNode->hasAttribute(szid.unicodeForm()))
				{
					MyStr szval(pNode->getAttribute(szid.unicodeForm()));
					int id =atoi(szval.unicodeForm());
					if(m_nMaxId <= id)
						m_nMaxId = id + 1;
				}
			}
		}
	}
	catch(const OutOfMemoryException& e)
	{
		MyStr szerr(e.getMessage());
		throw std::runtime_error(std::string("OutOfMemoryException:") + szerr.unicodeForm());
	}
	catch(const DOMException& e)
	{
		MyStr szerr(e.getMessage());
		throw std::runtime_error(std::string("dom exception:") + szerr.unicodeForm());
	}
	catch (...) {
		//generate exception.
		throw std::runtime_error("unkown exception.");
	}
}

void CXMLNode::Clear()
{
}

int CXMLNode::SetAttribute(/*[in]*/const char* szName,/*[in]*/const char* szVal)
{
	try{
		MyWStr wszname(szName);
		MyWStr wszval(szVal);
		m_pElement->setAttribute(wszname.unicodeForm(),wszval.unicodeForm());
		return 0;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("set attribute failed.");
//		throw std::runtime_error("set attribute failed.");
	}

	return ERR_SET_ATTRIBUTE_FAILED;
}

int CXMLNode::GetAttribute(/*[in]*/const char* szName,/*[out]*/std::string& szVal)
{
	try
	{
		MyWStr wszname(szName);
		MyStr sztmp(m_pElement->getAttribute(wszname.unicodeForm()));
		szVal = sztmp.unicodeForm();
		return 0;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
//		throw std::runtime_error("get attribute failed.");
		PRINTF_EXCEPTION("unknown exception:");
	}
	
	return ERR_GET_ATTRIBUTE_FAILED;
}

int CXMLNode::RemoveAttribute(/*[in]*/const char* szName)
{
	try{
		MyWStr wszname(szName);
		m_pElement->removeAttribute(wszname.unicodeForm());
		return 0;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
//		throw std::runtime_error("remove attribute failed.");
		PRINTF_EXCEPTION("unknown exception:");
	}
	return ERR_REMOVE_ATTRIBUTE_FAILED;
}

int CXMLNode::Clone(/*[out]*/IXMLNode** ppNode)
{
	DOMElement* pnewNode = (DOMElement*)m_pElement->cloneNode(true);
	*ppNode = NULL;
	if(pnewNode)
	{
		CXMLNode* pnewXml = new CXMLNode(pnewNode,m_pMng);
		*ppNode  = (IXMLNode*)pnewXml;
	}
	return 0;
}

int CXMLNode::CreateChildNode(/*[in]*/const char* szName,/*[in]*/long lPos,/*[out]*/IXMLNode** ppNode)
{
	try{
		
		MyWStr wszname(szName);
		DOMDocument* doc = m_pElement->getOwnerDocument();
		DOMElement* pele = doc->createElement(wszname.unicodeForm());
//		CXMLNode* pTmp = GetChildByIndex(lPos);
		DOMElement* pTmp = GetChildDOMByIndex(lPos);
		//long lCurPos = lPos;
		bool bAdd =false;
		if(pTmp)
		{
			m_pElement->insertBefore(pele,pTmp);
//			pTmp->Release();
		}
		else
		{
			bAdd = true;
			m_pElement->appendChild(pele);
		}
		CXMLNode* pNew = new CXMLNode(pele,m_pMng);
		
		*ppNode = reinterpret_cast<IXMLNode*>(pNew);
		return 0;
	}
	catch(const OutOfMemoryException& e)
	{
		PRINTF_EXCEPTION("OutOfMemoryException:",e.getMessage());
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
//		throw std::runtime_error("create child node generate exception.");
		PRINTF_EXCEPTION("unknown exception:");
	}


	return ERR_CREATE_CHILDNODE_FAILED;
}

int CXMLNode::RemoveChildNode(/*[in]*/IXMLNode* ppNode)
{

	try{
		if(!m_pElement->hasChildNodes())
			return ERR_NODE_UNEXIST;
		DOMNodeList* pList = reinterpret_cast<DOMNode*>(m_pElement)->getChildNodes();
		long lCount = pList->getLength();
		CXMLNode* pDelNode = reinterpret_cast<CXMLNode*>(ppNode);
		for(long i=0;i<lCount;i++)
		{
			DOMElement* pNode = reinterpret_cast<DOMElement*>(pList->item(i));
			if(pNode == pDelNode->m_pElement)
			{
				m_pElement->removeChild(reinterpret_cast<DOMNode*>(pDelNode->m_pElement));
				return 0;
			}
		}
		
	}
	catch(const OutOfMemoryException& e)
	{
		PRINTF_EXCEPTION("OutOfMemoryException:",e.getMessage());
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
//		throw std::runtime_error("remove child node gerenate exception.");
		PRINTF_EXCEPTION("unknown exception:");
		return ERR_REMOVE_NODE_EXCEPTION;
	}


	return ERR_NODE_UNEXIST;
}

int CXMLNode::GetChildCount(/*[out]*/long* plCount)
{
	*plCount = 0;
	try{
		if(!m_pElement->hasChildNodes())
			return 0;
		DOMNodeList* pList = reinterpret_cast<DOMNode*>(m_pElement)->getChildNodes();
		long lCount = pList->getLength();
		for(long i=0;i<lCount;i++)
		{
			DOMElement* pNode = reinterpret_cast<DOMElement*>(pList->item(i));
			if(pNode)
			{
				MyStr szstr(pNode->getNodeName());
				const char* szname = szstr.unicodeForm();
				//				char* szname = XMLString::transcode(pNode->getNodeName());
				//////////////////////////////////////////////////////////////////////////
				//if node include textcontent,it add a child node whose node name is "#text"
				int cmpResult = CompareNodeString(szname,"#text");
				if( cmpResult == 0)
				{
					continue;
				}
				cmpResult = CompareNodeString(szname,"#comment");
				if( cmpResult == 0)
					continue;
				(*plCount)++;
			}
		}
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
		return ERR_GET_CHILDCOUNT_EXCEPTION;
	}

	return 0;
/*	*plCount = 0;
	if(!m_pElement->hasChildNodes())
		return 0;
	DOMNodeList* pList = reinterpret_cast<DOMNode*>(m_pElement)->getChildNodes();
	*plCount = pList->getLength();
	return 0;*/
}

int CXMLNode::GetChildByIndex(/*[in]*/long lPos,/*[out]*/IXMLNode** ppNode)
{
	*ppNode = reinterpret_cast<IXMLNode*>(GetChildByIndex(lPos));
	
	return *ppNode ? 0:ERR_GET_CHILD_FAILED;
}

int CXMLNode::GetChildByNameIndex(/*[in]*/const char* szName,/*[in]*/long lPos,/*[out]*/IXMLNode** ppNode)
{
	*ppNode = GetChildByIndex(lPos,szName);
	return *ppNode ? 0:ERR_GET_CHILD_FAILED;
}

int CXMLNode::GetPrefixName(std::string& szName)
{
	try
	{
		MyStr szval(m_pElement->getNodeName());
		szName = FindNameSpace(szval.unicodeForm());
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
		return ERR_GET_PREFIXNAME_FAILED;
	}

	return 0;

}

int CXMLNode::GetLocalName(std::string& szName)
{
	try
	{
		MyStr szval(m_pElement->getNodeName());
		std::string tmpName = szval.unicodeForm();
		int nfind = tmpName.find_first_of(':');
		if(nfind > 0)
			tmpName = tmpName.substr(nfind+1);
		szName = tmpName;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
		return ERR_GET_LOCALNAME_FAILED;
	}

	return 0;

}

int CXMLNode::GetNodeName(/*[out]*/std::string& szName)
{
	try
	{
		MyStr szval(m_pElement->getNodeName());
		szName = szval.unicodeForm();
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
		return ERR_GET_NODENAME_FAILED;
	}
	return 0;

}

int CXMLNode::OutputNodeString(std::string& stroutput)
{
	try{
		MyWStr sztmp("LS");
		DOMImplementation *impl          = DOMImplementationRegistry::getDOMImplementation(sztmp.unicodeForm());
		if(impl == NULL)
			return ERR_OUTPUT_NODESTRING_FAILED;
		DOMWriter         *theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
		if(theSerializer == NULL)
			return ERR_OUTPUT_NODESTRING_FAILED;
		
		// set user specified output encoding
		theSerializer->setEncoding(0);
		
		XMLCh* chData = theSerializer->writeToString(*m_pElement);
		MyStr szdata(chData);
		stroutput = szdata.unicodeForm();
		XMLString::release(&chData);
		
		delete theSerializer;
		
	}
	catch(const OutOfMemoryException& e)
	{
		PRINTF_EXCEPTION("OutOfMemoryException:",e.getMessage());
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
		return ERR_OUTPUT_NODESTRING_FAILED;
	}
	return 0;
}

int CXMLNode::GetMaxid(/*[out]*/int* pnMaxid)
{
	*pnMaxid = m_nMaxId;
	m_nMaxId++;
	return 0;
}

int CXMLNode::SetTextcontent(const char* sztext)
{
	try{
		MyWStr wsztext(sztext);
		m_pElement->setTextContent(wsztext.unicodeForm());
		return 0;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
	}
	return ERR_SET_TEXTCONTENT_FAILED;
}

int CXMLNode::GetTextContent(std::string& sztext)
{
	try{
		MyStr szval(m_pElement->getTextContent());
		sztext = szval.unicodeForm();
		return 0;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
	}
	return ERR_GET_TEXTCONTENT_FAILED;
}

int CXMLNode::GetChildCountByName(const char* szName,long* plCount)
{
	*plCount = 0;
	try{
		if(!szName || !m_pElement->hasChildNodes())
			return 0;
		DOMNodeList* pList = reinterpret_cast<DOMNode*>(m_pElement)->getChildNodes();
		long lCount = pList->getLength();
		MyWStr strInput(szName);
		XMLCh* strnewName = NULL;//strInput.unicodeForm();
//		bool bFound = false;
		for(long i=0;i<lCount;i++)
		{
			DOMElement* pNode = reinterpret_cast<DOMElement*>(pList->item(i));
			if(pNode)
			{
				const XMLCh* wsznodename = pNode->getNodeName();
				if(strnewName)
				{
					if(XMLString::compareString(wsznodename,strnewName) == 0)
					{
						(*plCount)++;
						continue;
					}
				}
				else if(QueryNamespace(wsznodename,szName))
				{
					int nlen = XMLString::stringLen(wsznodename);
					strnewName = new XMLCh[nlen + 1];
					XMLString::copyString(strnewName,wsznodename);
//					strnewName = (XMLCh*)wsznodename;
					(*plCount)++;
				}
//				MyStr sznodename(pNode->getNodeName());
//				if( strcmp(sznodename.unicodeForm(),szName) == 0)
//					(*plCount)++;
			}
		}
		if(strnewName)
		{
			delete []strnewName;
			strnewName = NULL;
		}
//			XMLString::release(&strnewName);
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
		return ERR_GET_CHILDCOUNT_EXCEPTION;
	}
	
	return 0;
}

DOMElement* CXMLNode::GetChildDOMByIndex(int lpos,const char* szName)
{
	try{
		if(!m_pElement->hasChildNodes())
			return NULL;
		DOMNodeList* pList = reinterpret_cast<DOMNode*>(m_pElement)->getChildNodes();
		long lCount = pList->getLength();
		int nItem = 0;
		for(long i=0;i<lCount;i++)
		{
			DOMElement* pNode = reinterpret_cast<DOMElement*>(pList->item(i));
			if(pNode)
			{
				if(szName)
				{
					if(QueryNamespace(pNode->getNodeName(),szName))
					{
						if(nItem == lpos)
							return pNode;
						nItem++;
					}
				}
				else
				{
					if(nItem == lpos)
						return pNode;
					nItem++;
				}
			}
		}
	}
	catch(const OutOfMemoryException& e)
	{
		MyStr szerr(e.getMessage());
		std::string strerr = std::string("OutOfMemoryException:") + szerr.unicodeForm();
		throw std::runtime_error(strerr);
	}
	catch(const DOMException& e)
	{
		MyStr szerr(e.getMessage());
		std::string strerr = std::string("dom exception:") + szerr.unicodeForm();
		throw std::runtime_error(strerr);
	}
	catch (...) {
		throw std::runtime_error("get child node exception.");
	}

	return NULL;
}

CXMLNode* CXMLNode::GetChildByIndex(long lPos,const char* szName)
{
/*	try{
		if(!m_pElement->hasChildNodes())
			return NULL;
		DOMNodeList* pList = reinterpret_cast<DOMNode*>(m_pElement)->getChildNodes();
		long lCount = pList->getLength();
		int nItem = 0;
		for(long i=0;i<lCount;i++)
		{
			DOMElement* pNode = reinterpret_cast<DOMElement*>(pList->item(i));
			if(pNode)
			{
				if(szName)
				{
					if(QueryNamespace(pNode->getNodeName(),szName))
//					MyStr sznodename(pNode->getNodeName());
//					if( strcmp(sznodename.unicodeForm(),szName) == 0)
					{
						if(nItem == lPos)
						{
							return new CXMLNode(pNode,m_pMng);
						}
						nItem++;
					}
				}
				else
				{
					if(nItem == lPos)
						return new CXMLNode(pNode,m_pMng);
					nItem++;
				}
			}
		}
	}
	catch(const OutOfMemoryException& e)
	{
		MyStr szerr(e.getMessage());
		std::string strerr = std::string("OutOfMemoryException:") + szerr.unicodeForm();
		throw std::runtime_error(strerr);
	}
	catch(const DOMException& e)
	{
		MyStr szerr(e.getMessage());
		std::string strerr = std::string("dom exception:") + szerr.unicodeForm();
		throw std::runtime_error(strerr);
	}
	catch (...) {
		throw std::runtime_error("get child node exception.");
	}*/
	DOMElement* pNode = GetChildDOMByIndex(lPos,szName);
	if(pNode)
		return new CXMLNode(pNode,m_pMng);

	return NULL;
	
}

int CXMLNode::GetChildById(int id, IXMLNode **ppNode)
{
	try{
		if(!m_pElement->hasChildNodes())
			return ERR_GET_CHILD_FAILED;
		DOMNodeList* pList = reinterpret_cast<DOMNode*>(m_pElement)->getChildNodes();
		long lCount = pList->getLength();
		MyWStr wszid("id");
		for(long i=0;i<lCount;i++)
		{
			DOMElement* pNode = reinterpret_cast<DOMElement*>(pList->item(i));
			if(pNode && pNode->hasAttribute(wszid.unicodeForm()))
			{
				MyStr szid(pNode->getAttribute(wszid.unicodeForm()));
				int idtmp = atoi(szid.unicodeForm());
				if(idtmp == id )
				{
					CXMLNode* pGetNode = new CXMLNode(pNode,m_pMng);
					*ppNode = pGetNode;
					return 0;
				}
			}
		}
	}
	catch(const OutOfMemoryException& e)
	{
		PRINTF_EXCEPTION("OutOfMemoryException:",e.getMessage());
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
		return ERR_GET_CHILD_EXCEPTION;
	}
	
	return ERR_GET_CHILD_FAILED;
}

DOMElement* CXMLNode::GetXMLElementpt()const
{
	return m_pElement;
}

int CXMLNode::InsertChildNode(const IXMLNode* pInputNode,long lPos,IXMLNode** ppNode)
{
	try{
		DOMElement* pTmpDom = ((CXMLNode*)pInputNode)->GetXMLElementpt();
//		CXMLNode* pNew = (CXMLNode*)pNode;
		DOMDocument* doc = m_pElement->getOwnerDocument();
		DOMNode* pele = doc->importNode((DOMNode*)pTmpDom,true);//pNew->GetXMLElementpt();
//		CXMLNode* pTmp = GetChildByIndex(lPos);
		DOMElement* pTmp = GetChildDOMByIndex(lPos);
		//long lCurPos = lPos;
		bool bAdd =false;
		if(pTmp)
		{
			m_pElement->insertBefore(pele,pTmp);
		}
		else
		{
			bAdd = true;
			m_pElement->appendChild(pele);
		}
		CXMLNode* pNew = new CXMLNode((DOMElement*)pele,m_pMng);
		
		*ppNode = reinterpret_cast<IXMLNode*>(pNew);
		return 0;
	}
	catch(const OutOfMemoryException& e)
	{
		PRINTF_EXCEPTION("OutOfMemoryException:",e.getMessage());
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
	}


	return ERR_INSERT_CHILDNODE_FAILED;
}

int CXMLNode::InsertChildNode(const void* pDomNode,long lPos,IXMLNode** ppNode)
{
	try{
		DOMDocument* doc = m_pElement->getOwnerDocument();

		DOMElement* pTmpDom = (DOMElement*)pDomNode;
		DOMNode* pele = doc->importNode((DOMNode*)pTmpDom,true);

//		CXMLNode* pTmp = GetChildByIndex(lPos);
		DOMElement* pTmp = GetChildDOMByIndex(lPos);
		bool bAdd =false;
		if(pTmp)
		{
			m_pElement->insertBefore(pele,pTmp);
		}
		else
		{
			bAdd = true;
			m_pElement->appendChild(pele);
		}
		
		CXMLNode* pNew = new CXMLNode((DOMElement*)pele,m_pMng);
		*ppNode = reinterpret_cast<IXMLNode*>(pNew);
		return 0;
	}
    catch (const OutOfMemoryException& e)
    {
		PRINTF_EXCEPTION("OutOfMemoryException:",e.getMessage());
		return ERR_OUTOFMEMORY_EXCEPTION;
    }
    catch (const DOMException& e)
    {
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
    }
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
	}
	return ERR_INSERT_CHILDNODE_FAILED;
}

int CXMLNode::GetDomElement(void** ppDomNode)
{
	*ppDomNode = (void*)m_pElement;
	return 0;
}

int CXMLNode::GetAttributeCount(long* plcount)
{
	DOMNamedNodeMap* plistattr = m_pElement->getAttributes();
	*plcount = plistattr->getLength();
	return 0;
}

int CXMLNode::GetAttributeByNameIndex(int nIndex,std::string& strname,std::string& strval)
{
	try{
		DOMNamedNodeMap* plistattr = m_pElement->getAttributes();
		DOMAttr* pReAttr = NULL;
		if(strname.size() <= 0)
		{//属性名为空，直接根据序号获取
			if(nIndex >= 0 && nIndex < (int)plistattr->getLength())
				pReAttr = reinterpret_cast<DOMAttr*>(plistattr->item(nIndex));
		}
		else{
			MyWStr wstrtmp(strname.c_str());
			if(m_pElement->hasAttribute(wstrtmp.unicodeForm()))
			{//存在该属性，根据所给名称获取属性
				MyStr valtmp(m_pElement->getAttribute(wstrtmp.unicodeForm()));
				strval = valtmp.unicodeForm();
				return 0;
			}
			//不存在该属性，给所给属性名称前面增加名字空间，轮寻，找到第一个跳出
			int nItem = 0;
			for(XMLSize_t i=0;i<plistattr->getLength();i++)
			{
				DOMAttr* pattr = reinterpret_cast<DOMAttr*>(plistattr->item(i));
				if(QueryNamespace(pattr->getName(),strname))
				{
					if(nItem == nIndex)
					{
						pReAttr = pattr;
						break;
					}
					nItem ++;
				}
			}
		}
		if(pReAttr)
		{
			MyStr szname(pReAttr->getName());
			MyStr szval(pReAttr->getValue());
			strname = szname.unicodeForm();
			strval = szval.unicodeForm();
			return 0;
		}
	}
	catch(const OutOfMemoryException& e)
	{
		PRINTF_EXCEPTION("OutOfMemoryException:",e.getMessage());
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch(const DOMException& e)
	{
		PRINTF_EXCEPTION("dom exception:",e.getMessage());
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		PRINTF_EXCEPTION("unknown exception:");
	}
	return ERR_GET_ATTRIBUTE_FAILED;
}

bool CXMLNode::QueryNamespace(const XMLCh* xszname,const std::string strname)
{
	MyStr strtmp(xszname);
	if(strname == strtmp.unicodeForm())
		return true;

	std::string strtmpname = strname;
	int nfind = strtmpname.find_first_of(':');
	if(nfind > 0)
		strtmpname = strtmpname.substr(nfind+1);
	std::map<std::string,std::string>& mapNamespace = m_pMng->GetNamespace();
	std::map<std::string,std::string>::iterator ite = mapNamespace.begin();
	while (ite != mapNamespace.end())
	{
		std::string strnew = ite->first + ":";
		strnew += strtmpname;
		if(strnew == strtmp.unicodeForm())
			return true;
		ite++;
	}
	return false;

}

int CXMLNode::Save(const char* szFileName)
{
	return m_pMng->SaveXMLFile(szFileName,this);

}
