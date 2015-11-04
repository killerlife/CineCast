/*
Copyright (c) 2004-2007, John Hurst
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*! \file    AS_DCP_MPEG2.cpp
    \version $Id: AS_DCP_MPEG2.cpp,v 1.24 2008/02/01 00:42:17 msheby Exp $       
    \brief   AS-DCP library, MPEG2 essence reader and writer implementation
*/

#include "AS_DCP_internal.h"


//------------------------------------------------------------------------------------------

static std::string MPEG_PACKAGE_LABEL = "File Package: SMPTE 381M frame wrapping of MPEG2 video elementary stream";
static std::string MPEG_S_PACKAGE_LABEL = "File Package: LEONIS frame wrapping of stereoscopic MPEG2 video elementary stream";
static std::string PICT_DEF_LABEL = "Picture Track";

//
ASDCP::Result_t
#ifdef MPEG3D
MD_to_MPEG2_VDesc(MXF::MPEG2VideoDescriptor* VDescObj, MPEG2::VideoDescriptor& VDesc, ASDCP::Rational m_EditRate)
#else
MD_to_MPEG2_VDesc(MXF::MPEG2VideoDescriptor* VDescObj, MPEG2::VideoDescriptor& VDesc)
#endif
{
  ASDCP_TEST_NULL(VDescObj);

  VDesc.SampleRate             = VDescObj->SampleRate;
#ifdef MPEG3D
  VDesc.EditRate               = m_EditRate; //VDescObj->SampleRate;
#else
  VDesc.EditRate               = VDescObj->SampleRate;
#endif
  VDesc.FrameRate              = VDescObj->SampleRate.Numerator;
  assert(VDescObj->ContainerDuration <= 0xFFFFFFFFL);
  VDesc.ContainerDuration      = (ui32_t) VDescObj->ContainerDuration;

  VDesc.FrameLayout            = VDescObj->FrameLayout;
  VDesc.StoredWidth            = VDescObj->StoredWidth;
  VDesc.StoredHeight           = VDescObj->StoredHeight;
  VDesc.AspectRatio            = VDescObj->AspectRatio;

  VDesc.ComponentDepth         = VDescObj->ComponentDepth;
  VDesc.HorizontalSubsampling  = VDescObj->HorizontalSubsampling;
  VDesc.VerticalSubsampling    = VDescObj->VerticalSubsampling;
  VDesc.ColorSiting            = VDescObj->ColorSiting;
  VDesc.CodedContentType       = VDescObj->CodedContentType;

  VDesc.LowDelay               = VDescObj->LowDelay == 0 ? false : true;
  VDesc.BitRate                = VDescObj->BitRate;
  VDesc.ProfileAndLevel        = VDescObj->ProfileAndLevel;
  return RESULT_OK;
}


//
ASDCP::Result_t
MPEG2_VDesc_to_MD(MPEG2::VideoDescriptor& VDesc, MXF::MPEG2VideoDescriptor* VDescObj)
{
  ASDCP_TEST_NULL(VDescObj);

#ifdef MPEG3D
  VDescObj->SampleRate = VDesc.EditRate;
#else
  VDescObj->SampleRate = VDesc.SampleRate;
#endif
  VDescObj->ContainerDuration = VDesc.ContainerDuration;

  VDescObj->FrameLayout = VDesc.FrameLayout;
  VDescObj->StoredWidth = VDesc.StoredWidth;
  VDescObj->StoredHeight = VDesc.StoredHeight;
  VDescObj->AspectRatio = VDesc.AspectRatio;

  VDescObj->ComponentDepth = VDesc.ComponentDepth;
  VDescObj->HorizontalSubsampling = VDesc.HorizontalSubsampling;
  VDescObj->VerticalSubsampling = VDesc.VerticalSubsampling;
  VDescObj->ColorSiting = VDesc.ColorSiting;
  VDescObj->CodedContentType = VDesc.CodedContentType;

  VDescObj->LowDelay = VDesc.LowDelay ? 1 : 0;
  VDescObj->BitRate = VDesc.BitRate;
  VDescObj->ProfileAndLevel = VDesc.ProfileAndLevel;
  return RESULT_OK;
}


