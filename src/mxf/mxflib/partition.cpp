/*! \file	partition.cpp
 *	\brief	Implementation of Partition class
 *
 *			The Partition class holds data about a partition, either loaded 
 *          from a partition in the file or built in memory
 *
 *	\version $Id: partition.cpp,v 1.3 2007/10/09 03:40:50 baiqian Exp $
 *
 */
/*
 *	Copyright (c) 2003, Matt Beard
 *	Portions Copyright (c) 2003, Metaglue Corporation
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

using namespace mxflib;


//! Add a metadata object to the header metadata belonging to a partition
/*! Note that any strongly linked objects are also added */
void mxflib::Partition::AddMetadata(MDObjectPtr NewObject)
{
	// Start out without a target
	bool has_target = false;

	// Start out not (strong) reffed
	bool linked = false;

	// Add us to the list of all items
	AllMetadata.push_back(NewObject);

	// Add this object to the ref target list if it is one. At the same time any objects
	// linked from this object (before this function was called) are added as well
	// Note: although nothing currently does it it is theoretically possible to
	//       have more than one target entry in a set
	MDObjectULList::iterator it = NewObject->begin();
	while(it != NewObject->end())
	{
		DictRefType RefType = (*it).second->GetRefType();

		if(RefType == DICT_REF_TARGET)
		{
			if((*it).second->Value->GetData().Size != 16)
			{
				error("Metadata Object \"%s/%s\" should be a reference target (a UUID), but has size %d\n",
					  NewObject->Name().c_str(), (*it).second->Name().c_str(), (*it).second->Value->GetData().Size);
			}
			else
			{
				has_target = true;

				UUIDPtr ID = new UUID((*it).second->Value->PutData()->Data);
				RefTargets.insert(std::map<UUID, MDObjectPtr>::value_type(*ID, NewObject));
			
				// Try and satisfy all refs to this set
				for(;;)
				{
					std::multimap<UUID, MDObjectPtr>::iterator mit = UnmatchedRefs.find(*ID);
					
					// Exit when no more refs to this object
					if(mit == UnmatchedRefs.end()) break;

					// Sanity check!
					if((*mit).second->GetLink())
					{
						error("Internal error - Object in UnmatchedRefs but already linked!");
					}

					// Make the link
					(*mit).second->SetLink(NewObject);

					// If we are the tagert of a strong ref we won't get added to the top level
					if((*mit).second->GetRefType() == DICT_REF_STRONG) linked = true;

					// Remove from the unmatched refs map
					UnmatchedRefs.erase(mit);

					// loop for any more refs to this set
				}
			}
		}
		else if(RefType == DICT_REF_STRONG)
		{
			MDObjectPtr Link = (*it).second->GetLink();
			if(Link)
			{
				AddMetadata(Link);

				// Prevent the new item being top-level (which it may be as we are not added yet)
				// DRAGONS: There is surely a better way than this!!
				TopLevelMetadata.remove(Link);
			}
		} 
		else if(!((*it).second->empty()))
		{
			MDObjectULList::iterator it2 = (*it).second->begin();
			MDObjectULList::iterator itend2 = (*it).second->end();
			while(it2 != itend2)
			{
				if((*it2).second->GetRefType() == DICT_REF_STRONG)
				{
					MDObjectPtr Link = (*it2).second->GetLink();
					if(Link)
					{
						AddMetadata(Link);

						// Prevent the new item being top-level (which it may be as we are not added yet)
						// DRAGONS: There is surely a better way than this!!
						TopLevelMetadata.remove(Link);
					}
				}
				else if(!((*it2).second->empty()))
				{
					error("Internal error for object %s - Cannot process nesting > 2 in AddMetadata()\n",
						   (*it2).second->FullName().c_str());
				}
				it2++;
			}
		}

		it++;
	}

	// If we are not yet (strong) reffed then we are top level
	if(!linked) TopLevelMetadata.push_back(NewObject);

	// Satisfy, or record as un-matched, all outgoing references
	ProcessChildRefs(NewObject);
}


