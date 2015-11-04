/*! \file	esp_mpeg2ts.cpp
 *	\brief	Implementation of class that handles parsing of MPEG-2 ts streams
 *
 *	\version $Id: esp_mpeg2ts.cpp,v 1.1.1.1 2007/04/03 02:08:42 baiqian Exp $
 *
 */

#include <mxflib/mxflib.h>

#include <math.h>	
#include <cassert>
using namespace mxflib;

//! Local definitions
namespace
{
	//! Modified UUID for MPEG2-VES
	const UInt8 MPEG2_TS_Format[] = { 0x45, 0x54, 0x57, 0x62,  0xd6, 0xb4, 0x2e, 0x4e,  0xf3, 0xd2, 'M', 'P',  'E', 'G', '2', 'T' };
}


//! Report the extensions of files this sub-parser is likely to handle
StringList MPEG2_TS_EssenceSubParser::HandledExtensions(void)
{
	StringList ExtensionList;

	ExtensionList.push_back("M2t");
	ExtensionList.push_back("TS");

	return ExtensionList;
}
bool MPEG2_TS_EssenceSubParser::decidePacketLen(FileHandle InFile)
{
	FileSeek(InFile, 0);
	UInt8 buf[204];
	int   readBytes = (int)FileRead(InFile, buf, 188);
	if(readBytes !=188 || buf[0]!=0x47)
		return false;
	readBytes = (int)FileRead(InFile, buf, 188);
	if(buf[0]!=0x47)
	{
		if(buf[6] ==0x47)
			m_packLen=204;
		else
			return false;
	}
	else
		m_packLen=188;

	//read some packet ,and assert?
	FileSeek(InFile, 0);
	for(int i=0;i<100;++i)
	{
		int readBytes = (int)FileRead(InFile, buf, 188);
		if(readBytes ==0)
			break;
		if(readBytes !=m_packLen || buf[0]!=0x47)
			return false;
	}
	FileSeekEnd(InFile);
	UInt64 fileLen=FileTell(InFile);
	if( fileLen%m_packLen !=0)
		return false;
    m_packNum =fileLen/m_packLen;
	return true;
}

//! Examine the open file and return a list of essence descriptors
/*! \note Valid MPEG2-TS files with > 510 extra zeroes before the first start code
 *	      will not be identifed!
 */
EssenceStreamDescriptorList MPEG2_TS_EssenceSubParser::IdentifyEssence(FileHandle InFile)
{

	EssenceStreamDescriptorList Ret;
	if(!decidePacketLen(InFile))
		return Ret;
		
	MDObjectPtr DescObj = BuildMPEG2TsDescriptor(InFile, 0);
	
	// Quit here if we couldn't build an essence descriptor
	if(!DescObj) return Ret;

	// Build a descriptor with a zero ID (we only support single stream files)
	EssenceStreamDescriptor Descriptor;
	Descriptor.ID = 0;
	Descriptor.Description = "MPEG2 ts essence";
	Descriptor.SourceFormat.Set(MPEG2_TS_Format);
	Descriptor.Descriptor = DescObj;

	// Record a pointer to the descriptor so we can check if we are asked to process this source
	CurrentDescriptor = DescObj;

	// Set the single descriptor
	Ret.push_back(Descriptor);

	return Ret;
}


//! Examine the open file and return the wrapping options known by this parser
/*! \param InFile The open file to examine (if the descriptor does not contain enough info)
 *	\param Descriptor An essence stream descriptor (as produced by function IdentifyEssence)
 *		   of the essence stream requiring wrapping
 *	\note The options should be returned in an order of preference as the caller is likely to use the first that it can support
 */
