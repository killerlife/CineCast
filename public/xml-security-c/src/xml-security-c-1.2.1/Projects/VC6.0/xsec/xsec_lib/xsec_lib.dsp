# Microsoft Developer Studio Project File - Name="xsec_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=xsec_lib - Win32 Debug No Xalan
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xsec_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xsec_lib.mak" CFG="xsec_lib - Win32 Debug No Xalan"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xsec_lib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xsec_lib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xsec_lib - Win32 Debug No Xalan" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xsec_lib - Win32 Release No Xalan" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xsec_lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../Build/Win32/VC6/Release"
# PROP Intermediate_Dir "../../../../Build/Win32/VC6/Release/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSEC_LIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSEC_LIB_EXPORTS" /D "PROJ_CANON" /D "PROJ_DSIG" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 xerces-c_2.lib Xalan-C_1.lib crypt32.lib libeay32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../../../Build/Win32/VC6/Release/xsec_1_2_0.dll" /implib:"../../../../Build/Win32/VC6/Release/xsec_1.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "xsec_lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../Build/Win32/VC6/Debug"
# PROP Intermediate_Dir "../../../../Build/Win32/VC6/Debug/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSEC_LIB_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSEC_LIB_EXPORTS" /D "PROJ_CANON" /D "PROJ_DSIG" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib Xalan-C_1D.lib crypt32.lib libeay32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:0.2 /dll /debug /machine:I386 /out:"../../../../Build/Win32/VC6/Debug/xsec_1_2_0D.dll" /implib:"../../../../Build/Win32/VC6/Debug/xsec_1D.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "xsec_lib - Win32 Debug No Xalan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "xsec_lib___Win32_Debug_No_Xalan"
# PROP BASE Intermediate_Dir "xsec_lib___Win32_Debug_No_Xalan"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../Build/Win32/VC6/Debug"
# PROP Intermediate_Dir "../../../../Build/Win32/VC6/Debug/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "%LIBWWW%\Library\External" /I "%LIBWWW%\Library\Src" /I "../../../../src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSEC_LIB_EXPORTS" /D "PROJ_CANON" /D "PROJ_DSIG" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "%LIBWWW%\Library\External" /I "%LIBWWW%\Library\Src" /I "../../../../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSEC_LIB_EXPORTS" /D "PROJ_CANON" /D "PROJ_DSIG" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib xerces-c_2D.lib Xalan-C_1D.lib libeay32.lib /nologo /version:0.2 /dll /debug /machine:I386 /out:"../../../../Build/Win32/VC6/Debug/xsec_lib_01D.dll" /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib libeay32.lib crypt32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:0.2 /dll /debug /machine:I386 /out:"../../../../Build/Win32/VC6/Debug/xsec_1_2_0D.dll" /implib:"../../../../Build/Win32/VC6/Debug/xsec_1D.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "xsec_lib - Win32 Release No Xalan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "xsec_lib___Win32_Release_No_Xalan"
# PROP BASE Intermediate_Dir "xsec_lib___Win32_Release_No_Xalan"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../Build/Win32/VC6/Release"
# PROP Intermediate_Dir "../../../../Build/Win32/VC6/Release/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../../../src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSEC_LIB_EXPORTS" /D "PROJ_CANON" /D "PROJ_DSIG" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSEC_LIB_EXPORTS" /D "PROJ_CANON" /D "PROJ_DSIG" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib xerces-c_2.lib Xalan-C_1.lib libeay32.lib /nologo /dll /machine:I386 /out:"../../../../Build/Win32/VC6/Release/xsec_lib_01.dll"
# ADD LINK32 xerces-c_2.lib libeay32.lib crypt32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../../../Build/Win32/VC6/Release/xsec_1_2_0.dll" /implib:"../../../../Build/Win32/VC6/Release/xsec_1.lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "xsec_lib - Win32 Release"
# Name "xsec_lib - Win32 Debug"
# Name "xsec_lib - Win32 Debug No Xalan"
# Name "xsec_lib - Win32 Release No Xalan"
# Begin Group "canon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\canon\XSECC14n20010315.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\canon\XSECC14n20010315.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\canon\XSECCannon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\canon\XSECCanon.hpp
# End Source File
# End Group
# Begin Group "dsig"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGConstants.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGConstants.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoMgmtData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoMgmtData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoPGPData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoPGPData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoSPKIData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoSPKIData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoValue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoValue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoX509.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGKeyInfoX509.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGReferenceList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGReferenceList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGSignature.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGSignature.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGSignedInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGSignedInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransform.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformBase64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformBase64.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformC14n.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformC14n.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformEnvelope.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformEnvelope.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformXPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformXPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformXPathFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformXPathFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformXSL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGTransformXSL.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGXPathFilterExpr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGXPathFilterExpr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGXPathHere.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dsig\DSIGXPathHere.hpp
# End Source File
# End Group
# Begin Group "enc"

