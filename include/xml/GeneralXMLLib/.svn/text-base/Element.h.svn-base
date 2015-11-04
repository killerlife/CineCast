// Element.h: interface for the CElement class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_ELEMENT_H__AE7D594D_2877_410F_86E9_44997527D7FD__INCLUDED_)
//#define AFX_ELEMENT_H__AE7D594D_2877_410F_86E9_44997527D7FD__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#ifndef __ELEMENT_H_
#define __ELEMENT_H_

#include "GeneralXMLLib.h"
#include <datatype/DataType.h>
#include <string>

using namespace _xmlDataType;
struct IXMLNode;
//class CXMLManage;
	enum SOURCEFLAG{
		CREATE_FLAG=0,//create
		OPENFILE_FLAG,//open file
		BUFFER_FLAG//read buffer
	};

class GENERALXMLLIB_API CElement  
{
	CElement();
public:
	//////////////////////////////////////////////////////////////////////////
	//	参数说明：pNode，节点指针，必须有效，如果为NULL，抛出异常std::exception 
	CElement(IXMLNode* pNode);

	//////////////////////////////////////////////////////////////////////////
	/*参数说明: rootName,根节点名称。当nFlag为CREATE_FLAG时，该名称必须写全；当nFlag
					为OPENFILE_FLAG或者BUFFER_FLAG时，该名称可以不带前缀（即名字空间）
				szInfo,信息，当nFlag为CREATE_FLAG时，表示名字空间属性，可以为空；当
					nFlag为OPENFILE_FLAG时，表示文件路径；当nFlag为BUFFER_FLAG时，表示
					需解析的内存字符串。
				nFlag,标志,为CREATE_FLAG时,表示创建;为OPENFILE_FLAG时，表示解析文件，
					为BUFFER_FLAG时，表示解析内存字符串。

				如果构造失败，抛出异常：std::exception
	*/
	CElement(const char* rootName,const char* szInfo=NULL,SOURCEFLAG nFlag=CREATE_FLAG);

	//////////////////////////////////////////////////////////////////////////
	//拷贝构造
	CElement(const CElement& cref);

	//////////////////////////////////////////////////////////////////////////
	virtual ~CElement();

	//////////////////////////////////////////////////////////////////////////
	//

	//Add by Jaontolt
	bool isNull() const;
	//End
public:
	CElement& operator =(const CElement& cref);

	//////////////////////////////////////////////////////////////////////////
	//保存当前节点（包含其本身以及其所有子节点的内容）
	int Save(const char* szfile);

	//attribute text
public:
	//////////////////////////////////////////////////////////////////////////
	//得到属性的总个数
	int GetAttributeCount()const;
	//////////////////////////////////////////////////////////////////////////
	//根据序号以及名称得到属性值，名称可以为空
	std::string GetAttributeByNameIndex(std::string& strname,int nIndex=0)const;
	//string
	bool SetAttribute(const char* szName,const char* szVal);
	std::string GetAttribute(const char* szName)const;
	//datetime
	bool SetAttribute(const char* szName,const DATETIME& szVal);
	DATETIME GetAttributeDatetime(const char* szName)const;
	//rational
	bool SetAttribute(const char* szName,const RATIONAL& szVal);
	RATIONAL GetAttributeRational(const char* szName)const;
	//Int64
	bool SetAttribute(const char* szName,const Int64 szVal);
	Int64 GetAttributeInt64(const char* szName)const;
	//long
	bool SetAttribute(const char* szName,const long szVal);
	long GetAttributelong(const char* szName)const;
	//int
	bool SetAttribute(const char* szName,const int szVal);
	int GetAttributeint(const char* szName)const;

	//////////////////////////////////////////////////////////////////////////
	//得到该节点的字符串信息（包含其本身以及其所有子节点的内容）
	std::string GetDataString()const;
	//////////////////////////////////////////////////////////////////////////
	//得到节点的全名称（饱含名字空间和名称）
	std::string GetNodeName()const;
	//////////////////////////////////////////////////////////////////////////
	//得到节点的本地名称（不含名字空间）
	std::string GetLocalName()const;
	//////////////////////////////////////////////////////////////////////////
	//得到节点的前缀（即名字空间）
	std::string GetPrefix()const;
	//////////////////////////////////////////////////////////////////////////
	//得到id
	int Getid()const;

	int SetTextcontent(const char* szval);
	std::string GetTextcontent()const;
	
