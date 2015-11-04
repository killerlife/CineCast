#ifndef MXFAPP_ITACOMPOSITION_READER_H
#define MXFAPP_ITACOMPOSITION_READER_H

#ifdef  _WIN32

#ifdef DCPREADER_EXPORTS
#define DCPREADER_API __declspec(dllexport)
#else
#define DCPREADER_API __declspec(dllimport)
#endif

#else
#define DCPREADER_API 

#endif

#include <mxfEssenceReader/mxfEssenceReader.h>
#include <vector>


struct ReelInfo
{
	int			   m_reelIndex; //[0,...]
	EncryptInfo    m_videoEncInfo;
	EncryptInfo    m_audioEncInfo;
	mxflib::UInt64 m_startFrame; //in composition frame range
	mxflib::UInt64 m_endFrame;   //in composition frame range
	bool		   m_seamless;
	bool isViedoEncrypt()const
	{
		return m_videoEncInfo.isValid();
	}
	bool isAudioEncrypt()const
	{
		return m_audioEncInfo.isValid();
	}
	ReelInfo():m_reelIndex(-1),m_startFrame(0),m_endFrame(0),m_seamless(true)
	{
	}

};

class DCPREADER_API ITaCompositionReader
{
public:
	enum ESSENCE_TYPE
	{
		essence_video =0,
		essence_audio =1,
		essence_subtitle=2,
			
	};
	// input filename: composition file name and packinglist file name,these decide a composition/movie;
	virtual bool	open(const char* compositionListFile,const char* packingListFile)=0;
	virtual void	close()=0;
	//now ,frameRate maybe 24/1 or 48/1
	virtual	 mxflib::Rational	frameRate()=0;
	//duration: =  total numbers of frame
	virtual  mxflib::UInt64		duration()=0;
	//always return true,for DCP must include video data;
	virtual  bool				videoProperty(VideoProperty& property)=0;
	//false: mean no audio data;
	virtual  bool				audioProperty(AudioProperty& property)=0;
	//false: mean no subtitle data;
	virtual  bool				subTitleProperty(SubtileProperty& property)=0;
	//read one frame	
	virtual  FrameData			readVideoFrame()=0;
	virtual  FrameData			readAudioFrame()=0;
	//seek some place, frameIndex=[0,duration()]
	virtual  bool 				seek(mxflib::UInt64  frameIndex)=0;
	//get current frameIndex;
	virtual  mxflib::UInt64		nextPosition(ESSENCE_TYPE type=essence_video)=0;
	
	virtual  const std::vector<ReelInfo>& getReelInfo()=0;
	
	virtual  int				curReel()=0;

	//TODO:
	virtual std::string getLastError()=0;
};

DCPREADER_API ITaCompositionReader* createCompositionReader();
DCPREADER_API void  releaseCompositionReader(ITaCompositionReader*);

#endif
