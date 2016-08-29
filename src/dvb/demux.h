#ifndef _DEMUX_H_
#define _DEMUX_H_

#include <sys/poll.h>
#include "mytype.h"
#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)

struct filter {
	int fd;
	int pid;
	int tid;
};

enum SECTION_TYPE{
	SEC_NOTIFY,
	SEC_START,
	SEC_CANCEL,
	SEC_FINISH,
	SEC_PAT,
	SEC_PMT,
	SEC_DATA,
	SEC_UNKNOW = -1,
	SEC_ERROR = -2,
};

struct NotifyDescriptor {
	uint8 DescriptorTag;
	uint8 DescriptorLength;
	uint32 FilmId;
	uint8 Version;
	uint8 Type;
	uint8 Reserved;
	uint32 MachineCount;
	uint32 *MachineId;
	NotifyDescriptor()
	{
		MachineId = NULL;
	};
	~NotifyDescriptor()
	{
		if (MachineId != NULL)
		{
			delete[] MachineId;
			MachineId = NULL;
		}
	}
};

struct StartDescriptor {
	uint8 DescriptorTag;
	uint8 DescriptorLength;
	uint32 FilmId;
	uint8 Version;
	uint8 Type;
	uint64 DCPLength;
	uint64 ReservedInfo;
	uint8 FilmNameLength;
	char *FileName;
	uint8 UUIDLength;
	char *UUID;
	uint8 IssueDateLength;
	char *IssueDate;
	uint8 IssuerLength;
	char *Issuer;
	uint8 CreatorLength;
	char *Creator;
	StartDescriptor()
	{
		FileName = NULL;
		UUID = NULL;
		IssueDate = NULL;
		Issuer = NULL;
		Creator = NULL;
	};
	~StartDescriptor()
	{
		if (FileName != NULL)
		{
			delete[] FileName;
			FileName = NULL;
		}
		if (UUID != NULL)
		{
			delete[] UUID;
			UUID = NULL;
		}
		if (IssueDate != NULL)
		{
			delete [] IssueDate;
			IssueDate = NULL;
		}
		if (Issuer != NULL)
		{
			delete[] Issuer;
			Issuer = NULL;
		}
		if (Creator != NULL)
		{
			delete[] Creator;
			Creator = NULL;
		}
	}
};

struct FileDescriptor {
	uint8 DescriptorTag;
	uint8 DescriptorLength;
	uint64 FileLength;
	uint32 SegmentLength;
	uint8 Option;
	char* FileName;
	FileDescriptor()
	{
		FileName = NULL;
	};
	~FileDescriptor()
	{
		if (FileName != NULL)
		{
			delete[] FileName;
			FileName = NULL;
		}
	}
};

struct CancelDescriptor {
	uint8 DescriptorTag;
	uint8 DescriptorLength;
	uint32 FilmId;
	uint8 Version;
	uint8 Type;
};

struct FinishDescriptor {
	uint8 DescriptorTag;
	uint8 DescriptorLength;
	uint32 FilmId;
	uint8 Version;
	uint8 Type;
};

struct PatDescriptor {
	uint16 count;
	uint16 *NetworkId;
	uint16 *ProgramId;
	PatDescriptor()
	{
		NetworkId = NULL;
		ProgramId = NULL;
	};
	~PatDescriptor()
	{
		if (NetworkId != NULL)
		{
			delete[] NetworkId;
			NetworkId = NULL;
		}
		if (ProgramId != NULL)
		{
			delete[] ProgramId;
			ProgramId = NULL;
		}
	}
};

struct ProgramDescriptor {
	uint8 DescriptorTag;
	uint8 DescriptorLength;
	uint32 FilmId;
	uint8 Version;
	uint8 FileNumber;
	uint8 Type;
	ProgramDescriptor()
	{
		FileNumber = 0;
	}
};

struct SubfolderDescriptor {
	uint8 DescriptorTag;
	uint8 DescriptorLength;
	char *SubfolderName;
	SubfolderDescriptor()
	{
		SubfolderName = NULL;
	};
	~SubfolderDescriptor()
	{
		if (SubfolderName != NULL)
		{
#if 0
			printf("delete SubfolderName %x ", SubfolderName);
#endif // DEBUG
			delete[] SubfolderName;
			SubfolderName = NULL;
		}
	}
};

struct SubfolderCountDescriptor {
	uint8 DescriptorTag;
	uint8 DescriptorLength;
	uint8 SubfolderCount;
	struct SubfolderDescriptor* subfolderDescriptor;
	SubfolderCountDescriptor()
	{
		subfolderDescriptor = NULL;
		SubfolderCount = 0;
	};
	~SubfolderCountDescriptor()
	{
		if (subfolderDescriptor != NULL)
		{
#if 0
			printf("delete subfolderDescriptor %x ", subfolderDescriptor);
#endif // DEBUG
			delete[] subfolderDescriptor;
			subfolderDescriptor = NULL;
		}
	}
};

struct PmtDescriptor {
	uint8 StreamType;
	uint16 ElementaryPid;
	struct SubfolderCountDescriptor *subfolderCountDescriptor;
	struct FileDescriptor *fileDescriptor;
	PmtDescriptor()
	{
		subfolderCountDescriptor = NULL;
		fileDescriptor = NULL;
	};
	~PmtDescriptor()
	{
		if(subfolderCountDescriptor != NULL)
		{
			delete subfolderCountDescriptor;
			subfolderCountDescriptor = NULL;
		}
		if (fileDescriptor != NULL)
		{
			delete fileDescriptor;
			fileDescriptor = NULL;
		}
	}
	bool operator()(const PmtDescriptor* pPmt1,
		const PmtDescriptor* pPmt2)
	{
		return pPmt1->ElementaryPid < pPmt2->ElementaryPid;
	}
};

struct FilmDataDescriptor{
	uint8 DescriptorTag;
	uint8 DescriptorLength;
	uint32 FilmId;
	uint8 Version;
	uint8 Type;
	uint32 SegmentNumber;
	char* Data;
	FilmDataDescriptor()
	{
		Data = NULL;
	}
	~FilmDataDescriptor()
	{
		if(Data != NULL)
		{
			delete[] Data;
			Data = NULL;
		}
	}
};

unsigned int getBits (const unsigned char *buf,
					  int startbit,
					  int bitlen);

int start_filter(int pid,
				 int tid);

int stop_filter(int pid,
				int tid);

void poll_demux(struct pollfd *poll_fds,
				  int counts,
				  struct filter *filters);

int read_sections(struct filter *f);

int section_type(char* buf);

int notify_parser(char* buf,
				  struct NotifyDescriptor* notifyDescriptor);

int start_parser(char* buf,
				 struct StartDescriptor *startDescriptor,
				 struct FileDescriptor *fileDescriptor);

int cancel_parser(char* buf,
				  struct CancelDescriptor* cancelDescriptor);

int finish_parser(char* buf,
				  struct FinishDescriptor* finishDescriptor);

int pat_parser(char* buf,
			   struct PatDescriptor* patDescriptor);

int pmt_parser(char* buf,
			   struct ProgramDescriptor* programDescriptor,
			   struct PmtDescriptor** pPmtDescriptor);

int film_parser(char* buf,
				struct FilmDataDescriptor *filmDataDescriptor);

uint32 calc_crc32(uint8 *pData,
				  int32 nLength);

#endif