//! Satisfy, or record as un-matched, all outgoing references
void mxflib::Partition::ProcessChildRefs(MDObjectPtr ThisObject)
{
	MDObjectULList::iterator it = ThisObject->begin();
	while(it != ThisObject->end())
	{
		// Only try to match references if not already matched
		if(!(*it).second->GetLink())
		{
			DictRefType Ref = (*it).second->GetRefType();
			if((Ref == DICT_REF_STRONG) || (Ref == DICT_REF_WEAK))
			{
				if(!(*it).second->Value)
				{
					error("Metadata Object \"%s/%s\" should be a reference source (a UUID), but has no valid value\n",
						  ThisObject->Name().c_str(), (*it).second->Name().c_str());
				}
				else if((*it).second->Value->GetData().Size != 16)
				{
					error("Metadata Object \"%s/%s\" should be a reference source (a UUID), but has size %d\n",
						  ThisObject->Name().c_str(), (*it).second->Name().c_str(), (*it).second->Value->GetData().Size);
				}
				else
				{
					UUIDPtr ID = new UUID((*it).second->Value->PutData()->Data);
					std::map<UUID, MDObjectPtr>::iterator mit = RefTargets.find(*ID);

					if(mit == RefTargets.end())
					{
						// Not matched yet, so add to the list of outstanding refs
						UnmatchedRefs.insert(std::multimap<UUID, MDObjectPtr>::value_type(*ID, (*it).second));
					}
					else
					{
						// Make the link
						(*it).second->SetLink((*mit).second);

						// If we have made a strong ref, remove the target from the top level
						if(Ref == DICT_REF_STRONG) TopLevelMetadata.remove((*mit).second);
					}
				}
			}
		}

		// Recurse to process sub-children if they exist
		if(!(*it).second->empty()) ProcessChildRefs((*it).second);

		it++;
	}
}


//! Read a full set of header metadata from this partition's source file (including primer)
/*!  \return The number of bytes read (<b>including</b> any preceeding filler)
 *   \return 0 if no header metadata in this partition
 */
Length mxflib::Partition::ReadMetadata(void)
{
	Length MetadataSize = GetInt64(HeaderByteCount_UL);
	if(MetadataSize == 0) return 0;

	MXFFilePtr ParentFile = Object->GetParentFile();

	if(!ParentFile)
	{
		error("Call to Partition::ReadMetadata() on a partition that is not read from a file\n");
		return 0;
	}

	// Find the start of the metadata 
	// DRAGONS: not the most efficient way - we could store a pointer to the end of the pack
	ParentFile->Seek(Object->GetLocation() + 16);
	Length Len = ParentFile->ReadBER();
	ParentFile->Seek(ParentFile->Tell() + Len);

	return ReadMetadata(ParentFile, MetadataSize);
}


//! Read a full set of header metadata from a file (including primer)
/*! \note The value of "Size" does not include the size of any filler before
 *        the primer, but the return value does
 *  \return The number of bytes read (<b>including</b> any preceeding filler)
 */