//
void
ASDCP::MPEG2::VideoDescriptorDump(const VideoDescriptor& VDesc, FILE* stream)
{
  if ( stream == 0 )
    stream = stderr;

  fprintf(stream, "\
        SampleRate: %d/%d\n\
       FrameLayout: %u\n\
       StoredWidth: %u\n\
      StoredHeight: %u\n\
       AspectRatio: %d/%d\n\
    ComponentDepth: %u\n\
 HorizontalSubsmpl: %u\n\
   VerticalSubsmpl: %u\n\
       ColorSiting: %u\n\
  CodedContentType: %u\n\
          LowDelay: %u\n\
           BitRate: %u\n\
   ProfileAndLevel: %u\n\
 ContainerDuration: %u\n",
	  VDesc.SampleRate.Numerator ,VDesc.SampleRate.Denominator,
	  VDesc.FrameLayout,
	  VDesc.StoredWidth,
	  VDesc.StoredHeight,
	  VDesc.AspectRatio.Numerator ,VDesc.AspectRatio.Denominator,
	  VDesc.ComponentDepth,
	  VDesc.HorizontalSubsampling,
	  VDesc.VerticalSubsampling,
	  VDesc.ColorSiting,
	  VDesc.CodedContentType,
	  VDesc.LowDelay,
	  VDesc.BitRate,
	  VDesc.ProfileAndLevel,
	  VDesc.ContainerDuration
	  );
}

//------------------------------------------------------------------------------------------
//
// hidden, internal implementation of MPEG2 reader

class ASDCP::MPEG2::MXFReader::h__Reader : public ASDCP::h__Reader
{
  ASDCP_NO_COPY_CONSTRUCT(h__Reader);

public:
  VideoDescriptor m_VDesc;        // video parameter list

  h__Reader() {}
  ~h__Reader() {}
  Result_t    OpenRead(const char*);
  Result_t    ReadFrame(ui32_t, FrameBuffer&, AESDecContext*, HMACContext*);
  Result_t    ReadFrameGOPStart(ui32_t, FrameBuffer&, AESDecContext*, HMACContext*);
  Result_t    FindFrameGOPStart(ui32_t, ui32_t&);
};


//
//
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::h__Reader::OpenRead(const char* filename)
{
  Result_t result = OpenMXFRead(filename);

  if( ASDCP_SUCCESS(result) )
    {
      InterchangeObject* Object;
      if ( ASDCP_SUCCESS(m_HeaderPart.GetMDObjectByType(OBJ_TYPE_ARGS(MPEG2VideoDescriptor), &Object)) )
	{
	  assert(Object);
	  result = MD_to_MPEG2_VDesc((MXF::MPEG2VideoDescriptor*)Object, m_VDesc, 
		  ((MXF::MPEG2VideoDescriptor*)Object)->SampleRate);
	}
    }

  if( ASDCP_SUCCESS(result) )
    result = InitMXFIndex();

  if( ASDCP_SUCCESS(result) )
    result = InitInfo();

  return result;
}


//
//
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::h__Reader::ReadFrameGOPStart(ui32_t FrameNum, FrameBuffer& FrameBuf,
						      AESDecContext* Ctx, HMACContext* HMAC)
{
  ui32_t KeyFrameNum;

  Result_t result = FindFrameGOPStart(FrameNum, KeyFrameNum);

  if ( ASDCP_SUCCESS(result) )
    result = ReadFrame(KeyFrameNum, FrameBuf, Ctx, HMAC);

  return result;
}


//
//
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::h__Reader::FindFrameGOPStart(ui32_t FrameNum, ui32_t& KeyFrameNum)
{
  KeyFrameNum = 0;

  if ( ! m_File.IsOpen() )
    return RESULT_INIT;

  // look up frame index node
  IndexTableSegment::IndexEntry TmpEntry;

  if ( ASDCP_FAILURE(m_FooterPart.Lookup(FrameNum, TmpEntry)) )
    {
      DefaultLogSink().Error("Frame value out of range: %u\n", FrameNum);
      return RESULT_RANGE;
    }

  KeyFrameNum = FrameNum - TmpEntry.KeyFrameOffset;

  return RESULT_OK;
}


//
//
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::h__Reader::ReadFrame(ui32_t FrameNum, FrameBuffer& FrameBuf,
					      AESDecContext* Ctx, HMACContext* HMAC)
{
  if ( ! m_File.IsOpen() )
    return RESULT_INIT;

  Result_t result = ReadEKLVFrame(FrameNum, FrameBuf, Dict::ul(MDD_MPEG2Essence), Ctx, HMAC);

  if ( ASDCP_FAILURE(result) )
    return result;

  IndexTableSegment::IndexEntry TmpEntry;
  m_FooterPart.Lookup(FrameNum, TmpEntry);

  switch ( ( TmpEntry.Flags >> 4 ) & 0x03 )
    {
    case 0:  FrameBuf.FrameType(FRAME_I); break;
    case 2:  FrameBuf.FrameType(FRAME_P); break;
    case 3:  FrameBuf.FrameType(FRAME_B); break;
    default: FrameBuf.FrameType(FRAME_U);
    }

  FrameBuf.TemporalOffset(TmpEntry.TemporalOffset);
  FrameBuf.GOPStart(TmpEntry.Flags & 0x40 ? true : false);
  FrameBuf.ClosedGOP(TmpEntry.Flags & 0x80 ? true : false);

  return RESULT_OK;
}

//------------------------------------------------------------------------------------------


