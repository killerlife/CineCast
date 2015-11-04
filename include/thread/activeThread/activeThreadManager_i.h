#ifndef __BRUNT__ACTIVE_ITHREADMANAGER_H
#define __BRUNT__ACTIVE_ITHREADMANAGER_H

#include "exportdef.h"
#include "activeThread.h"
#include <list>



namespace brunt
{

enum RUNPOLICY
{
	POLICY_ERROR_CONTINUE,
       POLICY_ERROR_EXIT,
};

struct ThreadRunPolicy
{
	int runLevel;
	unsigned int order;
	RUNPOLICY policy;
	ThreadRunPolicy():runLevel(0),order(0),policy(POLICY_ERROR_CONTINUE)
	{
	}
};

struct CThreadRunInfo
{ 
		 CActiveThread*  m_pthread;
         	 ThreadRunPolicy m_policy;

		 CThreadRunInfo(CActiveThread* pthread,const ThreadRunPolicy& policy):m_pthread(pthread),m_policy(policy)
		 {
		 
		 }
		 CThreadRunInfo(CActiveThread* pthread):m_pthread(pthread)
		 {
		 
		 }
};

class ACTIVETHREADMANAGER_API IThreadManager
{
public:
	virtual ~IThreadManager(){};
	virtual int addThread(const CThreadRunInfo& threadObject) =0;
	virtual unsigned int count(int runOrder = -1) =0;
	virtual CThreadRunInfo getObject(int index,unsigned int runOrder) =0;
    virtual void  setParallelNum(unsigned int runOrder, int parallelNumber) =0;

	virtual int run() =0;
	virtual int terminate() =0;

	virtual unsigned int currentRunOder() =0;
    virtual bool isEnd() =0;
	virtual std::list<CThreadRunInfo>& getFailList() =0;
	virtual std::list<CThreadRunInfo>& getUnDoList() =0;

};

ACTIVETHREADMANAGER_API IThreadManager* createThreadManager();
ACTIVETHREADMANAGER_API void            releaseThreadManager(IThreadManager*);

}
#endif
