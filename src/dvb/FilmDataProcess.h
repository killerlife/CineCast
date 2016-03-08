#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include "demux.h"
#include "SatelliteRecv.h"
#include <stdio.h>
#include <stdlib.h>

//class ILog;

#include <thread/activeThread/activeThreadManager_i.h>

// class IFilmDataObserver
// {
// public:
// 	virtual void notify(int status, uint64 nIsEnd) = 0;
// };

struct FilmDataBuf
{
	uint8 *pBuf;
	uint16 count;
public:
	FilmDataBuf()
	{
		pBuf = NULL;
		count = 0;
	}
	~FilmDataBuf()
	{
		if (pBuf != NULL)
		{
			delete[] pBuf;
			pBuf = NULL;
		}
	}
};

#if 0
struct LostBuf
{
	uint32 m_size;
	uint8 *m_buf;
public:
	LostBuf()
	{
		m_size = 0;
		m_buf = NULL;
	}
	~LostBuf()
	{
		if (m_buf)
		{
			delete[] m_buf;
			m_size = 0;
		}
	}
};
#endif

class DataProcessThread;

class FilmDataThread: public IFilterProcessor, public brunt::CActiveThread
{
public:
	FilmDataThread();
	virtual ~FilmDataThread();
	bool Init(void *param1, void *param2);
	bool Start();
	bool Stop();

	uint64 ReciveLength();
	uint64 FileLength();
	uint64 ReciveSegment();
	uint64 LostSegment();
	uint64 CRCError();
	uint64 TotalSegment();

	void UpdateInfo();

	int GetStatus() { return m_status; };

	void WriteFile(uint64 pos, uint8 *pbuf, uint16 size);
	void UpdateZtMem(uint32 nSegNum);
	void UpdateZtFile();
	void UpdateFile();

	//Buffer function
	void AllocBufPool();
	void FreeBufPool();

	struct FilmDataBuf* GetFreeBuf();
	struct FilmDataBuf* GetDataBuf();
	void PushIntoFreeBuf(struct FilmDataBuf* pBuf);
	void pushIntoDataBuf(struct FilmDataBuf* pBuf);

	bool isStop() { return bStop; };

	bool haveSegment(uint32 nSegNum);

	std::string GetLostSegment();

	uint32 GetFilmId() {return filmId;};
#if 0
	struct LostBuf* GetLostSegment();
#endif

	bool UnzipSubtitle();

	bool SaveData(char* fn, char* pData, uint32 segNum, uint32 len);

	std::string FindAssetmap();
	bool IsSameDCP(std::string path);
	bool IsReady();
	bool RoundCleanCounter();

public:
	struct PmtDescriptor* m_pPmtDescriptor;
	uint64 m_ReciveLength, m_ReciveSegment, m_LostSegment, m_CRCError, m_TotalSegment;

	FILE *m_pFilmFile;

private:
	virtual void doit();

private:
	int m_status;
// 	IFilmDataObserver *m_pObserver;
	std::string m_strFileName, m_strFileNameReport;
	std::string m_strZtFileName;
	int m_nZtBufSize;
	uint8 *m_pZtBuf;
	//struct LostBuf m_sLostBuf;
	Filter* m_pFilter;
	uint8 m_buffer[4096];
	uint32 m_nSegBasic;

	uint32 filmId;
	FILE *m_pZtFilmFile;

	//Buffer pool
	std::list<struct FilmDataBuf*> m_bufFreePool;
	std::list<struct FilmDataBuf*> m_bufDataPool;
	volatile uint8 m_freeMutex;
	volatile uint8 m_dataMutex;
	volatile uint8 m_writeMutex;

	uint16 m_allocSize;

	brunt::IThreadManager* m_pManager;
	DataProcessThread* m_pDataThread;

	bool bStop;

	uint64 m_ztPos;
	bool m_Ready;
	
	volatile bool bSequence;
	uint32 m_lastSegNum;
	
// 	ILog *pLog;
};