Length mxflib::Partition::ReadMetadata(MXFFilePtr File, Length Size)
{
	Length Bytes = 0;
	Length FillerBytes = 0;

	// Clear any existing metadata
	ClearMetadata();

	// Quick return for NULL metadata
	if(Size == 0) return 0;

	// Record the position of the current item
	Position Location = File->Tell();

	// Check for a leading filler item
	{
		ULPtr FirstUL = File->ReadKey();
		if(!FirstUL)
		{
			error("Error reading first KLV after %s at 0x%s in %s\n", FullName().c_str(), 
				  Int64toHexString(GetLocation(),8).c_str(), GetSource().c_str());
			return 0;
		}

		MDOTypePtr FirstType = MDOType::Find(FirstUL);

		if(FirstType && FirstType->Name() == "KLVFill")
		{
			// Skip over the filler, recording how far we went
			Position NewLocation = File->ReadBER();
			NewLocation += File->Tell();
			FillerBytes = NewLocation - Location;
			Location = NewLocation;
		}
	}

	// Read enough bytes for the metadata
	File->Seek(Location);
	DataChunkPtr Data = File->Read(Size);

	if(Data->Size != Size)
	{
		error("Header Metadata starting at 0x%s should contain 0x%s bytes, but only 0x%s could be read\n",
			  Int64toHexString(Location,8).c_str(), Int64toHexString(Size,8).c_str(), Int64toHexString(Data->Size,8).c_str());

		Size = Data->Size;
	}

	// Start of data buffer
	const UInt8 *BuffPtr = Data->Data;

	while(Size)
	{
		Length BytesAtItemStart = Bytes;
		if(Size < 16)
		{
			error("Less than 16-bytes of header metadata available after reading 0x%s bytes at 0x%s in file \"%s\"\n", 
				 Int64toHexString(Bytes, 8).c_str(), Int64toHexString(File->Tell(),8).c_str(), File->Name.c_str() );
			break;
		}

/*		// Sanity check the keys
		if((BuffPtr[0] != 6) || (BuffPtr[1] != 0x0e))
		{
			error("Invalid KLV key found at 0x%s in file \"%s\"\n", Int64toHexString(File->Tell(),8).c_str(), File->Name.c_str() );
			break;
		}
*/
		// Build an object (it may come back as an "unknown")
		ULPtr NewUL = new UL(BuffPtr);
		MDObjectPtr NewItem = new MDObject(NewUL);
		ASSERT(NewItem);

		BuffPtr += 16;
		Size -= 16;
		Bytes += 16;

		if(Size < 1)
		{
			error("Incomplete BER length at 0x%s in file \"%s\"\n", Int64toHexString(File->Tell(),8).c_str(), File->Name.c_str() );
			break;
		}

		Length Len = *BuffPtr++;
		Size--;
		Bytes++;
		if(Len >= 0x80)
		{
			UInt32 i = (UInt32)Len & 0x7f;
			if(Size < i)
			{
				error("Incomplete BER length at 0x%s in \"%s\"\n", Int64toHexString(File->Tell(),8).c_str(), File->Name.c_str() );
				break;
			}

			Len = 0;
			while(i--) 
			{
				Len = ((Len<<8) + *(BuffPtr++));
				Size--;
				Bytes++;
			}
		}

		// DRAGONS: KLV Size limit!!
		if(Len > 0xffffffff)
		{
			error("Current implementation KLV size limit of 0xffffffff bytes exceeded at 0x%s in file \"%s\"\n", 
				  Int64toHexString(Location + Bytes,8).c_str(), File->Name.c_str() );
			break;
		}

		if(Size < Len)
		{
			error("KLV length is %s but available data size is only %s after reading 0x%s of header metadata at 0x%s in \"%s\"\n", 
				  UInt64toString(Len).c_str(), UInt64toString(Size).c_str(), Int64toHexString(Bytes, 8).c_str(),
				  Int64toHexString(Location + Bytes,8).c_str(), File->Name.c_str() );

			// Try reading what we have
			Len = Size;
		}

		// Check for the primer until we have found it
		if(!PartitionPrimer)
		{
			if(NewItem->Name() == "Primer")
			{
				PartitionPrimer = new Primer;
				UInt32 ThisBytes = PartitionPrimer->ReadValue(BuffPtr, (UInt32)Len);
				Size -= ThisBytes;
				Bytes += ThisBytes;
				BuffPtr += ThisBytes;

				// Skip further processing for the primer
				continue;
			}
		}

		// Skip any filler items
		if(NewItem->Name() == "KLVFill")
		{
			Size -= Len;
			Bytes += Len;
			BuffPtr += Len;
			
			// Don't add the filler
			continue;
		}
		
		if(Len)
		{
			NewItem->SetParent(File, BytesAtItemStart + Location,(UInt32)( Bytes - BytesAtItemStart));

			UInt32 ThisBytes = NewItem->ReadValue(BuffPtr,(UInt32) Len, PartitionPrimer);

			Size -= ThisBytes;
			Bytes += ThisBytes;
			BuffPtr += ThisBytes;
		}

		AddMetadata(NewItem);
	}

	return Bytes + FillerBytes;
}


