/*! \file	esp_mpeg2ves.cpp
 *	\brief	Implementation of class that handles parsing of MPEG-2 video elementary streams
 *
 *	\version $Id: esp_mpeg2ves.cpp,v 1.1.1.1 2007/04/03 02:08:42 baiqian Exp $
 *
 */
/*
 *	Copyright (c) 2003, Matt Beard
 *
 *	This software is provided 'as-is', without any express or implied warranty.
 *	In no event will the authors be held liable for any damages arising from
 *	the use of this software.
 *
 *	Permission is granted to anyone to use this software for any purpose,
 *	including commercial applications, and to alter it and redistribute it
 *	freely, subject to the following restrictions:
 *
 *	  1. The origin of this software must not be misrepresented; you must
 *	     not claim that you wrote the original software. If you use this
 *	     software in a product, an acknowledgment in the product
 *	     documentation would be appreciated but is not required.
 *	
 *	  2. Altered source versions must be plainly marked as such, and must
 *	     not be misrepresented as being the original software.
 *	
 *	  3. This notice may not be removed or altered from any source
 *	     distribution.
 */

#include <mxflib/mxflib.h>

#include <math.h>	// For "floor"

using namespace mxflib;

//! Local definitions
namespace
{
	//! Modified UUID for MPEG2-VES
	const UInt8 MPEG2_VES_Format[] = { 0x45, 0x54, 0x57, 0x62,  0xd6, 0xb4, 0x2e, 0x4e,  0xf3, 0xd2, 'M', 'P',  'E', 'G', '2', 'V' };
}


//! Report the extensions of files this sub-parser is likely to handle
StringList MPEG2_VES_EssenceSubParser::HandledExtensions(void)
{
	StringList ExtensionList;

	ExtensionList.push_back("M2V");
	ExtensionList.push_back("MPG");
	ExtensionList.push_back("MPEG");

	return ExtensionList;
}


//! Examine the open file and return a list of essence descriptors
/*! \note Valid MPEG2-VES files with > 510 extra zeroes before the first start code
 *	      will not be identifed!
 */
