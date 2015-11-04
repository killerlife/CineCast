// Make_Track.cpp: implementation of the CMake_Track class.
//
//////////////////////////////////////////////////////////////////////

#include "Make_Track.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMake_Track::CMake_Track(IXMLNode* pNode)
	: CElement(pNode)
{
}

CMake_Track::~CMake_Track()
{

}

bool CMake_Track::SetSrc(const char *szVal)
{
	return (SetAttribute("src",szVal)>=0) ? true:false;
	
}

std::string CMake_Track::GetSrc()const
{
	return GetAttribute("src");
	
}

bool CMake_Track::SetDst(const char *szVal)
{
	return (SetAttribute("dst",szVal)>=0) ? true:false;
	
}

std::string CMake_Track::GetDst()const
{
	return GetAttribute("dst");
	
}

bool CMake_Track::SetSrcEditrate(const char *szVal)
{
	return (SetAttribute("src_editrate",szVal)>=0) ? true:false;
	
}

std::string CMake_Track::GetSrcEditrate()const
{
	return GetAttribute("src_editrate");
	
}

bool CMake_Track::SetSrcOrigin(const char *szVal)
{
	return (SetAttribute("src_origin",szVal)>=0) ? true:false;
	
}

std::string CMake_Track::GetSrcOrigin()const
{
	return GetAttribute("src_origin");
	
}

bool CMake_Track::SetSrcDuration(const char *szVal)
{
	return (SetAttribute("src_duration",szVal)>=0) ? true:false;
	
}

std::string CMake_Track::GetSrcDuration()const
{
	return GetAttribute("src_duration");
	
}

bool CMake_Track::SetOtherwrapOption(const char *szVal)
{
	return (SetAttribute("otherwrapoption",szVal)>=0) ? true:false;
	
}

std::string CMake_Track::GetOtherwrapOption()const
{
	return GetAttribute("otherwrapoption");
	
}
