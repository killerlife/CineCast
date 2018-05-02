#include "demux.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <glob.h>
#include <ctype.h>

#include <linux/dvb/version.h>
#include <linux/dvb/frontend.h>
#include <linux/dvb/dmx.h> 
#include <linux/dvb/audio.h>

#include <list>

#define MAX_RUNNING 128
struct pollfd poll_fds[MAX_RUNNING];
struct filter filters[MAX_RUNNING];

std::list<struct filter*> filterList;

unsigned int getBits (const unsigned char *buf,
					  int startbit,
					  int bitlen)
{
	const unsigned char *b;
	unsigned int mask,tmp_long;
	int bitHigh,i;

	b = &buf[startbit / 8];
	startbit %= 8;

	bitHigh = 8;
	tmp_long = b[0];
	for (i = 0; i < ((bitlen-1) >> 3); i++) {
		tmp_long <<= 8;
		tmp_long  |= b[i+1];
		bitHigh   += 8;
	}

	startbit = bitHigh - startbit - bitlen;
	tmp_long = tmp_long >> startbit;
	mask     = (1ULL << bitlen) - 1;
	return tmp_long & mask;
}

int start_filter(int pid,
				 int tid)
{
	std::list<struct filter*>::iterator itor;
	for(itor = filterList.begin(); itor != filterList.end(); ++itor)
	{
		struct filter * f = *itor;
		if (f->pid == pid && f->tid == tid)
		{
			return 0;
		}
	}

	struct filter * f = new struct filter;
	if (f == NULL)
	{
		return -1;
	}

	if ((f->fd = open("/dev/dvb/adapter0/demux0", O_RDWR|O_NONBLOCK)) < 0)
	{
		perror("Open demux0 error");
		delete f;
		return -1;
	}
	struct dmx_sct_filter_params df;
	memset(&df, 0, sizeof(df));
	df.pid = pid;
	if(tid < 0x100 && tid > 0)
	{
		df.filter.filter[0] = tid;
		df.filter.mask[0] = 0xff;
	}
	f->pid = pid;
	f->tid = tid;

	df.timeout = 0;
	df.flags = DMX_IMMEDIATE_START|DMX_CHECK_CRC;
	if(ioctl(f->fd, DMX_SET_FILTER, &df) == -1)
	{
		perror("DMX_SET_FILTER failed");
		ioctl(f->fd, DMX_STOP);
		close(f->fd);
		f->fd = -1;
		delete f;
		return -1;
	}
	int i;
	filterList.push_back(f);
	for (i = 0; i < MAX_RUNNING; i++)
	{
		poll_fds[i].fd = -1;
	}
	for(itor = filterList.begin(), i = 0; itor != filterList.end(); ++itor, i++)
	{
		struct filter * f = *itor;
		poll_fds[i].fd = f->fd;
		filters[i] = *f;
	}
	return 0;
}

int stop_filter(int pid,
				int tid)
{
	std::list<struct filter*>::iterator itor;
	for(itor = filterList.begin(); itor != filterList.end(); ++itor)
	{
		struct filter * f = *itor;
		if (f->pid == pid && f->tid == tid)
		{
			delete f;
			filterList.erase(itor);
		}
	}
	int i;
	for (i = 0; i < MAX_RUNNING; i++)
	{
		poll_fds[i].fd = -1;
	}
	for(itor = filterList.begin(), i = 0; itor != filterList.end(); ++itor, i++)
	{
		struct filter * f = *itor;
		poll_fds[i].fd = f->fd;
		poll_fds[i].events = POLLIN;
		filters[i] = *f;
	}
	return 0;
}

void poll_demux(struct pollfd *poll_fds,
				  int counts,
				  struct filter *filters)
{
	int n = poll(poll_fds, counts, 1000);
	if(n == -1)
		perror("poll");
	int i;
	for (i = 0; i < counts; i++)
	{
		if(poll_fds[i].revents)
			read_sections(&filters[i]);
	}
}

static int fileNo = 0;

