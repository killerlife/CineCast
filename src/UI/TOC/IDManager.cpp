#include "IDManager.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QByteArray>
#include <QDomProcessingInstruction>
#include <QTextStream>

IDManager gIDManager;

IDManager* GetIDManager()
{
	return &gIDManager;
}

IDManager::IDManager(void)
{
	ReadIdFile();
}

IDManager::~IDManager(void)
{
}

void IDManager::ReadIdFile(void)
{
	QFile file("Devices.xml");
	if(!file.open(QFile::ReadOnly|QFile::Text))
		return;
	QString errStr;
	int errLine;
	int errColumn;
	QDomDocument doc;
	if (!doc.setContent(&file, true, &errStr, &errLine, &errColumn))
	{
		file.close();
		return;
	}
	file.close();
	id_list.clear();
	QDomElement root = doc.documentElement();
	QDomNodeList list = root.elementsByTagName("Device");
	for (int i = 0; i < list.count(); i++)
	{
		QDomElement e = list.at(i).toElement();
		QDomElement t = e.firstChildElement("ID");
		_ID_ ID;
		ID.id = t.text().toLong();
		t = e.firstChildElement("Key");
		ID.key = t.text();
		id_list.append(ID);
	}
}

bool IDManager::GetHardKeyById(uint32 id, char* key)
{
	for (int i = 0; i <= id_list.size(); i++)
	{
		_ID_ tid = id_list.at(i);
		if(tid.id == id)
		{
			sscanf(tid.key.toStdString().c_str(), "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", 
				key+0, key+1, key+2, key+3, key+4, key+5, key+6, key+7,
				key+8, key+9, key+10, key+11, key+12, key+13, key+14, key+15);
// 			memcpy(key, tid.key.toStdString().c_str(), 16);
			return true;
		}
	}
	return false;
}
