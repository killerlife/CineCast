#ifndef __V15_PROGRAM_INTERFACE_H
#define __V15_PROGRAM_INTERFACE_H

//#include <ProgramQuery.h>
//#include <ProgramParse.h>
//#include <activeThread.h>

#include <sstream>
#include <string>
#include <vector>

struct InfoData ;

class CV15ProgramInfo
{
public:
	// type of getStatus() return's value
	enum{UPDATEOK=0, UPDATING};

	CV15ProgramInfo();
	virtual ~CV15ProgramInfo();

//	std::vector<struct InfoData>& getProgramFileList(int nSrc,int type);
	int getStatus();
	int update(const vector<int>& updateSrcList);	// start the thread
	int getProgramFileList(int nSrc, std::vector<struct InfoData>& infoList);

protected:
//   void loadInfo();

	void loadInfo(int nSrc);
	std::string getProgramPath(const std::string& programId);
	inline const char* getProgramSrc(int src);
	void cleanThread();

private:
// 	std::vector<struct InfoData>      m_programInfoList[6];//zhujiaqi 20081114 [2]; //for listView
 	std::vector<struct InfoData>      m_programInfoList[V15S_SIZEOF];	// 0: hdisk , 1: USB
	bool m_infoValidMark[V15S_SIZEOF];
   	int m_error; //0: hdisk, 1:usb
	vector<int> m_updateSrcList;		// loadInfo() parameters
	std::string m_curProgramId;
	int m_updateStatus;
	boost::mutex m_mutexInfoList;
};
#endif
