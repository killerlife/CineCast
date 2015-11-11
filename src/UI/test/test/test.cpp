// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

int calccount(int n);

int calcmask(std::string mask)
{
	if (mask.empty())
	{
		return 8;
	}
	char *buf = new char[mask.size() + 1];
	memset(buf, 0, mask.size() + 1);
	memcpy(buf, mask.c_str(), mask.size());

	char * pos = strstr(buf, ".");
	int count = 0;
	if (pos != NULL)
	{
		char s[10];
		memset(s, 0, 10);
		int n;
		memcpy(s, buf, (int)(pos - buf));
		sscanf(s, "%d", &n);
		count += calccount(n);

		pos++;
		char *pos2 = strstr(pos, ".");
		memset(s, 0, 10);
		memcpy(s, pos, (int)(pos2 - pos));
		sscanf(s, "%d", &n);
		count += calccount(n);

		pos2++;
		pos = strstr(pos2, ".");
		memset(s, 0, 10);
		memcpy(s, pos2, (int)(pos - pos2));
		sscanf(s, "%d", &n);
		count += calccount(n);

		pos++;
		sscanf(pos, "%d", &n);
		count += calccount(n);
		delete[] buf;
		return count;
	}
	delete[] buf;
	return 8;
}

int calccount(int n)
{
	if(n > 255)
		return 8;
	char c = (char)n;

	int count = 0;
	for(int i = 0; i < 8; i++)
	{
		if(c & 0x80)
		{
			count++;
			c = c<<1;
		}
		else
			break;
	}
	return count;
}

std::string prefix2mask(int n){
	int t = n;
	int a,b,c,d;
	a = b = c = d = 0;
	if( t <= 8)
	{
		for (int i  = 0; i < t; i++)
		{
			a |= (0x80 >> i);
		}
	}
	else
	{
		a = 0xff;
		t -= 8;
		if(t <=8 )
		{
			for (int i = 0; i < t; i++)
			{
				b |= (0x80 >> i);
			}
		}
		else
		{
			b = 0xff;
			t -= 8;
			if (t <= 8)
			{
				for(int i = 0; i < t; i++)
					c |= (0x80 >> i);
			}
			else
			{
				c = 0xff;
				t -= 8;
				for (int i = 0; i < t; i++)
				{
					d |= (0x80 >> i);
				}
			}
		}
	}
	char s[20];
	sprintf(s, "%d.%d.%d.%d", a, b, c, d);
	std::string ss = s;
	return ss;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string  s = "255.255.255.0";
	int n = calcmask(s);
	std::string ss = prefix2mask(n);

	s = "255.255.255.224";
	n = calcmask(s);
	ss = prefix2mask(n);
	
	s = "255.0.0.0";
	n = calcmask(s);
	ss = prefix2mask(n);

	s = "255.255.0.0";
	n = calcmask(s);
	ss = prefix2mask(n);

	return 0;
}

