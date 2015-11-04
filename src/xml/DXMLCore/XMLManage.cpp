// XMLManage.cpp : Implementation of CXMLCoreManage
//#include "XMLCore.h"
#include "XMLManage.h"
#include <string/mystring.h>
#include <errordef.h>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/SAXException.hpp>

/////////////////////////////////////////////////////////////////////////////
// CXMLCoreManage
/*
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMTreeErrorReporter.hpp>

#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <xercesc/util/OutOfMemoryException.hpp>
*/
#include "XMLNode.h"
#include <iostream>
#include <MyXMLException.h>

extern void PRINTF_EXCEPTION(const char* szmsg,const XMLCh* szdes=NULL);

static int sgl_initRefCount = 0;

void Terminate()
{
	sgl_initRefCount--;

	if(sgl_initRefCount == 0)
		XMLPlatformUtils::Terminate();
}

void InitXMLPlatInfo()
{
	if(sgl_initRefCount <= 0)
	{

		try
		{
			XMLPlatformUtils::Initialize();
		}
		catch(const XMLException& e){
			MyStr szval(e.getMessage());
			std::string strerr = std::string("initialize exception:") + szval.unicodeForm();
			std::cout<<"xml exception:"<<strerr.c_str()<<std::endl;
			throw std::runtime_error(strerr);
		}
		catch(...){
			std::cout<<"initialize xerces-c failed"<<std::endl;
			throw std::runtime_error("initialize xerces-c failed");
		}
		sgl_initRefCount = 0;
	}
	sgl_initRefCount++;

}

IXMLManage* Createxml(const char* szname,const char* sznamespace)
{
	CXMLCoreManage* pManage = new CXMLCoreManage;
	std::string errinfo;
	int nre = pManage->CreateXMLDoc(szname,sznamespace,errinfo);
	if(nre < 0)
	{
		delete pManage;
		throw CMyXMLException(nre,errinfo);
//		char szerr[255];
//		sprintf(szerr,"create xml failed, error code: %d,descriptor£º%s",nre,errinfo.c_str());
//		throw std::runtime_error(szerr);
	}
	return (IXMLManage*)pManage;
}

IXMLManage* Parsexml(const char* szdata,int nFlag)
{
	CXMLCoreManage* pManage = new CXMLCoreManage;
	int nre = -1;
	std::string errinfo;
	if(nFlag == 0)//open file
	{
		nre = pManage->OpenXMLFile(szdata,errinfo);
	}
	else//parse 
	{
		nre = pManage->prase(szdata,errinfo);
	}

	if(nre < 0)
	{
		delete pManage;
		throw CMyXMLException(nre,errinfo);
//		char szerr[255];
//		sprintf(szerr,"parse xml failed, error code: %d, descriptor£º%s",nre,errinfo.c_str());
//		throw std::runtime_error(szerr);
	}
	return (IXMLManage*)pManage;
}

/*
IXMLManage*  createXMLManage()
{
	CXMLCoreManage* pManage = new CXMLCoreManage;
	return reinterpret_cast<IXMLManage*>(pManage);
}

void releaseXMLManage(IXMLManage* pManage)
{
	delete reinterpret_cast<CXMLCoreManage*>(pManage);
}
*/

const std::string FindNameSpace(const std::string& strval)
{
	int nfind = strval.find_last_of(':');
	if(nfind <= 0)
		return "";
	return strval.substr(0,nfind);

}
#ifdef _DEBUG
static int s_mngcreatecount = 0;
static int s_mngreleasecount = 0;
#endif
//////////////////////////////////////////////////////////////////////////
//CXMLCoreManage cpp
CXMLCoreManage::CXMLCoreManage()
{
	m_pDoc = NULL;
	m_pParser = NULL;
	m_lRefCount = 0;
	InitXMLPlatInfo();
#ifdef _DEBUG
	s_mngcreatecount++;
	std::cout<<"xml manage count++:"<<s_mngcreatecount<<std::endl;
#endif
}

 CXMLCoreManage::~CXMLCoreManage()
{
	FreeXMLResource();

#ifdef _DEBUG
	s_mngreleasecount++;
	std::cout<<"xml manage count++:"<<s_mngcreatecount<<std::endl;
	std::cout<<"xml manage count--:"<<s_mngreleasecount<<std::endl;
#endif
	Terminate();
}