//! Read any index table segments from this partition's source file
MDObjectListPtr mxflib::Partition::ReadIndex(void)
{
	UInt64 IndexSize = GetInt64(IndexByteCount_UL);
	printf("indexSize= %ld\n", IndexSize);
	if(IndexSize == 0) return new MDObjectList;

	MXFFilePtr ParentFile = Object->GetParentFile();

	if(!ParentFile)
	{
		error("Call to Partition::ReadIndex() on a partition that is not read from a file\n");
		return new MDObjectList;
	}

	UInt64 MetadataSize = GetInt64(HeaderByteCount_UL);

	// Find the start of the index table
	// DRAGONS: not the most efficient way - we could store a pointer to the end of the metadata
	ParentFile->Seek(Object->GetLocation() + 16);
	Length Len = ParentFile->ReadBER();
	Position Location = ParentFile->Tell() + Len;

	ParentFile->Seek(Location);
	ULPtr FirstUL = ParentFile->ReadKey();
	if(!FirstUL)
	{
		error("Error reading first KLV after %s at 0x%s in %s\n", FullName().c_str(), 
			  Int64toHexString(GetLocation(),8).c_str(), GetSource().c_str());
		return new MDObjectList;
	}

	MDOTypePtr FirstType = MDOType::Find(FirstUL);
	if(FirstType->Name() == "KLVFill")
	{
		// Skip over the filler
		Len = ParentFile->ReadBER();
		Location = ParentFile->Tell() + Len;
	}

	// Move to the start of the index table segments
	ParentFile->Seek(Location + MetadataSize);

	return ReadIndex(ParentFile, IndexSize);
}

//! Read any index table segments from a file
MDObjectListPtr mxflib::Partition::ReadIndex(MXFFilePtr File, UInt64 Size)
{
	MDObjectListPtr Ret = new MDObjectList;
	printf("index size= %ld\n",Size);
	
	while(Size)
	{
		UInt64 Location = File->Tell();
		UInt64 Bytes=0;
		MDObjectPtr NewIndex = File->ReadObject(NULL);
		if(NewIndex)
		{
			if((NewIndex->Name() == "IndexTableSegment") || (NewIndex->Name() == "V10IndexTableSegment"))
			{
				Ret->push_back(NewIndex);
				Bytes = File->Tell() - Location;
			}
			else if( NewIndex->Name() == "KLVFill" )
			{
				// Skip over the filler
				Bytes = File->Tell() - Location;
			}
			else
			{
				error("Expected to find an IndexTableSegment - found %s at %s\n", 
					  NewIndex->FullName().c_str(), NewIndex->GetSourceLocation().c_str());
				break;
			}
		}
		else
		{
			error("Error reading IndexTableSegment at 0x%s in %s\n", 
				   Int64toHexString(Location,8).c_str(), File->Name.c_str());
				   
		}

		if(Bytes > Size) break;

		Size -= Bytes;
	}

	return Ret;
}


//! Read raw index table data from this partition's source file
DataChunkPtr mxflib::Partition::ReadIndexChunk(void)
{
	DataChunkPtr Ret;

	UInt64 IndexSize = GetInt64(IndexByteCount_UL);
	if(IndexSize == 0) return Ret;

	MXFFilePtr ParentFile = Object->GetParentFile();

	if(!ParentFile)
	{
		error("Call to Partition::ReadIndexChunk() on a partition that is not read from a file\n");
		return Ret;
	}

	UInt64 MetadataSize = GetInt64(HeaderByteCount_UL);

	// Find the start of the index table
	// DRAGONS: not the most efficient way - we could store a pointer to the end of the metadata
	ParentFile->Seek(Object->GetLocation() + 16);
	Length Len = ParentFile->ReadBER();
	Position Location = ParentFile->Tell() + Len;

	ParentFile->Seek(Location);
	ULPtr FirstUL = ParentFile->ReadKey();
	if(!FirstUL)
	{
		error("Error reading first KLV after %s at 0x%s in %s\n", FullName().c_str(), 
			  Int64toHexString(GetLocation(),8).c_str(), GetSource().c_str());
		return Ret;
	}

	MDOTypePtr FirstType = MDOType::Find(FirstUL);
	if(FirstType->Name() == "KLVFill")
	{
		// Skip over the filler
		Len = ParentFile->ReadBER();
		Location = ParentFile->Tell() + Len;
	}

	// Move to the start of the index table segments
	ParentFile->Seek(Location + MetadataSize);

	// Read the specified number of bytes
	Ret = ParentFile->Read(IndexSize);

	/* Remove any trailing filler */

	// Scan backwards from the end of the index data
	if(Ret->Size >= 16)
	{
		UInt32 Count = Ret->Size - 16;
		UInt8 *p = &Ret->Data[Count - 16];

		// Do the scan (slightly optimized)
		while(Count--)
		{
			if(*p == 0x06)
			{
				if(memcmp(p, KLVFill_UL.GetValue(), 16) == 0)
				{
					Ret->Resize((UInt32)(p - Ret->Data));
					break;
				}
			}
			p--;
		}
	}

	return Ret;
}



