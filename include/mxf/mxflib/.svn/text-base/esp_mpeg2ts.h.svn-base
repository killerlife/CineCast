/*! \file	esp_mpeg2ts.h
 *	\brief	Definition of class that handles parsing of MPEG-2 ts streams
 *
 *	\version $Id: esp_mpeg2ts.h,v 1.1.1.1 2007/04/03 02:08:42 baiqian Exp $
 *
 */

#ifndef MXFLIB__ESP_MPEG2TS_H
#define MXFLIB__ESP_MPEG2TS_H


#include <math.h>	


namespace mxflib
{
	//! Class that handles parsing of MPEG-2 ts streams
	class MPEG2_TS_EssenceSubParser : public EssenceSubParserBase
	{
	protected:
		Rational NativeEditRate;							//!< The native edit rate of this essence
		Rational SelectedEditRate;							//!< Selected edit rate of this essence

		Position PictureNumber;								//!< Current picture number
		Position CurrentPos;								//!< Current position in the input file
														

		bool EditPoint;										//!< Set true each time an edit point (sequence header of a closed GOP) and false for other frames
															/*!< \note This flag can be checked by calling SetOption("EditPoint") which will return the flag.
															 *         The flag tells only if the last frame is the start of a new sequence.
															 */
													       
		Position m_Duration;
		bool EndOfStream;									//!< True once the end of the stream has been read
		
		MDObjectParent CurrentDescriptor;					//!< Pointer to the last essence descriptor we built
															/*!< This is used as a quick-and-dirty check that we know how to process this source */


		int		m_packLen;
		UInt64  m_packNum;
		EncryptObject* m_pEncryptObject;
	public:
		//! Class for EssenceSource objects for parsing/sourcing MPEG-VES essence
		class ESP_EssenceSource : public EssenceSubParserBase::ESP_EssenceSource
		{
		protected:
			Position EssencePos;
			Position EssenceBytePos;
			Position ByteCount;
			Position Offset;
			EncryptObject* m_pEncryptObject;
			

		public:
			//! Construct and initialise for essence parsing/sourcing
			ESP_EssenceSource(EssenceSubParserPtr TheCaller, FileHandle InFile, UInt32 UseStream, UInt64 Count = 1)
				: EssenceSubParserBase::ESP_EssenceSource(TheCaller, InFile, UseStream, Count) 
			{
				MPEG2_TS_EssenceSubParser *pCaller = SmartPtr_Cast(Caller, MPEG2_TS_EssenceSubParser);
				EssencePos = pCaller->PictureNumber;
				EssenceBytePos = pCaller->CurrentPos;
				//baiqian add:
				m_pEncryptObject=NULL;
			};
			~ESP_EssenceSource()
			{
				if(m_pEncryptObject)
					delete m_pEncryptObject;
			}
			//! Get the size of the essence data in bytes
			/*! \note There is intentionally no support for an "unknown" response 
			 */
			virtual Length GetEssenceDataSize(void) 
			{
				Offset = 0;
				MPEG2_TS_EssenceSubParser *pCaller = SmartPtr_Cast(Caller, MPEG2_TS_EssenceSubParser);
				
				if(pCaller->SelectedWrapping->ThisWrapType == WrappingOption::Clip)
					ByteCount = pCaller->ReadInternal(File, Stream, 0);
				else
					ByteCount = pCaller->ReadInternal(File, Stream, RequestedCount);
				
				//return ByteCount;
				if( !m_pEncryptObject )
					return ByteCount;
				return m_pEncryptObject->getEncryptLen(ByteCount);
			};

			//! Get the next "installment" of essence data
			/*! \return Pointer to a data chunk holding the next data or a NULL pointer when no more remains
			 *	\note If there is more data to come but it is not currently available the return value will be a pointer to an empty data chunk
			 *	\note If Size = 0 the object will decide the size of the chunk to return
			 *	\note On no account will the returned chunk be larger than MaxSize (if MaxSize > 0)
			 */
			virtual DataChunkPtr GetEssenceData(UInt64 Size = 0, UInt64 MaxSize = 0)
			{
				// Allow us to differentiate the first call
				if(!Started)
				{
					Offset = 0;
					Started = true;
				}

				//return BaseGetEssenceData(Size, MaxSize);
				DataChunkPtr dataPtr = BaseGetEssenceData(Size, MaxSize);
				if(!m_pEncryptObject || !dataPtr)
					return dataPtr;
				return m_pEncryptObject->getEncryptData(dataPtr);
			}
			//! Is the last data read the start of an edit point?
			virtual bool IsEditPoint(void) 
			{
				MPEG2_TS_EssenceSubParser *pCaller = SmartPtr_Cast(Caller, MPEG2_TS_EssenceSubParser);
				return pCaller->EditPoint; 
			}
			virtual void SetEncrypt(Encrypt_Base *Enc,UUIDPtr ContextID, Length PlaintextOffset = 0)
			{
				if(m_pEncryptObject)
					delete m_pEncryptObject;
				m_pEncryptObject=new EncryptObject(Enc,ContextID,PlaintextOffset);
				MPEG2_TS_EssenceSubParser *pCaller = SmartPtr_Cast(Caller, MPEG2_TS_EssenceSubParser);

				pCaller->SetEncrypt(m_pEncryptObject);
			}
			
		};

