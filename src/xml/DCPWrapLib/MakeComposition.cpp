// MakeComposition.cpp: implementation of the CMakeComposition class.
//
//////////////////////////////////////////////////////////////////////

#include "MakeComposition.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMake_Composition::CMake_Composition(IXMLNode* pNode)
	: CElement(pNode)
{
}

CMake_Composition::~CMake_Composition()
{

}

CMake_Reel CMake_Composition::CreateReel(long lPos)
{
	CMake_Reel Reel(CreateChildNodeRept("reel",lPos,true));
	return Reel;
	
}

CMake_Reel CMake_Composition::GetReelByIndex(int nIndex)const
{
	CMake_Reel Reel(GetChildByNameIndexRept("reel",nIndex));
	return Reel;
	
}

int CMake_Composition::RemoveReelByIndex(int nIndex)
{
	return RemoveChildByNameIndex("reel",nIndex);
//	CMake_Reel reel = GetReelByIndex(nIndex);
//	return RemoveChildNode(&reel);
	
}

int CMake_Composition::RemoveReel(CMake_Reel *pReel)
{
	return RemoveChildNode(pReel);
	
}

bool CMake_Composition::SetPlaylist(const char *szVal)
{
	return SetAttribute("playlistfile",szVal);
	
}

std::string CMake_Composition::GetPlaylist()const
{
	return GetAttribute("playlistfile");
	
}

CMake_Reel CMake_Composition::GetReelByid(int id) const
{
	CMake_Reel Reel(GetChildByIdRept(id));
	return Reel;
}

int CMake_Composition::RemoveReelByid(int id)
{
	return RemoveChildById(id);
//	CMake_Reel reel = GetReelByid(id);
//	return RemoveChildNode(&reel);
	
}
