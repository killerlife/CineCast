//////////////////////////////////////////////////////////////////////////
//mystring.h
#ifndef __MYSTRING_H_
#define __MYSTRING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>

#include <xercesc/framework/LocalFileFormatTarget.hpp>



#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <xercesc/util/OutOfMemoryException.hpp>

#ifdef _WIN32
#define ENCODING_FLAG "gb2312"
#else
#define ENCODING_FLAG "utf-8"
#endif

//#include <comutil.h>
XERCES_CPP_NAMESPACE_USE


class MyWStr: public XMLFormatTarget
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    MyWStr(const char* const toTranscode, const char * const encoding =ENCODING_FLAG):m_value(0),m_fFormatter(0),m_fCharSizeBuf(0)
    {
        // Call the private transcoding method
		XMLCh* fEncodingUsed = XMLString::transcode(encoding);
        m_fFormatter = new XMLFormatter(fEncodingUsed
                                     ,this
                                     ,XMLFormatter::NoEscapes
                                     ,XMLFormatter::UnRep_CharRef);
	    XMLString::release(&fEncodingUsed);
	  
	    const unsigned int  srcCount=XMLString::stringLen(toTranscode);
		int valueLen =srcCount*3+10;

		m_value=new XMLCh[valueLen];
		unsigned char* m_fCharSizeBuf =new unsigned char[valueLen*2];
		
		XMLTranscoder * pTranscoder = (XMLTranscoder *)m_fFormatter->getTranscoder();		
		unsigned int bytesEaten;
		unsigned int size = pTranscoder->transcodeFrom((XMLByte *)toTranscode,srcCount,\
					m_value,valueLen,bytesEaten,m_fCharSizeBuf);
		m_value[size] =0;

	}
	
    ~MyWStr()
    {
		if(m_fFormatter)
			delete m_fFormatter;
		if(m_value)
			delete [] m_value;
		if(m_fCharSizeBuf)
			delete [] m_fCharSizeBuf;

    }
	
    const XMLCh* unicodeForm() const
    {
        return m_value;
    }
protected:
	void writeChars(const XMLByte* const  toWrite
                                  , const unsigned int    count
                                  , XMLFormatter* const   formatter)
	{
   	
	}
	

private:
   XMLCh*   m_value;
   XMLFormatter * m_fFormatter;
   unsigned char* m_fCharSizeBuf;
};

class MyStr: public XMLFormatTarget
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    MyStr(const XMLCh* toTranscode, const char * const encoding =ENCODING_FLAG):m_toTranscode(0)/*,m_buf(0)*/
    {
        // Call the private transcoding method
		 XMLCh* fEncodingUsed = XMLString::transcode(encoding);
        m_fFormatter = new XMLFormatter(fEncodingUsed
                                     ,this
                                     ,XMLFormatter::NoEscapes
                                     ,XMLFormatter::UnRep_CharRef);
	    XMLString::release(&fEncodingUsed);
	    m_toTranscode = toTranscode;
	    //m_buf = new char[1];
	    //m_buf[0] = 0;
	    
	}
	
    ~MyStr()
    {
		if(m_fFormatter)
			delete m_fFormatter;
	    //if(m_buf)
	      // delete [] m_buf;
		
    }
	
    const char* unicodeForm() const
    {
    	//std::cout <<"begin" << std::endl;
		if(m_bufStr.empty())
		{
			*m_fFormatter << m_toTranscode;
		}
	
    	//std::cout <<"end" << std::endl;
		return m_bufStr.c_str();
    }
protected:
    void writeChars(const XMLByte* const toWrite,const unsigned int count, XMLFormatter* const formatter)
	{

		int len = m_bufStr.length(); 
        m_bufStr += (char*)toWrite;
		m_bufStr.resize(len+count);

	/*	if(m_buf)
	     {
	     	  delete [] (m_buf);
	     	  m_buf = 0;
	     }
	     m_buf = new char[count +1];
	     memcpy(m_buf,toWrite,count);
	     m_buf[count] = 0;*/
	}	
	
private:
    const XMLCh*   m_toTranscode;
    XMLFormatter * m_fFormatter;
	std::string    m_bufStr;
};


//#define WX(str) MyWStr(str).unicodeForm()
//#define CX(str) MyStr(str).unicodeForm()

//#define X(str) WX(str)

#endif

