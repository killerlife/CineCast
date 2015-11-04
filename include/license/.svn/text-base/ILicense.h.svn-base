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
	// 释放
	virtual void Release() = 0;
	// 验证license是否有效
	virtual LICENSEVERIFY verify() = 0;
	// 取得license的类型(LICENSETYPE)
	virtual int getType() = 0;
	// 检查lincese的类型(LICENSETYPE)是否正确
	virtual bool checkType() = 0;
	// 取得license的详细信息
	virtual int getInfo(std::vector<TLicenseInfo>& infos) = 0;
	// 取得Movie Id,  sample: 'b8d1b012-083c-4419-8954-f91809d49f9e'
	virtual const std::string& getMovieId() = 0;
	// 移除movieCode影片的license
	virtual int remove(const std::string& movieCode) = 0;
	// 导入movieCode影片的license
	virtual int import(const std::string& movieCode) = 0;
	// 返回出错信息
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
	// 查找是否存在license，有的话获取ILicenseCtrl。type (LICENSE_TYPE)
	virtual ILicenseCtrl* queryILicenseCtrl(const std::string& movieCode) = 0;
};

LICENSE_API ILicenseManager* getLicenseManager();


#endif	//__ILICENSE_H_200811251533__

