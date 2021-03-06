/*! \file	datachunk.cpp
 *	\brief	Simple re-sizable data chunk object
 *
 *	\version $Id: datachunk.cpp,v 1.1.1.1 2007/04/03 02:08:42 baiqian Exp $
 *
 */
/*
 *	Copyright (c) 2004, Matt Beard
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mxflib/mxflib.h>

using namespace mxflib;


//! Resize the data chunk, preserving contents if requested
void mxflib::DataChunk::Resize(UInt32 NewSize, bool PreserveContents /*=true*/)
{
	if(Size == NewSize) return;

	// Simple truncate or resize within the existing buffer size
	if(DataSize >= NewSize) 
	{
		Size = NewSize;
		return;
	}

	UInt32 AllocSize = NewSize;
	if(AllocationGranularity)
	{
//debug("AllocGran = %u, NewSize = %u, ", AllocationGranularity, NewSize);
		// Apply allocation granularity
		AllocSize = (NewSize-1) / AllocationGranularity;
//debug("AllocSize = %u, ", AllocSize);
		AllocSize = (AllocSize+1) * AllocationGranularity;
//debug("AllocSize = %u\n", AllocSize);
	}

	UInt8 *NewData = new UInt8[AllocSize];
	if(PreserveContents && (Size != 0)) memcpy(NewData, Data, Size);

//debug("Changing Buffer @ 0x%08x -> 0x%08x (0x%04x)\n", (int)Data, (int)NewData, (int)AllocSize);
	if((!ExternalBuffer) && (Data)) delete[] Data;
	ExternalBuffer = false;
	
	Data = NewData;
	DataSize = AllocSize;
	Size = NewSize;
}


//! Resize the data buffer, preserving contents if requested
/*! The buffer is resized to <b>at least</b> NewSize, but Size remains unchanged */
void mxflib::DataChunk::ResizeBuffer(UInt32 NewSize, bool PreserveContents /*=true*/)
{
	if(DataSize >= NewSize) return;

	if(AllocationGranularity)
	{
		// Apply allocation granularity
		NewSize = (NewSize-1) / AllocationGranularity;
		NewSize = (NewSize+1) * AllocationGranularity;
	}

	UInt8 *NewData = new UInt8[NewSize];
	if(PreserveContents && (Size != 0)) memcpy(NewData, Data, Size);

//debug("Changing Buffer @ 0x%08x -> 0x%08x (0x%04x)+\n", (int)Data, (int)NewData, (int)NewSize);
	if((!ExternalBuffer) && (Data)) delete[] Data;
	ExternalBuffer = false;
	
	Data = NewData;
	DataSize = NewSize;
}


//! Get a (hex) string representation of the data in the buffer
std::string mxflib::DataChunk::GetString(void)
{
	std::string Ret;
	unsigned i;
	for(i=0; i<Size; i++) 
	{
		if(i != 0) Ret += " ";
		Ret += Int2HexString(Data[i], 2);
	}

	return Ret;
};


//! Set an external buffer as the data buffer
/*! The DataChunk does not take ownership of the buffer
 *  \note If an external buffer has been set for a DataChunk it may not
 *		  stay as the buffer in use. This is because there may not be
 *		  enough room in the buffer to hold new data. Therefore it is
 *		  important that the value of property <tt><b>Data</b></tt> is checked
 *		  before assuming the external buffer is still in use.
 */
void mxflib::DataChunk::SetBuffer(UInt8 *Buffer, UInt32 BuffSize, UInt32 AllocatedSize /*=0*/)
{
//debug("Setting Buffer @ 0x%08x -> 0x%08x\n", (int)Data, (int)Buffer);
	if((!ExternalBuffer) && (Data)) delete[] Data;

	Size = BuffSize;
	Data = Buffer;

	if(AllocatedSize == 0) DataSize = BuffSize;
	else DataSize = AllocatedSize;

	ExternalBuffer = true;
}


//! Transfer ownership of a data buffer from another DataChunk
/*! This is a very efficient way to set one DataChunk to the value of another.
 *  However it partially destroys the source DataChunk by stealing its buffer.
 *  \return true on success, false on failure
 */
bool mxflib::DataChunk::TakeBuffer(DataChunk &OldOwner, bool MakeEmpty /*=false*/ )
{
	UInt32 BuffSize = OldOwner.Size;
	UInt32 AllocatedSize = OldOwner.DataSize;
	bool ExtBuff = OldOwner.ExternalBuffer;

	// Steal the old buffer
	UInt8 *Buffer = OldOwner.StealBuffer(MakeEmpty);

	// Fail if the old owner does not own its buffer!
	if(!Buffer) return false;

	// Release any old buffer
	if((!ExternalBuffer) && (Data)) delete[] Data;

	// Set the new details
	Size = BuffSize;
	DataSize = AllocatedSize;
	Data = Buffer;
	ExternalBuffer = ExtBuff;

	return true;
}


//! Transfer ownership of a data buffer from another DataChunk (via a smart pointer)
/*! This is a very efficient way to set one DataChunk to the value of another.
 *  However it partially destroys the source DataChunk by stealing its buffer.
 *  \return true on success, false on failure
 */
bool mxflib::DataChunk::TakeBuffer(DataChunkPtr &OldOwner, bool MakeEmpty /*=false*/ )
{
	UInt32 BuffSize = OldOwner->Size;
	UInt32 AllocatedSize = OldOwner->DataSize;
	bool ExtBuff = OldOwner->ExternalBuffer;

	// Steal the old buffer
	UInt8 *Buffer = OldOwner->StealBuffer(MakeEmpty);

	// Fail if the old owner does not own its buffer!
	if(!Buffer) return false;

	// Release any old buffer
	if((!ExternalBuffer) && (Data)) delete[] Data;

	// Set the new details
	Size = BuffSize;
	DataSize = AllocatedSize;
	Data = Buffer;
	ExternalBuffer = ExtBuff;

	return true;
}