//! Locate start of Essence Container
/*! Moves the file pointer for the parent file to the start of the essence container in this partition
 *  \note If there is no essence in this partition the pointer will be moved to the start of the following partition (or the start of the RIP if this is the footer)
 */
bool mxflib::Partition::SeekEssence(void)
{
	MXFFilePtr File = Object->GetParentFile();
	if(!File) { error("Call to Partition::SeekEssence() on a non-file partition\n"); return false; }

	Position BodyLocation = 0;

	Length MetadataSize = GetInt64(HeaderByteCount_UL);
	Length IndexSize = GetInt64(IndexByteCount_UL);

	// Skip over Partition Pack (and any trailing filler)
	File->Seek( Object->GetLocation() + 16 );
	Length Len = File->ReadBER();
	BodyLocation = SkipFill( File->Tell() + Len );
	if(!BodyLocation) return false;

	// Skip over Metadata (and any trailing filler)
	BodyLocation += MetadataSize;

	// Skip over Index (and any trailing filler)
	BodyLocation += IndexSize;

	// Perform the seek
	File->Seek(BodyLocation);
    //baiqian : copy from skipFill, and modify it
    // return false,mean there are no essence data!

	// check in case we've hit the next Partition Pack
	ULPtr NextUL = File->ReadKey();
	File->Seek(BodyLocation);
	if(!NextUL) return false;
	
	// Is this a partition pack?
	if(IsPartitionKey(NextUL->GetValue()))
	{
		UInt8 byte14 = (NextUL->GetValue())[13];
		if( byte14 == 2 || byte14 == 3 || byte14 == 4 )	return false;
		// we've found a Partition Pack - end of Body -- DRAGONS:?? Not true!!
		
		if( byte14 == 0x11 )	return false;
		// we've found a RIP - end of Body
	}
	return true;
}



// Sequential access to the Elements of the Body

// goto start of body...set the member variables _BodyLocation, _NextBodyLocation
// DRAGONS: Need to document return value!!
bool mxflib::Partition::StartElements(UInt64 startPos)
{
	//baiqian mod: for seek
	_BodyLocation = 0;
	if(!Object->GetParentFile()) { error("Call to Partition::StartElements() on a non-file partition\n"); return false; }
	MXFFilePtr PF = Object->GetParentFile();
    UInt64 MetadataSize = GetInt64(HeaderByteCount_UL);
	UInt64 IndexSize = GetInt64(IndexByteCount_UL);

	// skip over Partition Pack (and any leading Fill on Header)
	PF->Seek( Object->GetLocation() + 16 );
	Length Len = PF->ReadBER();
	_NextBodyLocation = SkipFill( PF->Tell() + Len );
	if( !_NextBodyLocation ) return false;

	// skip over Metadata (and any leading Fill on Index)
	_NextBodyLocation = SkipFill( _NextBodyLocation + MetadataSize );
	if( !_NextBodyLocation ) return false;

	// skip over Index (and any leading Fill on Body)
	_NextBodyLocation = SkipFill( _NextBodyLocation + IndexSize );

	if( startPos >0)
	{
		//check key?
	   _NextBodyLocation+=startPos;
	}
	return _NextBodyLocation != 0;
}

