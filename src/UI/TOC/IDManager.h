#pragma once
#include <QList>
#include <QString>
#include "mytype.h"

class _ID_
{
public:
	uint32 id;
	QString key;
};

class IDManager
{
public:
	IDManager(void);
	~IDManager(void);

private:
	void ReadIdFile(void);

private:
	QList<_ID_> id_list;
public:
	bool GetHardKeyById(uint32 id, char* key);
};

IDManager* GetIDManager();