int read_sections(struct filter *f)
{
	unsigned char buffer[4096];
	int section_length, count;

	memset(buffer, 0, sizeof(buffer));
	if (((count = read(f->fd, buffer, sizeof(buffer))) < 0) && errno == EOVERFLOW)
		count = read(f->fd, buffer, sizeof(buffer));
	if (count < 0)
	{
		DPRINTF("read_sections: read error\n");
		return -1;
	}
	if (count < 4)
	{
		return -1;
	}
	section_length = getBits(buffer, 12, 12);

	if (count != section_length + 3)
	{
		DPRINTF("Ignoring section, read %d, while section length + 3 = %d\n", count, section_length + 3);
		return -1;
	}
	//===========================================
	//Test only
	FILE *fp;
	char s[100];
	sprintf(s, "%05d.section", fileNo++);
	fp = fopen(s, "wb");
	fwrite(buffer, 1, count, fp);
	fclose(fp);

	int i;
	struct PatDescriptor patDescriptor;
	struct ProgramDescriptor programDescriptor;
	struct PmtDescriptor *pmtDescriptor;
	//===========================================
	switch(section_type((char*)buffer))
	{
	case SEC_NOTIFY:
		break;
	case SEC_START:
		break;
	case SEC_PAT:
		pat_parser((char*)buffer, &patDescriptor);
		for(i = 0; i < patDescriptor.count; i++)
		{
			start_filter(patDescriptor.ProgramId[i], 0x02);
		}
		break;
	case SEC_PMT:
		pmt_parser((char*)buffer, &programDescriptor, &pmtDescriptor);
		for (i = 0; i < programDescriptor.FileNumber; i ++)
		{
			start_filter(pmtDescriptor[i].ElementaryPid, 0x80);
		}
		break;
	case SEC_DATA:
		break;
	case SEC_FINISH:
		break;
	case SEC_CANCEL:
		break;
	case SEC_ERROR:
		break;
	case SEC_UNKNOW:
		break;
	}
	return 0;
}

int section_type(char* buf)
{
	unsigned int tid = getBits((unsigned char*)buf, 0, 8);
	switch (tid)
	{
	case 0x91:
		return SEC_NOTIFY;
	case 0x92:
		return SEC_START;
	case 0x93:
		return SEC_FINISH;
	case 0x94:
		return SEC_CANCEL;
	case 0x80:
		return SEC_DATA;
	case 0x00:
		return SEC_PAT;
	case 0x02:
		return SEC_PMT;
	default:
		return SEC_ERROR;
	}
	return SEC_ERROR;
}

int notify_parser(char* buf,
				  struct NotifyDescriptor* notifyDescriptor)
{
	uint8* pdata = (uint8*)buf;
	uint16 len = getBits((uint8 *)buf, 12, 12);
	uint32 crc = calc_crc32((uint8 *)(buf), len-1);
	uint32 crc1 = (*((uint8*)(buf + len - 1)) << 24)|(*((uint8*)(buf + len)) << 16) | (*((uint8*)(buf + len + 1)) << 8) | (*((uint8*)(buf + len + 2)));
	if (crc1 != crc)
	{
		perror("crc incorrect");
		return -1;
	}

	pdata += 3;
	notifyDescriptor->DescriptorTag = *pdata++;
	notifyDescriptor->DescriptorLength = *pdata++;
	notifyDescriptor->FilmId = getBits((uint8 *)pdata, 0, 32);
	pdata += 4;
	notifyDescriptor->Version = *pdata++;
	notifyDescriptor->Type = *pdata++;
	notifyDescriptor->Reserved = *pdata++;
	notifyDescriptor->MachineCount = getBits((uint8 *)pdata, 0, 32);
	pdata += 4;

	if(notifyDescriptor->MachineId != NULL)
	{
		delete[] notifyDescriptor->MachineId;
	}
	notifyDescriptor->MachineId = new uint32[notifyDescriptor->MachineCount];
	if (notifyDescriptor->MachineId == NULL)
	{
		return -1;
	}
	for (unsigned int i = 0; i < notifyDescriptor->MachineCount; i++)
	{
		notifyDescriptor->MachineId[i] = getBits((uint8 *)pdata, 0, 32);
		pdata += 5;
	}
	return 0;
}

