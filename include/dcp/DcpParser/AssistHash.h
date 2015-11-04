#ifndef ASSISTHASH_H_
#define ASSISTHASH_H_
#include "DcpParser.h"

struct _hash_file_info
{
	std::string    fileName;
	std::string    hashValue;
};

class DCPPARSE_API CAssistHash
{
public:
	CAssistHash(const CPackingList& packingList, const std::string& path);
	virtual ~CAssistHash();

public:
	bool GetHashInfo(std::vector<_hash_file_info>& infos)const;
private:
	const CPackingList& 			m_packingList;
	std::string                 m_basePath;
};
#endif /*ASSISTHASH_H_*/
