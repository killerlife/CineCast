#ifndef __BRUNT_ASYNCOBJECTTHREAD_H__
#define __BRUNT_ASYNCOBJECTTHREAD_H__

#include "activeThread.h"

namespace brunt
{
class CAsyncObjectThread;
class ACTIVETHREADMANAGER_API IAsyncObject
{
public:
	virtual ~IAsyncObject(){};
	virtual int doWork(CAsyncObjectThread& threadObject) =0;

};


class ACTIVETHREADMANAGER_API CAsyncObjectThread : public CActiveThread
{
public:
	CAsyncObjectThread(IAsyncObject& object):m_asyncObject(object),m_result(0)
	{
	}
	
protected:
    virtual void doit();
	virtual int  getResult()const
	{
		return m_result;
	}
private:
    IAsyncObject&     m_asyncObject;
	int               m_result;

};

}
#endif
