#include "tmstest.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QString>
#include <QByteArray>
#include <QDomProcessingInstruction>

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/md5.h>

tmstest::tmstest(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(&dev, SIGNAL(connected()), this, SLOT(connected()));
}

tmstest::~tmstest()
{

}
#include <QFile>
#include <vector>

#pragma pack(1)

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned long uint64;

typedef struct _ENCODEBLOCK_
{
	uint64 EncodeBegin;
	uint64 EncodeEnd;
}ENCODEBLOCK;

typedef struct _MD5BLOCK_
{
	uint64 Md5Begin;
	uint64 Md5End;
	char md5[16];
	_MD5BLOCK_()
	{
		memset(md5, 0, 16);
	}
}MD5BLOCK;

typedef struct _ENCODEFILE_
{
	std::string Name;
	uint16 Id;
	bool Encode;
	bool Md5;
	std::string SubPath;
	std::vector<ENCODEBLOCK> EncodeBlockList;
	std::vector<MD5BLOCK> Md5BlockList;
public:
	void ENCODEFILE()
	{
		Encode = false;
		Md5 = false;
	}
	~_ENCODEFILE_()
	{
		EncodeBlockList.clear();
		Md5BlockList.clear();
	};
	void clear()
	{
		EncodeBlockList.clear();
		Md5BlockList.clear();
	};
}ENCODEFILE;

typedef struct _ENCODEFILELIST_
{
	std::string Name;
	uint32 Id;
	std::string ChallengeCode;
	std::vector<ENCODEFILE> EncodeFileList;
public:
	void clear()
	{
		for(int i = 0; i < EncodeFileList.size(); i++)
		{
			EncodeFileList[i].clear();
		}
		EncodeFileList.clear();
	}
} ENCODEFILELIST;

ENCODEFILELIST efl;

bool Md5Parser(char *buf)
{
	QString errStr;
	int errLine;
	int errColumn;

	QString uuidText;
	QString msg;

	QDomDocument doc;
	try
	{
		if (doc.setContent(QByteArray(buf),
			true, &errStr, &errLine, &errColumn))
		{
			efl.clear();
			QDomElement root = doc.firstChildElement("EncodeInfo");
			QDomElement EncodeFileList = root.firstChildElement("EncodeFileList");
			QString id = EncodeFileList.attribute("ID");
			QString name = EncodeFileList.attribute("Name");
			QDomElement CCode = EncodeFileList.firstChildElement("ChallengeCode");
			QString code = CCode.text();
			efl.Id = id.toULong();
			efl.Name = name.toStdString();
			efl.ChallengeCode = code.toStdString();

			QDomNodeList EncodeFile = EncodeFileList.elementsByTagName("EncodeFile");
			for (int i = 0; i < EncodeFile.count(); i++)
			{
				ENCODEFILE ef;
				QDomElement element = EncodeFile.at(i).toElement();
				QString s = element.attribute("Name");
				ef.Name = s.toStdString();
				s = element.attribute("ID");
				ef.Id = s.toUShort();
				s = element.attribute("EncodeCount");
				ef.Encode = (bool)s.toUShort();
				s = element.attribute("MD5Count");
				ef.Md5 = (bool)s.toUShort();
				s = element.attribute("SubPath");
				ef.SubPath = s.toStdString();

				if(ef.Encode)
				{
					QDomElement EBL = element.firstChildElement("EncodeBlockList");
					QDomNodeList elist = EBL.elementsByTagName("EncodeBlock");
					uint32 c = elist.count();
					uint32 ii;
					for (ii = 0; ii < elist.count(); ii++)
					{
						ENCODEBLOCK eb;
						QDomElement e = elist.at(ii).toElement();
						QDomElement tmp = e.firstChildElement("EncodeBeginPoint");
						QString s = tmp.text();
						eb.EncodeBegin = s.toULongLong();
						tmp = e.firstChildElement("EncodeEndPoint");
						s = tmp.text();
						eb.EncodeEnd = s.toULongLong();
						ef.EncodeBlockList.push_back(eb);
					}
				}
				if(ef.Md5)
				{
					QDomElement MD5 = element.firstChildElement("MD5BlockList");
					QDomNodeList mlist = MD5.elementsByTagName("MD5Block");
					uint32 c = mlist.count();
					uint32 a = 0;
					for (a = 0; a < mlist.count(); a++)
					{
						MD5BLOCK mb;
						QDomElement e = mlist.at(a).toElement();
						QDomElement tmp = e.firstChildElement("MD5BeginPoint");
						QString s = tmp.text();
						mb.Md5Begin = s.toULongLong();
						tmp = e.firstChildElement("MD5EndPoint");
						s = tmp.text();
						mb.Md5End = s.toULongLong();
						tmp = e.firstChildElement("MD5");
						s = tmp.text();
						sscanf(s.toStdString().c_str(),
							"%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
							&mb.md5[0], &mb.md5[1], &mb.md5[2], &mb.md5[3],
							&mb.md5[4], &mb.md5[5], &mb.md5[6], &mb.md5[7],
							&mb.md5[8], &mb.md5[9], &mb.md5[10], &mb.md5[11],
							&mb.md5[12], &mb.md5[13], &mb.md5[14], &mb.md5[15]);
						ef.Md5BlockList.push_back(mb);
					}
				}
				efl.EncodeFileList.push_back(ef);
			}
			return true;
		}
		else
		{
#if 0
			if(gLog)
			{
				gLog->Write(LOG_ERROR, "[NetCommThread] Md5 Parse error.");
			}
#endif
			throw -1;
		}
	}
	catch(int&)
	{
		return false;
	}
}

