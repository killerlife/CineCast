#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <iostream>
#include "activeThread.h"
using namespace std;
using namespace boost;

namespace brunt
{
struct activeMutex::Impl
{
	boost::mutex     m_mutex;
};

activeMutex::activeMutex():_impl(new Impl())
{
	
}

activeMutex::~activeMutex()
{
	delete _impl;
}


struct activeLock::Impl
{
	boost::mutex::scoped_lock     m_lock;
	Impl(boost::mutex& mutex):m_lock(mutex)
	{
	}
};

activeLock::activeLock(activeMutex& mutex):_impl(new Impl(mutex._impl->m_mutex))
{
	
}

activeLock::~activeLock()
{
	delete _impl;
}



//////////////////////////////////////////////////////////////////
class CActiveThread_adapter
#ifdef BAIQIAN_BOOST_THREAD
	:public boost::thread_adaptor
#endif
{
public:
    CActiveThread_adapter(void (*func)(CActiveThread&), CActiveThread& threadOb)
        : _func(func), _threadObject(threadOb)
    {
    }

#ifdef BAIQIAN_BOOST_THREAD
    void run() const { 
//    	cout << " XXXXXXXXXXXXXXX CActiveThread_adapter run() " << endl;
		_func(_threadObject);	
    }
#else
	void operator()() const{
//    	cout << " XXXXXXXXXXXXXXX CActiveThread_adapter operator()() " << endl;
		_func(_threadObject);	
	}
#endif

private:
    void (*_func)(CActiveThread&);
	CActiveThread& _threadObject;
};

/////////////////////////////////////
struct CActiveThread::Impl
{
	int              m_id;
	int              m_status;
	boost::mutex     m_mutex;
	boost::condition m_cond;
	boost::thread*	  m_pThread;
	CActiveThread_adapter* m_pAdapter;
	Impl():m_id(0),m_status(thread_ready),m_pThread(NULL),m_pAdapter(NULL)
	{
	}
};
/////////////////////////////////////////////////////////////////
CActiveThread::CActiveThread():_impl(new Impl())
{
	
};

CActiveThread::~CActiveThread()
{
	stop();
	delete _impl;
}

bool CActiveThread::isStopped()const
{
	return _impl->m_status == thread_stopped;	
}

int  CActiveThread::status()const
{
	return _impl->m_status;
}

int CActiveThread::result()const
{
	return  getResult();
}

bool CActiveThread::start()
{
	if(!(_impl->m_status == thread_stopped || _impl->m_status == thread_ready ))
		return true;
    _impl->m_status = thread_starting;
	if(_impl->m_pThread == NULL)
	{

		_impl->m_pAdapter = new CActiveThread_adapter(&CActiveThread::do_thread,*this);

		_impl->m_pThread  = new boost::thread(*_impl->m_pAdapter);

	}
	return _impl->m_pThread != NULL;
}

void CActiveThread::stop()
{
	if(_impl->m_status == thread_startted)
		_impl->m_status = thread_stoping;
	
	if(_impl->m_pThread)
	{
		_impl->m_cond.notify_all();
		_impl->m_pThread->join();
		delete _impl->m_pThread;
		_impl->m_pThread = NULL; 
		delete _impl->m_pAdapter;
		_impl->m_pAdapter = NULL;
	}
}

void CActiveThread::setId(int id)
{
	_impl->m_id = id;
}

int  CActiveThread::getId()const
{
	return _impl->m_id;
}

void CActiveThread::do_thread(CActiveThread& server)
{
	server.threadFunction();
}

void CActiveThread::init()
{
	_impl->m_status = thread_startted;
}

void CActiveThread::release()
{
	_impl->m_status = thread_stopped;	
}

int  CActiveThread::getResult()const
{
		return 0;
}

void CActiveThread::doit()
{
	while(!(_impl->m_status == thread_stoping))
		timed_wait(1000);
}

void  CActiveThread::threadFunction()
{
	init();
	doit();
 	release();
}

void CActiveThread::timed_wait(int ms)
{
		
		boost::xtime xt;
		boost::xtime_get(&xt, boost::TIME_UTC_);
		xt.sec +=ms/1000;
		xt.nsec+=1000*1000*(ms%1000);
		
		boost::mutex::scoped_lock lock(_impl->m_mutex);
		_impl->m_cond.timed_wait(lock,xt);
}

void CActiveThread::wait()
{
	boost::mutex::scoped_lock lock(_impl->m_mutex);
	_impl->m_cond.wait(lock);
}

void CActiveThread::wait(CWaitPred& pred)
{
	boost::mutex::scoped_lock lock(_impl->m_mutex);
	while (!pred())
	{
        _impl->m_cond.wait(lock);
	}
}

void CActiveThread::notify_one()
{
	_impl->m_cond.notify_one();
}

void CActiveThread::notify_all()
{
	_impl->m_cond.notify_all();
}

}
