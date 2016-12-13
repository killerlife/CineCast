#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
#include <exception>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "IContentParser.h"
#include "../brunt/myini/myini.h"
#include <dvb/mytype.h>

using namespace std;
namespace fs = boost::filesystem;
using namespace brunt;

#include <log/Log.h>

class ContentParser :public IContentParser
{
public:
	ContentParser(){
		pLog = CreateLog();
	}
	virtual ~ContentParser()
	{
		close();
		if (pLog)
		{
			ReleaseLog(pLog);
		}
	}

	//for get packinglistFile and composition's absolute file name,path must be absolute path!
	virtual bool open(const std::string& path);
	virtual void close();

	virtual int getProgramNum();
	virtual int parseProgramInfo(DcpInfo& info, int index = 0);
	virtual int getProgramFilePath(std::vector<ReelInfo>&filePathList, int index = 0); //(add by qwy)uuidõcplļ(·)
	virtual int getProgramId(std::string programId, int index = 0);

	virtual int getLastError()
	{
		return m_error;
	}

protected:
	bool my_find_file( const fs::path & dir_path,std::vector<std::string>& childs,bool dir=false);
	void getBasePath(std::string&basePath);
	bool findRecvStatusFiles(const std::string&path, std::string& recvStatus);
	void findXmlFiles(const std::string& path, std::vector<std::string>& xmlFiles);

	std::string getRecvRatio();

private:
	int m_error;

	typedef struct {
		std::string FilmId;
		std::string FilmName;
		std::string Issuer;
		std::string IssueDate;
		std::string Creator;
		std::string ReceiveLength;
		std::string ReceiveStatus;
		std::string LostSegment;
		std::string TotalSegment;
		std::string ReceiveSegment;
		std::string CRCError;
		std::string FilmLength;
		std::string stereoScopic;
		std::string DateTime;
		std::string path;
	}STATUS_INFO;
	STATUS_INFO m_status;
	vector<std::string> m_pklList;
	vector<std::string> m_idList;

	ILog* pLog;
};

ContentParser g_cParser;

IContentParser* CreateContentParser()
{
	return &g_cParser;
}

void ReleaseContentParser(IContentParser** ppParser)
{

}