//
void
ASDCP::MPEG2::FrameBuffer::Dump(FILE* stream, ui32_t dump_len) const
{
  if ( stream == 0 )
    stream = stderr;

  fprintf(stream, "Frame: %06u, %c%-2hu, %7u bytes",
	  m_FrameNumber, FrameTypeChar(m_FrameType), m_TemporalOffset, m_Size);

  if ( m_GOPStart )
    fprintf(stream, " (start %s GOP)", ( m_ClosedGOP ? "closed" : "open"));
  
  fputc('\n', stream);

  if ( dump_len > 0 )
    Kumu::hexdump(m_Data, dump_len, stream);
}


//------------------------------------------------------------------------------------------

ASDCP::MPEG2::MXFReader::MXFReader()
{
  m_Reader = new h__Reader;
}


ASDCP::MPEG2::MXFReader::~MXFReader()
{
}

// Open the file for reading. The file must exist. Returns error if the
// operation cannot be completed.
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::OpenRead(const char* filename) const
{
  return m_Reader->OpenRead(filename);
}

//
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::ReadFrame(ui32_t FrameNum, FrameBuffer& FrameBuf,
				   AESDecContext* Ctx, HMACContext* HMAC) const
{
  if ( m_Reader && m_Reader->m_File.IsOpen() )
    return m_Reader->ReadFrame(FrameNum, FrameBuf, Ctx, HMAC);

  return RESULT_INIT;
}


//
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::ReadFrameGOPStart(ui32_t FrameNum, FrameBuffer& FrameBuf,
					   AESDecContext* Ctx, HMACContext* HMAC) const
{
  if ( m_Reader && m_Reader->m_File.IsOpen() )
    return m_Reader->ReadFrameGOPStart(FrameNum, FrameBuf, Ctx, HMAC);

  return RESULT_INIT;
}


//
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::FindFrameGOPStart(ui32_t FrameNum, ui32_t& KeyFrameNum) const
{
  if ( m_Reader && m_Reader->m_File.IsOpen() )
    return m_Reader->FindFrameGOPStart(FrameNum, KeyFrameNum);

  return RESULT_INIT;
}


// Fill the struct with the values from the file's header.
// Returns RESULT_INIT if the file is not open.
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::FillVideoDescriptor(VideoDescriptor& VDesc) const
{
  if ( m_Reader && m_Reader->m_File.IsOpen() )
    {
      VDesc = m_Reader->m_VDesc;
      return RESULT_OK;
    }

  return RESULT_INIT;
}


// Fill the struct with the values from the file's header.
// Returns RESULT_INIT if the file is not open.
ASDCP::Result_t
ASDCP::MPEG2::MXFReader::FillWriterInfo(WriterInfo& Info) const
{
  if ( m_Reader && m_Reader->m_File.IsOpen() )
    {
      Info = m_Reader->m_Info;
      return RESULT_OK;
    }

  return RESULT_INIT;
}

//
void
ASDCP::MPEG2::MXFReader::DumpHeaderMetadata(FILE* stream) const
{
  if ( m_Reader->m_File.IsOpen() )
    m_Reader->m_HeaderPart.Dump(stream);
}


//
void
ASDCP::MPEG2::MXFReader::DumpIndex(FILE* stream) const
{
  if ( m_Reader->m_File.IsOpen() )
    m_Reader->m_FooterPart.Dump(stream);
}


//------------------------------------------------------------------------------------------

//
class ASDCP::MPEG2::MXFWriter::h__Writer : public ASDCP::h__Writer
{
public:
  VideoDescriptor m_VDesc;
  ui32_t          m_GOPOffset;
  byte_t          m_EssenceUL[SMPTE_UL_LENGTH];

  ASDCP_NO_COPY_CONSTRUCT(h__Writer);

  h__Writer() : m_GOPOffset(0) {
    memset(m_EssenceUL, 0, SMPTE_UL_LENGTH);
  }

  ~h__Writer(){}

  Result_t OpenWrite(const char*, ui32_t HeaderSize);
  Result_t SetSourceStream(const VideoDescriptor&);
  Result_t WriteFrame(const FrameBuffer&, AESEncContext* = 0, HMACContext* = 0);
  Result_t Finalize();
};


// Open the file for writing. The file must not exist. Returns error if
// the operation cannot be completed.
ASDCP::Result_t
ASDCP::MPEG2::MXFWriter::h__Writer::OpenWrite(const char* filename, ui32_t HeaderSize)
{
  if ( ! m_State.Test_BEGIN() )
    return RESULT_STATE;

  Result_t result = m_File.OpenWrite(filename);

  if ( ASDCP_SUCCESS(result) )
    {
      m_HeaderSize = HeaderSize;
      m_EssenceDescriptor = new MPEG2VideoDescriptor;
      result = m_State.Goto_INIT();
    }

  return result;
}

