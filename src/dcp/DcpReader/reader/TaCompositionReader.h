#ifndef MXFAPP_TACOMPOSITION_READER_H
#define MXFAPP_TACOMPOSITION_READER_H
#include "ITaCompositionReader.h"
#include "TaTrackFileReader.h"
#include <string>
#include <list>

class CTrack;

class CTaCompositionReader :public ITaCompositionReader
{
public:

	CTaCompositionReader();
	virtual ~CTaCompositionReader();
	
	virtual bool	open(const char* compositionListFile,const char* packingListFile);
	virtual void	close();
	
	virtual mxflib::Rational	frameRate();
	virtual mxflib::UInt64		duration();
	
	virtual  bool				videoProperty(VideoProperty& property);
	virtual  bool				audioProperty(AudioProperty& property);
	virtual  bool				subTitleProperty(SubtileProperty& property);
	
	virtual  FrameData			readVideoFrame();
	virtual  FrameData			readAudioFrame();
	
	virtual  bool 				seek(mxflib::UInt64 frameIndex);
	virtual  mxflib::UInt64		nextPosition(ESSENCE_TYPE type = essence_video);
	
	virtual  const std::vector<ReelInfo>& getReelInfo();
	virtual  int				curReel();
	virtual std::string getLastError()
	{
		return m_error;
	}
	
private:
	bool    checkXmlFile(const char* compositionListFile,const char* packingListFile);
	bool    setOffsetAndDur(CTaTrackFileReader& reader,CComposition_TrackFileAsset& track);
	void	clear();
	bool	addReader(CPackingList& packingList,CComposition_TrackFileAsset& track,std::list<CTaTrackFileReader>& readers);
	bool    addVideoReader(CPackingList& packingList,CComposition_Reel& reel);
	bool	addAudioReader(CPackingList& packingList,CComposition_Reel& reel);
	bool    checkReaders();
	bool    initReel();
		
    FrameData readFrame(std::list<CTaTrackFileReader>& readers,std::list<CTaTrackFileReader>::iterator& curIte);
	bool    seekEssence(mxflib::UInt64 time,std::list<CTaTrackFileReader>& readers,std::list<CTaTrackFileReader>::iterator& curIte);

private:
	bool m_isOpen;
	mxflib::UInt64	   m_duration;
	std::string		   m_basePath,m_error;

	std::list<CTaTrackFileReader> m_videoReaders;
	std::list<CTaTrackFileReader> m_audioReaders;
	std::list<CTaTrackFileReader>::iterator  m_curVideoReader,m_curAudioReader;
	
	std::vector<ReelInfo> m_reels;
};
#endif