bool ContentParser::my_find_file( const fs::path & dir_path,std::vector<std::string>& childs,bool dir) 
{
	char str[512];
	if ( !fs::exists( dir_path ) )
	{
		//		cout << "my_find_file error." << endl;
		sprintf(str, "[ContentParser] my_find_file error: %s not exists.", dir_path.native().c_str());
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		return false;
	}
	try
	{
		fs::directory_iterator end_itr; // default construction yields past-the-end
		for ( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
		{
			//			cout << itr->native_file_string() << " dir:" << fs::is_directory( *itr ) << endl;    // liang test

			if (dir && fs::is_directory( *itr ) )
			{

				string str=itr->path().native();
				childs.push_back(str);

			}
			else if (!dir&&!fs::is_directory( *itr ) ) 
			{
				string str=itr->path().native();
				childs.push_back(str);

			}
		}
	}
	catch(const fs::filesystem_error& e)
	{
		sprintf(str, "[ContentParser] my_find_file: except %s.", e.what());
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		//cout <<"catch exception:" << e.what() << "," << childs.size() << endl;
	}
	return true;
}


bool ContentParser::findRecvStatusFiles(const std::string&path,std::string& recvStatus)
{
	fs::path thepath(path);
	vector<string> childFiles;

	if (!my_find_file(thepath, childFiles))
	{
		DPRINTF("[ContentParser] Directory not exist:%s\n", path.c_str());
		return false;
	}
	for(unsigned int i=0; i<childFiles.size(); ++i)
	{
		if (childFiles[i].find(".RecvStatus") != std::string::npos)
		{
			recvStatus = childFiles[i];
			return true;
		}
	}
	return false;
}

void ContentParser::findXmlFiles(const std::string& path, std::vector<std::string>& xmlFiles)
{
	fs::path thepath(path);
	vector<string> childFiles;
	DPRINTF("[ContentParser] findXmlFiles %s\n", path.c_str());

	//	cout << "findPKLFiles(" <<path << ")" << endl;

	if(!my_find_file(thepath,childFiles) )
	{
		return; 
	}

	//	cout << "child size: " << childFiles.size() << endl;

	for(unsigned int i=0; i<childFiles.size(); ++i)
	{
		//		cout << "looking:" << childFiles[i] << endl;

		//Add by Jaontolt
		//Check XML file first!
		if (childFiles[i].find(".xml") != std::string::npos)
		{
			xmlFiles.push_back(childFiles[i]);
		}
	}
}

bool ContentParser::open(const std::string& path)
{
	close();	// to be look at more safety

	char str[512];
	DPRINTF("[ContentParse] open %s\n", path.c_str());
	
	std::string recvStatus;
	if(!findRecvStatusFiles(path, recvStatus))
	{
		sprintf(str, "[ContentParse] open: %s .RecvStatus not exist", path.c_str());
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		DPRINTF("[ContentParse] .RecvStatus not exist\n");
		return false;
	}
	DPRINTF("[ContentParse] RecvStatus found.\n");

	m_status.path = path;

	ICMyini* ini = createMyini();
	if(!ini->load(recvStatus.c_str()))
	{
		sprintf(str, "[ContentParse] open %s error\n", recvStatus.c_str());
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
		DPRINTF("[ContentParse] open %s error\n", recvStatus.c_str());
		return false;
	}

	ini->read(" ", "FilmId", m_status.FilmId);
	ini->read(" ", "FilmName", m_status.FilmName);
	ini->read(" ", "FilmLength", m_status.FilmLength);
	ini->read(" ", "Creator", m_status.Creator);
	ini->read(" ", "CRCError", m_status.CRCError);
	ini->read(" ", "IssueDate", m_status.IssueDate);
	ini->read(" ", "Issuer", m_status.Issuer);
	ini->read(" ", "LostSegment", m_status.LostSegment);
	ini->read(" ", "ReceiveLength", m_status.ReceiveLength);
	ini->read(" ", "ReceiveStatus", m_status.ReceiveStatus);
	ini->read(" ", "TotalSegment", m_status.TotalSegment);
	ini->read(" ", "ReceiveSegment", m_status.ReceiveSegment);
	ini->read(" ", "DateTime", m_status.DateTime);
	//Add for display 99% while one dcp not finish in task
#ifdef ENABLE_RAID
	if(m_status.ReceiveSegment == m_status.TotalSegment && !(strncmp("/raid", path.c_str(), 5) == 0 || strncmp("/storage/ftp", path.c_str(), 12) == 0))
	{
		uint64 seg;
		sscanf(m_status.ReceiveSegment.c_str(), "%lld", &seg);
		char str[50];
		sprintf(str, "%lld", seg - 1);
		m_status.ReceiveSegment = str;
		sscanf(m_status.ReceiveLength.c_str(), "%lld", &seg);
		sprintf(str, "%lld", seg - 1);
		m_status.ReceiveLength = str;
		sprintf(str, "[ContentParser] open raid 99 error: %s.", path.c_str());
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
		}
	}
#else
	if(m_status.ReceiveSegment == m_status.TotalSegment && strncmp("/storage/ftp", path.c_str(), 12) != 0)
	{
		uint64 seg;
		sscanf(m_status.ReceiveSegment.c_str(), "%lld", &seg);
		char str[50];
		sprintf(str, "%lld", seg - 1);
		m_status.ReceiveSegment = str;
		sscanf(m_status.ReceiveLength.c_str(), "%lld", &seg);
		sprintf(str, "%lld", seg - 1);
		m_status.ReceiveLength = str;
		sprintf(str, "[ContentParser] open ftp 99 error: %s.", path.c_str());
		if (pLog)
		{
			pLog->Write(LOG_ERROR, str);
	}
	}
#endif
#if 0
	printf("%s\n", m_status.FilmId.c_str());
	printf("%s\n", m_status.FilmName.c_str());
	printf("%s\n", m_status.FilmLength.c_str());
	printf("%s\n ", m_status.Creator.c_str());
	printf("%s\n ", m_status.CRCError.c_str());
	printf("%s\n ", m_status.IssueDate.c_str());
	printf("%s\n ", m_status.Issuer.c_str());
	printf("%s\n ", m_status.LostSegment.c_str());
	printf("%s\n ", m_status.ReceiveLength.c_str());
	printf("%s\n ", m_status.ReceiveStatus.c_str());
	printf("Total %s\n ", m_status.TotalSegment.c_str());
	printf("Receive %s\n ", m_status.ReceiveSegment.c_str());
	printf("%s\n ", m_status.DateTime.c_str());
#endif
	std::vector<std::string> filesXml;
	findXmlFiles(path, filesXml);

	m_status.stereoScopic = "2D";

	for(int k=0; k<filesXml.size(); k++)
	{
		FILE *fp = fopen(filesXml[k].c_str(), "rb");
		fs::path path(filesXml[k]);
		if(fs::file_size(path) > 1024*5000)
		{
			fclose(fp);
			continue;
		}
		char *buf = new char[fs::file_size(path)];
		fread(buf, fs::file_size(path), 1, fp);
		if(strstr(buf, "PackingList")!=NULL)
			m_pklList.push_back(filesXml[k]);
		delete[] buf;
		fclose(fp);
	}

	return true;
}