int start_parser(char* buf,
				 struct StartDescriptor *startDescriptor,
				 struct FileDescriptor *fileDescriptor)
{
	uint8 *pdata = (uint8 *)buf;
	uint16 len = getBits((uint8 *)buf, 12, 12);
	uint32 crc = calc_crc32((uint8 *)(buf), len-1);
	uint32 crc1 = (*((uint8*)(buf + len - 1)) << 24)|(*((uint8*)(buf + len)) << 16) | (*((uint8*)(buf + len + 1)) << 8) | (*((uint8*)(buf + len + 2)));
	if (crc1 != crc)
	{
		return -1;
	}

	pdata += 3;
	startDescriptor->DescriptorTag = *pdata++;
	startDescriptor->DescriptorLength = *pdata++;
	startDescriptor->FilmId = getBits((uint8 *)pdata, 0, 32);
	pdata += 4;
	startDescriptor->Version = *pdata++;
	startDescriptor->Type = *pdata++;
	startDescriptor->DCPLength = ((uint64)*pdata << 56)| ((uint64)*(pdata+1) << 48) |
		((uint64)*(pdata+2) << 40) | ((uint64)*(pdata+3) << 32) | ((uint64)*(pdata+4) << 24) |
		((uint64)*(pdata+5) << 16) | ((uint64)*(pdata+6) << 8) | ((uint64)*(pdata+7)); //getBits((uint8*)buf, count, 64);
	pdata +=8;
	startDescriptor->ReservedInfo = ((uint64)*pdata << 56)| ((uint64)*(pdata+1) << 48) |
		((uint64)*(pdata+2) << 40) | ((uint64)*(pdata+3) << 32) | ((uint64)*(pdata+4) << 24) |
		((uint64)*(pdata+5) << 16) | ((uint64)*(pdata+6) << 8) | ((uint64)*(pdata+7));
	pdata +=8;
	startDescriptor->FilmNameLength = *pdata++;

	if (startDescriptor->FileName != NULL)
	{
		delete[] startDescriptor->FileName;
	}
	startDescriptor->FileName = new char[startDescriptor->FilmNameLength];
	if (startDescriptor->FileName == NULL)
	{
		return -1;
	}
	int i;
	for (i = 0; i < startDescriptor->FilmNameLength; i++)
	{
		startDescriptor->FileName[i] = *pdata++;
	}

	startDescriptor->UUIDLength = *pdata++;
	if (startDescriptor->UUID != NULL)
	{
		delete[] startDescriptor->UUID;
	}
	startDescriptor->UUID = new char[startDescriptor->UUIDLength];
	if (startDescriptor->UUID == NULL)
	{
		return -1;
	}
	for (i = 0; i < startDescriptor->UUIDLength; i++)
	{
		startDescriptor->UUID[i] = *pdata++;
	}

	startDescriptor->IssueDateLength = *pdata++;
	if (startDescriptor->IssueDate != NULL)
	{
		delete[] startDescriptor->IssueDate;
	}
	startDescriptor->IssueDate = new char[startDescriptor->IssueDateLength];
	if (startDescriptor->IssueDate == NULL)
	{
		return -1;
	}
	for (i = 0; i < startDescriptor->IssueDateLength; i++)
	{
		startDescriptor->IssueDate[i] = *pdata++;
	}

	startDescriptor->IssuerLength = *pdata++;
	if (startDescriptor->Issuer != NULL)
	{
		delete[] startDescriptor->Issuer;
	}
	startDescriptor->Issuer = new char[startDescriptor->IssuerLength];
	if (startDescriptor->Issuer == NULL)
	{
		return -1;
	}
	for (i = 0; i < startDescriptor->IssuerLength; i++)
	{
		startDescriptor->Issuer[i] = *pdata++;
	}

	startDescriptor->CreatorLength = *pdata++;
	if (startDescriptor->Creator != NULL)
	{
		delete[] startDescriptor->Creator;
	}
	startDescriptor->Creator = new char[startDescriptor->CreatorLength];
	if (startDescriptor->Creator == NULL)
	{
		return -1;
	}
	for (i = 0; i < startDescriptor->CreatorLength; i++)
	{
		startDescriptor->Creator[i] = *pdata++;
	}

