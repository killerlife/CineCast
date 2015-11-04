#ifndef __BRUNT__ACTIVE_THREADMANAGER_H
#define __BRUNT__ACTIVE_THREADMANAGER_H

#include "activeThreadManager_i.h"
#include <list>
#include <map>

namespace brunt
{

class CThreadManager : public IThreadManager,public CActiveThread
{
public:
	CThreadManager();
	virtual ~CThreadManager();
	
	virtual int addThread(const CThreadRunInfo& threadObject);
	virtual unsigned int count(int runOrder = -1);
	virtual CThreadRunInfo getObject(int index,unsigned int runOrder);

       virtual void  setParallelNum(unsigned int runOrder, int parallelNumber);

	virtual int run();
	virtual int terminate();

	virtual unsigned int currentRunOder()
	{
		return m_order;
	}
       virtual bool isEnd()
	{
		return m_isEnd;
	}
	
	virtual std::list<CThreadRunInfo>& getFailList();
   	virtual std::list<CThreadRunInfo>& getUnDoList();


protected:
	void  doit();

private:
	

    typedef std::list<CThreadRunInfo>  CThreadList;
    typedef std::map<unsigned int,CThreadList*> CThreadMap; //<oder,threadlist*>

	CThreadMap		m_threadObjects; 
	std::map<unsigned int,int> m_parallelNum;
	CThreadList		m_failThreadObjects,m_undoThreadObjects;

	unsigned int  m_order;
	bool          m_isEnd;
    
	struct CRunNumInfo
	{
		int stopNum;
		int failNum;
		int readyNum;
		int runningNum;
		
		void clear()
		{
		  stopNum = failNum = readyNum = runningNum =0;
		}
	
	};
private:
	void clear();
	int  getParallelNum(unsigned int runOrder);
	
	void startThreads(CThreadList* pList,int num);
	void stopThreads(CThreadList* pList);
    
	bool getStoppedAndFailNum(CThreadList* pList,CRunNumInfo& runNumInfo);
	
};

}
#endif
