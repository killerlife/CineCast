#if _MSC_VER > 1000     // VC++
#pragma warning( disable : 4786 )   // disable warning debug symbol > 255...
#endif // _MSC_VER > 1000


#include <dcpwrapinclude.h>

#include <cassert>
#include <sstream>
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include "TaCompositionReader.h"
#include "assetMap.h"
using namespace std;
using namespace mxflib;
namespace fs = boost::filesystem;

bool my_find_file( const fs::path & dir_path,std::vector<std::string>& childs,bool dir=false) 
{
	if ( !fs::exists( dir_path ) )
	{
		//cout <<"find_file fail,"<<dir_path.native_file_string()<<endl;
		return false;
	}
	fs::directory_iterator end_itr; // default construction yields past-the-end
	for ( fs::directory_iterator itr( dir_path );
	itr != end_itr;
	++itr )
	{
		
		if (dir && fs::is_directory( *itr ) )
		{
		
            string str=itr->path().native();
			childs.push_back(str);
			
		}
		else if (!dir&&!fs::is_directory( *itr ) ) 
		{
			string str=itr->path().native();
			childs.push_back(str);
			
		}
	}
	return true;
}

DCPREADER_API ITaCompositionReader* createCompositionReader()
{
	return new CTaCompositionReader();

}

DCPREADER_API void  releaseCompositionReader(ITaCompositionReader* pReader)
{
	delete (CTaCompositionReader*)pReader;
}

CTaCompositionReader::CTaCompositionReader():m_isOpen(false),m_duration(0)
{
	
}

CTaCompositionReader::~CTaCompositionReader()
{
	close();
}	

std::string getFileSrc(CComposition_TrackFileAsset& track , CPackingList& packing)
{
	try
	{
		string uuid = track.GetUUIDElement();
		CPack_AssetList assetList = packing.GetAssetList();
		for(int i = 0;i < assetList.GetChildCount(); ++i)
		{
			CPack_Asset asset = assetList.GetAssetByIndex(i);
			if(asset.GetUUIDElement() == uuid)
			{
				return asset.GetOriginalFileNameElement().strValue;
			}
		}
	}
	catch(std::exception&)
	{
	  
	}
	return "";
}

bool CTaCompositionReader::setOffsetAndDur(CTaTrackFileReader& reader,CComposition_TrackFileAsset& track)
{
	reader.setEssenceDuration(track.GetDurationElement());
	reader.setOffset(track.GetEntryPointElement());
	return true;
}

bool CTaCompositionReader::addReader(CPackingList& packingList,CComposition_TrackFileAsset& track,std::list<CTaTrackFileReader>& readers)
{

	std::string file = m_basePath+getFileSrc(track,packingList);
	if(file == m_basePath)
	{
		CTaAssetMap assertMap;
		if(assertMap.open(m_basePath))
		{
			file = assertMap.findFile(track.GetUUIDElement());
			assertMap.close();
		}
		if(file.empty())
		{
			m_error = "track file is null!";
			return false;
		}
		file = m_basePath + file;
	}
	ITaMxfEssenceReader* peader = createEssenceReader();
	if(peader->open(file.c_str()))
	{
		CTaTrackFileReader Reader(peader);
		setOffsetAndDur(Reader,track);
		readers.push_back(Reader);
		return true;
	}
	releaseEssenceReader(peader);
	ostringstream os;
	os <<"open " <<file << " fail!";
	m_error = os.str();
	return false;
}

bool CTaCompositionReader::addVideoReader(CPackingList& packingList,CComposition_Reel& reel)
{
	CComposition_AssetList assetList = reel.GetAssetList();
	try
	{
		CComposition_PictureTrackFileAsset asset = assetList.GetPicture();
		return addReader(packingList,asset,m_videoReaders);
	}
	catch (std::exception&) 
	{
		m_error ="no video track,catch an exception when get video track! ";
	}
	return false;
}

bool CTaCompositionReader::addAudioReader(CPackingList& packingList,CComposition_Reel& reel)
{
	CComposition_AssetList assetList = reel.GetAssetList();
	try
	{
		CComposition_SoundTrackFileAsset asset = assetList.GetSound();
		return addReader(packingList,asset,m_audioReaders);
	}
	catch (std::exception&)
	{
		// no aduio track
	}
	//It's normal that no audio track
	CTaTrackFileReader Reader(NULL);
	m_audioReaders.push_back(Reader);
	return true;
}

