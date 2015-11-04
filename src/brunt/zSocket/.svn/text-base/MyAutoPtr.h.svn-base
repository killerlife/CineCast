// CMyAutoPtr (auto-pointer) class
// Created by liang 
#ifndef _MYAUTOPTR_200712271502_H__
#define _MYAUTOPTR_200712271502_H__

#include <vector>
#include <list>
#include <assert.h>

using namespace std;

template<class T>
class CMyAutoPtr{
public:
	// local class CMyRefCount ->
	template<class T1>
	class CMyRefCount{
	public:
		CMyRefCount():_ptr(0),_count(0){}
		CMyRefCount(T1* p):_ptr(p),_count(1){
		}
		virtual ~CMyRefCount(){
			delete _ptr;
		}
		int Release(){
			if(_ptr)
			{
				assert(_count>0);
				_count--;
				if(_count==0){
					delete _ptr; _ptr = 0;
				}
			}
			return _count;
		}
		void AddRef(unsigned int cnt=1){
			_count += cnt;
		}
		T1* GetPtr(){return _ptr;}
		//bool IsEmpty(){return _count==0;};
		int GetRefCount(){return _count;};
	private:
		T1*  _ptr;
		int _count;
	};
	// local class CMyRefCount <-

public:
	CMyAutoPtr():_count_ptr(0){}
	CMyAutoPtr(T* p){
		_count_ptr = new CMyRefCount<T>(p);
	}
	CMyAutoPtr(const CMyAutoPtr& v){
		*this = v;
	}
	virtual ~CMyAutoPtr(){
		Release();
	}
	CMyAutoPtr& operator =(const CMyAutoPtr& a){
		if(this!=&a)
		{
			_count_ptr = a._count_ptr;
			if(_count_ptr)
				_count_ptr->AddRef();
		}
		return *this;
	}
	CMyAutoPtr& operator =(T* p){
		CMyRefCount<T>* save = _count_ptr;
		_count_ptr = p ? new CMyRefCount<T>(p) : 0;
		if(save && save->Release()==0)
		{
			delete save;
		}
		return *this;
	}
	T* operator *(){
		return _count_ptr==0?0:_count_ptr->GetPtr();
	}
	T* operator ->(){
		assert(GetPtr()!=0);
		return GetPtr();
	}
	operator T*(){
		assert(GetPtr()!=0);
		return GetPtr();
	}
	T* GetPtr(){
		return _count_ptr==0?0:_count_ptr->GetPtr();
	}
	void Release(){
		if(_count_ptr && _count_ptr->Release()==0)
		{
			delete _count_ptr;
			_count_ptr = 0;
		}
	}
	void AddRef(unsigned int cnt=1){
		if(_count_ptr)
			_count_ptr->AddRef(cnt);
	}
	int GetRefCount(){
		return _count_ptr?_count_ptr->GetRefCount():0;
	}
	bool IsEmpty(){return _count_ptr==0;};
private:
	CMyRefCount<T>*  _count_ptr;
};


#endif//_MYAUTOPTR_200712271502_H__
