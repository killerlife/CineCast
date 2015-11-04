// DCPMaker.cpp: implementation of the CDCPMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "DCPMaker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCPMaker::CDCPMaker()
:CElement("dcpmaker",NULL,CREATE_FLAG)
{

}


CDCPMaker::CDCPMaker(const char* szprefix,const char* sznamespace)
:CElement((std::string(szprefix)+std::string(":dcpmaker")).c_str(),sznamespace,CREATE_FLAG)
{

}

CDCPMaker::CDCPMaker(const char* szInfo,SOURCEFLAG nFlag)
:CElement("dcpmaker",szInfo,nFlag)
{

}

CDCPMaker::~CDCPMaker()
{

}

bool CDCPMaker::SetPacklist(const char *szVal)
{
	return SetAttribute(STR_PACKINGLIST,szVal);
	
}

std::string CDCPMaker::GetPacklist()const
{
	return GetAttribute(STR_PACKINGLIST);
	
}

CMake_Composition CDCPMaker::CreateComposition(long lPos)
{
	CMake_Composition comp(CreateChildNodeRept(STR_COMPOSITION_PLAYLIST,lPos,true));
	return comp;
	
}

CMake_Composition CDCPMaker::GetCompositionByIndex(int nIndex)const
{
	CMake_Composition comp(GetChildByNameIndexRept(STR_COMPOSITION_PLAYLIST,nIndex));
	return comp;
	
}

int CDCPMaker::RemoveCompositionByIndex(int nIndex)
{
//	CMake_Composition reel = GetCompositionByIndex(nIndex);
//	return RemoveChildNode(&reel);
	return RemoveChildByNameIndex(STR_COMPOSITION_PLAYLIST,nIndex);
	
}

int CDCPMaker::RemoveComposition(CMake_Composition *pComposition)
{
	return RemoveChildNode(pComposition);
	
}