// goto _NextBodyLocation
KLVObjectPtr mxflib::Partition::NextElement()
{
	_BodyLocation = _NextBodyLocation;

	// skip the present Object
	_NextBodyLocation = Skip( _NextBodyLocation );

	// skip any trailing KLVFill
	_NextBodyLocation = SkipFill( _NextBodyLocation );

	if(!Object->GetParentFile()) { error("Call to Partition::StartElements() on a non-file partition\n"); return NULL; }

	MXFFilePtr PF = Object->GetParentFile();

	// construct a new Object
	if( !_BodyLocation ) return 0;
	else
	{
		PF->Seek( _BodyLocation );
		
		KLVObjectPtr pObj = PF->ReadKLV();

		return pObj; 
	}
}
//baiqian add: NextElement is too slow for our's DCI Player,so do this:
KLVObjectPtr mxflib::Partition::NextElementEx(bool ifGetKlv)
{
	_BodyLocation = _NextBodyLocation;
	
	if(!Object->GetParentFile()) { error("Call to Partition::StartElements() on a non-file partition\n"); return NULL; }
	
	MXFFilePtr PF = Object->GetParentFile();
	
	// construct a new Object
	if( !_BodyLocation ) return 0;
	else
	{
		PF->Seek( _BodyLocation );

		while(1)
		{
		
			KLVObjectPtr pObj = PF->ReadKLV();
			if(!pObj) return 0;
			if(IsPartitionKey(pObj->GetUL()->GetValue())) return 0;
			// skip any trailing KLVFill
			MDOTypePtr FirstType = MDOType::Find(pObj->GetUL());
			if(FirstType && FirstType->Name() == "KLVFill")
			{
				// Skip over the KLVFill
				Length Len = PF->ReadBER();
				PF->Seek( PF->Tell() + Len );
				continue;	
			} 
			else
			{
				if(ifGetKlv)
					pObj->ReadDataEx();
				else
					pObj->ReadData();
				_NextBodyLocation = PF->Tell();
				return pObj; 
			}
		}
		
	}
}
KLVObjectPtr mxflib::Partition::NextElementEx2(std::vector<unsigned char*> bufs, UInt32 buflen, int& readSize, UInt32& lastLen)
{
	_BodyLocation = _NextBodyLocation;
	readSize = 0;
	if(!Object->GetParentFile()) { error("Call to Partition::StartElements() on a non-file partition\n"); return NULL; }
	
	MXFFilePtr PF = Object->GetParentFile();
	
	// construct a new Object
	if( !_BodyLocation ) return 0;
	else
	{
		PF->Seek( _BodyLocation );

		while(1)
		{
		
			KLVObjectPtr pObj = PF->ReadKLV();
			if(!pObj) return 0;
			if(IsPartitionKey(pObj->GetUL()->GetValue())) return 0;
			// skip any trailing KLVFill
			MDOTypePtr FirstType = MDOType::Find(pObj->GetUL());
			if(FirstType && FirstType->Name() == "KLVFill")
			{
				// Skip over the KLVFill
				Length Len = PF->ReadBER();
				PF->Seek( PF->Tell() + Len );
				continue;	
			}
			else
			{
                          // Initially plan to read all the bytes available
				Length BytesToRead = pObj->Source.OuterLength;
				// Limit to specified size if > 0 and if < available
				if(BytesToRead > bufs.size() *buflen)
				{
					pObj->ReadData();
				}
				else
				{
					// Seek to the start of the requested data
					pObj->Source.File->Seek(pObj->Source.Offset + pObj->Source.KLSize);
					Length leftBytes = BytesToRead;
					for(int i = 0; i < bufs.size() && leftBytes > 0; ++i)
					{
						
						Length Bytes = (Length)pObj->Source.File->Read(bufs[i], (leftBytes > buflen)?buflen:leftBytes);
						leftBytes -= Bytes;
						lastLen = (UInt32)Bytes;
						++readSize;
					}	
				}
				_NextBodyLocation = PF->Tell();
				return pObj; 
			}
		}
		
	}
}

