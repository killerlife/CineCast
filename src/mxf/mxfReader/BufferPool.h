#ifndef _BUFFERPOOL_200807091832_H__
#define _BUFFERPOOL_200807091832_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <iostream>
using namespace std;

#include <boost/thread/mutex.hpp>

#include "brunt/debugprint.h"

#define BP_MAXSIZE_POOL		1024
#define BP_MAXSIZE_MAP		(1024/8)
#define BP_MAPITEM_BITS		8

class CBufferPool
{
protected:
	int ResizeMap(int size)
	{
		return 0;
	}

public:
	CBufferPool(int size, int numb=BP_MAXSIZE_POOL):m_size(size), m_numb(numb), m_pool(0), m_count(0)
	{
		m_UseMapSize = (m_numb+BP_MAPITEM_BITS-1)/BP_MAPITEM_BITS;
		m_UseMap = new char[m_UseMapSize];
		memset(m_UseMap, 0, m_UseMapSize);
		m_pool = (char**)new char[sizeof(char*) * numb];
		for(int index=0; index < m_numb; index++)
			m_pool[index] = 0;
	}
	virtual ~CBufferPool()
	{
		ReleasePool();
		delete m_pool;
		delete m_UseMap;
	}

	int GetBuffer(void** buff)
	{
		int index = 0;
		for(int i=0; i<m_UseMapSize; i++)
		{
			for(int j=0; j<BP_MAPITEM_BITS && index<m_numb; j++)
			{
				boost::mutex::scoped_lock lock(m_UseMapMutex);
				if(((m_UseMap[i]>>j)&0x01)==0)
				{
					assert(index<m_numb);
					if(!m_pool[index])
						m_pool[index] = new char[m_size];
					*buff = m_pool[index];
					m_UseMap[i] |= (1<<j);
					m_count++;
	//				DPRINT((DP_Error, "CBufferPool", "++++ get buffer %d ++++", m_count));
					return index;
				}
				index++;
			}
		}
	//	assert(false);
		DPRINT((DP_Error, "CBufferPool", "++++ get buffer (%d) error ++++", m_count));
		return -1;
	}

	int ReleaseBuffer(void* buff)
	{
		if(!buff) return -1;
		int index = 0;
		for(; index < m_numb; index++)
		{
			if(m_pool[index]==buff)
			{
				boost::mutex::scoped_lock lock(m_UseMapMutex);
				assert(((m_UseMap[index/BP_MAPITEM_BITS]>>(index%BP_MAPITEM_BITS))&0x01)==1);
				m_UseMap[index/BP_MAPITEM_BITS] ^= (1<<(index%BP_MAPITEM_BITS));
				m_count--;
	//			DPRINT((DP_Error, "CBufferPool", "++++ del buffer %d ++++", m_count));
				return index;
			}
		}
	//	assert(false);
		DPRINT((DP_Error, "CBufferPool", "---- del buffer (%d) error ----", m_count));
		return -1;
	}

	void ReleasePool()
	{
		for(int index=0; index < m_numb; index++)
		{
			if(m_pool[index])
				delete m_pool[index];
			m_pool[index] = 0;
		}
		boost::mutex::scoped_lock lock(m_UseMapMutex);
		memset(m_UseMap, 0, m_UseMapSize);
	}

	int GetBufferSize(){
		return m_size;
	}

	int GetPoolSize(){
		return m_numb;
	}

private:
	int		m_numb;			// number of buffers in the pool
	int		m_size;			// the size of each buffer in the pool
	int		m_UseMapSize;
//	int		m_capacity;						// max size 
	char*	m_UseMap;		// map array
	char**	m_pool;
	int		m_count;		// count of the used buffer
	boost::mutex m_UseMapMutex;
};


#endif//_BUFFERPOOL_200807091832_H__
