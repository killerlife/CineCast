#ifndef GETSPACE_
#define GETSPACE_
#include <vector>
#include <string>
class CGetSpace
{
public:
	unsigned int getAvalibleSpace(const std::string path);
	unsigned int getFileSize(const std::vector<std::string>& files);
	// Aavan: 2008-7-3
	bool copy_find_file(const std::string& dpath, std::vector<std::string>& childs, bool &isdms);
	std::string getSaveFile();
	
private:
	void separateString(const std::string& str, char ch, std::vector<std::string>& str_vec, unsigned int separatenum = 0xffffffff);
private:
	std::string m_sSaveFile;

};
#endif /*GETSPACE_*/
