#ifndef __FVSFILE_HEADER__
#define __FVSFILE_HEADER__

#include <stdio.h>
#include "fvstypes.h"


typedef enum FileOptions_t
{
    FILE_READ   = (1<<1),
    FILE_WRITE  = (1<<2),
    FILE_CREATE = (1<<3)
} FileOptions_t;


class FvsFile
{
	public: 
		// constructors/destructor
		// default constructor
		FvsFile(void); 
		// constructor that also opens files
		FvsFile(const String_t name, const uint32_t flags); 
		// destructor
		~FvsFile(void);


		// Open/close functions
		FvsError_t Open(const String_t name, const uint32_t flags);
		FvsError_t Close(void);

		// Read/Write functions
		uint32_t Read(pointer_t data, uint32_t len);
		uint32_t Write(pointer_t data, uint32_t len);

		// Read by byte or word
		uint8_t GetByte(void);
		uint16_t GetWord(void);

		// Test functions
		bool_t IsOpen(void) const;
		bool_t IsAtEOF(void) const;

		// Commit changes
		FvsError_t Commit(void);

		// Seek functions
		FvsError_t SeekToBegin(void);
		FvsError_t SeekToEnd(void);
		FvsError_t Seek(uint32_t newpos);

		// GetPosition function
		uint32_t GetPosition(void) const;

	//private:
		// File pointer
		FILE *fp;
};


#endif //__FVSFILE_HEADER__

