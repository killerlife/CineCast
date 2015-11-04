#include "activeThreadManager.h"

namespace brunt
{

using namespace std;

IThreadManager* createThreadManager()
{
	return new CThreadManager();
}
void  releaseThreadManager(IThreadManager* pManager)
{
	delete (CThreadManager*)pManager;
}
//////////////////////////////////////////////////////////////////////////
CThreadManager::CThreadManager():m_order(0),m_isEnd(false)
{

}
CThreadManager::~CThreadManager()
{
	clear();

}
	
int CThreadManager::addThread(const CThreadRunInfo& threadObject)
{
	CThreadMap::iterator ite = m_threadObjects.find(threadObject.m_policy.order);
	if(ite == m_threadObjects.end())
	{
		CThreadList* pList = new CThreadList();
		pList->push_back(threadObject);
		m_threadObjects.insert(pair<unsigned int,CThreadList*>(threadObject.m_policy.order,pList));
	
	}
	else
	{
        CThreadList* pList = ite->second;
        pList->push_back(threadObject);
	}
	return 0;
}

unsigned int CThreadManager::count(int runOrder /*= -1*/)
{
	CThreadMap::iterator ite = m_threadObjects.find(runOrder);
	if(ite != m_threadObjects.end())
	{
        CThreadList* pList = ite->second;
        return static_cast<unsigned int>(pList->size());
	}
	unsigned int num = 0;
	ite = m_threadObjects.begin();
	for(;ite != m_threadObjects.end(); ++ite)
	{
		CThreadList* pList = ite->second;
		num+= (unsigned int)pList->size();
	}
	return num;
}

CThreadRunInfo CThreadManager::getObject(int index,unsigned int runOrder)
{
	CThreadMap::iterator ite = m_threadObjects.find(runOrder);
	if(ite != m_threadObjects.end())
	{
        CThreadList* pList = ite->second;
		CThreadList::iterator listIte = pList->begin();
		int i = 0;
		for(; listIte != pList->end() && i <= index; ++listIte,++i)
		{
			if(i == index)
				return *listIte;
			
		}
        
	}
	return CThreadRunInfo(0);
}
void  CThreadManager::setParallelNum(unsigned int runOrder, int parallelNumber)
{
	  m_parallelNum.erase(runOrder);
      m_parallelNum.insert(pair<unsigned int,int>(runOrder,parallelNumber));
}

int CThreadManager::run()
{
	if(!start())
		return -1;

	return 0;
}

int CThreadManager::terminate()
{
	stop();
	clear();
	return 0;
}

std::list<CThreadRunInfo>& CThreadManager::getFailList()
{
    //m_failThreadObjects.clear();
	if(m_failThreadObjects.empty())
	{
		CThreadMap::iterator ite = m_threadObjects.begin();
		for(;ite != m_threadObjects.end(); ++ite)
		{
			CThreadList* pList = ite->second;
			CThreadList::iterator listIte  = pList->begin();
			for(; listIte != pList->end(); ++listIte)
			{
	  			CThreadRunInfo& threadInfo = *listIte;
				if(threadInfo.m_pthread->status() == thread_stopped && threadInfo.m_pthread->result() < 0)
				{
				m_failThreadObjects.push_back(threadInfo);
				}
			}
		}
	}
	return m_failThreadObjects;
}
std::list<CThreadRunInfo>& CThreadManager::getUnDoList()
{
	if(m_undoThreadObjects.empty())
	{
		CThreadMap::iterator ite = m_threadObjects.begin();
		for(;ite != m_threadObjects.end(); ++ite)
		{
			CThreadList* pList = ite->second;
			CThreadList::iterator listIte  = pList->begin();
			for(; listIte != pList->end(); ++listIte)
			{
	  			CThreadRunInfo& threadInfo = *listIte;
				if(threadInfo.m_pthread->status() == thread_ready)
				{
				   m_undoThreadObjects.push_back(threadInfo);
				}
			}
		}
	}
	return m_undoThreadObjects;

}
void  CThreadManager::doit()
{
	m_isEnd = false;

       bool needExit = false;
	CThreadMap::iterator ite = m_threadObjects.begin();
	for(;ite != m_threadObjects.end() && !needExit && status() != thread_stoping; ++ite)
	{
        
		m_order = ite->first;
              CThreadList* pList = ite->second;
		
		//start some thread.
		int maxNum = getParallelNum(m_order);
		startThreads(pList,maxNum);

		while(status() != thread_stoping)
		{
                 timed_wait(500);
		   
		   CRunNumInfo runNumInfo; 
		   needExit = getStoppedAndFailNum(pList,runNumInfo);
		   if(needExit)
		   {
			   break;
		   }
		   // if all are end?
		   if(runNumInfo.stopNum == (int)pList->size())
			   break; 
		   else if(runNumInfo.runningNum < maxNum)
		   {
		       //start one 
                    startThreads(pList,maxNum-runNumInfo.runningNum);
		   }
		   
		}
		stopThreads(pList);
		 
		
	}
	m_isEnd = true;
	
}

void CThreadManager::clear()
{
	CThreadMap::iterator ite = m_threadObjects.begin();
	for(;ite != m_threadObjects.end(); ++ite)
	{
		delete ite->second;
	}
	m_threadObjects.clear();
	m_failThreadObjects.clear();
	m_undoThreadObjects.clear();
	m_isEnd = false;
}

int  CThreadManager::getParallelNum(unsigned int runOrder)
{
	  int num = 2;
	  std::map<unsigned int,int>::iterator ite = m_parallelNum.find(runOrder);
         if(ite != m_parallelNum.end())
	  {
		num = ite->second;
	  }
	  return num;
	  
}

void CThreadManager::startThreads(CThreadList* pList,int num)
{

		int i = 0;
		CThreadList::iterator listIte = pList->begin();
		for(; listIte != pList->end() && i < num ; ++listIte)
		{
			CThreadRunInfo& threadInfo = *listIte;
			if(threadInfo.m_pthread->status() == thread_ready)
			{
					threadInfo.m_pthread->start();
					++i;
			}
		}

}
void CThreadManager::stopThreads(CThreadList* pList)
{
		CThreadList::iterator listIte = pList->begin();
		for(; listIte != pList->end(); ++listIte)
		{
			CThreadRunInfo& threadInfo = *listIte;
			threadInfo.m_pthread->stop();
		}
}
bool CThreadManager::getStoppedAndFailNum(CThreadList* pList,CRunNumInfo& runNumInfo)
{
	   bool needExit = false;
	   runNumInfo.clear();

	   CThreadList::iterator listIte  = pList->begin();
       for(; listIte != pList->end(); ++listIte)
       {
	  		CThreadRunInfo& threadInfo = *listIte;
			int st =  threadInfo.m_pthread->status();
			if( st == thread_stopped)
			{
					++runNumInfo.stopNum;
                    if(threadInfo.m_pthread->result() < 0 )
					{
						//
						++runNumInfo.failNum;
						if(threadInfo.m_policy.policy == POLICY_ERROR_EXIT)
						{
							needExit = true;
						}
					}
			}
			else if( st == thread_ready)
			{
			   ++runNumInfo.readyNum;
			}
			else 
			{
			   ++runNumInfo.runningNum;
			}

		}
	   return needExit;
}

}
