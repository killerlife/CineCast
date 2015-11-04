#ifndef __IMXFREADER_H_200801082100__
#define __IMXFREADER_H_200801082100__


#ifdef _WIN32
	#ifdef MXFREADER_EXPORTS
	#define MXFREADER_API __declspec(dllexport)
	#else
	#define MXFREADER_API __declspec(dllimport)
	#endif
#else 
	#define MXFREADER_API
#endif


enum MediaType
{
	MEDIA_UNKNOWN = 0,
	MEDIA_VIDEO,
	MEDIA_AUDIO,
};

struct MediaFrame
{
	MediaType      type;
	unsigned char* data;
	unsigned int   offset;
	unsigned int   len;
	unsigned int   buflen;
	MediaFrame():data(0),offset(0),len(0),buflen(0)
	{
	}
};

struct TMxfFileItem
{
	char uuid[50];		// ex: ec847337-ae98-822a-f6ae-87aeab949f4c
	char filename[260];
	long entry;
	long duration;
	char key[16];
	MediaType  type;			// MEDIA_VIDEO: video = MEDIA_VIDEO, audio = MEDIA_AUDIO. 
	TMxfFileItem():entry(0),duration(0),type(MEDIA_UNKNOWN)
	{
		uuid[0] = 0;
		filename[0] = 0;
		key[0] = 0;
	}
};

struct TMyRational{
	int Numerator;
	int Denominator;
};

struct TVideoProperty
{
	unsigned int StoredHeight;
	unsigned int StoredWidth;
	TMyRational AspectRatio;
};

struct TAudioProperty
{
	unsigned int Channels;
	unsigned int BitIndeepness;
	TMyRational SampleRate;
	unsigned int BitRate;
};

struct TMediaProperty{
	MediaType type;
	union {
		TVideoProperty video;
		TAudioProperty audio;
	}property;
};

class IMxfFileReader
{
public:
	virtual int open(TMxfFileItem* items, int count) = 0;
	// note : readAudioFrame / readVideoFrame / releaseFrame cannot be called simultaneously in the same instance
	virtual int readAudioFrame(MediaFrame* frame) = 0;
	virtual int readVideoFrame(MediaFrame* frame) = 0;
	virtual int releaseFrame(MediaFrame* frame) = 0;
	virtual int seek(long ms) = 0;
	virtual long duration() = 0;
	virtual MediaType getMediaType() = 0;
	virtual const TMediaProperty* getMediaProperty(unsigned int index = 0) = 0;
	virtual void close() = 0;
	
	virtual ~IMxfFileReader(){};
};

MXFREADER_API IMxfFileReader* createIMxfFileReader();
MXFREADER_API void releaseIMxfFileReader(IMxfFileReader*& p);

#endif//__IMXFREADER_H_200801082100__