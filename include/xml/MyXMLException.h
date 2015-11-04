//////////////////////////////////////////////////////////////////////////
#ifndef __MyXMLException_H_
#define __MyXMLException_H_

#include <stdexcept>
#include <string>

class CMyXMLException : public std::runtime_error
{
public:
	explicit CMyXMLException(int error,const std::string& message)
	:std::runtime_error(message),m_errorCode(error)
	{
	}
	int m_errorCode;

};

#endif //__MyXMLException_H_