// Automatically sets the MXF file's metadata from the MPEG stream.
ASDCP::Result_t
ASDCP::MPEG2::MXFWriter::h__Writer::SetSourceStream(const VideoDescriptor& VDesc)
{
  if ( ! m_State.Test_INIT() )
    return RESULT_STATE;

  m_VDesc = VDesc;
  Result_t result = MPEG2_VDesc_to_MD(m_VDesc, (MPEG2VideoDescriptor*)m_EssenceDescriptor);

  if ( ASDCP_SUCCESS(result) )
      result = WriteMXFHeader(MPEG_PACKAGE_LABEL, UL(Dict::ul(MDD_MPEG2_VESWrapping)), 
			      PICT_DEF_LABEL,     UL(Dict::ul(MDD_PictureDataDef)),
			      m_VDesc.EditRate, 24 /* TCFrameRate */);

  if ( ASDCP_SUCCESS(result) )
    {
      memcpy(m_EssenceUL, Dict::ul(MDD_MPEG2Essence), SMPTE_UL_LENGTH);
      m_EssenceUL[SMPTE_UL_LENGTH-1] = 1; // first (and only) essence container
      result = m_State.Goto_READY();
    }

  return result;
}

// Writes a frame of essence to the MXF file. If the optional AESEncContext
// argument is present, the essence is encrypted prior to writing.
// Fails if the file is not open, is finalized, or an operating system
// error occurs.
//
ASDCP::Result_t
ASDCP::MPEG2::MXFWriter::h__Writer::WriteFrame(const FrameBuffer& FrameBuf, AESEncContext* Ctx,
					       HMACContext* HMAC)
{
  Result_t result = RESULT_OK;

  if ( m_State.Test_READY() )
    result = m_State.Goto_RUNNING(); // first time through, get the body location

  IndexTableSegment::IndexEntry Entry;
  Entry.StreamOffset = m_StreamOffset;

  if ( ASDCP_SUCCESS(result) )
    result = WriteEKLVPacket(FrameBuf, m_EssenceUL, Ctx, HMAC);

  if ( ASDCP_FAILURE(result) )
    return result;

  // create mxflib flags
  int Flags = 0;

  switch ( FrameBuf.FrameType() )
    {
    case FRAME_I: Flags = 0x00; break;
    case FRAME_P: Flags = 0x22; break;
    case FRAME_B: Flags = 0x33; break;
    }

  if ( FrameBuf.GOPStart() )
    {
      m_GOPOffset = 0;
      Flags |= 0x40;

      if ( FrameBuf.ClosedGOP() )
	Flags |= 0x80;
    }

  // update the index manager
  Entry.TemporalOffset = - FrameBuf.TemporalOffset();
  Entry.KeyFrameOffset = 0 - m_GOPOffset;
  Entry.Flags = Flags;
  /*
  fprintf(stderr, "to: %4hd   ko: %4hd   c1: %4hd   c2: %4hd   fl: 0x%02x\n",
	  Entry.TemporalOffset, Entry.KeyFrameOffset,
	  m_GOPOffset + Entry.TemporalOffset,
	  Entry.KeyFrameOffset - Entry.TemporalOffset,
	  Entry.Flags);
  */
  m_FooterPart.PushIndexEntry(Entry);
  m_FramesWritten++;
  m_GOPOffset++;

  return RESULT_OK;
}


// Closes the MXF file, writing the index and other closing information.
//
ASDCP::Result_t
ASDCP::MPEG2::MXFWriter::h__Writer::Finalize()
{
  if ( ! m_State.Test_RUNNING() )
    return RESULT_STATE;

  m_State.Goto_FINAL();

  return WriteMXFFooter();
}


//------------------------------------------------------------------------------------------



ASDCP::MPEG2::MXFWriter::MXFWriter()
{
}

ASDCP::MPEG2::MXFWriter::~MXFWriter()
{
}

// Open the file for writing. The file must not exist. Returns error if
// the operation cannot be completed.
ASDCP::Result_t
ASDCP::MPEG2::MXFWriter::OpenWrite(const char* filename, const WriterInfo& Info,
				   const VideoDescriptor& VDesc, ui32_t HeaderSize)
{
  m_Writer = new h__Writer;
  m_Writer->m_Info = Info;
  
  Result_t result = m_Writer->OpenWrite(filename, HeaderSize);

  if ( ASDCP_SUCCESS(result) )
    result = m_Writer->SetSourceStream(VDesc);

  if ( ASDCP_FAILURE(result) )
    m_Writer.release();

  return result;
}


// Writes a frame of essence to the MXF file. If the optional AESEncContext
// argument is present, the essence is encrypted prior to writing.
// Fails if the file is not open, is finalized, or an operating system
// error occurs.
ASDCP::Result_t
ASDCP::MPEG2::MXFWriter::WriteFrame(const FrameBuffer& FrameBuf, AESEncContext* Ctx, HMACContext* HMAC)
{
  if ( m_Writer.empty() )
    return RESULT_INIT;

  return m_Writer->WriteFrame(FrameBuf, Ctx, HMAC);
}

