#include <iostream>
#include "v15Interface.h"
//#include "v15ProgramInterface.h"

using namespace std ;

int main(int argc,char** argv)
{
	
	if(argc!=3){
		cout<<"Usage:"<<endl;
		cout<<"\t\t test 1 1"<<endl;
		return 0 ;
	}
	vector<InfoData> pInfo;
	//v15GetProgramList(HARDDISK,TYPE_MOVIE,pInfo);
	int nSrc = atoi(argv[1]);
	int type = atoi(argv[2]);
cout<<"Test v15GetProgramList Start"<<endl;
	
	int ret = v15GetProgramList((StorageSource)nSrc,(ProgramType)type,pInfo);
	vector<struct InfoData>::iterator pvector = pInfo.begin();
	struct InfoData* pp = NULL ;
	cout<<"Total Count : "<<pInfo.size()<<endl;
	for(int i = 0 ;pvector != pInfo.end();i++,pvector++)
	{
		pp = &(*pvector) ;
		cout<<"ID 		: "<<pp->pData[0]<<endl;
		cout<<"Name 	: "<<pp->pData[1]<<endl;
		cout<<"TYPE 	: "<<pp->pData[2]<<endl;
		cout<<"Lisence  : "<<pp->pData[3]<<endl;
		cout<<"VideoType: "<<pp->pData[4]<<endl;
		cout<<"PackTYPE : "<<pp->pData[5]<<endl;
		cout<<"Drration : "<<pp->pData[6]<<endl;
		cout<<"Resolution: "<<pp->pData[7]<<endl;
		cout<<"Issuer 	: "<<pp->pData[8]<<endl;
		cout<<"IssueDate: "<<pp->pData[9]<<endl;
		cout<<"Session	: "<<pp->pData[10]<<endl;
		cout<<"Country  : "<<pp->pData[11]<<endl;
		cout<<"CountrySend: "<<pp->pData[12]<<endl;
		cout<<"Language : "<<pp->pData[13]<<endl;
	}
cout<<"Test v15GetProgramList End"<<endl<<endl<<endl;
cout<<"Test v15GetProgramCount Start"<<endl;
	ret = v15GetProgramsCount((StorageSource)nSrc,(ProgramType)type);
	cout<<"Total Count : "<<ret<<endl;
cout<<"Test v15GetProgramCount End"<<endl<<endl<<endl;
cout<<"Test v15GetProgramDetail Start"<<endl;
	struct InfoData tmp;
	ret = v15GetProgramDetail((StorageSource)nSrc,(ProgramType)type,pp->pData[0],tmp);
		cout<<"ID 		: "<<tmp.pData[0]<<endl;
		cout<<"Name 	: "<<tmp.pData[1]<<endl;
		cout<<"TYPE 	: "<<tmp.pData[2]<<endl;
		cout<<"Lisence  : "<<tmp.pData[3]<<endl;
		cout<<"VideoType: "<<tmp.pData[4]<<endl;
		cout<<"PackTYPE : "<<tmp.pData[5]<<endl;
		cout<<"Drration : "<<tmp.pData[6]<<endl;
		cout<<"Resolution: "<<tmp.pData[7]<<endl;
		cout<<"Issuer 	: "<<tmp.pData[8]<<endl;
		cout<<"IssueDate: "<<tmp.pData[9]<<endl;
		cout<<"Session	: "<<tmp.pData[10]<<endl;
		cout<<"Country  : "<<tmp.pData[11]<<endl;
		cout<<"CountrySend: "<<tmp.pData[12]<<endl;
		cout<<"Language : "<<tmp.pData[13]<<endl;

	/*for(int i = 0 ;i<pInfo.size();i++)
	{
		cout<<"ID 		: "<<pInfo[i].id<<endl;
		cout<<"PATH 	: "<<*pp.path<<endl;
		cout<<"TYPE 	: "<<*pp.type<<endl;
		cout<<"Drration : "<<*pp.dur<<endl;
	}*/
cout<<"Test v15GetProgramDetail End"<<endl<<endl<<endl;
	return 0 ;
}