	//child element
protected:
	//////////////////////////////////////////////////////////////////////////
	/*
		以下返回值为IXMLNode*类型的函数，在调用时取出其返回值，并用CElement或者
		其派生类封装，如果直接使用指针，则在最后不用时，调用指针的Release来释放。
		如果不取出其返回值，则会造成xml资源不释放。
	*/
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//插入一个CElement到一个节点的某个位置
	IXMLNode* InsertChildElementRept(const CElement& element,long lpos=-1);
	//////////////////////////////////////////////////////////////////////////
	//插入一个DOMElement*指针到一个节点的某个位置
	IXMLNode* InsertChildElementRept(const void* pDomNode,long lpos=-1);
	//////////////////////////////////////////////////////////////////////////
	//创建一个子节点，bid表示是否创建一个带索引号属性的子节点
	IXMLNode* CreateChildNodeRept(const char* szName,long lPos=-1,bool bid=false);

	IXMLNode* GetChildByNameIndexRept(const char* szName,long lPos=0)const;
	IXMLNode* GetChildByIndexRept(long lPos=0)const;
	IXMLNode* GetChildByIdRept(int id)const;

	int RemoveChildNode(IXMLNode* pNode);
	//////////////////////////////////////////////////////////////////////////
	//克隆一个新的node，
//	IXMLNode* Clone();


public:
	//////////////////////////////////////////////////////////////////////////
	/*
	以下返回值为CElement类型的函数，在调用时产生的异常包含自定义异常类CMyXMLException。
	*/
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//插入一个CElement到一个节点的某个位置
	CElement InsertChildElement(const CElement& element,long lpos=-1);
	//////////////////////////////////////////////////////////////////////////
	//插入一个DOMElement*指针到一个节点的某个位置
	CElement InsertChildElement(const void* pDomNode,long lpos=-1);
	//////////////////////////////////////////////////////////////////////////
	//创建一个子节点，bid表示是否创建一个带索引号属性的子节点
	CElement CreateChildNode(const char* szName,long lPos=-1,bool bid=false);

	CElement GetChildByNameIndex(const char* szName,long lPos=0)const;
	CElement GetChildByIndex(long lPos=0)const;
	CElement GetChildById(int id)const;

	CElement Clone();

	long GetChildCountByName(const char* szName)const;
	long GetChildCount()const;
	int RemoveChildNode(CElement* element);

	int RemoveChildByIndex(int nIndex);
	int RemoveChildById(int id);

	int RemoveChildByNameIndex(const char* szname,int nIndex=0);
	
	//set child element text
public:
	//////////////////////////////////////////////////////////////////////////
	//以下函数set的功能：如果该节点包含其名称为szName的子节点element，则给该子节点设置值szval
	//					如果不包含，则创建一个名称为szName的子节点element，并设置值
	//		  get的功能：如果该节点包含其名称为szName的子节点element，则返回该子节点的值
	//					如果不包含，则返回一个空值
	//////////////////////////////////////////////////////////////////////////
	//optional
	bool SetChildElementText(const char* szName,const OPTIONAL_ATTR& szVal,long lpos=-1);
	int GetChildElementTextOptionalText(const char* szName,OPTIONAL_ATTR& opattrval)const;
	//scope
	bool SetChildElementText(const char* szName,const SCOPEATTR& szVal,long lpos=-1);
	SCOPEATTR GetChildElementTextScopetext(const char* szName)const;
	//string
	bool SetChildElementText(const char* szname,const char* szval,long lpos=-1);
	std::string GetChildElementText(const char* szname)const;
	//datetime
	bool SetChildElementText(const char* szname,const DATETIME& szval,long lpos=-1);
	DATETIME GetChildElementTextDatetime(const char* szname)const;
	//usertext
	bool SetChildElementText(const char* szName,const USERTEXT& szVal,long lpos=-1);
	USERTEXT GetChildElementTextUsertext(const char* szName)const;
	//rational
	bool SetChildElementText(const char* szname,const RATIONAL& szval,long lpos=-1);
	RATIONAL GetChildElementTextRational(const char* szname)const;
	//Int64
	bool SetChildElementText(const char* szname,const Int64 szval,long lpos=-1);
	Int64 GetChildElementTextInt64(const char* szname)const;
	//long
	bool SetChildElementText(const char* szname,const long szval,long lpos=-1);
	long GetChildElementTextlong(const char* szname)const;
	//int
	bool SetChildElementText(const char* szname,const int szval,long lpos=-1);
	int GetChildElementTextint(const char* szname)const;

public:
	//////////////////////////////////////////////////////////////////////////
	//得到一个DOMElement*的指针。使用该指针，须包含apach的xml库及其头文件
	void* GetDomElement();
	//////////////////////////////////////////////////////////////////////////
	//得到一个错误号
	int getLastError(){return m_nerror;};

protected:
	////////////////////////////////////////////////////////////////////////////
	//创建一个名称唯一的子节点，如果该节点已经存在，则不创建，并返回该节点的指针
	IXMLNode* CreateSingleChildNode(const char* szName, long lPos, bool bid=false);

	void EqualNode(const IXMLNode* pOriNode);
	void AddRef();
	IXMLNode* m_pNode;
	int m_nerror;
private:
};

#endif // __ELEMENT_H_