bool CTaCompositionReader::checkReaders()
{
	if(0 == m_videoReaders.size())
	{
		m_error = "there's no video track! ";
		return false;
	}
	if(m_audioReaders.size() != m_videoReaders.size())
	{
		m_error = "audio track number is not equal to video! ";
		return false;
	}
	{
		//set begtime of video and audio
		mxflib::UInt64 begTime = 0;
		std::list<CTaTrackFileReader>::iterator vite = m_videoReaders.begin();
		std::list<CTaTrackFileReader>::iterator aite = m_audioReaders.begin();
		for(; vite != m_videoReaders.end(); ++vite,++aite)
		{ 
			vite->setBegTime(begTime);
			aite->setBegTime(begTime);
			begTime += vite->getDuration();
		}
	}
	{
		//just the duration of audio reader to conform to video;
		std::list<CTaTrackFileReader>::iterator vite = m_videoReaders.begin();
		std::list<CTaTrackFileReader>::iterator aite = m_audioReaders.begin();
		for(int i = 0; vite != m_videoReaders.end(); ++vite,++aite,++i)
		{ 
			aite->setDuration(vite->getDuration());
		}
	}
	return true;
}	

bool CTaCompositionReader::initReel()
{
	std::list<CTaTrackFileReader>::iterator vite = m_videoReaders.begin();
	std::list<CTaTrackFileReader>::iterator aite = m_audioReaders.begin();
	for(int i = 0; vite != m_videoReaders.end(); ++vite,++aite,++i)
	{
	
		ReelInfo rInfo;
		rInfo.m_reelIndex = i;
		rInfo.m_videoEncInfo = vite->encryptInfo();
		rInfo.m_audioEncInfo = aite->encryptInfo();
		rInfo.m_startFrame = vite->getBegTime();
		rInfo.m_endFrame = vite->getBegTime() + vite->getDuration();
		m_reels.push_back(rInfo);
	}
	return true;

}

bool CTaCompositionReader::checkXmlFile(const char* compositionListFile,const char* packingListFile)
{	
		try
		{
			CCompositionPlayList pComposition(compositionListFile);
		}
		catch(std::exception&)
		{
			ostringstream os;
			os<< "open compositionListFile: "<< compositionListFile <<" fail"<<endl;
			m_error = os.str();
			//cout << m_error << endl;
			return false;
		}
		try
		{
			CPackingList packingList(packingListFile);
		}
		catch(std::exception&)
		{
			ostringstream os;
			os<< "open packinglist file:" << packingListFile <<" fail" <<endl;
			m_error = os.str();
			//cout << m_error << endl;
			return false;
		}
		return true;
}

bool CTaCompositionReader::open(const char* compositionListFile,const char* packingListFile)
{
	close();
	if(!checkXmlFile(compositionListFile,packingListFile))
		return false;
	try
	{
 		string path = packingListFile;
		string::size_type pos = path.find_last_of("/\\");
		if(pos != string::npos)
			m_basePath = path.substr(0,pos+1);
		CCompositionPlayList pComposition(compositionListFile);
		CPackingList packingList(packingListFile);
		
		CComposition_ReelList reelList = pComposition.GetReelList();
		int i,reelNum = reelList.GetChildCountByName("Reel");
		for(i = 0; i < reelNum; ++i)
		{
 			CComposition_Reel reel = reelList.GetReelByIndex(i);
			if(!addVideoReader(packingList,reel))
				break;
	  
			if(!addAudioReader(packingList,reel))
				break;	

		}
		if( checkReaders() && initReel())
		{
			m_isOpen = true;
			m_curVideoReader = m_videoReaders.begin();
			m_curAudioReader = m_audioReaders.begin();
		}
		else
			clear();
	}
	catch(std::exception&)
	{
	}
	return m_isOpen;

}

void CTaCompositionReader::close()
{
	if(!m_isOpen)
		return;
	m_isOpen = false;
	clear();
}

void CTaCompositionReader::clear()
{
	std::list<CTaTrackFileReader>::iterator ite = m_videoReaders.begin();
	for(; ite != m_videoReaders.end(); ++ite)
	{
		(*ite).release();
	}
	m_videoReaders.clear();

	ite = m_audioReaders.begin();
	for(;ite != m_audioReaders.end(); ++ite)
	{
		(*ite).release();
	}
	m_audioReaders.clear();

	m_duration = 0;
	m_basePath = m_error = "";
	m_reels.clear();
}