	fileDescriptor->DescriptorTag = *pdata++;
	fileDescriptor->DescriptorLength = *pdata++;
	fileDescriptor->FileLength = ((uint64)*pdata << 32) | ((uint64)*(pdata+1) << 24) |
		((uint64)*(pdata+2) << 16) | ((uint64)*(pdata+3) << 8) | ((uint64)*(pdata+4));
	pdata += 5;
	fileDescriptor->SegmentLength = getBits((uint8 *)pdata, 0, 32);
	pdata += 4;
	fileDescriptor->Option = *pdata++;
	
	len = fileDescriptor->DescriptorLength - 1 - 5 - 4;
	if(fileDescriptor->FileName != NULL)
		delete[] fileDescriptor->FileName;
	fileDescriptor->FileName = new char[len];
	if (fileDescriptor->FileName == NULL)
	{
		return -1;
	}
	for (i = 0; i < len; i++)
	{
		fileDescriptor->FileName[i] = *pdata++;
	}
	return 0;
}

int cancel_parser(char* buf,
				  struct CancelDescriptor* cancelDescriptor)
{
	uint8 *pdata = (uint8 *)buf;
	uint16 len = getBits((uint8 *)buf, 12, 12);
	uint32 crc = calc_crc32((uint8 *)(buf), len-1);
	uint32 crc1 = (*((uint8*)(buf + len - 1)) << 24)|(*((uint8*)(buf + len)) << 16) | (*((uint8*)(buf + len + 1)) << 8) | (*((uint8*)(buf + len + 2)));
	if (crc1 != crc)
	{
		return -1;
	}

	pdata += 3;
	cancelDescriptor->DescriptorTag = *pdata++;
	cancelDescriptor->DescriptorLength = *pdata++;
	cancelDescriptor->FilmId = getBits((uint8 *)pdata, 0, 32);
	pdata += 4;
	cancelDescriptor->Version = *pdata++;
	cancelDescriptor->Type = *pdata++;
	return 0;
}

int finish_parser(char* buf,
				  struct FinishDescriptor* finishDescriptor)
{
	uint8 *pdata = (uint8 *)buf;
	uint16 len = getBits((uint8 *)buf, 12, 12);
	uint32 crc = calc_crc32((uint8 *)(buf), len-1);
	uint32 crc1 = (*((uint8*)(buf + len - 1)) << 24)|(*((uint8*)(buf + len)) << 16) | (*((uint8*)(buf + len + 1)) << 8) | (*((uint8*)(buf + len + 2)));
	if (crc1 != crc)
	{
		return -1;
	}
	pdata += 3;
	finishDescriptor->DescriptorTag = *pdata++;
	finishDescriptor->DescriptorLength = *pdata++;
	finishDescriptor->FilmId = getBits((uint8 *)pdata, 0, 32);
	pdata += 4;
	finishDescriptor->Version = *pdata++;
	finishDescriptor->Type = *pdata++;
	return 0;
}

int pat_parser(char* buf,
			   struct PatDescriptor* patDescriptor)
{
	uint8 *buff = (uint8 *)buf;
	uint16 section_len = getBits((uint8 *)buf, 12, 12);
	uint32 crc = calc_crc32((uint8 *)(buf), section_len-1);
	uint32 crc1 = (*((uint8*)(buf + section_len - 1)) << 24)|(*((uint8*)(buf + section_len)) << 16) | (*((uint8*)(buf + section_len + 1)) << 8) | (*((uint8*)(buf + section_len + 2)));
	if (crc1 != crc)
	{
		return -1;
	}

	buff += 8;
	section_len -= 4;
	section_len -= 5;
	patDescriptor->count = section_len/4;
	if(patDescriptor->ProgramId != NULL)
	{
		delete[] patDescriptor->ProgramId;
	}
	patDescriptor->ProgramId = new uint16[patDescriptor->count];
	if (patDescriptor->ProgramId == NULL)
	{
		return -1;
	}
	if(patDescriptor->NetworkId != NULL)
		delete[] patDescriptor->NetworkId;
	patDescriptor->NetworkId = new uint16[patDescriptor->count];
	if (patDescriptor->NetworkId == NULL)
	{
		return -1;
	}
	int i = 0;
	uint16 program_number;
	while(section_len > 0)
	{
		program_number = (buff[0] << 8) | buff[1];
		if (program_number != 0)
		{
			patDescriptor->ProgramId[i] = ((buff[2] & 0x1f) << 8) | buff[3];
			i++;
		}
		buff += 4;
		section_len -= 4;
	}
	patDescriptor->count = i;
	return 0;
}