// Closes the MXF file, writing the index and other closing information.
ASDCP::Result_t
ASDCP::MPEG2::MXFWriter::Finalize()
{
  if ( m_Writer.empty() )
    return RESULT_INIT;

  return m_Writer->Finalize();
}

#ifdef MPEG3D
class ASDCP::MPEG2::MXFSWriter::h__SWriter : public ASDCP::h__Writer
{
	StereoscopicPhase_t m_NextPhase;
public:
	VideoDescriptor m_VDesc;
	ui32_t          m_GOPOffset;
	byte_t          m_EssenceUL[SMPTE_UL_LENGTH];

	ASDCP_NO_COPY_CONSTRUCT(h__SWriter);

	h__SWriter() : m_GOPOffset(0), m_NextPhase(SP_LEFT) {
		memset(m_EssenceUL, 0, SMPTE_UL_LENGTH);
	}

	~h__SWriter(){}

	Result_t OpenWrite(const char*, ui32_t HeaderSize);
	Result_t SetSourceStream(const VideoDescriptor&);
	Result_t WriteFrame(const FrameBuffer&, StereoscopicPhase_t phase, AESEncContext* = 0, HMACContext* = 0);
	Result_t Finalize();
};

// Automatically sets the MXF file's metadata from the MPEG stream.
ASDCP::Result_t
ASDCP::MPEG2::MXFSWriter::h__SWriter::SetSourceStream(const VideoDescriptor& VDesc)
{
  if ( ! m_State.Test_INIT() )
    return RESULT_STATE;

  m_VDesc = VDesc;
  Result_t result = MPEG2_VDesc_to_MD(m_VDesc, (MPEG2VideoDescriptor*)m_EssenceDescriptor);

  if ( ASDCP_SUCCESS(result) )
      result = WriteMXFHeader(MPEG_S_PACKAGE_LABEL, UL(Dict::ul(MDD_MPEG2_VESWrapping)), 
			      PICT_DEF_LABEL,     UL(Dict::ul(MDD_PictureDataDef)),
			      ASDCP::EditRate_24, 24 /* TCFrameRate */);

  if ( ASDCP_SUCCESS(result) )
    {
      memcpy(m_EssenceUL, Dict::ul(MDD_MPEG2Essence), SMPTE_UL_LENGTH);
      m_EssenceUL[SMPTE_UL_LENGTH-1] = 1; // first (and only) essence container
      result = m_State.Goto_READY();
    }

  return result;
}

// Open the file for writing. The file must not exist. Returns error if
// the operation cannot be completed.
ASDCP::Result_t
ASDCP::MPEG2::MXFSWriter::h__SWriter::OpenWrite(const char* filename, ui32_t HeaderSize)
{
	if ( ! m_State.Test_BEGIN() )
		return RESULT_STATE;

	Result_t result = m_File.OpenWrite(filename);

	if ( ASDCP_SUCCESS(result) )
	{
		m_HeaderSize = HeaderSize;
		m_EssenceDescriptor = new MPEG2VideoDescriptor;

		InterchangeObject* StereoSubDesc = new StereoscopicPictureSubDescriptor;
		m_EssenceSubDescriptorList.push_back(StereoSubDesc);
		GenRandomValue(StereoSubDesc->InstanceUID);
		m_EssenceDescriptor->SubDescriptors.push_back(StereoSubDesc->InstanceUID);
		result = m_State.Goto_INIT();
	}

	return result;
}