KLVObjectPtr mxflib::Partition::NextElementEx_SMP(SMP_FrameBufferParam& param)
{
	_BodyLocation = _NextBodyLocation;
	if(!Object->GetParentFile()) { error("Call to Partition::StartElements() on a non-file partition\n"); return NULL; }
	
	MXFFilePtr PF = Object->GetParentFile();
	
	// construct a new Object
	if( !_BodyLocation ) return 0;
	else
	{
		PF->Seek( _BodyLocation );

		while(1)
		{
		
			KLVObjectPtr pObj = PF->ReadKLV();
			if(!pObj) return 0;
			if(IsPartitionKey(pObj->GetUL()->GetValue())) return 0;
			// skip any trailing KLVFill
			MDOTypePtr FirstType = MDOType::Find(pObj->GetUL());
			if(FirstType && FirstType->Name() == "KLVFill")
			{
				// Skip over the KLVFill
				Length Len = PF->ReadBER();
				PF->Seek( PF->Tell() + Len );
				continue;	
			}
			else
			{
                          // Initially plan to read all the bytes available
				Length BytesToRead = pObj->Source.OuterLength;
				// Limit to specified size if > 0 and if < available
				if(BytesToRead > param.length())
				{
					pObj->ReadData();
				}
				else
				{
					// Seek to the start of the requested data
					Length leftBytes = BytesToRead;
					Length optionLen = 0;
				      	pObj->Source.File->Seek(pObj->Source.Offset + pObj->Source.KLSize);
					if(param.encrypted)
					{
						
						unsigned char tempBuf[116];
						Length Bytes = (Length)pObj->Source.File->Read(tempBuf, sizeof(tempBuf));
						UInt8 *p = tempBuf;
						UInt8 *Prev_p = p;
						Length ItemLength = ReadBER(&p, (int)Bytes);
						Bytes -= (p - Prev_p);
						if((ItemLength != 16) || (Bytes < 16))
						{
							//error;
							return 0;
						}

						p += 16;
						Bytes -= 16;
						Prev_p = p;
						ItemLength = ReadBER(&p, (int)Bytes);
						Bytes -= (p - Prev_p);
						if((ItemLength != 8) || (Bytes < 8))
						{
							return 0;
						}
						Length PlaintextOffset = (Length)GetU64(p);
						// Update pointer and count
						p += 8;
						Bytes -= 8;


						// Read the BER length and move the pointer
						Prev_p = p;
						ItemLength = ReadBER(&p, (int)Bytes);
						Bytes -= (p - Prev_p);

						if((ItemLength != 16) || (Bytes < 16))
						{	
							return 0;
						}

						p += 16;
						Bytes -= 16;


						Prev_p = p;
						ItemLength = ReadBER(&p,(int) Bytes);
						Bytes -= (p - Prev_p);

						if((ItemLength <= 0) || (ItemLength >= 9) || (Bytes < ItemLength))
						{
								return 0;
						}

						// If not 8-bytes it can't be UInt64 - otherwise it may be BER or UInt64 so see if it is valid BER
						// This will fail for unimaginably large UInt64 coded SourceLengths
						UInt64 ValueLength=0;
						if((ItemLength != 8) || (*p == 0x87))
						{
							// Read the BER SourceLength
							Prev_p = p;
							ValueLength = ReadBER(&p, (int)Bytes);
							Bytes -= (p - Prev_p);
						}
						else
						{
							// Read the UInt64 SourceLength
							ValueLength = GetU64(p);
							p += 8;
							Bytes -= 8;
						}

						if((UInt64)PlaintextOffset > ValueLength)
						{
							return 0;
						}

						// Read and the BER length of the Encrypted Source Value
						Prev_p = p;
						Length ESVLength = ReadBER(&p, (int)Bytes);
						Bytes -= (p - Prev_p);

						// Check that we have enough left for the IV and Check Value
						const int EncryptionGranularity=16;
						const int EncryptionOverhead=32;
						if(Bytes < EncryptionGranularity)
						{
							return 0;
						}

						UInt8 IV[16];
						// Store the initialization value
						memcpy(IV, p, 16);
						// Store the check value
						memcpy(param.result.IV, &p[16], 16);
						Bytes -= 32;
						//get Check, and seek to plainoffset;
	                    pObj->Source.File->Seek(pObj->Source.Offset + pObj->Source.KLSize + sizeof(tempBuf) -Bytes );


						Length EncryptedLength = ((ValueLength-PlaintextOffset) + EncryptionGranularity) / EncryptionGranularity;
						//FIXME:
						if((ValueLength-PlaintextOffset)%EncryptionGranularity == 0)
						{
							EncryptedLength = (ValueLength-PlaintextOffset);
						}
						else
							EncryptedLength *= EncryptionGranularity;			
					
						EncryptedLength += PlaintextOffset;
			

					    //set param.result.secOffset;
						param.result.secOffset = (int)PlaintextOffset;
						param.result.plainLen = (int)ValueLength;

                        leftBytes -= (sizeof(tempBuf) -Bytes);
						optionLen = leftBytes - EncryptedLength;
						//debug: for all decrpted mxf and has a option data;
						//printf("optionLen= %ld, should be 12\n", optionLen);						
						//if(optionLen != 12)
						//	printf("optionlen error\n");						
						
						leftBytes -= optionLen;
						//leftBytes shall be multiple of 16 
						//debug:
						//if(leftBytes % 16 != 0)			
						//	printf("leftBytes error\n");
					}


					param.result.len = (int)leftBytes;
					for(int i = 0; i < param.frameBufs.size() && leftBytes > 0; ++i)
					{
						Length Bytes = (Length)pObj->Source.File->Read(param.frameBufs[i].data(), (leftBytes > param.frameBufs[i].validLen())?param.frameBufs[i].validLen():leftBytes);
						leftBytes -= Bytes;
					}	

					if(optionLen > 0)
					{
						//read it from file and discard it;
						unsigned char discardBuf[256];
						Length Bytes = (Length)pObj->Source.File->Read(discardBuf, optionLen);
					}
				}
				_NextBodyLocation = PF->Tell();
				return pObj; 
			}
		}
		
	}

}