bool GetMd5(FILE* fp, MD5BLOCK mb)
{
	if(fp <= 0)
		return false;

	MD5_CTX c;
	MD5_Init(&c);
	
	char buf[1024*1024];
	int count = (mb.Md5End - mb.Md5Begin)/1024/1024;
	fseek(fp, mb.Md5Begin, SEEK_SET);

	for(int i = 0; i < count; i++)
	{
		fread(buf, 1024*1024, 1, fp);
		MD5_Update(&c, buf, 1024*1024);
	}
	count = (mb.Md5End - mb.Md5Begin)%(1024/1024);
	if (count > 0)
	{
		fread(buf, count, 1, fp);
		MD5_Update(&c, buf, count);
	}

	char md[16];
	memset(md, 0, 16);
	MD5_Final((unsigned char*)md, &c);
	if (memcmp(md, mb.md5, 16) == 0)
	{
		return true;
	}
	return false;
}

bool Md5Verify()
{
	std::string fn;
	uint64 rollback = 0;
	for (int i = 0; i < efl.EncodeFileList.size(); i++)
	{
		ENCODEFILE ef = efl.EncodeFileList.at(i);
		if(ef.Md5)
		{
			fn = "/storage/" + ef.SubPath + "/" + ef.Name;
			FILE *fp = fopen(fn.c_str(), "rb");
			for (int j = 0; j < ef.Md5BlockList.size(); i++)
			{
				MD5BLOCK mb = ef.Md5BlockList.at(i);
				if(GetMd5(fp, mb))
					rollback += (mb.Md5End - mb.Md5Begin);
				else
				{
					fclose(fp);
					return rollback;
				}
			}
			fclose(fp);
		}
	}
	return rollback;
}

void tmstest::on_pushButton_clicked()
{
	FILE*fp = fopen("r:\\1.xml", "rb");

	char buf[4096];

	memset(buf, 0, 4096);

	int i = fread(buf, 1, 4096, fp);
	fclose(fp);
	Md5Parser(buf);


	fp = fopen("r:\\02.mxf.zt", "rb");
	MD5BLOCK mb;
	mb.Md5Begin = 0;
	mb.Md5End = 9648129;
	GetMd5(fp, mb);


#if 0
	QDomDocument doc;
	QDomProcessingInstruction instruction;  
	instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");  
	doc.appendChild(instruction); 

	QDomElement resp = doc.createElement("response");
	doc.appendChild(resp);

	resp.setAttribute("status", "OK");
	resp.setAttribute("version", "2");
	QDomElement list = doc.createElement("dcp_list");
	resp.appendChild(list);
	
	QDomElement dcp = doc.createElement("dcp");
	list.appendChild(dcp);

	QDomText txt;
	QDomElement item;
	item = doc.createElement("uuid");
	dcp.appendChild(item);
	txt = doc.createTextNode("sdfsjfksfsf");
	item.appendChild(txt);

	item = doc.createElement("issueDate");
	dcp.appendChild(item);
	txt = doc.createTextNode("sdfsjfksfsf");
	item.appendChild(txt);

	item = doc.createElement("issuer");
	dcp.appendChild(item);
	txt = doc.createTextNode("sdfsjfksfsf");
	item.appendChild(txt);

	item = doc.createElement("creator");
	dcp.appendChild(item);
	txt = doc.createTextNode("sdfsjfksfsf");
	item.appendChild(txt);

	QString ss = doc.toString();
#endif
}