// Writes a frame of essence to the MXF file. If the optional AESEncContext
// argument is present, the essence is encrypted prior to writing.
// Fails if the file is not open, is finalized, or an operating system
// error occurs.
ASDCP::Result_t
ASDCP::MPEG2::MXFSWriter::h__SWriter::WriteFrame(const FrameBuffer& FrameBuf, StereoscopicPhase_t phase, AESEncContext* Ctx, HMACContext* HMAC)
{
  if (m_NextPhase != phase)
	  return RESULT_SPHASE;

  bool add_index = false;
  if (phase == SP_LEFT)
  {
	  m_NextPhase = SP_RIGHT;
	  add_index = true;
  }
  else
  {
	  m_NextPhase = SP_LEFT;
	  add_index = false;
  }

  Result_t result = RESULT_OK;

  if ( m_State.Test_READY() )
    result = m_State.Goto_RUNNING(); // first time through, get the body location

  IndexTableSegment::IndexEntry Entry;
  Entry.StreamOffset = m_StreamOffset;

  if ( ASDCP_SUCCESS(result) )
    result = WriteEKLVPacket(FrameBuf, m_EssenceUL, Ctx, HMAC);

  if ( ASDCP_FAILURE(result) )
    return result;

  // create mxflib flags
  int Flags = 0;

  switch ( FrameBuf.FrameType() )
    {
    case FRAME_I: Flags = 0x00; break;
    case FRAME_P: Flags = 0x22; break;
    case FRAME_B: Flags = 0x33; break;
    }

  if ( FrameBuf.GOPStart() )
    {
      m_GOPOffset = 0;
      Flags |= 0x40;

      if ( FrameBuf.ClosedGOP() )
	Flags |= 0x80;
    }

  if (ASDCP_SUCCESS(result) && add_index)
  {
  // update the index manager
	  Entry.TemporalOffset = - FrameBuf.TemporalOffset();
	  Entry.KeyFrameOffset = 0 - m_GOPOffset;
	  Entry.Flags = Flags;
	  m_FooterPart.PushIndexEntry(Entry);
  }
  
  /*
  fprintf(stderr, "to: %4hd   ko: %4hd   c1: %4hd   c2: %4hd   fl: 0x%02x\n",
	  Entry.TemporalOffset, Entry.KeyFrameOffset,
	  m_GOPOffset + Entry.TemporalOffset,
	  Entry.KeyFrameOffset - Entry.TemporalOffset,
	  Entry.Flags);
  */
  m_FramesWritten++;
  m_GOPOffset++;

  return result;
}

// Closes the MXF file, writing the index and other closing information.
ASDCP::Result_t
ASDCP::MPEG2::MXFSWriter::h__SWriter::Finalize()
{
	if (m_NextPhase != SP_LEFT)
		return RESULT_SPHASE;

	assert(m_FramesWritten % 2 == 0);
	m_FramesWritten /=2;

	if ( ! m_State.Test_RUNNING() )
		return RESULT_STATE;

	m_State.Goto_FINAL();

	return WriteMXFFooter();
}

//------------------------------------------------------------------------------------------


ASDCP::MPEG2::MXFSWriter::MXFSWriter()
{
}

ASDCP::MPEG2::MXFSWriter::~MXFSWriter()
{
}

// Open the file for writing. The file must not exist. Returns error if
// the operation cannot be completed.
ASDCP::Result_t
ASDCP::MPEG2::MXFSWriter::OpenWrite(const char* filename, const WriterInfo& Info,
				   const VideoDescriptor& VDesc, ui32_t HeaderSize)
{
  m_Writer = new h__SWriter;
  m_Writer->m_Info = Info;
  
  if (VDesc.EditRate != ASDCP::EditRate_24)
  {
	  DefaultLogSink().Error("Stereoscopic wrapping requires 24 fps input streams.\n");
	  return RESULT_FORMAT;
  }
  
  Result_t result = m_Writer->OpenWrite(filename, HeaderSize);

  if ( ASDCP_SUCCESS(result) )
  {
	  VideoDescriptor TmpVDesc = VDesc;
	  TmpVDesc.EditRate = ASDCP::EditRate_48;
	  result = m_Writer->SetSourceStream(TmpVDesc);
  }

  if ( ASDCP_FAILURE(result) )
    m_Writer.release();

  return result;
}


// Writes a frame of essence to the MXF file. If the optional AESEncContext
// argument is present, the essence is encrypted prior to writing.
// Fails if the file is not open, is finalized, or an operating system
// error occurs.
ASDCP::Result_t
ASDCP::MPEG2::MXFSWriter::WriteFrame(const FrameBuffer& FrameBuf, StereoscopicPhase_t phase, AESEncContext* Ctx, HMACContext* HMAC)
{
  if ( m_Writer.empty() )
    return RESULT_INIT;

  return m_Writer->WriteFrame(FrameBuf, phase, Ctx, HMAC);
}

// Closes the MXF file, writing the index and other closing information.
ASDCP::Result_t
ASDCP::MPEG2::MXFSWriter::Finalize()
{
  if ( m_Writer.empty() )
    return RESULT_INIT;

  return m_Writer->Finalize();
}


//--------------------------------------------------------------------------

class ASDCP::MPEG2::MXFSReader::h__SReader : public ASDCP::h__Reader
{
	ASDCP_NO_COPY_CONSTRUCT(h__SReader);

	MPEG2VideoDescriptor* m_EssenceDescriptor;
	ASDCP::Rational               m_EditRate;
	ui32_t m_StereoFrameReady;

public:
	VideoDescriptor m_VDesc;        // video parameter list

	h__SReader(): m_StereoFrameReady(0xffffffff) {}
	~h__SReader() {}
	Result_t    OpenRead(const char*);
	Result_t    ReadFrame(ui32_t, FrameBuffer&, AESDecContext*, HMACContext*);
	Result_t    ReadFrame(ui32_t, StereoscopicPhase_t, FrameBuffer&, 
		AESDecContext* Ctx, HMACContext* HMAC);
	Result_t    ReadFrameGOPStart(ui32_t, FrameBuffer&, AESDecContext*, HMACContext*);
	Result_t    FindFrameGOPStart(ui32_t, ui32_t&);
};


