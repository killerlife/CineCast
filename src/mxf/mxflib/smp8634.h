#ifndef MXFLIB__SMP8634__H
#define MXFLIB__SMP8634__H
#include <vector>
//baiqian 
//the structs are for DMA buffer reading of smp8634;
namespace mxflib
{
	


struct SMP_FrameBuffer
{
	unsigned char* buf;
	int   buflen;
	int 	startOffset;
	unsigned char* data()
	{
		return buf + startOffset;
	}
	int validLen()
	{
		return buflen - startOffset;	
	}
	SMP_FrameBuffer(unsigned char* pbuf = 0, int len = 0, int offset = 0):buf(pbuf),buflen(len),startOffset(offset)
	{
	}
};

struct SMP_Frame
{
	int len;
	int secOffset;
	int plainLen;
	unsigned char IV[16];
	//std::vector<SMP_FrameBuffer>& frameBufs;
	SMP_Frame():len(0),secOffset(0),plainLen(0)//,frameBufs(ref)
	{
	}
	bool empty()
	{
		return len == 0;
	}

};

struct SMP_FrameBufferParam
{
	bool                         encrypted; //in
	std::vector<SMP_FrameBuffer> frameBufs; //in 
	SMP_Frame		          result;    //out
	SMP_FrameBufferParam():encrypted(false)
	{
	}
	int length()
	{
		int len = 0;
		for(unsigned int i = 0; i < frameBufs.size(); ++i)
			len += frameBufs[i].validLen();
		return len;
	}
};

}
#endif