WrappingOptionList MPEG2_TS_EssenceSubParser::IdentifyWrappingOptions(FileHandle InFile, EssenceStreamDescriptor &Descriptor)
{
	//TODO: change baseUL;
	UInt8 BaseUL[16] = { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02, 0x0d, 0x01, 0x03, 0x01, 0x02, 0x04, 0x60, 0x01 };
	WrappingOptionList Ret;

	// If the source format isn't MPEG2-VES then we can't wrap the essence
	if(memcmp(Descriptor.SourceFormat.GetValue(), MPEG2_TS_Format, 16) != 0) return Ret;

	// The identify step configures some member variables so we can only continue if we just identified this very source
	if((!CurrentDescriptor) || (Descriptor.Descriptor != CurrentDescriptor)) return Ret;

	// Build a WrappingOption for frame wrapping
	WrappingOptionPtr FrameWrap = new WrappingOption;

	FrameWrap->Handler = this;							// Set us as the handler
	FrameWrap->Description = "SMPTE XXXM frame wrapping of MPEG2 ts stream";

	BaseUL[15] = 0x01;									// Frame wrapping
	FrameWrap->WrappingUL = new UL(BaseUL);				// Set the UL
	FrameWrap->GCEssenceType = 0x15;					// GC Compound wrapping type
	FrameWrap->GCElementType = 0x05;					// Frame wrapped picture elemenet
	FrameWrap->ThisWrapType = WrappingOption::Frame;	// Frame wrapping
	FrameWrap->CanSlave = true;						// Can only use the correct edit rate
	FrameWrap->CanIndex = false;							// We can index this essence
	FrameWrap->CBRIndex = true;						// This essence uses VBR indexing
	FrameWrap->BERSize = 0;								// No BER size forcing

	// Build a WrappingOption for clip wrapping
	WrappingOptionPtr ClipWrap = new WrappingOption;

	ClipWrap->Handler = this;							// Set us as the handler
	ClipWrap->Description = "SMPTE XXXM clip wrapping of MPEG2 ts stream";

	BaseUL[15] = 0x02;									// Clip wrapping
	ClipWrap->WrappingUL = new UL(BaseUL);				// Set the UL
	ClipWrap->GCEssenceType = 0x15;					// GC Compound wrapping type
	ClipWrap->GCElementType = 0x06;						// Clip wrapped picture elemenet
	ClipWrap->ThisWrapType = WrappingOption::Clip;		// Clip wrapping
	FrameWrap->CanSlave = true;							// Can use non-native edit rate
	FrameWrap->CanIndex = false;						// We CANNOT currently index this essence
	FrameWrap->CBRIndex = true;							// This essence uses CBR indexing
	ClipWrap->BERSize = 0;								// No BER size forcing

	// Add the two wrapping options
	Ret.push_back(FrameWrap);
	Ret.push_back(ClipWrap);

	return Ret;
}


//! Set a wrapping option for future Read and Write calls
void MPEG2_TS_EssenceSubParser::Use(UInt32 Stream, WrappingOptionPtr &UseWrapping)
{
	SelectedWrapping = UseWrapping;
	SelectedEditRate = NativeEditRate;
	PictureNumber = 0;
	CurrentPos = 0;
	m_Duration = 0;
}


//! Set a non-native edit rate
/*! \return true if this rate is acceptable */
bool MPEG2_TS_EssenceSubParser::SetEditRate(Rational EditRate)
{

	if(EditRate.Numerator>0 && EditRate.Denominator ==1)
		SelectedEditRate =EditRate;
	return true;
	
}
Rational MPEG2_TS_EssenceSubParser::GetPreferredEditRate(void)
{
	return Rational(100,1);
}

//! Get the current position in SetEditRate() sized edit units
/*! \return 0 if position not known
 */
Position MPEG2_TS_EssenceSubParser::GetCurrentPosition(void)
{
	return PictureNumber;
}


//! Read a number of wrapping items from the specified stream and return them in a data chunk
/*! If frame or line mapping is used the parameter Count is used to
 *	determine how many items are read. In frame wrapping it is in
 *	units of EditRate, as specified in the call to Use(), which may
 *  not be the frame rate of this essence
 *	\note This is going to take a lot of memory in clip wrapping! 
 */
DataChunkPtr MPEG2_TS_EssenceSubParser::Read(FileHandle InFile, UInt32 Stream, UInt64 Count /*=1*/ /*, IndexTablePtr Index *//*=NULL*/) 
{ 
	DataChunkPtr Ret;
	if(m_Duration > 0)
	{

		Length maxReadCount = (m_Duration - PictureNumber);
		if((UInt64)maxReadCount < Count)
			Count = maxReadCount;
	}

	if(m_Duration > 0 && PictureNumber >= m_Duration)
		return Ret;
	FileSeek(InFile, CurrentPos);
	
	UInt64 Bytes = 0;
	Bytes = ReadInternal(InFile, Stream, Count);	// Scan the stream and find out how many bytes to read
	Ret = new DataChunk;
	Ret->Resize((UInt32)Bytes);
	if( FileRead(InFile, Ret->Data, Bytes) !=Bytes)	   // Read the data
	{
		error("MPEG2_TS_EssenceSubParser read file fail\n");
		return DataChunkPtr();
		
	}
	if(Ret->Data[0]!=0x47)
	{
	//	warning("MPEG2_TS_EssenceSubParser data !=0x47,pic=%d\n",PictureNumber);
	}
					
	// Update the file pointer
	CurrentPos = FileTell(InFile);
	return Ret; 
};