//
//
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::h__SReader::OpenRead(const char* filename)
{
	Result_t result = OpenMXFRead(filename);

	if( ASDCP_SUCCESS(result) )
	{
		InterchangeObject* Object;
		if ( ASDCP_SUCCESS(m_HeaderPart.GetMDObjectByType(OBJ_TYPE_ARGS(MPEG2VideoDescriptor), &Object)) )
		{
			assert(Object);

			std::list<InterchangeObject*> ObjectList;
			m_HeaderPart.GetMDObjectsByType(OBJ_TYPE_ARGS(Track), ObjectList);

			if ( ObjectList.empty() )
			{
				DefaultLogSink().Error("MXF Metadata contains no Track Sets.\n");
				return RESULT_FORMAT;
			}

			m_EditRate = ((Track*)ObjectList.front())->EditRate;
			m_EssenceDescriptor = static_cast<MPEG2VideoDescriptor*>(Object);
			if ( ! ( m_EditRate == EditRate_24 && m_EssenceDescriptor->SampleRate == EditRate_48 ) )
			{
				DefaultLogSink().Error("EditRate and SampleRate not correct for 24/48 stereoscopic essence.\n");
				return RESULT_FORMAT;
			}

			result = MD_to_MPEG2_VDesc((MXF::MPEG2VideoDescriptor*)Object, m_VDesc, m_EditRate);
		}
	}

	if( ASDCP_SUCCESS(result) )
		result = InitMXFIndex();

	if( ASDCP_SUCCESS(result) )
		result = InitInfo();

	return result;
}


//
//
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::h__SReader::ReadFrameGOPStart(ui32_t FrameNum, FrameBuffer& FrameBuf,
													  AESDecContext* Ctx, HMACContext* HMAC)
{
	ui32_t KeyFrameNum;

	Result_t result = FindFrameGOPStart(FrameNum, KeyFrameNum);

	if ( ASDCP_SUCCESS(result) )
		result = ReadFrame(KeyFrameNum, FrameBuf, Ctx, HMAC);

	return result;
}


//
//
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::h__SReader::FindFrameGOPStart(ui32_t FrameNum, ui32_t& KeyFrameNum)
{
	KeyFrameNum = 0;

	if ( ! m_File.IsOpen() )
		return RESULT_INIT;

	// look up frame index node
	IndexTableSegment::IndexEntry TmpEntry;

	if ( ASDCP_FAILURE(m_FooterPart.Lookup(FrameNum, TmpEntry)) )
	{
		DefaultLogSink().Error("Frame value out of range: %u\n", FrameNum);
		return RESULT_RANGE;
	}

	KeyFrameNum = FrameNum - TmpEntry.KeyFrameOffset;

	return RESULT_OK;
}


//
//
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::h__SReader::ReadFrame(ui32_t FrameNum, FrameBuffer& FrameBuf,
											  AESDecContext* Ctx, HMACContext* HMAC)
{
	if ( ! m_File.IsOpen() )
		return RESULT_INIT;

	Result_t result = ReadEKLVFrame(FrameNum, FrameBuf, Dict::ul(MDD_MPEG2Essence), Ctx, HMAC);

	if ( ASDCP_FAILURE(result) )
		return result;

	IndexTableSegment::IndexEntry TmpEntry;
	m_FooterPart.Lookup(FrameNum, TmpEntry);

	switch ( ( TmpEntry.Flags >> 4 ) & 0x03 )
	{
	case 0:  FrameBuf.FrameType(FRAME_I); break;
	case 2:  FrameBuf.FrameType(FRAME_P); break;
	case 3:  FrameBuf.FrameType(FRAME_B); break;
	default: FrameBuf.FrameType(FRAME_U);
	}

	FrameBuf.TemporalOffset(TmpEntry.TemporalOffset);
	FrameBuf.GOPStart(TmpEntry.Flags & 0x40 ? true : false);
	FrameBuf.ClosedGOP(TmpEntry.Flags & 0x80 ? true : false);

	return RESULT_OK;
}

ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::h__SReader::ReadFrame(ui32_t FrameNum, StereoscopicPhase_t phase, FrameBuffer& FrameBuf,
	AESDecContext* Ctx, HMACContext* HMAC)
{
	IndexTableSegment::IndexEntry TmpEntry;
	if (ASDCP_FAILURE(m_FooterPart.Lookup(FrameNum, TmpEntry)))
	{
		DefaultLogSink().Error("Frame value out of range: %u\n", FrameNum);
		return RESULT_RANGE;
	}
	
	Kumu::fpos_t FilePosition = m_EssenceStart + TmpEntry.StreamOffset;
	Result_t result = RESULT_OK;
	if (phase == SP_LEFT)
	{
		if (FilePosition != m_LastPosition)
		{
			m_LastPosition = FilePosition;
			result = m_File.Seek(FilePosition);
		}

		m_StereoFrameReady = FrameNum;
	}
	else if (phase == SP_RIGHT)
	{
		if (m_StereoFrameReady != FrameNum)
		{
			if (FilePosition != m_LastPosition)
			{
				m_LastPosition = FilePosition;
				result = m_File.Seek(FilePosition);
			}

			KLReader Reader;
			result = Reader.ReadKLFromFile(m_File);

			if (ASDCP_SUCCESS(result))
			{
				Kumu::fpos_t new_pos = FilePosition + SMPTE_UL_LENGTH + Reader.KLLength() + Reader.Length();
				result = m_File.Seek(new_pos);
			}	
		}

		m_StereoFrameReady = 0xffffffff;
	}
	else
	{
		DefaultLogSink().Error("Unexpected stereoscopic phase value: %u\n", phase);
		return RESULT_STATE;
	}

	if (ASDCP_SUCCESS(result))
	{
		ui32_t SequenceNum = FrameNum*2;
		SequenceNum += (phase == SP_RIGHT) ? 2 : 1;
		result = ReadEKLVPacket(FrameNum, SequenceNum, FrameBuf, Dict::ul(MDD_MPEG2Essence), Ctx, HMAC);
	}	
	return result;
}
//------------------------------------------------------------------------------------------