Rational CTaCompositionReader::frameRate()
{
	if(m_isOpen)
	{
		std::list<CTaTrackFileReader>::iterator ite = m_videoReaders.begin();
		return (*ite).editRate();	
	}
	return Rational();
}

mxflib::UInt64  CTaCompositionReader::duration()
{
	if(m_isOpen && 0 == m_duration)
	{
		std::list<CTaTrackFileReader>::iterator ite = m_videoReaders.begin();
		for(; ite != m_videoReaders.end(); ++ite)
		{
			m_duration += (*ite).getDuration();	
		}
	}
	return m_duration;
}

bool CTaCompositionReader::videoProperty(VideoProperty& property)
{
	if(m_isOpen)
	{
		std::list<CTaTrackFileReader>::iterator ite = m_videoReaders.begin();
		return (*ite).videoProperty(property);	
	}
	return false ;
}

bool CTaCompositionReader::audioProperty(AudioProperty& property)
{
	if(m_isOpen && m_audioReaders.size() > 0)
	{
		std::list<CTaTrackFileReader>::iterator ite = m_audioReaders.begin();
		for(; ite != m_audioReaders.end(); ++ite)
		{
            if((*ite).audioProperty(property))
				return true;
		}
	}
	return false;
}

bool CTaCompositionReader::subTitleProperty(SubtileProperty& property)
{
	return false;
}

FrameData CTaCompositionReader::readFrame(std::list<CTaTrackFileReader>& readers,std::list<CTaTrackFileReader>::iterator& curIte)
{
	FrameData  frameData;
	while(curIte != readers.end())
	{
		//if end of current reader?
		frameData = curIte->readFrame();
		if(!frameData.frame)
		{
			++curIte;
			continue;
		}
		frameData.reelIndex = distance(readers.begin(),curIte);
		break;
	}
	return frameData;
}

FrameData CTaCompositionReader::readVideoFrame()
{
    if(m_isOpen)
	{
		return readFrame(m_videoReaders,m_curVideoReader);
	}
	return FrameData();
}

FrameData CTaCompositionReader::readAudioFrame()
{
	if(m_isOpen)
	{
		return readFrame(m_audioReaders,m_curAudioReader);
	}
	return FrameData();
}

bool CTaCompositionReader::seek(mxflib::UInt64 frameIndex)
{
	if(m_isOpen )
	{
		seekEssence(frameIndex, m_videoReaders, m_curVideoReader);
		seekEssence(frameIndex, m_audioReaders, m_curAudioReader);
		return true;
	}
	return false;
}

bool CTaCompositionReader::seekEssence(mxflib::UInt64 time,std::list<CTaTrackFileReader>& readers,std::list<CTaTrackFileReader>::iterator& curIte)
{
	std::list<CTaTrackFileReader>::iterator ite = readers.begin();
	for(; ite != readers.end(); ++ite)
	{
		if(time >= ite->getBegTime() && 
		   time < (ite->getBegTime() + ite->getDuration()))
		{
			curIte = ite;
			return curIte->seek(time - ite->getBegTime());
		}
	}
	//fail ,seek to end?
	curIte = readers.end();
	return true;
}

mxflib::UInt64 CTaCompositionReader::nextPosition(ESSENCE_TYPE type)
{
	std::list<CTaTrackFileReader>::iterator ite,endite;
	if( type == essence_video )
	{
		ite = m_curVideoReader;
		endite = m_videoReaders.end();
	}
	else if(type == essence_audio)
	{
		if(m_audioReaders.size() == 0)
			return 0;
		ite = m_curAudioReader;
		endite = m_audioReaders.end();
		
	}
	else if(type == essence_subtitle)
	{
		//TODO;		
	}
	if(endite == ite)
		--ite;
	return (*ite).getBegTime()+(*ite).nextPosition();
}

const vector<ReelInfo>&  CTaCompositionReader::getReelInfo()
{
	return m_reels;

}

int CTaCompositionReader::curReel()
{
	if(m_isOpen)
	{
		return distance(m_videoReaders.begin(),m_curVideoReader);
	}
	return -1;

}