//! Write a number of wrapping items from the specified stream to an MXF file
/*! If frame or line mapping is used the parameter Count is used to
 *	determine how many items are read. In frame wrapping it is in
 *	units of EditRate, as specified in the call to Use(), which may
 *  not be the frame rate of this essence stream
 *	\note This is the only safe option for clip wrapping
 *	\return Count of bytes transferred
 */
Length MPEG2_TS_EssenceSubParser::Write(FileHandle InFile, UInt32 Stream, MXFFilePtr OutFile, UInt64 Count /*=1*//*, IndexTablePtr Index *//*=NULL*/)
{
	assert(false);
	//FIXME: baiqian comment:
	//code below was copyed from esp_mpeg2ves.cpp 
	// and obviously there are some error!(not delete Buffer)
	//but now, I haven't use this member function;
	
	const UInt64 BUFFERSIZE = 32768;
	UInt8 *Buffer = new UInt8[BUFFERSIZE];

	// Scan the stream and find out how many bytes to transfer
	UInt64 Bytes = ReadInternal(InFile, Stream, Count /*, Index*/);
	UInt64 Ret = Bytes;

	while(Bytes)
	{
		int ChunkSize;
		
		// Number of bytes to transfer in this chunk
		if(Bytes < BUFFERSIZE) ChunkSize = (int)Bytes; else ChunkSize = BUFFERSIZE;

		FileRead(InFile, Buffer, ChunkSize);
		OutFile->Write(Buffer, ChunkSize);

		Bytes -= ChunkSize;
	}

	return Ret; 
}

//! Read the sequence header at the specified position in an MPEG2 file to build an essence descriptor
/*! DRAGONS: Currently rather scrappy */
MDObjectPtr MPEG2_TS_EssenceSubParser::BuildMPEG2TsDescriptor(FileHandle InFile, UInt64 Start /*=0*/)
{
	MDObjectPtr Ret;
	NativeEditRate.Numerator = 100;
	NativeEditRate.Denominator = 1;
	
	Ret = new MDObject(MPEG2TSDescriptor_UL);
	char Buffer[32];
	sprintf(Buffer, "%d/1", NativeEditRate.Numerator);
	Ret->SetString(SampleRate_UL, Buffer);
	//nothing to do!;
	return Ret;
}


bool MPEG2_TS_EssenceSubParser::SetOriginAndDuration(FileHandle InFile, Position EditUnit, Position Duration)
{

	return false;
}
Length MPEG2_TS_EssenceSubParser::ReadInternal(FileHandle InFile, UInt32 Stream, UInt64 Count)
{
	// Don't bother if there is no more data
	if(EndOfStream) return 0;
	UInt64 leftPackNum=(m_packNum-PictureNumber*SelectedEditRate.Numerator);
	UInt64 readCount =Count*SelectedEditRate.Numerator;
	if( readCount >leftPackNum)
	{
		readCount = leftPackNum;
		if(leftPackNum>0)
			PictureNumber++;
		EndOfStream =true;
	}
	else
		PictureNumber+=Count;
	return readCount*m_packLen;
}


//! Set a parser specific option
/*! \return true if the option was successfully set */
bool MPEG2_TS_EssenceSubParser::SetOption(std::string Option, Int64 Param /*=0*/ )
{
	if(Option == "EditPoint") 
		return EditPoint;

	warning("MPEG2_Ts_EssenceSubParser::SetOption(\"%s\", Param) not a known option\n", Option.c_str());
	
	return false; 
}
UInt64 MPEG2_TS_EssenceSubParser::GetDuration()
{
	if(m_Duration)
		return m_Duration;
	return m_packNum/SelectedEditRate.Numerator+((m_packNum%SelectedEditRate.Numerator) ==0?0:1);
}
UInt32 MPEG2_TS_EssenceSubParser::GetBytesPerEditUnit(UInt32 KAGSize )
{
				
	UInt32 Ret = SelectedEditRate.Numerator*m_packLen;
	
	if(SelectedWrapping->ThisWrapType == WrappingOption::Frame) 
	{
		// FIXME: This assumes that 4-byte BER coding will be used - this needs to be adjusted or forced to be true!!
		Ret += 16 + 4;
		
		// Adjust for whole KAGs if required
		if(KAGSize > 1)
		{
			// Work out how much short of the next KAG boundary we would be
			UInt32 Remainder = Ret % KAGSize;
			if(Remainder) Remainder = KAGSize - Remainder;
			
			// Round up to the start of the next KAG
			Ret += Remainder;
			
			// If there is not enough space to fit a filler in the remaining space an extra KAG will be required
			if((Remainder > 0) && (Remainder < 17)) Ret++;
		}
	}
	if(m_pEncryptObject)
		return (UInt32)m_pEncryptObject->getEncryptLen(Ret);
	return Ret;
}