ASDCP::MPEG2::MXFSReader::MXFSReader()
{
	m_Reader = new h__SReader;
}


ASDCP::MPEG2::MXFSReader::~MXFSReader()
{
}

// Open the file for reading. The file must exist. Returns error if the
// operation cannot be completed.
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::OpenRead(const char* filename) const
{
	return m_Reader->OpenRead(filename);
}

ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::ReadFrame(ui32_t FrameNum, SFrameBuffer& FrameBuffer,
									AESDecContext* Ctx, HMACContext* HMAC) const
{
	Result_t result = RESULT_INIT;
	if (m_Reader && m_Reader->m_File.IsOpen())
	{
		result = m_Reader->ReadFrame(FrameNum, SP_LEFT, FrameBuffer.Left, Ctx, HMAC);
		if (ASDCP_SUCCESS(result))
			result = m_Reader->ReadFrame(FrameNum, SP_RIGHT, FrameBuffer.Right, Ctx, HMAC);
	}
	return result;
}
//
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::ReadFrame(ui32_t FrameNum, StereoscopicPhase_t phase, FrameBuffer& FrameBuf,
								   AESDecContext* Ctx, HMACContext* HMAC) const
{
	if ( m_Reader && m_Reader->m_File.IsOpen() )
		return m_Reader->ReadFrame(FrameNum, phase, FrameBuf, Ctx, HMAC);

	return RESULT_INIT;
}


//
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::ReadFrameGOPStart(ui32_t FrameNum, FrameBuffer& FrameBuf,
										   AESDecContext* Ctx, HMACContext* HMAC) const
{
	if ( m_Reader && m_Reader->m_File.IsOpen() )
		return m_Reader->ReadFrameGOPStart(FrameNum, FrameBuf, Ctx, HMAC);

	return RESULT_INIT;
}


//
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::FindFrameGOPStart(ui32_t FrameNum, ui32_t& KeyFrameNum) const
{
	if ( m_Reader && m_Reader->m_File.IsOpen() )
		return m_Reader->FindFrameGOPStart(FrameNum, KeyFrameNum);

	return RESULT_INIT;
}


// Fill the struct with the values from the file's header.
// Returns RESULT_INIT if the file is not open.
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::FillVideoDescriptor(VideoDescriptor& VDesc) const
{
	if ( m_Reader && m_Reader->m_File.IsOpen() )
	{
		VDesc = m_Reader->m_VDesc;
		return RESULT_OK;
	}

	return RESULT_INIT;
}


// Fill the struct with the values from the file's header.
// Returns RESULT_INIT if the file is not open.
ASDCP::Result_t
ASDCP::MPEG2::MXFSReader::FillWriterInfo(WriterInfo& Info) const
{
	if ( m_Reader && m_Reader->m_File.IsOpen() )
	{
		Info = m_Reader->m_Info;
		return RESULT_OK;
	}

	return RESULT_INIT;
}

//
void
ASDCP::MPEG2::MXFSReader::DumpHeaderMetadata(FILE* stream) const
{
	if ( m_Reader->m_File.IsOpen() )
		m_Reader->m_HeaderPart.Dump(stream);
}


//
void
ASDCP::MPEG2::MXFSReader::DumpIndex(FILE* stream) const
{
	if ( m_Reader->m_File.IsOpen() )
		m_Reader->m_FooterPart.Dump(stream);
}

#endif
//
// end AS_DCP_MPEG2.cpp
//
