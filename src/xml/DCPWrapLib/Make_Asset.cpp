// Make_Track.cpp: implementation of the CMake_Asset class.
//
//////////////////////////////////////////////////////////////////////

#include "Make_Asset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMake_Asset::CMake_Asset(IXMLNode* pNode)
	: CElement(pNode)
{
}

CMake_Asset::~CMake_Asset()
{

}

bool CMake_Asset::SetSrc(const char *szVal)
{
	return SetAttribute("src",szVal);
	
}

std::string CMake_Asset::GetSrc()const
{
	return GetAttribute("src");
	
}

bool CMake_Asset::SetDst(const char *szVal)
{
	return SetAttribute("dst",szVal);
	
}

std::string CMake_Asset::GetDst()const
{
	return GetAttribute("dst");
	
}

bool CMake_Asset::SetSrcEditrate(const RATIONAL& szVal)
{
	return SetAttribute("src_editrate",szVal);
	
}

RATIONAL CMake_Asset::GetSrcEditrate()const
{
	return GetAttributeRational("src_editrate");
	
}

bool CMake_Asset::SetSrcOrigin(const Int64 nVal)
{
	return SetAttribute("src_origin",nVal);
	
}

Int64 CMake_Asset::GetSrcOrigin()const
{
	return GetAttributeInt64("src_origin");
	
}

bool CMake_Asset::SetSrcDuration(const Int64 nVal)
{
	return SetAttribute("src_duration",nVal);
	
}

Int64 CMake_Asset::GetSrcDuration()const
{
	return GetAttributeInt64("src_duration");
}

bool CMake_Asset::SetOtherwrapOption(const char *szVal)
{
	return SetAttribute("otherwrapoption",szVal);
}

std::string CMake_Asset::GetOtherwrapOption()const
{
	return GetAttribute("otherwrapoption");
}

std::string CMake_Asset::GetType() const
{
	return GetAttribute("type");
}


bool CMake_Asset::SetType(const char *szVal)
{
	return SetAttribute("type",szVal);
}

std::string CMake_Asset::GetTrackID() const
{
	return GetAttribute("trackid");
}


bool CMake_Asset::SetTrackID(const char *szVal)
{
	return SetAttribute("trackid",szVal);
}

int CMake_Asset::GetPlayListID() const
{
	return GetAttributeint("playlistid");
}


bool CMake_Asset::SetPlayListID(const int nVal)
{
	return SetAttribute("playlistid",nVal);
}