# PROP Default_Filter ""
# Begin Group "OpenSSL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\enc\openssl\OpenSSLCryptoBase64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\openssl\OpenSSLCryptoBase64.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\openssl\OpenSSLCryptoHash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\openssl\OpenSSLCryptoHash.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoHashHMAC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoHashHMAC.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoKeyDSA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoKeyDSA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoKeyHMAC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoKeyHMAC.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoKeyRSA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoKeyRSA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\openssl\OpenSSLCryptoProvider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\openssl\OpenSSLCryptoProvider.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoSymmetricKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoSymmetricKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoX509.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\OpenSSL\OpenSSLCryptoX509.hpp
# End Source File
# End Group
# Begin Group "XSCrypt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSCrypt\XSCryptCryptoBase64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSCrypt\XSCryptCryptoBase64.hpp
# End Source File
# End Group
# Begin Group "WinCAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoHash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoHash.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoHashHMAC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoHashHMAC.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoKeyDSA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoKeyDSA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoKeyHMAC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoKeyHMAC.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoKeyRSA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoKeyRSA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoProvider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoProvider.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoSymmetricKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoSymmetricKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoX509.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\WinCAPI\WinCAPICryptoX509.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoBase64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoBase64.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoHash.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoKeyDSA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoKeyHMAC.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoKeyRSA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoProvider.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoSymmetricKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoX509.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECCryptoX509.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECKeyInfoResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECKeyInfoResolverDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\enc\XSECKeyInfoResolverDefault.hpp
# End Source File
# End Group
# Begin Group "utils"

# PROP Default_Filter ""
# Begin Group "winutils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\utils\winutils\XSECBinHTTPURIInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\winutils\XSECBinHTTPURIInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\winutils\XSECSOAPRequestorSimpleWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\winutils\XSECURIResolverGenericWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\winutils\XSECURIResolverGenericWin32.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECBinTXFMInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECBinTXFMInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECDOMUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECDOMUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECNameSpaceExpander.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECNameSpaceExpander.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECPlatformUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECPlatformUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECSafeBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECSafeBuffer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECSafeBufferFormatter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECSafeBufferFormatter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECSOAPRequestor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECSOAPRequestorSimple.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECSOAPRequestorSimple.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECTXFMInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECTXFMInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECXPathNodeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\utils\XSECXPathNodeList.hpp
# End Source File
# End Group
# Begin Group "framework"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECAlgorithmHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECAlgorithmMapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECAlgorithmMapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECDefs.hpp

!IF  "$(CFG)" == "xsec_lib - Win32 Release"

# Begin Custom Build - Copying include files from src directory
InputPath=..\..\..\..\src\framework\XSECDefs.hpp

"..\..\..\..\include\xsec\framework\dummy.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mkdir ..\..\..\..\include 
	mkdir ..\..\..\..\include\xsec 
	del /q /s ..\..\..\..\include\xsec\*.* 
	xcopy /I /s /f  ..\..\..\..\src\*.hpp ..\..\..\..\include\xsec 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "xsec_lib - Win32 Debug"

# Begin Custom Build - Copying include files from src directory
InputPath=..\..\..\..\src\framework\XSECDefs.hpp

"..\..\..\..\include\xsec\framework\dummy.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mkdir ..\..\..\..\include 
	mkdir ..\..\..\..\include\xsec 
	del /q /s ..\..\..\..\include\xsec\*.* 
	xcopy /I /s /f  ..\..\..\..\src\*.hpp ..\..\..\..\include\xsec 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "xsec_lib - Win32 Debug No Xalan"

# Begin Custom Build - Copying include files from src directory
InputPath=..\..\..\..\src\framework\XSECDefs.hpp

"..\..\..\..\include\xsec\framework\dummy.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mkdir ..\..\..\..\include 
	mkdir ..\..\..\..\include\xsec 
	del /q /s ..\..\..\..\include\xsec\*.* 
	xcopy /I /s /f  ..\..\..\..\src\*.hpp ..\..\..\..\include\xsec 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "xsec_lib - Win32 Release No Xalan"

