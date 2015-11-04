// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "../../../include/xmldatetime.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const std::vector<DATEZONE> IntersectionPlayDate(const std::vector<DATEZONE>& arrFirst,const std::vector<DATEZONE>& arrSecond)
{
	std::vector<DATEZONE> arrReval;
	arrReval.clear();
	DATEZONE dzTmp;
	for(unsigned int i=0;i<arrFirst.size();i++)
	{
#if _DEBUG
		std::cout<<"starttime:"<<arrFirst[i].startDate.GetString()<<"--endtime:"<<arrFirst[i].endDate.GetString()<<std::endl;
#endif
		for(unsigned int j=0;j<arrSecond.size();j++)
		{
#if _DEBUG
			std::cout<<"starttime:"<<arrSecond[j].startDate.GetString()<<"--endtime:"<<arrSecond[j].endDate.GetString()<<std::endl;
#endif
			if(arrFirst[i].startDate >= arrSecond[j].endDate || arrFirst[i].endDate <= arrSecond[j].startDate)
				continue;
			dzTmp = arrFirst[i];
			if(arrFirst[i].startDate < arrSecond[j].startDate)
				dzTmp.startDate = arrSecond[j].startDate;
			if(arrFirst[i].endDate > arrSecond[j].endDate)
				dzTmp.endDate = arrSecond[j].endDate;
			arrReval.push_back(dzTmp);
		}
	}
	return arrReval;
}

void testtime()
{
	std::vector<DATEZONE> arrdt;
	DATEZONE dtz;
	dtz.startDate = _xmldatetime(2006,9,24,10,12,1,0,true);
	dtz.endDate = _xmldatetime(2006,9,28,20,12,1,0,true,7);
	arrdt.push_back(dtz);
	dtz.startDate = _xmldatetime(2006,9,29,10,12,1,0,true);
	dtz.endDate = _xmldatetime(2006,9,30,20,12,1,0,true,7);
	arrdt.push_back(dtz);
	std::vector<DATEZONE> arrcompare = arrdt;
	std::vector<DATEZONE> arrtmp = IntersectionPlayDate(arrdt,arrcompare);
	std::cout<<"count:"<<arrtmp.size()<<std::endl;
}

void testZeroTime()
{
	_xmldatetime zerot("0000-00-00 00:00:00");
	zerot.GmtToLocal();
	zerot.LocalToGmt();
	std::cout<<zerot.GetString()<<std::endl;
}

int main(int argc, char* argv[])
{
	testZeroTime();
	testtime();
	_xmldatetime tmp(1899);
	DATEZONE dz;
	_xmldatetime tt("");
	cout<<tt.GetString()<<std::endl;
	//<
	_xmldatetime tt1(2006,9,27,23,0,0,0,true,0,0);
	if(tt1 < tt)
		cout<<"tt1 < tt is true" <<endl;
	else
		cout<<"tt1 < tt is false" <<endl;
	tt1.GmtToLocal();
	cout<<"local time : "<<tt1.GetString()<<endl;
//	tt + tt1;
	_xmldatetime tt2(2006,9,26,23,0,0,0);
	if(tt2 < tt)
		cout<<"tt2 < tt is true "<<endl;
	else
		cout<<"tt2 < tt is false" <<endl;

	_xmldatetime tt3(2006,9,28,23,0,0,0,true,0,0);
	if(tt3 < tt)
		cout<<"tt3 < tt is true "<<endl;
	else if(tt3 > tt)
		cout<<"tt3 > tt is true" <<endl;
	else 
		cout<<"do not compare"<<endl;

	_xmldatetime tt4(2006,9,28,17,0,0,0);
	if(tt4 < tt)
		cout<<"tt4 < tt is true "<<endl;
	else if(tt4 > tt)
		cout<<"tt4 > tt is true" <<endl;
	else 
		cout<<"do not compare"<<endl;
	_xmldatetime tt5(2006,9,29,17,0,0,0,true,0,0);
	if(tt5 < tt)
		cout<<"tt5 < tt is true "<<endl;
	else if(tt5 > tt)
		cout<<"tt5 > tt is true" <<endl;
	else 
		cout<<"do not compare"<<endl;
	_xmldatetime tt6(2006,9,29,7,0,0,0);
	if(tt6 < tt)
		cout<<"tt6 < tt is true "<<endl;
	else if(tt6 > tt)
		cout<<"tt6 > tt is true" <<endl;
	else 
		cout<<"do not compare"<<endl;

	_xmldatetime tt7(2006,9,28,7,12,10,0,true,0,0);
	_xmldatetime tt8(2006,9,28,15,12,10,0,true,8,0);
	if(tt7 == tt8)
		cout<<"tt7 == tt8 is true "<<endl;
	else
		cout<<"tt7 == tt8 is false" <<endl;
	tt8.LocalToGmt();
	cout<<"gmt time : "<<tt8.GetString()<<endl;
	_xmldatetime tt9(2006,1,1,0,0,0,0,true,8,0);
	tt9.LocalToGmt();
	cout<<"gmt time : "<<tt9.GetString()<<endl;

	return 0;
}

