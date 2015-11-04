#ifndef __BRUNT_ACTIVETHREAD_H__
#define __BRUNT_ACTIVETHREAD_H__

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>

#include "threadStatus.h"

#ifdef _WIN32

#ifdef ACTIVETHREADMANAGER_EXPORTS
#define ACTIVETHREADMANAGER_API __declspec(dllexport)
#else
#define ACTIVETHREADMANAGER_API __declspec(dllimport)
#endif

#else
#define ACTIVETHREADMANAGER_API 
#endif



namespace brunt
{
	class activeLock;
	class activeMutex
	{
		friend class activeLock;
	public:
		activeMutex();
		~activeMutex();
	private:
		struct Impl;
		Impl*  _impl;

	};

	class activeLock
	{
	public:
		activeLock(activeMutex&);
		~activeLock();
	private:
		struct Impl;
		Impl*  _impl;

	};


	class ACTIVETHREADMANAGER_API CWaitPred
	{
	public:
		virtual ~CWaitPred(){};
		bool operator()()
		{
			return pred();

		}
	protected:
		virtual bool pred()
		{
			return true;
		}
	};

	class ACTIVETHREADMANAGER_API CActiveThread 
	{
	private:
		CActiveThread(const CActiveThread& rSrc);// no implementation
		void operator=(const CActiveThread& rSrc);// no implementation

	public:
		CActiveThread();
		virtual ~CActiveThread();

		bool isStopped()const;
		int  status()const;

		int  result()const;

		virtual bool start();
		virtual void stop();

		void setId(int id);
		int  getId()const;

	protected:
		static void do_thread(CActiveThread& server);

		void timed_wait(int ms);
		void wait();
		void wait(CWaitPred& pred);

		void notify_one();
		void notify_all();

		void threadFunction();

	protected:
		//three functions executed in threadFunction();
		virtual void init();
		virtual void doit();
		virtual void release();
		virtual int  getResult()const;

	protected:
		struct Impl;
		Impl*  _impl;

	};

}

#endif//__BRUNT_ACTIVETHREAD_H__