		// Give our essence source class privilaged access
		friend class MPEG2_TS_EssenceSubParser::ESP_EssenceSource;

	public:
		// TODO: Check why properties are not initialised here!
		MPEG2_TS_EssenceSubParser()
		{
			EditPoint=false;
			EndOfStream = false;
			m_packLen=188;
			m_packNum=0;
			m_pEncryptObject=NULL;
		}

		//! Build a new parser of this type and return a pointer to it
		virtual EssenceSubParserPtr NewParser(void) const { return new MPEG2_TS_EssenceSubParser; }

		//! Report the extensions of files this sub-parser is likely to handle
		virtual StringList HandledExtensions(void);

		//! Examine the open file and return a list of essence descriptors
		virtual EssenceStreamDescriptorList IdentifyEssence(FileHandle InFile);

		//! Examine the open file and return the wrapping options known by this parser
		virtual WrappingOptionList IdentifyWrappingOptions(FileHandle InFile, EssenceStreamDescriptor &Descriptor);

		//! Set a wrapping option for future Read and Write calls
		virtual void Use(UInt32 Stream, WrappingOptionPtr &UseWrapping);

		//! Set a non-native edit rate
		virtual bool SetEditRate(Rational EditRate);

		//! Get the current edit rate
		virtual Rational GetEditRate(void) { return SelectedEditRate; }
		virtual Rational GetPreferredEditRate(void);
		//! Get BytesPerEditUnit, if Constant
		virtual UInt32 GetBytesPerEditUnit(UInt32 KAGSize = 1);
	
		//! Get the current position in SetEditRate() sized edit units
		virtual Position GetCurrentPosition(void);

		// Index table functions
		
		//! Set the IndexManager for this essence stream (and the stream ID if we are not the main stream)
		/*! Also force reordering to be used */
		virtual void SetIndexManager(IndexManagerPtr &TheManager, int StreamID = 0)
		{
			EssenceSubParserBase::SetIndexManager(TheManager, StreamID);
			TheManager->SetPosTableIndex(StreamID, -1);
		}

		//! Read a number of wrapping items from the specified stream and return them in a data chunk
		virtual DataChunkPtr Read(FileHandle InFile, UInt32 Stream, UInt64 Count = 1);

		//! Build an EssenceSource to read a number of wrapping items from the specified stream
		virtual EssenceSubParserBase::ESP_EssenceSource *GetEssenceSource(FileHandle InFile, UInt32 Stream, UInt64 Count = 1)
		{
			return new ESP_EssenceSource(this, InFile, Stream, Count/*, Index*/);
		};

		//! Write a number of wrapping items from the specified stream to an MXF file
		virtual Length Write(FileHandle InFile, UInt32 Stream, MXFFilePtr OutFile, UInt64 Count = 1/*, IndexTablePtr Index = NULL*/);
		
		virtual bool SetOriginAndDuration(FileHandle InFile,Position EditUnit,Position Duration);
		Length  CountPos(FileHandle InFile, Position Pos, Length Count, Position& picNumber);
		virtual UInt64 GetDuration();
		
		//! Set a parser specific option
		/*! \return true if the option was successfully set */
		virtual bool SetOption(std::string Option, Int64 Param = 0);

	protected:
		//! Read the sequence header at the specified position in an MPEG2 file to build an essence descriptor
		MDObjectPtr BuildMPEG2TsDescriptor(FileHandle InFile, UInt64 Start = 0);
		//! Scan the essence to calculate how many bytes to transfer for the given edit unit count
		Length ReadInternal(FileHandle InFile, UInt32 Stream, UInt64 Count);
	private:
		bool decidePacketLen(FileHandle InFile);
	public:
		void SetEncrypt(EncryptObject* pEncryptObject)
		{
			m_pEncryptObject=pEncryptObject;
		}
	};

}


#endif // MXFLIB__ESP_MPEG2VES_H





