int pmt_parser(char* buf,
			   struct ProgramDescriptor* programDescriptor,
			   struct PmtDescriptor** pPmtDescriptor)
{
	uint8 *buff = (uint8 *)buf;
	uint16 len = getBits((uint8 *)buf, 12, 12);
	uint32 crc = calc_crc32((uint8 *)(buf), len-1);
	uint32 crc1 = (*((uint8*)(buf + len - 1)) << 24)|(*((uint8*)(buf + len)) << 16) | (*((uint8*)(buf + len + 1)) << 8) | (*((uint8*)(buf + len + 2)));
	if (crc1 != crc)
	{
		return -1;
	}

	buff += 12;
	
	programDescriptor->DescriptorTag = *buff++;
	programDescriptor->DescriptorLength = *buff++;
	programDescriptor->FilmId = getBits((uint8*)buff, 0, 32);
	buff += 4;
	programDescriptor->Version = *buff++;
	programDescriptor->Type = *buff++;
	programDescriptor->FileNumber = *buff++;

	struct PmtDescriptor * pmtDescriptor;
	pmtDescriptor = new struct PmtDescriptor[programDescriptor->FileNumber];
	(*pPmtDescriptor) = pmtDescriptor;
	for (int i = 0; i < programDescriptor->FileNumber; i++)
	{
		pmtDescriptor[i].StreamType = *buff++;
		pmtDescriptor[i].ElementaryPid = ((buff[0] & 0x1f) << 8) | buff[1];
		buff += 4;
		pmtDescriptor->subfolderCountDescriptor = new struct SubfolderCountDescriptor;
		pmtDescriptor->subfolderCountDescriptor->DescriptorTag = *buff++;
		pmtDescriptor->subfolderCountDescriptor->DescriptorLength = *buff++;
		pmtDescriptor->subfolderCountDescriptor->SubfolderCount = *buff++;
		if (pmtDescriptor->subfolderCountDescriptor->SubfolderCount > 0)
		{
			pmtDescriptor->subfolderCountDescriptor->subfolderDescriptor = 
				new struct SubfolderDescriptor[pmtDescriptor->subfolderCountDescriptor->SubfolderCount];
			for(int j = 0; j < pmtDescriptor->subfolderCountDescriptor->SubfolderCount; j++)
			{
				pmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorTag = *buff++;
				pmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength = *buff++;
				pmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName = 
					new char[pmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength];
				for (int k = 0; k < (pmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].DescriptorLength); k++)
				{
					pmtDescriptor->subfolderCountDescriptor->subfolderDescriptor[j].SubfolderName[k] = *buff++;
				}
			}
		}
		pmtDescriptor->fileDescriptor = new struct FileDescriptor;
		pmtDescriptor->fileDescriptor->DescriptorTag = *buff++;
		pmtDescriptor->fileDescriptor->DescriptorLength = *buff++;
		pmtDescriptor->fileDescriptor->FileLength = ((uint64)*buff << 32) | ((uint64)*(buff+1) << 24) |
			((uint64)*(buff+2) << 16) | ((uint64)*(buff+3) << 8) | ((uint64)*(buff+4));
		buff += 5;
		pmtDescriptor->fileDescriptor->SegmentLength = getBits((uint8*)buff, 0, 32);
		buff += 4;
		pmtDescriptor->fileDescriptor->Option = *buff++;
		pmtDescriptor->fileDescriptor->FileName = new char[pmtDescriptor->fileDescriptor->DescriptorLength - 1 - 5 - 4];
		for (int j = 0; j < pmtDescriptor->fileDescriptor->DescriptorLength - 10; j++)
		{
			pmtDescriptor->fileDescriptor->FileName[j] = *buff++;
		}
	}
	return 0;
}

