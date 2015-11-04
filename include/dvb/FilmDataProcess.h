#pragma once
#include <thread/activeThread/activeThread.h>
#include <string>
#include "demux.h"
#include "SatelliteRecv.h"
#include <stdio.h>
#include <stdlib.h>

// class IFilmDataObserver
// {
// public:
// 	virtual void notify(int status, uint64 nIsEnd) = 0;
// };

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
	int GetStatus() { return m_status; };
// 	void SetObserver(IFilmDataObserver* pObserver) { m_pObserver = pObserver; };
	void UpdateMem(uint32 nSegNum);
	void UpdateFile();

private:
	virtual void doit();
private:
	uint64 m_ReciveLength;
	int m_status;
// 	IFilmDataObserver *m_pObserver;
	struct PmtDescriptor* m_pPmtDescriptor;
	std::string m_strFileName;
	std::string m_strZtFileName;
	uint8 *m_pZtBuf;
	int m_nZtBufSize;
	Filter* m_pFilter;
	uint8 m_buffer[4096];
	FILE *m_pFilmFile;
	uint32 m_nSegBasic;
};