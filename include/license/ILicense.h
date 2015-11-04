//////////////////////////////////////////////////////////////////////////
//license control h
#ifndef __ILICENSE_H_200811251533__
#define __ILICENSE_H_200811251533__

#include <string>
#include <vector>
#include "LicenseDef.h"


class LICENSE_API ILicenseCtrl
{
public:
	// �ͷ�
	virtual void Release() = 0;
	// ��֤license�Ƿ���Ч
	virtual LICENSEVERIFY verify() = 0;
	// ȡ��license������(LICENSETYPE)
	virtual int getType() = 0;
	// ���lincese������(LICENSETYPE)�Ƿ���ȷ
	virtual bool checkType() = 0;
	// ȡ��license����ϸ��Ϣ
	virtual int getInfo(std::vector<TLicenseInfo>& infos) = 0;
	// ȡ��Movie Id,  sample: 'b8d1b012-083c-4419-8954-f91809d49f9e'
	virtual const std::string& getMovieId() = 0;
	// �Ƴ�movieCodeӰƬ��license
	virtual int remove(const std::string& movieCode) = 0;
	// ����movieCodeӰƬ��license
	virtual int import(const std::string& movieCode) = 0;
	// ���س�����Ϣ
	virtual char* getLastError() = 0;

	virtual ~ILicenseCtrl(){};
};

typedef enum{
	LT_ECP = 0,
	LT_DMS,
	LT_KDM
}LICENSE_TYPE;

class LICENSE_API ILicenseManager
{
public:
	// �����Ƿ����license���еĻ���ȡILicenseCtrl��type (LICENSE_TYPE)
	virtual ILicenseCtrl* queryILicenseCtrl(const std::string& movieCode) = 0;
};

LICENSE_API ILicenseManager* getLicenseManager();


#endif	//__ILICENSE_H_200811251533__

