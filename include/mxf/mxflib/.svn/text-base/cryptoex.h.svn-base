#ifndef MXFLIB__CRYPTO_EXTEND_H
#define MXFLIB__CRYPTO_EXTEND_H

//baiqian 

// Forward refs
namespace mxflib
{
	
	class Encrypt_Base;
}

namespace mxflib
{
	
	class EncryptObject
	{
	private:
			Encrypt_Base*   m_Enc;
			UUIDPtr			m_encrptContextID;
			UInt64			m_encrptLen;
			int 			m_type;
			unsigned int  m_index;
	public:
			EncryptObject(Encrypt_Base *Enc,UUIDPtr ContextID, UInt64 encrptLen = 0, int type = 0);
			DataChunkPtr getEncryptData(DataChunkPtr& dataPtr);
			UInt64	getEncryptLen(UInt64 ValueLength);
			
	private:
			UInt64	_getEncryptLen(UInt64 ValueLength, UInt64 enLen);
		
					
	};
}
#endif


