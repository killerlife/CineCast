#include "tmstest.h"
#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QByteArray>
#include <QDomProcessingInstruction>

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

void tmstest::on_pushButton_clicked()
{

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
}

void tmstest::on_pushButton_2_clicked()
{
	dev.connectToHost("169.254.1.230", 20080);
}

void tmstest::connected()
{
	login();

	ftp();
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
	i = dev.read(buf, 1024);
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
	txt = doc.createTextNode("urn:uuid:8ab8ea15-2a40-4816-b731-52f7c943c301");
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
	int i = dev.read(buf, 1024);
}