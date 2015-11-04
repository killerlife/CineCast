# Microsoft Developer Studio Project File - Name="DCPWrapLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DCPWrapLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DCPWrapLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DCPWrapLib.mak" CFG="DCPWrapLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DCPWrapLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DCPWrapLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DCPWrapLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DCPWRAPLIB_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DCPWRAPLIB_EXPORTS" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 DcpXmlCore.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../bin/DcpXmlWrapLib.dll" /implib:"..\..\lib\DcpXmlWrapLib.lib" /libpath:"..\..\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DCPWrapLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DCPWRAPLIB_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_USRDLL" /D "DCPWRAPLIB_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 DcpXmlCored.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/DcpXmlWrapLibd.dll" /implib:"../../lib/DcpXmlWrapLibd.lib" /pdbtype:sept /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DCPWrapLib - Win32 Release"
# Name "DCPWrapLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "compositioncpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Composition_AssetList.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_AuxSoundTrackFileAsset.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_ContentVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_GenericAsset.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_Marker.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_MarkerAsset.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_MarkerList.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_PictureTrackFileAsset.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_Rating.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_RatingList.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_Reel.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_ReelList.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_SoundTrackFileAsset.cpp
# End Source File
# Begin Source File

SOURCE=.\Composition_TrackFileAsset.cpp
# End Source File
# Begin Source File

SOURCE=.\CompositionPlayList.cpp
# End Source File
# End Group
# Begin Group "signcpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sign_BaseMethod.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_DSAKeyValue.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_IdElement.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_KeyInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_KeyValue.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_Mainfest.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_PGPData.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_Reference.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_RetrievalMethod.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_RSAKeyValue.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_SignatureMethod.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_SignaturePropertieList.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_SignatureProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_SignInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_SPKIData.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_Transform.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_TransformList.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_X509Data.cpp
# End Source File
# Begin Source File

SOURCE=.\Sign_X509IssuerSerial.cpp
# End Source File
# Begin Source File

SOURCE=.\Signature.cpp
# End Source File
# Begin Source File

SOURCE=.\Signer.cpp
# End Source File
# End Group
# Begin Group "dcpmakercpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DCPMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\Make_Asset.cpp
# End Source File
# Begin Source File

SOURCE=.\Make_Reel.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeComposition.cpp
# End Source File
# End Group
# Begin Group "packinglistcpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Pack_Asset.cpp
# End Source File
# Begin Source File

SOURCE=.\Pack_AssetList.cpp
# End Source File
# Begin Source File

SOURCE=.\PackingList.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\DCPWrapLib.cpp
# End Source File
# Begin Source File

SOURCE=.\Element.cpp
# End Source File
# Begin Source File

SOURCE=.\IssuerElement.cpp
# End Source File
# Begin Source File

SOURCE=.\ManageSign.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectElement.cpp
# End Source File
# Begin Source File

SOURCE=.\util\util.cpp
# End Source File
# Begin Source File

SOURCE=.\UUIDElement.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLManage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "sign"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sign_BaseMethod.h
# End Source File
# Begin Source File

SOURCE=.\Sign_DSAKeyValue.h
# End Source File
# Begin Source File

SOURCE=.\Sign_IdElement.h
# End Source File
# Begin Source File

SOURCE=.\Sign_KeyInfo.h
# End Source File
# Begin Source File

SOURCE=.\Sign_KeyValue.h
# End Source File
# Begin Source File

SOURCE=.\Sign_Mainfest.h
# End Source File
# Begin Source File

SOURCE=.\Sign_Object.h
# End Source File
# Begin Source File

SOURCE=.\Sign_PGPData.h
# End Source File
# Begin Source File

SOURCE=.\Sign_Reference.h
# End Source File
# Begin Source File

SOURCE=.\Sign_RetrievalMethod.h
# End Source File
# Begin Source File

SOURCE=.\Sign_RSAKeyValue.h
# End Source File
# Begin Source File

SOURCE=.\Sign_SignatureMethod.h
# End Source File
# Begin Source File

SOURCE=.\Sign_SignaturePropertieList.h
# End Source File
# Begin Source File

SOURCE=.\Sign_SignatureProperty.h
# End Source File
# Begin Source File

SOURCE=.\Sign_SignInfo.h
# End Source File
# Begin Source File

SOURCE=.\Sign_SPKIData.h
# End Source File
# Begin Source File

SOURCE=.\Sign_Transform.h
# End Source File
# Begin Source File

SOURCE=.\Sign_TransformList.h
# End Source File
# Begin Source File

SOURCE=.\Sign_X509Data.h
# End Source File
# Begin Source File

SOURCE=.\Sign_X509IssuerSerial.h
# End Source File
# Begin Source File

SOURCE=.\Signature.h
# End Source File
# Begin Source File

SOURCE=.\Signer.h
# End Source File
# End Group
# Begin Group "composition"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Composition_AssetList.h
# End Source File
# Begin Source File

SOURCE=.\Composition_AuxSoundTrackFileAsset.h
# End Source File
# Begin Source File

SOURCE=.\Composition_ContentVersion.h
# End Source File
# Begin Source File

SOURCE=.\Composition_GenericAsset.h
# End Source File
# Begin Source File

SOURCE=.\Composition_Marker.h
# End Source File
# Begin Source File

SOURCE=.\Composition_MarkerAsset.h
# End Source File
# Begin Source File

SOURCE=.\Composition_MarkerList.h
# End Source File
# Begin Source File

SOURCE=.\Composition_PictureTrackFileAsset.h
# End Source File
# Begin Source File

SOURCE=.\Composition_Rating.h
# End Source File
# Begin Source File

SOURCE=.\Composition_RatingList.h
# End Source File
# Begin Source File

SOURCE=.\Composition_Reel.h
# End Source File
# Begin Source File

SOURCE=.\Composition_ReelList.h
# End Source File
# Begin Source File

SOURCE=.\Composition_SoundTrackFileAsset.h
# End Source File
# Begin Source File

SOURCE=.\Composition_TrackFileAsset.h
# End Source File
# Begin Source File

SOURCE=.\CompositionPlayList.h
# End Source File
# End Group
# Begin Group "dcpmaker"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DCPMaker.h
# End Source File
# Begin Source File

SOURCE=.\Make_Asset.h
# End Source File
# Begin Source File

SOURCE=.\Make_Reel.h
# End Source File
# Begin Source File

SOURCE=.\MakeComposition.h
# End Source File
# End Group
# Begin Group "packinglist"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Pack_Asset.h
# End Source File
# Begin Source File

SOURCE=.\Pack_AssetList.h
# End Source File
# Begin Source File

SOURCE=.\PackingList.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\util\DataType.h
# End Source File
# Begin Source File

SOURCE=..\..\include\dcpwrapinclude.h
# End Source File
# Begin Source File

SOURCE=.\dcpwraplib.h
# End Source File
# Begin Source File

SOURCE=.\Element.h
# End Source File
# Begin Source File

SOURCE=.\IssuerElement.h
# End Source File
# Begin Source File

SOURCE=.\ManageSign.h
# End Source File
# Begin Source File

SOURCE=.\util\MyMacroDef.h
# End Source File
# Begin Source File

SOURCE=.\ProjectElement.h
# End Source File
# Begin Source File

SOURCE=.\util\util.h
# End Source File
# Begin Source File

SOURCE=.\UUIDElement.h
# End Source File
# Begin Source File

SOURCE=.\XMLManage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