# Begin Custom Build - Copying include files from src directory
InputPath=..\..\..\..\src\framework\XSECDefs.hpp

"..\..\..\..\include\xsec\framework\dummy.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mkdir ..\..\..\..\include 
	mkdir ..\..\..\..\include\xsec 
	del /q /s ..\..\..\..\include\xsec\*.* 
	xcopy /I /s /f  ..\..\..\..\src\*.hpp ..\..\..\..\include\xsec 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECEnv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECEnv.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECError.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECProvider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECProvider.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECURIResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECURIResolverXerces.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECURIResolverXerces.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\framework\XSECW32Config.hpp
# End Source File
# End Group
# Begin Group "transformers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMBase64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMBase64.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMC14n.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMC14n.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMChain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMChain.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMCipher.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMCipher.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMConcatChains.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMConcatChains.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMDocObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMDocObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMEnvelope.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMEnvelope.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMMD5.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMMD5.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMOutputFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMOutputFile.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMParser.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMSB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMSB.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMSHA1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMSHA1.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMURL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMURL.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMXPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMXPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMXPathFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMXPathFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMXSL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\transformers\TXFMXSL.hpp
# End Source File
# End Group
# Begin Group "resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\framework\version.rc
# End Source File
# End Group
# Begin Group "xenc"

# PROP Default_Filter ""
# Begin Group "impl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCAlgorithmHandlerDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCAlgorithmHandlerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCCipherDataImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCCipherDataImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCCipherImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCCipherImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCCipherReferenceImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCCipherReferenceImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCCipherValueImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCCipherValueImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCEncryptedDataImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCEncryptedDataImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCEncryptedKeyImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCEncryptedKeyImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCEncryptedTypeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCEncryptedTypeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCEncryptionMethodImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\impl\XENCEncryptionMethodImpl.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\xenc\XENCCipher.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\XENCCipherData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\XENCCipherReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\XENCCipherValue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\XENCEncryptedData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\XENCEncryptedKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\XENCEncryptedType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xenc\XENCEncryptionMethod.hpp
# End Source File
# End Group
# Begin Group "xkms"

# PROP Default_Filter ""
# Begin Group "impl No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSAuthenticationImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSAuthenticationImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSCompoundRequestImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSCompoundRequestImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSCompoundResultImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSCompoundResultImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSKeyBindingAbstractTypeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSKeyBindingAbstractTypeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSKeyBindingImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSKeyBindingImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSLocateRequestImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSLocateRequestImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSLocateResultImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSLocateResultImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSMessageAbstractTypeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSMessageAbstractTypeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSMessageFactoryImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSMessageFactoryImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSNotBoundAuthentication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSNotBoundAuthenticationImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSPendingRequestImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSPendingRequestImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSPrototypeKeyBindingImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSPrototypeKeyBindingImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSQueryKeyBindingImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSQueryKeyBindingImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSRegisterRequestImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSRegisterRequestImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSRegisterResultImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSRegisterResultImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSRequestAbstractTypeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSRequestAbstractTypeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSRespondWithImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSRespondWithImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSResponseMechanismImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSResponseMechanismImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSResultImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSResultImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSResultTypeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSResultTypeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSStatusImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSStatusImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSStatusRequestImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSStatusRequestImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSStatusResultImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSStatusResultImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSUnverifiedKeyBindingImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSUnverifiedKeyBindingImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSUseKeyWithImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSUseKeyWithImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSValidateRequestImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSValidateRequestImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSValidateResultImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSValidateResultImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSValidityIntervalImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\impl\XKMSValidityIntervalImpl.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSAuthentication.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSCompoundRequest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSCompoundResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSConstants.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSConstants.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSKeyBinding.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSKeyBindingAbstractType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSLocateRequest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSLocateResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSMessageAbstractType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSMessageFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSNotBoundAuthentication.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSPendingRequest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSPrototypeKeyBinding.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSQueryKeyBinding.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSRegisterRequest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSRegisterResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSRequestAbstractType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSRespondWith.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSResponseMechanism.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSResultType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSStatus.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSStatusRequest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSStatusResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSUnverifiedKeyBinding.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSUseKeyWith.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSValidateRequest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSValidateResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xkms\XKMSValidityInterval.hpp
# End Source File
# End Group
# End Target
# End Project