void tmstest::on_pushButton_2_clicked()
{
	dev.connectToHost(ui.lineEdit->text(), 20080);
}

void tmstest::connected()
{
	login();

	//ftp();
	list();
}

void tmstest::login()
{
	char buf[100];
	memset(buf, 0, 100);
	buf[0] = 0x55;
	short cmd = 0x11;
	memcpy(buf + 1, &cmd, sizeof(cmd));
	int i = 0;
	memcpy(buf + 3, &i, sizeof(i));

	dev.write(buf, 3+ 4+ i+4);
	dev.waitForBytesWritten(-1);
	dev.waitForReadyRead(-1);
	memset(buf, 0, 100);
	i = dev.read(buf, 1024);
}

void tmstest::end()
{
	FILE*fp = fopen("r:\\test.xml", "rb");

	char buf[1024];
	char buf2[1024];

	memset(buf, 0, 1024);
	memset(buf2, 0 ,1024);

	int i = fread(buf, 1, 1024, fp);

	buf2[0] = 0x55;
	short cmd = 0x27;
	memcpy(buf2 + 1, &cmd, sizeof(cmd));
	memcpy(buf2 + 3, &i, sizeof(i));
	memcpy(buf2 + 3 + 4, buf, i);
	memcpy(buf2 + 3 + 4 + i, &i, sizeof(i));

	dev.write(buf2, 3+4+i+4);
	dev.waitForBytesWritten(-1);
	dev.waitForReadyRead(-1);
	i = dev.read(buf, 1024);
}

void tmstest::list()
{
	char buf[1024];
	memset(buf, 0, 100);
	buf[0] = 0x55;
	short cmd = 0x21;
	memcpy(buf + 1, &cmd, sizeof(cmd));
	int i = 0;
	memcpy(buf + 3, &i, sizeof(i));

	dev.write(buf, 3+ 4+ i+4);
	dev.waitForBytesWritten(-1);
	dev.waitForReadyRead(-1);
	i = dev.read(buf, 4+2+1);
	int len = *(uint32*)(buf+3);
	char *str = new char[len+4];
	i = dev.read(str, len+4);
	str[len] = '\0';
	ui.textBrowser->setText(str);
	delete[] str;
}

void tmstest::ftp()
{
	char buf[1024];
	buf[0] = 0x55;

	QDomDocument doc;
	QDomProcessingInstruction instruction;  
	instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");  
	doc.appendChild(instruction); 

	QDomElement resp = doc.createElement("response");
	doc.appendChild(resp);

	resp.setAttribute("status", "OK");
	resp.setAttribute("version", "2");
	QDomElement list = doc.createElement("uuid");
	resp.appendChild(list);

	QDomText txt;
	QString fuid = ui.lineEdit_2->text();
	txt = doc.createTextNode(fuid);
	list.appendChild(txt);

	QString s = doc.toString();
	short cmd = 0x24;
	memcpy(buf + 1, &cmd, sizeof(cmd));
	int len = s.toStdString().size();
	memcpy(buf + 1 + 2, &len, sizeof(len));

	char *pos = buf + 1 + 2 + 4;
	memcpy(pos, s.toStdString().c_str(), len);
	memcpy(pos + len, &len, sizeof(len));
	int setsize = len + 7 + sizeof(len);

	dev.write(buf, setsize);
	dev.waitForBytesWritten(-1);
	dev.waitForReadyRead(-1);
	int i = dev.read(buf, 4+2+1);
	len = *(uint32*)(buf+3);
	char *str = new char[len+4];
	i = dev.read(str, len+4);
	str[len] = '\0';
	ui.textBrowser_2->setText(str);
}

void tmstest::on_pushButton_3_clicked()
{
	ftp();
}