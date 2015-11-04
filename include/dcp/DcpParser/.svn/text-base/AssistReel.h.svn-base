#ifndef ASSITREEL_H_
#define ASSITREEL_H_
#include "DcpParser.h"

struct reel_v_file_info
{
public:
	std::string keyIdStr;
	std::string fileName;
	long    	entryPoint;
	long		duration;
	RATIONAL	aspectRatio;
	RATIONAL	frameRate;
	std::string videoFormat; //"JPEG2000 MPEG2 MPEG2TS Wav"
	reel_v_file_info()
		: keyIdStr("")
		, fileName("")
		, entryPoint(0)
		, duration(0)
		, aspectRatio(0, 0)
		, frameRate(0, 0)
		, videoFormat("")
	{};
};

struct reel_a_file_info
{
public:
	std::string keyIdStr;
	std::string fileName;
	std::string language;
	long    	entryPoint;
	long		duration;
	std::string audioFormat; //"JPEG2000 MPEG2 MPEG2TS Wav"
	int         channelCount;
	int         sampleRate;
	reel_a_file_info()
		: keyIdStr("")
		, fileName("")
		, language("")
		, entryPoint(0)
		, duration(0)
		, audioFormat("")
		, channelCount(0)
		, sampleRate(0)
	{};
};

struct _reel_info
{
	int					m_reelIndex; //[0,...]
	reel_v_file_info	m_videoInfo;
	reel_a_file_info	m_audioInfo;
	_reel_info():m_reelIndex(-1)
	{
	}
};

class DCPPARSE_API CAssistReel
{
public:
	CAssistReel(const CCompositionPlayList& cpl, const CPackingList& packingList, const std::string& path);
	virtual ~CAssistReel();

public:
	int GetReelCount()const;
	bool GetReelInfo(int index, _reel_info& info)const;
	bool GetReelPaths(std::vector<std::string>& paths);

protected:
	std::string getFilePath(CComposition_TrackFileAsset& track)const;
private:
	const CCompositionPlayList& m_cpl;
	const CPackingList& 		m_packingList;
	std::string                 m_basePath;
};

#endif /*ASSITREEL_H_*/