int CXMLCoreManage::CreateXMLDoc(const char* szName,const char* szNamespace,std::string& errinfo)
{
	// TODO: Add your implementation code here
	try
	{
		FreeXMLResource();
		MyWStr wszval("Core");
		m_strDefaultNamespace = FindNameSpace(szName);
		DOMImplementation *impl =  DOMImplementationRegistry::getDOMImplementation(wszval.unicodeForm());
		if (impl != NULL)
		{

			if(szNamespace)
				parseNamespace(szNamespace);
			MyWStr wszname(szName);
			if(m_strDefaultNamespace.size() > 0)
			{
				MyWStr wszurl(m_mapNamespace[m_strDefaultNamespace].c_str());
				m_pDoc = impl->createDocument(wszurl.unicodeForm(),wszname.unicodeForm(),0);
			}
			else
			{
				m_pDoc = impl->createDocument(
					0,                    // root element namespace URI.
					wszname.unicodeForm(),// root element name
					0);                   // document type object (DTD).
			}
			if(m_pDoc == NULL)
			{
				errinfo = "create document failed.";
				return ERR_CREATE_DOC_FAILED;
			}
			DOMElement* pRoot = m_pDoc->getDocumentElement();
			if(!pRoot)
			{
				errinfo = "when creating, get root node failed.";
				return ERR_GET_ROOTNODE_FAILED;
			}
			std::map<std::string,std::string>::iterator ite = m_mapNamespace.begin();
			while (ite != m_mapNamespace.end())
			{
				if(ite->first != m_strDefaultNamespace)
				{
					MyWStr wszOtherurl(ite->second.c_str());
					std::string name = "xmlns:" + ite->first;
					MyWStr wszNamespace(name.c_str());
					pRoot->setAttribute(wszNamespace.unicodeForm(),wszOtherurl.unicodeForm());
				}
				ite++;
			}
			return 0;
		}
	}  // (inpl != NULL)
	catch(const OutOfMemoryException& e)
	{
		MyStr szerr(e.getMessage());
		errinfo = std::string("creating generate OutOfMemoryException:") + szerr.unicodeForm();
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch(const DOMException& e)
	{
		MyStr szerr(e.getMessage());
		errinfo = std::string("creating generate dom exception:") + szerr.unicodeForm();
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		//generate exception.
		errinfo = "creating generate unkown exception.";
		return ERR_UNKNOWN_EXCEPTION;
	}
	

	errinfo = "create document failed.";
	return ERR_CREATE_DOC_FAILED;
}

int CXMLCoreManage::OpenXMLFile(const char* szFileName,std::string& errinfo)
{
	// TODO: Add your implementation code here
	//XERCES_STD_QUALIFIER cout << "CXMLCoreManage::OpenXMLFile try open xml file "<< szFileName << XERCES_STD_QUALIFIER endl;
	
    try
    {
		FreeXMLResource();
		MyWStr wszfilename(szFileName);
		m_pParser = new XercesDOMParser;
		m_pParser->parse(wszfilename.unicodeForm());
//		MyWStr sztmp("LS");
//		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(sztmp.unicodeForm());

		// get the DOM representation
		m_pDoc = m_pParser->getDocument();
		if(m_pDoc == NULL)
		{
			errinfo = "open xml file failed.";
			return ERR_LOAD_XMLFILE_FAILED;
		}
		DOMElement* pRoot = m_pDoc->getDocumentElement();
		if(!pRoot)
		{
			errinfo = "when opening file, get root node failed.";
			return ERR_GET_ROOTNODE_FAILED;
		}
		parseNamespace(m_pDoc,m_strDefaultNamespace,m_mapNamespace);
    }
	catch(const OutOfMemoryException& e)
	{
		MyStr szerr(e.getMessage());
		errinfo = std::string("opening file generate OutOfMemoryException:") + szerr.unicodeForm();
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch (const SAXException& e) {
		MyStr szerr(e.getMessage());
		errinfo = std::string("opening file generate sax exception:") + szerr.unicodeForm();
		return ERR_SAX_EXCEPTION;
	}
	catch (const XMLException& e) {
		MyStr szerr(e.getMessage());
		errinfo = std::string("opening file generate xml exception:") + szerr.unicodeForm();
		return ERR_XML_EXCPETION;
	}
	catch(const DOMException& e)
	{
		MyStr szerr(e.getMessage());
		errinfo = std::string("opening file generate dom exception:") + szerr.unicodeForm();
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		//generate exception.
		errinfo = "opening file generate unkown exception.";
		return ERR_UNKNOWN_EXCEPTION;
	}
     
	return 0;
}

int CXMLCoreManage::SaveXMLFile(const char* szFileName,const IXMLNode* pNode)
{
	// TODO: Add your implementation code here
	try{
		MyWStr sztmp("LS");
		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(sztmp.unicodeForm());
		if(impl == NULL)
			return ERR_SAVE_XMLFILE_FAILED;
		DOMWriter *theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
		if(theSerializer == NULL)
			return ERR_SAVE_XMLFILE_FAILED;
		// set user specified output encoding
		theSerializer->setEncoding(0);
		// StdOutFormatTarget prints the resultant XML stream
		// to stdout once it receives any thing from the serializer.
		//
		MyWStr wszfilename(szFileName);
		XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(wszfilename.unicodeForm());
		CXMLNode* pxmlNode = (CXMLNode*)pNode;
		DOMElement* pwriteNode = pxmlNode->GetXMLElementpt();
		DOMElement* pRoot = m_pDoc->getDocumentElement();
		if(pwriteNode != pRoot)
		{
			pwriteNode = (DOMElement*)pxmlNode->GetXMLElementpt()->cloneNode(true);

			std::map<std::string,std::string>::iterator ite = m_mapNamespace.begin();
			while (ite != m_mapNamespace.end())
			{
				MyWStr wszOtherurl(ite->second.c_str());
				std::string name = "xmlns:" + ite->first;
				MyWStr wszNamespace(name.c_str());
				pwriteNode->setAttribute(wszNamespace.unicodeForm(),wszOtherurl.unicodeForm());
				ite++;
			}
		}
		theSerializer->writeNode(myFormTarget, *(pwriteNode->getOwnerDocument()));
		theSerializer->release();
//		delete theSerializer;
		delete myFormTarget;
	}
	catch(const OutOfMemoryException& e)
	{
		PRINTF_EXCEPTION("saving file generate OutOfMemoryException:",e.getMessage());
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch(const XMLException& e)
	{
		PRINTF_EXCEPTION("saving file generate xml exception:",e.getMessage());
		return ERR_XML_EXCPETION;
	}
	catch(...)
	{
		PRINTF_EXCEPTION("saving file generate unknown exception.");
		return ERR_SAVE_XMLFILE_FAILED;
	}

	return 0;
}

int CXMLCoreManage::GetRootNode(IXMLNode **ppNode)
{
	// TODO: Add your implementation code here
	*ppNode = NULL;
	if(!m_pDoc)
		return 0;
	DOMElement* pRoot = NULL;
	try{
		pRoot = m_pDoc->getDocumentElement();
	}
	catch (...) {
		return ERR_GET_ROOTNODE_FAILED;
	} 
	
	if(!pRoot)
	{
		return ERR_GET_ROOTNODE_FAILED;
	}
	CXMLNode* pRootNode = new CXMLNode(pRoot,this);
	
	*ppNode = (IXMLNode*)pRootNode;
	
	return 0;
}

void CXMLCoreManage::FreeXMLResource()
{
	m_strDefaultNamespace = "";
	m_mapNamespace.clear();
	if(m_pParser)
	{
		delete m_pParser;
		m_pParser = NULL;
		m_pDoc = NULL;
	}
	else if(m_pDoc)
	{
		m_pDoc->release();
		m_pDoc = NULL;
	}
	
}

int CXMLCoreManage::prase(const char* stringbuf,std::string& errinfo)
{
	FreeXMLResource();
	int nlen = strlen(stringbuf);
	try{
		m_pParser = new XercesDOMParser;
		MemBufInputSource* pmemData = new MemBufInputSource((const XMLByte* const)stringbuf,nlen+1,"LS");
		m_pParser->parse(*pmemData);
		delete pmemData;
//		MyWStr sztmp("LS");
//		m_impl = DOMImplementationRegistry::getDOMImplementation(sztmp.unicodeForm());
		
		// get the DOM representation
		m_pDoc = m_pParser->getDocument();
		if(m_pDoc == NULL)
		{
			errinfo = "parse string failed.";
			return ERR_PARSE_STRING_FAILED;
		}
		DOMElement* pRoot = m_pDoc->getDocumentElement();
		if(!pRoot)
		{
			errinfo = "when parsing string, get root node failed.";
			return ERR_GET_ROOTNODE_FAILED;
		}
		parseNamespace(m_pDoc,m_strDefaultNamespace,m_mapNamespace);
	}
	catch(const OutOfMemoryException& e)
	{
		MyStr szerr(e.getMessage());
		errinfo = std::string("parsing string generate OutOfMemoryException:") + szerr.unicodeForm();
		return ERR_OUTOFMEMORY_EXCEPTION;
	}
	catch (const SAXException& e) {
		MyStr szerr(e.getMessage());
		errinfo = std::string("parsing string generate sax exception:") + szerr.unicodeForm();
		return ERR_SAX_EXCEPTION;
	}
	catch (const XMLException& e) {
		MyStr szerr(e.getMessage());
		errinfo = std::string("parsing string generate xml exception:") + szerr.unicodeForm();
		return ERR_XML_EXCPETION;
	}
	catch(const DOMException& e)
	{
		MyStr szerr(e.getMessage());
		errinfo = std::string("parsing string generate dom exception:") + szerr.unicodeForm();
		return ERR_DOM_EXCEPTION;
	}
	catch (...) {
		//generate exception.
		errinfo = "parsing string generate unkown exception.";
		return ERR_UNKNOWN_EXCEPTION;
	}
	return 0;
}

void CXMLCoreManage::parseNamespace(const DOMDocument* pDoc,std::string& strDefault,std::map<std::string,std::string>& mapNamespace)
{
	mapNamespace.clear();
	if(!pDoc)
		return;
	DOMElement* pRoot = pDoc->getDocumentElement();
	if(!pRoot)
		return;

	MyStr szname(pRoot->getNodeName());
	strDefault = FindNameSpace(szname.unicodeForm());
	DOMNamedNodeMap* plistattr = pRoot->getAttributes();
	for(XMLSize_t i=0;i<plistattr->getLength();i++)
	{
		DOMAttr* pattr = reinterpret_cast<DOMAttr*>(plistattr->item(i));
		MyStr szname(pattr->getName());
//		MyStr szPrefix(pattr->getPrefix());
//		MyStr szName(pattr->getLocalName());
		std::string strname = szname.unicodeForm();
		if(strname.substr(0,6) == "xmlns:")
		{
			std::string strnamespace = strname.substr(6);
			MyStr szval(pattr->getValue());
			std::string strurl = szval.unicodeForm();
			mapNamespace.insert(std::pair<std::string,std::string>(strnamespace,strurl));
		}
	}
}

void CXMLCoreManage::parseNamespace(const std::string& szval)
{
	if(szval.c_str() <= 0)
		return;
	int nfind = szval.find("xmlns:");
	int nlen = 6;
	std::string szleft = szval.substr(nfind + nlen);
	while (nfind >= 0)
	{
		nfind = szleft.find("xmlns:");
		if(nfind >= 0)
		{
			std::string szdata = szleft.substr(0,nfind);
			insertmap(szdata);
			szleft = szleft.substr(nfind + nlen);
		}
		else
		{
			//end namespace
			std::string szdata = szleft.substr(0,nfind);
			insertmap(szdata);
		}
	}
	
}

void CXMLCoreManage::insertmap(const std::string& strdata)
{
//	int nlen = strdata.size();
	int nfind = strdata.find_first_of('=');
	if(nfind <= 0)
		return ;

	std::string strnamespace = strdata.substr(0,nfind);
	std::string strurl = strdata.substr(nfind+1);
	unsigned int i = strurl.size()-1;
	while (i >=0 )
	{
		if(strurl[i] != ' ')
		{
			i++;
			if(i < strurl.size())
				strurl[i]=0;
			break;
		}
		i --;
	}
	m_mapNamespace.insert(std::pair<std::string,std::string>(strnamespace,strurl));
}