EssenceStreamDescriptorList MPEG2_VES_EssenceSubParser::IdentifyEssence(FileHandle InFile)
{
	int BufferBytes;
	UInt8 Buffer[512];
	UInt8 *BuffPtr;

	EssenceStreamDescriptorList Ret;

	// Read the first 512 bytes of the file to allow us to investigate it
	FileSeek(InFile, 0);
	BufferBytes = (int)FileRead(InFile, Buffer, 512);
	
	// If the file is smaller than 16 bytes give up now!
	if(BufferBytes < 16) return Ret;

	// If the file doesn't start with two zeros the it doesn't start
	// with a start code and so it can't be a valid MPEG2-VES file
	if((Buffer[0] != 0) || (Buffer[1] != 0)) return Ret;

	// Scan for the first start code
	BuffPtr = &Buffer[2];
	int StartPos = 0;						//!< Start position of sequence header (when found)
	int ScanLeft = BufferBytes - 3;			//!< Number of bytes left in buffer to scan
	while(!(*BuffPtr))
	{
		if(!--ScanLeft) break;
		StartPos++;
		BuffPtr++;
	}

	// If we haven't found a start code then quit
	if(*BuffPtr != 1) return Ret;

	// Check what type of start code we have found
	// Only accept MPEG2-VES which will always start with a sequence header
	BuffPtr++;
	if(*BuffPtr != 0xb3) return Ret;

	MDObjectPtr DescObj = BuildMPEG2VideoDescriptor(InFile, StartPos);
	
	// Quit here if we couldn't build an essence descriptor
	if(!DescObj) return Ret;

	// Build a descriptor with a zero ID (we only support single stream files)
	EssenceStreamDescriptor Descriptor;
	Descriptor.ID = 0;
	Descriptor.Description = "MPEG2 video essence";
	Descriptor.SourceFormat.Set(MPEG2_VES_Format);
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
WrappingOptionList MPEG2_VES_EssenceSubParser::IdentifyWrappingOptions(FileHandle InFile, EssenceStreamDescriptor &Descriptor)
{
	UInt8 BaseUL[16] = { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02, 0x0d, 0x01, 0x03, 0x01, 0x02, 0x04, 0x60, 0x01 };
	WrappingOptionList Ret;

	// If the source format isn't MPEG2-VES then we can't wrap the essence
	if(memcmp(Descriptor.SourceFormat.GetValue(), MPEG2_VES_Format, 16) != 0) return Ret;

	// The identify step configures some member variables so we can only continue if we just identified this very source
	if((!CurrentDescriptor) || (Descriptor.Descriptor != CurrentDescriptor)) return Ret;

	// Build a WrappingOption for frame wrapping
	WrappingOptionPtr FrameWrap = new WrappingOption;

	FrameWrap->Handler = this;							// Set us as the handler
	FrameWrap->Description = "SMPTE 381M frame wrapping of MPEG2 video elementary stream";

	BaseUL[15] = 0x01;									// Frame wrapping
	FrameWrap->WrappingUL = new UL(BaseUL);				// Set the UL
	FrameWrap->GCEssenceType = 0x15;					// GC Picture wrapping type
	FrameWrap->GCElementType = 0x05;					// Frame wrapped picture elemenet
	FrameWrap->ThisWrapType = WrappingOption::Frame;	// Frame wrapping
	FrameWrap->CanSlave = false;						// Can only use the correct edit rate
	FrameWrap->CanIndex = true;							// We can index this essence
	FrameWrap->CBRIndex = false;						// This essence uses VBR indexing
	FrameWrap->BERSize = 0;								// No BER size forcing

	// Build a WrappingOption for clip wrapping
	WrappingOptionPtr ClipWrap = new WrappingOption;

	ClipWrap->Handler = this;							// Set us as the handler
	ClipWrap->Description = "SMPTE 381M clip wrapping of MPEG2 video elementary stream";

	BaseUL[15] = 0x02;									// Clip wrapping
	ClipWrap->WrappingUL = new UL(BaseUL);				// Set the UL
	ClipWrap->GCEssenceType = 0x15;						// GC Picture wrapping type
	ClipWrap->GCElementType = 0x06;						// Clip wrapped picture elemenet
	ClipWrap->ThisWrapType = WrappingOption::Clip;		// Clip wrapping
	ClipWrap->CanSlave = true;							// Can use non-native edit rate (clip wrap only!)
	ClipWrap->CanIndex = true;							// We can index this essence
	ClipWrap->CBRIndex = false;							// This essence uses VBR indexing
	ClipWrap->BERSize = 0;								// No BER size forcing

	// Add the two wrapping options
	Ret.push_back(FrameWrap);
	Ret.push_back(ClipWrap);

	return Ret;
}


//! Set a wrapping option for future Read and Write calls
void MPEG2_VES_EssenceSubParser::Use(UInt32 Stream, WrappingOptionPtr &UseWrapping)
{
	SelectedWrapping = UseWrapping;
	SelectedEditRate = NativeEditRate;
	EditRatio = 1;
	PictureNumber = 0;
	AnchorFrame = 0;
	CurrentPos = 0;
	GOPOffset = 0;
	SequenceHeaderLength = 0;
	Origin = false;
	ClosedGOP = false;					// Start by assuming the GOP is closed
	m_Duration = 0;
	SeqHeadPos = 0;						// the first sequence header's position
//	IndexMap.clear();
}


//! Set a non-native edit rate
/*! \return true if this rate is acceptable */
bool MPEG2_VES_EssenceSubParser::SetEditRate(Rational EditRate)
{
	if(    (EditRate.Numerator == NativeEditRate.Numerator) 
		&& (EditRate.Denominator == NativeEditRate.Denominator) )return true;

	// We can clip-wrap at any rate!
	if(SelectedWrapping->ThisWrapType == WrappingOption::Clip)
	{
		SelectedEditRate = EditRate;
		return true;
	}

	// Prevent divide by zero
	if(NativeEditRate.Denominator == 0) return false;
	if(EditRate.Denominator == 0) return false;

	double FloatNative = double(NativeEditRate.Numerator) / double(NativeEditRate.Denominator);
	double FloatUse = double(EditRate.Numerator) / double(EditRate.Denominator);

	// Different representation for the same edit rate
	// E.G. 25/1 and 50/2
	if(FloatNative == FloatUse)
	{
		SelectedEditRate = EditRate;
		return true;
	}

	if(FloatUse == 0) return false;

	// Integer multiples of the native edit rate are valid
	double Ratio = FloatNative / FloatUse;
	if(Ratio == floor(Ratio))
	{
		EditRatio = (unsigned int)(Ratio);
		return true;
	}

	return false;
}


//! Get the current position in SetEditRate() sized edit units
/*! \return 0 if position not known
 */
Position MPEG2_VES_EssenceSubParser::GetCurrentPosition(void)
{
	if((SelectedEditRate.Numerator == NativeEditRate.Numerator) && (SelectedEditRate.Denominator == NativeEditRate.Denominator))
	{
		return PictureNumber;
	}

	if((SelectedEditRate.Denominator == 0) || (NativeEditRate.Denominator || 0)) 
		return 0;

	double Pos = (double)(PictureNumber * SelectedEditRate.Numerator * NativeEditRate.Denominator);
	Pos /= (SelectedEditRate.Denominator * NativeEditRate.Numerator);

	return (Position)floor(Pos + 0.5);
}

DataChunkPtr MPEG2_VES_EssenceSubParser::ReadSequenceHeader(FileHandle InFile, UInt32 Stream, UInt64 Count)
{
	DataChunkPtr RetSequence = new DataChunk;
	if(SequenceHeaderLength > 0)
		RetSequence->Resize((UInt32)SequenceHeaderLength);
	else
		return RetSequence;
	Position pos = GetCurrentPosition();
	FileSeek(InFile, SeqHeadPos);
	FileRead(InFile, RetSequence->Data, SequenceHeaderLength);	// Read the data
	FileSeek(InFile, pos);

	UInt64 Bytes = ReadInternal(InFile, Stream, Count);
	// Make a datachunk with enough space
	DataChunkPtr Ret = new DataChunk;
	Ret->Resize((UInt32)Bytes);
	
	// Read the data
	FileRead(InFile, Ret->Data, Bytes);
	RetSequence->Append(Ret);
	return RetSequence;
}
//! Read a number of wrapping items from the specified stream and return them in a data chunk
/*! If frame or line mapping is used the parameter Count is used to
 *	determine how many items are read. In frame wrapping it is in
 *	units of EditRate, as specified in the call to Use(), which may
 *  not be the frame rate of this essence
 *	\note This is going to take a lot of memory in clip wrapping! 
 */
DataChunkPtr MPEG2_VES_EssenceSubParser::Read(FileHandle InFile, UInt32 Stream, UInt64 Count /*=1*/ /*, IndexTablePtr Index *//*=NULL*/) 
{ 
	DataChunkPtr Ret;
	if(m_Duration > 0)
	{

		Length maxReadCount = (m_Duration - PictureNumber);
		if((UInt64)maxReadCount < Count)
			Count = maxReadCount;
	}

	if(Origin)
	{
		Origin = false;
		return ReadSequenceHeader(InFile, Stream, Count);
	}

	if(m_Duration > 0 && PictureNumber >= m_Duration)
		return Ret;
	
	UInt64 Bytes = 0;
	Bytes = ReadInternal(InFile, Stream, Count);	// Scan the stream and find out how many bytes to read
	Ret = new DataChunk;
	Ret->Resize((UInt32)Bytes);
	FileRead(InFile, Ret->Data, Bytes);	   // Read the data

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
Length MPEG2_VES_EssenceSubParser::Write(FileHandle InFile, UInt32 Stream, MXFFilePtr OutFile, UInt64 Count /*=1*//*, IndexTablePtr Index *//*=NULL*/)
{
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
MDObjectPtr MPEG2_VES_EssenceSubParser::BuildMPEG2VideoDescriptor(FileHandle InFile, UInt64 Start /*=0*/)
{
	MDObjectPtr Ret;
	UInt8 Buffer[12];

	// Read the sequence header
	FileSeek(InFile, Start);
	if(FileRead(InFile, Buffer, 12) < 12) return Ret;

	UInt32 HSize = (Buffer[4] << 4) | (Buffer[5] >> 4);
	UInt32 VSize = ((Buffer[5] & 0x0f) << 8) | (Buffer[6]);

	char *Aspect;
	switch(Buffer[7] & 0xf0)
	{
	default: Aspect = NULL; break;
	case 0x10: Aspect = "1/1"; break;
	case 0x20: Aspect = "4/3"; break;
	case 0x30: Aspect = "16/9"; break;
	case 0x40: Aspect = "221/100"; break;
	}

	int FrameRate = 0;
	bool DropFrame = false;
	switch(Buffer[7] & 0x0f)
	{
	case 0x01: FrameRate = 24; DropFrame = true; break;
	case 0x02: FrameRate = 24; break;
	case 0x03: FrameRate = 25; break;
	case 0x04: FrameRate = 30; DropFrame = true; break;
	case 0x05: FrameRate = 30; break;
	case 0x06: FrameRate = 50; break;
	case 0x07: FrameRate = 60; DropFrame = true; break;
	case 0x08: FrameRate = 60; break;
	}

	if(FrameRate == 0) error("Unknown frame rate!\n");

	UInt32 BitRate = (Buffer[8] << 10) | (Buffer[9] << 2) | (Buffer[10] >> 6);

	if(BitRate == 0x3ffff) warning("Building MPEG2VideoDescriptor - bit_rate = -1\n");

	// Assume some values if no extension found
	UInt8 PandL = 0;
	bool Progressive = true;
	int HChromaSub = 2;
	int VChromaSub = 2;
	bool LowDelay = false;

	UInt8 LoadIntra = Buffer[11] & 0x02;
	UInt8 LoadNonIntra;
	if(LoadIntra == 0)
	{
		LoadNonIntra = Buffer[11] & 0x01;
	}
	else
	{
		// Skip over the intra buffer and read the non-intra flag
		FileSeek(InFile, Start + 11 + 64);
		UInt8 Flags;
		FileRead(InFile, &Flags, 1);

		LoadNonIntra = Flags & 0x01;
	}

	// Work out where the sequence extension should be
	int ExtPos =(int)( Start + 12);
	if(LoadIntra) ExtPos += 64;
	if(LoadNonIntra) ExtPos += 64;

	FileSeek(InFile, ExtPos);
	// Read the sequence extention
	FileRead(InFile, Buffer, 10);

	if((Buffer[0] != 0) || (Buffer[1] != 0) || (Buffer[2] != 1) || (Buffer[3] != 0xb5))
	{
		warning("Building MPEG2VideoDescriptor - extension does not follow sequence header (possibly MPEG1), some assumptions made\n");
	}
	else
	{
		PandL = (Buffer[4] << 4) | (Buffer[5] >> 4);
		
		if(Buffer[5] & 0x08) Progressive = true; else Progressive = false;

		int Sub = ((Buffer[5] & 0x01) << 1) | (Buffer[6] >> 7);
		if(Sub >= 2) VChromaSub = 1;
		if(Sub == 3) HChromaSub = 1;

		HSize |= ((Buffer[5] & 0x01) << 13) | ((Buffer[6] & 0x80) << 5);
		VSize |= ((Buffer[6] & 0x60) << 7);
		BitRate |= ((Buffer[6] & 0x1f) << 25) | ((Buffer[7] & 0xfe) << 17);

		if(Buffer[9] & 0x80) LowDelay = true;

		int FR_n = ((Buffer[9] & 0x60) >> 5) + 1;
		int FR_d = (Buffer[9] & 0x1f) + 1;

		FrameRate *= FR_n;
		FrameRate /= FR_d;
	}

/*
if(PandL) printf("Profile-and-Level = 0x%02x\n", PandL); else printf("Profile-and-Level unknown!!\n");
printf("HSize = %d\n", HSize);
printf("VSize = %d\n", VSize);
if(Aspect) printf("Aspect Ratio = %s\n", Aspect); else printf("Aspect Ratio unknown!!\n");
printf("BitRate = %u\n", BitRate * 400);
printf("Low Delay mode %s\n", LowDelay ? "is used" : "is not used");
printf("FrameRate = %d%s\n", FrameRate, DropFrame ? " * 1000/1001" : "");
printf("Frame Layout = %s\n", Progressive ? "Progressive" : "Interlaced");
printf("Chroma horizontal sub-sampling = %d\n", HChromaSub);
printf("Chroma vertical sub-sampling = %d\n", VChromaSub);
*/

	// Build the essence descriptor, filling in all known values

	Ret = new MDObject(MPEG2VideoDescriptor_UL);
	if(!Ret) return Ret;

	char Buff[32];
	if(DropFrame)
	{
		sprintf(Buff, "%d000/1001", FrameRate);
		Ret->SetString(SampleRate_UL, Buff);
		
		NativeEditRate.Numerator = FrameRate * 1000;
		NativeEditRate.Denominator = 1001;
	}
	else
	{
		sprintf(Buff, "%d/1", FrameRate);
		Ret->SetString(SampleRate_UL, Buff);

		NativeEditRate.Numerator = FrameRate;
		NativeEditRate.Denominator = 1;
	}

	if(Progressive) Ret->SetInt(FrameLayout_UL, 0); else Ret->SetInt(FrameLayout_UL, 1);

	Ret->SetUInt(StoredWidth_UL, HSize);
	Ret->SetUInt(StoredHeight_UL, VSize);

	if(Aspect) Ret->SetString(AspectRatio_UL, Aspect); else Ret->SetDValue(AspectRatio_UL);

	MDObjectPtr Ptr = Ret->AddChild(VideoLineMap_UL);
	if(Ptr)
	{
		int F1 = 0;
		int F2 = 0;

		if(VSize == 576) { F1 = 1; F2 = 313; }
		else if(VSize == 480) { F1 = 4; F2 = 266; }
//		else if(VSize == 720) { F1 = 1; F2 = 0; }
//		else if((VSize == 1080) && Progressive) { F1 = 1; F2 = 0; }
//		else if(VSize == 1080) { F1 = 1; F2 = 564; }

		if((F1 == 0) & (F2 == 0))
		{
			Ptr->AddChild()->SetDValue();
			Ptr->AddChild()->SetDValue();
		}
		else
		{
			Ptr->AddChild()->SetUInt(F1);
			Ptr->AddChild()->SetUInt(F2);
		}
	}

	Ret->SetUInt(ComponentDepth_UL, 8);

	Ret->SetUInt(HorizontalSubsampling_UL, HChromaSub);
	Ret->SetUInt(VerticalSubsampling_UL, VChromaSub);

	if((HChromaSub == 2) && (VChromaSub == 2))
		Ret->SetUInt(ColorSiting_UL, 3);				// Quincunx 4:2:0
	else if((HChromaSub == 2) && (VChromaSub == 1))
		Ret->SetUInt(ColorSiting_UL, 4);				// Rec 601 style 4:2:2
	if((HChromaSub == 1) && (VChromaSub == 1))
		Ret->SetUInt(ColorSiting_UL, 0);				// 4:4:4

	if(Progressive)	Ret->SetUInt(CodedContentType_UL, 1); else 	Ret->SetUInt(CodedContentType_UL, 2);
	if(LowDelay)	Ret->SetUInt(LowDelay_UL, 1); else	Ret->SetUInt(LowDelay_UL, 0);

	if(BitRate != 0x3ffff) Ret->SetUInt(BitRate_UL, BitRate * 400);

	Ret->SetUInt(ProfileAndLevel_UL, PandL);

#if defined(AS_CNN)
	// AS-CNN only - default values
	//! DRAGONS: should be evaluated while wrapping and set when rewriting Header
	Ret->SetUInt(ClosedGOP_UL,			1);				// from IBP Descriptor, check while parsing
	Ret->SetUInt(IdenticalGOP_UL,	1);				// from IBP Descriptor, check while parsing
	Ret->SetUInt(MaxGOP_UL,				15);			// from IBP Descriptor, check while parsing
	Ret->SetUInt(BPictureCount_UL, 2);				// evaluate while parsing
#endif

	return Ret;
}


bool MPEG2_VES_EssenceSubParser::SetOriginAndDuration(FileHandle InFile, Position EditUnit, Position Duration)
{
	ASSERT(PictureNumber == 0);
	if(EditUnit > 0)
	{
		if(CurrentPos == 0) 
			CurrentPos = GetCurrentPosition();
		SeekIFrame(InFile, EditUnit);
		Origin = true;
	}
	if(Duration > 0)
	{
		//TODO: for es ,how to check Duration if valid?
		m_Duration = Duration;
	}
	return true;
}

bool MPEG2_VES_EssenceSubParser::SeekIFrame(FileHandle InFile, UInt64 Count)
{
	UInt64 IFramePos =static_cast<UInt64>(-1);
	bool FirstGOP = true, FirstSequence = true;

	Count *= EditRatio;	   // Apply any edit rate factor for integer multiples of native edit rate 

	while(Count)	// Return anything we can find if clip wrapping
	{
		UInt32 Scan = 0xffffffff;
		FileSeek(InFile, CurrentPos);
		BuffCount = 0;
		
		bool FoundStart = false;		  //! Set true once the start of a picture has been found
		bool SeqHead = false;

		for(;;)
		{
			int ThisByte = BuffGetU8(InFile);

			if(ThisByte == -1)
				Count = 1;				  // Force this to be the last item (cause the outer loop to end)

			Scan = (Scan << 8) | ThisByte;
			CurrentPos++;

			if(!FoundStart) 
			{
				if(Scan == 0x00000100)    // Picture start code!
				{
					FoundStart = true;
					
					int PictureData = (BuffGetU8(InFile) << 8) | BuffGetU8(InFile);
					//int TemporalReference = PictureData >> 6;
					int PictureType = (PictureData >> 3) & 0x07;
					if(PictureType ==1)
						IFramePos = CurrentPos - 4;
					CurrentPos += 2;
				}
				else if(Scan == 0x000001b8)    // GOP start code
				{
					if(FirstGOP)
					{
						FirstGOP = false;
						SequenceHeaderLength = (int)(CurrentPos - 4 - IFramePos);
					}
					GOPOffset = 0;
					BuffGetU8(InFile);
					BuffGetU8(InFile);
					BuffGetU8(InFile);

					ClosedGOP = (BuffGetU8(InFile) & 0x40) ? true : false;

					if( ClosedGOP ) 
						debug( "Closed GOP\n" ); 
					else 
						debug( "Open GOP\n" );

					CurrentPos += 4;
				}
				else if(Scan == 0x000001b3)	   // Sequence header start code
				{
					SeqHead = true;
					if(FirstSequence)
					{
						FirstSequence = false;
						SeqHeadPos = CurrentPos - 4;
					}
				}
			}
			else
			{
				if((Scan == 0x000001b3) || (Scan == 0x000001b8) || (Scan == 0x00000100))	// All signs of the start of the next picture
				{
					CurrentPos -= 4;	// Next scan starts at the start of this start_code
					break;
				}
			}
		}
		Count--;
	}

	CurrentPos = IFramePos;	   // Move to the start of the data
	FileSeek(InFile, IFramePos);
	return IFramePos != static_cast<UInt64>(-1);
}
//! Scan the essence to calculate how many bytes to transfer for the given edit unit count
/*! \note The file position pointer is moved to the start of the chunk at the end of 
 *		  this function, but CurrentPos points to the start of the next edit unit
 *
 *	\note PictureNumber is incremented for each picture found
 */
Length MPEG2_VES_EssenceSubParser::ReadInternal(FileHandle InFile, UInt32 Stream, UInt64 Count)
{
	// Don't bother if there is no more data
	if(EndOfStream) return 0;

	UInt64 CurrentStart = CurrentPos;

	// Apply any edit rate factor for integer multiples of native edit rate 
	Count *= EditRatio;

	// Return anything we can find if clip wrapping
	if((Count == 0) && (SelectedWrapping->ThisWrapType == WrappingOption::Clip)) Count = UINT64_C(0xffffffffffffffff);

	while(Count)
	{
		EditPoint = false;

		UInt32 Scan = 0xffffffff;
		FileSeek(InFile, CurrentPos);
		BuffCount = 0;

		bool FoundStart = false;			//! Set true once the start of a picture has been found
		bool SeqHead = false;

		for(;;)
		{
			int ThisByte = BuffGetU8(InFile);

			if(ThisByte == -1)
			{
				Count = 1;					// Force this to be the last item (cause the outer loop to end)
				EndOfStream = true;			// Flag that there is no more data - so we will not scan any more
				break;
			}

			Scan = (Scan << 8) | ThisByte;
			CurrentPos++;

			if(!FoundStart) 
			{
				// Picture start code!
				if(Scan == 0x00000100)
				{
					FoundStart = true;
					
					int PictureData = (BuffGetU8(InFile) << 8) | BuffGetU8(InFile);
					CurrentPos += 2;

					// If we don't have an index manager there is no need to calcluate index details, but we still check for edit points
					if(!Manager)
					{
						// Do we have a sequence header?
						if((SeqHead) && (ClosedGOP)) EditPoint = true;
					}
					// ...but if an index manager exists we do all calculations to keep anchor frame etc. in step
					// even if we aren't going to add an entry this time
					else
					{
						int TemporalReference = PictureData >> 6;
						int PictureType = (PictureData >> 3) & 0x07;

						int Flags;
						switch(PictureType)
						{
						case 1: default:
							AnchorFrame = PictureNumber;
							Flags = 0x00;
							break;
						case 2: Flags = 0x22; break;
						case 3: Flags = 0x33; break;
						}


						// Do we have a sequence header?
						if(SeqHead)
						{
							Flags |= 0x40;
							if(ClosedGOP) 
							{
								Flags |= 0x80;
								EditPoint = true;
							}
						}

						// Now we have determined if this is an anchor frame we can work out the anchor offset
						int AnchorOffset = (int)(PictureNumber - AnchorFrame);

						//
						// Offer this index table data to the index manager
						//

						Manager->OfferEditUnit(ManagedStreamID, PictureNumber, AnchorOffset, Flags);
						Manager->OfferTemporalOffset(PictureNumber - (GOPOffset - TemporalReference), GOPOffset - TemporalReference);

						// diagnostics
						if(PictureNumber < 35)
							debug( "  OfferEditUnit[%3d]: Tpres=%3d Aoff=%2d A=%3d 0x%02x. Reorder Toff[%2d]=%2d\n",
											(int)PictureNumber,
											(int)TemporalReference,
											(int)AnchorOffset,
											(int)AnchorFrame,
											(int)Flags,
                      (int)(PictureNumber - (GOPOffset - TemporalReference)),
                      (int)(GOPOffset - TemporalReference)
										 );
					}

					GOPOffset++;
				}
				// GOP start code
				else if(Scan == 0x000001b8)
				{
					GOPOffset = 0;
					BuffGetU8(InFile);
					BuffGetU8(InFile);
					BuffGetU8(InFile);

					ClosedGOP = (BuffGetU8(InFile) & 0x40)? true:false;

					//if( PictureNumber < 35 )
					if( ClosedGOP ) debug( "Closed GOP\n" ); else debug( "Open GOP\n" );

					CurrentPos += 4;
				}
				// Sequence header start code
				else if(Scan == 0x000001b3)
				{
					SeqHead = true;
				}
			}
			else
			{
				// All signs of the start of the next picture
				if((Scan == 0x000001b3) || (Scan == 0x000001b8) || (Scan == 0x00000100))
				{
					// Next scan starts at the start of this start_code
					CurrentPos -= 4;
					break;
				}
			}
		}

		Count--;
		PictureNumber++;
	}
//	// Move to the start of the data
	FileSeek(InFile, CurrentStart);
	return CurrentPos - CurrentStart;

}


//! Get a byte from the current stream
/*! \return -1 if end of file */
int MPEG2_VES_EssenceSubParser::BuffGetU8(FileHandle InFile)
{
	if(!BuffCount)
	{
		BuffCount = (int)FileRead(InFile, Buffer, MPEG2_VES_BUFFERSIZE);
		if(BuffCount == 0) return -1;

		BuffPtr = Buffer;
	}

	BuffCount--;
	return *(BuffPtr++);
}


//! Set a parser specific option
/*! \return true if the option was successfully set */
bool MPEG2_VES_EssenceSubParser::SetOption(std::string Option, Int64 Param /*=0*/ )
{
	if(Option == "EditPoint") 
		return EditPoint;

	warning("MPEG2_VES_EssenceSubParser::SetOption(\"%s\", Param) not a known option\n", Option.c_str());
	
	return false; 
}
UInt64 MPEG2_VES_EssenceSubParser::GetDuration()
{
	if(m_Duration)
		return m_Duration;
	return 0;
}