int film_parser(char* buf, struct FilmDataDescriptor *filmDataDescriptor)
{
	int count = 24;
	filmDataDescriptor->DescriptorTag = getBits((uint8 *)buf, count, 8);
	count+=8;
	filmDataDescriptor->DescriptorLength = getBits((uint8 *)buf, count, 8);
	count+=8;
	filmDataDescriptor->FilmId = getBits((uint8 *)buf, count, 32);
	count+=32;
	filmDataDescriptor->Version = getBits((uint8 *)buf, count, 8);
	count+=8;
	filmDataDescriptor->Type = getBits((uint8 *)buf, count, 8);
	count+=8;
	filmDataDescriptor->SegmentNumber = getBits((uint8*)buf, count, 32);
	count += 32;
#if 0
	int len = filmDataDescriptor->DescriptorLength - ;
	if (filmDataDescriptor->Data != NULL)
	{
		delete[] filmDataDescriptor->Data;
	}
	filmDataDescriptor->Data = new char[];
	char *buff = buf + ;
	memcpy(filmDataDescriptor->Data, buff);
#endif
	return 0;
}

static uint32 m_crc32Table[256] = {
	0x00000000,0x04C11DB7,0x09823B6E,0x0D4326D9,
	0x130476DC,0x17C56B6B,0x1A864DB2,0x1E475005,
	0x2608EDB8,0x22C9F00F,0x2F8AD6D6,0x2B4BCB61,
	0x350C9B64,0x31CD86D3,0x3C8EA00A,0x384FBDBD,
	0x4C11DB70,0x48D0C6C7,0x4593E01E,0x4152FDA9,
	0x5F15ADAC,0x5BD4B01B,0x569796C2,0x52568B75,
	0x6A1936C8,0x6ED82B7F,0x639B0DA6,0x675A1011,
	0x791D4014,0x7DDC5DA3,0x709F7B7A,0x745E66CD,
	0x9823B6E0,0x9CE2AB57,0x91A18D8E,0x95609039,
	0x8B27C03C,0x8FE6DD8B,0x82A5FB52,0x8664E6E5,
	0xBE2B5B58,0xBAEA46EF,0xB7A96036,0xB3687D81,
	0xAD2F2D84,0xA9EE3033,0xA4AD16EA,0xA06C0B5D,
	0xD4326D90,0xD0F37027,0xDDB056FE,0xD9714B49,
	0xC7361B4C,0xC3F706FB,0xCEB42022,0xCA753D95,
	0xF23A8028,0xF6FB9D9F,0xFBB8BB46,0xFF79A6F1,
	0xE13EF6F4,0xE5FFEB43,0xE8BCCD9A,0xEC7DD02D,
	0x34867077,0x30476DC0,0x3D044B19,0x39C556AE,
	0x278206AB,0x23431B1C,0x2E003DC5,0x2AC12072,
	0x128E9DCF,0x164F8078,0x1B0CA6A1,0x1FCDBB16,
	0x018AEB13,0x054BF6A4,0x0808D07D,0x0CC9CDCA,
	0x7897AB07,0x7C56B6B0,0x71159069,0x75D48DDE,
	0x6B93DDDB,0x6F52C06C,0x6211E6B5,0x66D0FB02,
	0x5E9F46BF,0x5A5E5B08,0x571D7DD1,0x53DC6066,
	0x4D9B3063,0x495A2DD4,0x44190B0D,0x40D816BA,
	0xACA5C697,0xA864DB20,0xA527FDF9,0xA1E6E04E,
	0xBFA1B04B,0xBB60ADFC,0xB6238B25,0xB2E29692,
	0x8AAD2B2F,0x8E6C3698,0x832F1041,0x87EE0DF6,
	0x99A95DF3,0x9D684044,0x902B669D,0x94EA7B2A,
	0xE0B41DE7,0xE4750050,0xE9362689,0xEDF73B3E,
	0xF3B06B3B,0xF771768C,0xFA325055,0xFEF34DE2,
	0xC6BCF05F,0xC27DEDE8,0xCF3ECB31,0xCBFFD686,
	0xD5B88683,0xD1799B34,0xDC3ABDED,0xD8FBA05A,
	0x690CE0EE,0x6DCDFD59,0x608EDB80,0x644FC637,
	0x7A089632,0x7EC98B85,0x738AAD5C,0x774BB0EB,
	0x4F040D56,0x4BC510E1,0x46863638,0x42472B8F,
	0x5C007B8A,0x58C1663D,0x558240E4,0x51435D53,
	0x251D3B9E,0x21DC2629,0x2C9F00F0,0x285E1D47,
	0x36194D42,0x32D850F5,0x3F9B762C,0x3B5A6B9B,
	0x0315D626,0x07D4CB91,0x0A97ED48,0x0E56F0FF,
	0x1011A0FA,0x14D0BD4D,0x19939B94,0x1D528623,
	0xF12F560E,0xF5EE4BB9,0xF8AD6D60,0xFC6C70D7,
	0xE22B20D2,0xE6EA3D65,0xEBA91BBC,0xEF68060B,
	0xD727BBB6,0xD3E6A601,0xDEA580D8,0xDA649D6F,
	0xC423CD6A,0xC0E2D0DD,0xCDA1F604,0xC960EBB3,
	0xBD3E8D7E,0xB9FF90C9,0xB4BCB610,0xB07DABA7,
	0xAE3AFBA2,0xAAFBE615,0xA7B8C0CC,0xA379DD7B,
	0x9B3660C6,0x9FF77D71,0x92B45BA8,0x9675461F,
	0x8832161A,0x8CF30BAD,0x81B02D74,0x857130C3,
	0x5D8A9099,0x594B8D2E,0x5408ABF7,0x50C9B640,
	0x4E8EE645,0x4A4FFBF2,0x470CDD2B,0x43CDC09C,
	0x7B827D21,0x7F436096,0x7200464F,0x76C15BF8,
	0x68860BFD,0x6C47164A,0x61043093,0x65C52D24,
	0x119B4BE9,0x155A565E,0x18197087,0x1CD86D30,
	0x029F3D35,0x065E2082,0x0B1D065B,0x0FDC1BEC,
	0x3793A651,0x3352BBE6,0x3E119D3F,0x3AD08088,
	0x2497D08D,0x2056CD3A,0x2D15EBE3,0x29D4F654,
	0xC5A92679,0xC1683BCE,0xCC2B1D17,0xC8EA00A0,
	0xD6AD50A5,0xD26C4D12,0xDF2F6BCB,0xDBEE767C,
	0xE3A1CBC1,0xE760D676,0xEA23F0AF,0xEEE2ED18,
	0xF0A5BD1D,0xF464A0AA,0xF9278673,0xFDE69BC4,
	0x89B8FD09,0x8D79E0BE,0x803AC667,0x84FBDBD0,
	0x9ABC8BD5,0x9E7D9662,0x933EB0BB,0x97FFAD0C,
	0xAFB010B1,0xAB710D06,0xA6322BDF,0xA2F33668,
	0xBCB4666D,0xB8757BDA,0xB5365D03,0xB1F740B4
};

#if 1
uint32 calc_crc32(uint8 *pData,
				  int32 nLength)
{
	uint32 crc32 = 0xffffffff;
	uint32 tmp1, tmp2, tmp3;
	int32 iIndex;

	for(iIndex = 0; iIndex < nLength; iIndex++)
	{
		tmp1 = (crc32<<8);
		tmp2 = m_crc32Table[((crc32 >> 24) ^ *pData) & 0xff];
		tmp3 = tmp1 ^ tmp2;
		crc32 = (crc32 << 8) ^ m_crc32Table[((crc32>>24) ^ *pData++) & 0xff];
	}
	return crc32;
}
#else
uint32 calc_crc32(uint8 *pData, int32 nLength)
{
	uint32 crc = 0;
	unsigned long bytes = nLength;


	while (bytes--) {
		crc = (crc << 8) ^ m_crc32Table[((crc >> 24) ^ *pData++) & 0xFF];
	}

	for (; nLength; nLength >>= 8) {
		crc = (crc << 8) ^ m_crc32Table[((crc >> 24) ^ nLength) & 0xFF];
	}

	crc = ~crc & 0xFFFFFFFF;

	return crc;
}
#endif