void ContentParser::close()
{
	m_pklList.clear();
	m_error = 0;
}

int ContentParser::getProgramNum()
{
	return m_pklList.size();
}

int ContentParser::getProgramFilePath(std::vector<ReelInfo>&filePathList, int index /* = 0 */)
{
	return 0;
}

int ContentParser::getProgramId(std::string programId, int index /* = 0 */)
{
	return 0;
}

#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QString>
#include <QByteArray>
#include <QFile>

int ContentParser::parseProgramInfo(DcpInfo& info, int index /* = 0 */)
{
	char str[512];
	info.recvRatio = getRecvRatio();
	info.filmLength = m_status.FilmLength;
	info.recvStatus = m_status.ReceiveStatus;
	info.totalSegment = m_status.TotalSegment;
	info.recvSegment = m_status.ReceiveSegment;
	info.lostSegment = m_status.LostSegment;
	info.crcError = m_status.CRCError;
	info.recvDateTime = m_status.DateTime;

	if (index < m_pklList.size())
	{
		info.pklFile = m_pklList[index];

		QFile file(info.pklFile.c_str());
		DPRINTF("[ContentParser] parseProgramInfo: %s\n", info.pklFile.c_str());
		if(!file.open(QFile::ReadOnly|QFile::Text))
			return -1;
		
		QString errStr;
		int errLine;
		int errColumn;

		QDomDocument doc;
		if (doc.setContent(&file,
			true, &errStr, &errLine, &errColumn))
		{
			QDomElement root = doc.firstChildElement("PackingList");
			if(root.isNull())
				return -1;
			QDomElement item = root.firstChildElement("Id");
			if (!item.isNull())
				info.id = item.text().toStdString();

			item = root.firstChildElement("AnnotationText");
			if(!item.isNull())
				info.title = item.text().toStdString();

			item = root.firstChildElement("IssueDate");
			if(!item.isNull())
				info.issueDate = item.text().toStdString();

			item = root.firstChildElement("Issuer");
			if(!item.isNull())
				info.issuer = item.text().toStdString();

			item = root.firstChildElement("Creator");
			if(!item.isNull())
				info.creator = item.text().toStdString();

			item = root.firstChildElement("AssetList");
			QDomNodeList list = item.elementsByTagName("Asset");
			for (int i = 0; i < list.size(); i++)
			{
				QDomNode a = list.at(i);
				QDomElement b = a.firstChildElement("Type");
				if(!b.isNull())
					if(b.text() == "text/xml;asdcpKind=CPL")
					{
						b = a.firstChildElement("OriginalFileName");
						if(b.isNull())
							continue;
						std::string p = m_status.path;
						if(p.at(p.length() - 1) != '/')
							p += '/';
						p += b.text().toStdString();
						DPRINTF("[ContentParser] parseProgramInfo: OriginalFileName is %s\n", p.c_str());

						info.stereoScopic = "2D";
						FILE *fp = fopen(p.c_str(), "rb");
						fs::path path(p);
						if(fs::file_size(path) > 1024*5000)
						{
							fclose(fp);
							return 0;
						}
						char *buf = new char[fs::file_size(path)];
						memset(buf, 0, fs::file_size(path));
						fread(buf, fs::file_size(path), 1, fp);
						if(strstr(buf, "MainStereoscopicPicture") != NULL)
							info.stereoScopic = "3D";
						delete[] buf;
						fclose(fp);
						return 0;
					}
			}
			return 0;
		}
		else
		{
			sprintf(str, "[ContentParser] parseProgramInfo error: %s.", errStr.toStdString().c_str());
			if (pLog)
			{
				pLog->Write(LOG_ERROR, str);
			}
			return -1;
	}
	}
	return -1;
}


std::string ContentParser::getRecvRatio()
{
	uint64 i,j;
	i = atoll(m_status.TotalSegment.c_str());
	j = atoll(m_status.ReceiveSegment.c_str());
// 	printf("T %s %d\n", m_status.TotalSegment.c_str(), i);
// 	printf("R %s %d\n", m_status.ReceiveSegment.c_str(), j);
	if(i != 0)
	i = j*100/i;
	char buf[10];
	sprintf(buf, "%d", i);
	std::string s = buf;
	return s;
}

