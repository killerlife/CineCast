#include <stdafx.h>
#include <stdio.h>
#include <stdlib.h>
// #include <signal.h> 
// #include <unistd.h>

int main(int argc, char **argv)
{
	long long filelength, m_nZtBufSize;
	filelength = atol(argv[1]);
	m_nZtBufSize = atol(argv[2]);
	char* m_pZtBuf = new char[m_nZtBufSize];
	FILE *fp = fopen(argv[3], "rb");
	if(fp)
	{
		fread(m_pZtBuf, 1, m_nZtBufSize, fp);
		fclose(fp);
	}

	long long ReceiveBytes = 0;
	int LostSegments = 0;
	int ReceiveSeg = 0;
	int sec_nums = 4128768;
	int i;
	for (i = 0; i < m_nZtBufSize - 1; i++)
	{
		char ch;
		ch = m_pZtBuf[i];
		for(int j = 0; j < 8; j++)
		{
			if(ch&0x1)
			{
				ReceiveBytes += 533;
				ReceiveSeg++;
			}
			else
			{
				LostSegments++;
			}
			ch = (ch>>1);
		}
	}
	long long nums = sec_nums%8;
	if(nums == 0)
	{
		char ch;
		ch = m_pZtBuf[i];
		for (int j = 0; j < 8; j++)
		{
			if (ch&0x1)
			{
				ReceiveBytes += 533;
				ReceiveSeg++;
			}
			else
			{
				LostSegments++;
			}
			ch = (ch>>1);
		}
	}
	else
	{
		char ch;
		ch = m_pZtBuf[i];
		for(int j = 0; j < nums; j++)
		{
			if (ch&0x1)
			{
				ReceiveBytes += 533;
				ReceiveSeg++;
			}
			else
				LostSegments++;
			ch = (ch>>1);
		}
	}
}