// skip over a KLV packet
UInt64 mxflib::Partition::Skip( UInt64 start )
{
	if( !start ) return 0;

	MXFFilePtr PF = Object->GetParentFile();

	PF->Seek( start );
	ULPtr FirstUL = PF->ReadKey();
	if(!FirstUL) return 0;

	// do the skip
	Length Len = PF->ReadBER();
	PF->Seek( PF->Tell() + Len );

	UInt64 ret = PF->Tell();

	// check in case we've hit the next Partition Pack
	ULPtr NextUL = PF->ReadKey();
	if(!NextUL) return 0;

	// Is this a partition pack?
	if(IsPartitionKey(NextUL->GetValue())) return 0;

	return ret;
}

// skip over any KLVFill
// DRAGONS: does not iterate - only copes with single KLVFill
UInt64 mxflib::Partition::SkipFill( UInt64 start )
{
	if( !start ) return 0;

	MXFFilePtr PF = Object->GetParentFile();

	PF->Seek( start );
	ULPtr FirstUL = PF->ReadKey();
	if(!FirstUL) return 0;

	MDOTypePtr FirstType = MDOType::Find(FirstUL);
	if(FirstType && FirstType->Name() == "KLVFill")
	{
		// Skip over the KLVFill
		Length Len = PF->ReadBER();
		PF->Seek( PF->Tell() + Len );
	}
	else
	{
		// was not KLVFill, so stay where we are
		PF->Seek( start );
	}

	UInt64 ret = PF->Tell();

	// check in case we've hit the next Partition Pack
	ULPtr NextUL = PF->ReadKey();
	if(!NextUL) return 0;

	// Is this a partition pack?
	if(IsPartitionKey(NextUL->GetValue()))
	{
		UInt8 byte14 = (NextUL->GetValue())[13];
		if( byte14 == 2 || byte14 == 3 || byte14 == 4 )	return 0;
		// we've found a Partition Pack - end of Body -- DRAGONS:?? Not true!!

		if( byte14 == 0x11 )	return 0;
		// we've found a RIP - end of Body
	}

	return ret;
}


//! Parse the current metadata sets into higher-level sets
MetadataPtr Partition::ParseMetadata(void)
{
	MetadataPtr Ret;

	// Locate the preface
	MDObjectList::iterator it = TopLevelMetadata.begin();
	while(it != TopLevelMetadata.end())
	{
		// If we find the preface, parse it
		if((*it)->IsA(Preface_UL))
		{
			Ret = Metadata::Parse(*it);
			break;
		}

		it++;
	}

	// If we failed to find the preface (or to parse it) this will be NULL
	return Ret;
}


//! Determine if the partition object is currently set as complete
bool Partition::IsComplete(void)
{
	if(    Object->IsA(OpenCompleteHeader_UL)        || Object->IsA(ClosedCompleteHeader_UL) || Object->IsA(CompleteFooter_UL)
		|| Object->IsA(OpenCompleteBodyPartition_UL) || Object->IsA(ClosedCompleteBodyPartition_UL) ) return true;

	return false;
}

//! Determine if the partition object is currently set as closed
bool Partition::IsClosed(void)
{
	if(    Object->IsA(ClosedHeader_UL)        || Object->IsA(ClosedCompleteHeader_UL) 
		|| Object->IsA(Footer_UL)			   || Object->IsA(CompleteFooter_UL)
		|| Object->IsA(ClosedBodyPartition_UL) || Object->IsA(ClosedCompleteBodyPartition_UL) ) return true;

	return false;
}


