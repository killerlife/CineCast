//////////////////////////////////////////////////////////////////////////
//
#ifndef _UUIDLIB__MYUUID_H_
#define _UUIDLIB__MYUUID_H_

// Required headers for non-system specific bits
//#include <time.h>
/************************************************/
/*           (Hopefully) Common types           */
/************************************************/
/* Defined here so they can be used in the rest */
/* of this file if required                     */
/************************************************/

// _WIN32 platform specific File I/O, GUID and time functions
#ifdef _xxWIN32
#include <windows.h>		//!< Required for system specifics (such as CoCreateGuid, GUID)

#else
#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
#define uintptr_t long
#endif

#ifdef HAVE_UUID_GENERATE
#include <uuid/uuid.h>
#endif //HAVE_UUID_GENERATE

#endif//_WIN32

#include <time.h>
#include <stdlib.h>						// Required for integer conversions

#include <string>

class uuidlib
{
public:

#ifdef _xxWIN32
	/******** UUID Generation ********/
	static void MakeUUID(unsigned char *Buffer)
	{
		CoCreateGuid(reinterpret_cast<GUID*>(Buffer));
	}

#else//not _WIN32
// Support for all platforms with ISO C++ compilers using autoconf environment
// including the _WIN32 platform with Mingw, Cygwin or Intel compilers.
	/********* Acurate time *********/
	/******** UUID Generation ********/
#ifdef HAVE_UUID_GENERATE
	static void MakeUUID(unsigned char *Buffer)
	{
		uuid_t u;
		uuid_generate(u);
		memcpy(Buffer, &u, sizeof(u));
	}
#else // HAVE_UUID_GENERATE
	static  void MakeUUID(unsigned char *Buffer)
	{
		static bool Inited = false;
		if(!Inited)
		{
			// Attempt a reasonably random seed to prevent duplicate UUIDs
			// The time is normally good enough as a seed, except multiple processes may run this code at the
			// same time across all machines in use at any time.  The address of the buffer will depend on the
			// target platform and other processes running on the same machine.  The value of clock() will
			// depend on how much CPU time has elapsed since the program started and is shifted to reduce the
			// chance that a system using the same granularity for this and time() will simply give the
			// program start time. Why also include a random number in the seed? Because if someone has already
			// seeded the generator with a decent random number this will be taken into account to prevent
			// degrading the randomness
			srand((time(NULL)) ^ ((uintptr_t)Buffer) ^ (clock() << 2) ^ rand());
			Inited = true;
		}
		int i;
		for(i=0; i<16; i++)
		{
			Buffer[i] = (unsigned char)rand();
		}

		// Set reserved bits (variant "10" = ISO/IEC 11578)
		Buffer[8] &= 0x3f;
		Buffer[8] |= 0x80;

		// Set version bits (version "0100" = random or pseudo-random)
		Buffer[7] &= 0x0f;
		Buffer[7] |= 0x40;
	}
#endif // HAVE_UUID_GENERATE

#endif//_WIN32

	static  std::string GetString(unsigned char* Ident,bool bxml=true)

	{

		char Buffer[100];

		

		// Check which format should be used

		if( !(0x80&Ident[8]) )

		{	// Half-swapped UL packed into a UUID datatype

			// Print as compact SMPTE format [bbaa9988.ddcc.ffee.00010203.04050607]

			// Stored with upper/lower 8 bytes exchanged

			// Stored in the following 0-based index order: 88 99 aa bb cc dd ee ff 00 01 02 03 04 05 06 07

			sprintf (Buffer, "[%02x%02x%02x%02x.%02x%02x.%02x%02x.%02x%02x%02x%02x.%02x%02x%02x%02x]",

				Ident[8], Ident[9], Ident[10], Ident[11], Ident[12], Ident[13], Ident[14], Ident[15],

				Ident[0], Ident[1], Ident[2], Ident[3], Ident[4], Ident[5], Ident[6], Ident[7]

				);

		}

		else

		{	// UUID

			// Stored in the following 0-based index order: 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff

			// (i.e. network byte order)

			// Print as compact GUID format {00112233-4455-6677-8899-aabbccddeeff}

			if(bxml)

			{

				sprintf (Buffer, "urn:uuid:%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",

					Ident[0], Ident[1], Ident[2], Ident[3], Ident[4], Ident[5], Ident[6], Ident[7],

					Ident[8], Ident[9], Ident[10], Ident[11], Ident[12], Ident[13], Ident[14], Ident[15]

					);

			}

			else

			{

				sprintf (Buffer, "{%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x}",

					Ident[0], Ident[1], Ident[2], Ident[3], Ident[4], Ident[5], Ident[6], Ident[7],

					Ident[8], Ident[9], Ident[10], Ident[11], Ident[12], Ident[13], Ident[14], Ident[15]

					);

			}

		}

		

		return std::string(Buffer);

	}

	

	static std::string GetUUID()

	{

		unsigned char pBuffer[16];

		MakeUUID(pBuffer);

		return GetString(pBuffer);

	}



};



#endif // _UUIDLIB__MYUUID_H_



