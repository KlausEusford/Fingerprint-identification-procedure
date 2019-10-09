#ifndef __FVSFILE_CPP__
#define __FVSFILE_CPP__

#include <stdio.h>
#include <string.h>
#include "fvstypes.h"
#include "fvsfile.h"

// Default constructor, set file pointer to NULL 
FvsFile::FvsFile(void)
{
	// Not needed 
	fp = NULL;
}

// Constructor that also opens a file
FvsFile::FvsFile(const String_t name, const uint32_t flags)
{
	fp = NULL;
	// call open function, it will take care of the rest
	Open(name, flags);
}

// Destructor
FvsFile::~FvsFile(void)
{
	// call close function it will take care of freeing file pointer
	Close();
}

// Open file for reading or writing
FvsError_t FvsFile::Open(const String_t name, const uint32_t flags)
{
	char strflags[10];

	// make sure file is not already open
	Close();

	// determine file flags needed
	strcpy(strflags,"");

	if (flags&FILE_READ && flags&FILE_WRITE) 
		strcat(strflags, "rw"); 
	else if (flags&FILE_READ)
		strcat(strflags, "r");
	else if (flags&FILE_WRITE)
		strcat(strflags, "w");

	
	// always open as binary file
	strcat(strflags,"b");

	// should we create file if it does not exist?
	if (flags&FILE_CREATE)
		strcat(strflags,"+");

	// open and set file pointer
	fp = fopen(name, strflags);

	// make sure that the file opened correctly
	if(IsOpen())
		return FvsOk;

	// the file did not open correctly return failure
	return FvsFailure;
}


// Close function, frees file pointer and allows class to be reused
FvsError_t FvsFile::Close(void)
{
	// make sure file is open before we close it
	if (IsOpen())
	{
		int nerr = fclose(fp);

		// set file pointer to NULL
		fp = NULL;

		// check if there was any problems
		if (nerr == 0)
			return FvsOk;
		else
			return FvsFailure;
	} 
	// File is not open
	return FvsFailure;
}

// Read from file
uint32_t FvsFile::Read(pointer_t data, uint32_t len)
{
	return fread(data, 1, len, fp);
}

// Write to file
uint32_t FvsFile::Write(pointer_t data, uint32_t len)
{
	return fwrite(data, 1, len, fp);
}

// Read only one byte
uint8_t FvsFile::GetByte()
{
	return fgetc(fp);
}

// Read two bytes (1 word)
uint16_t FvsFile::GetWord()
{
	uint16_t w = fgetc(fp);
	return (w<<8) + fgetc(fp);
}

// Checks if file pointer points to something
bool_t FvsFile::IsOpen(void) const
{
	return (fp != NULL)?TRUE:FALSE;
}

// Check if we have reached the end of the file
bool_t FvsFile::IsAtEOF(void) const 
{
	// make sure file is open first
	if (!IsOpen())
		return FALSE;
	return (feof(fp) != 0)?TRUE:FALSE;
}

// Commit change to file
FvsError_t FvsFile::Commit(void)
{
	return (fflush(fp) == 0)?FvsOk:FvsFailure;
}

// Move to beginning of file
FvsError_t FvsFile::SeekToBegin(void)
{
	// make sure file is open
	if (IsOpen())
	{
		fseek(fp, 0, SEEK_SET);
		return FvsOk;
	}
	
	// file not open, return failure
	return FvsFailure;
}

// Move to end of file
FvsError_t FvsFile::SeekToEnd(void)
{
	// make sure file is open
	if (IsOpen())
	{
		fseek(fp, 0, SEEK_END);
		return FvsOk;
	}

	// file not open, return failure
	return FvsFailure;
}

// Seek to specific location
FvsError_t FvsFile::Seek(uint32_t newpos)
{
	// make sure file is open
	if (IsOpen())
	{
		fseek(fp, newpos, SEEK_SET);
		return FvsOk;
	}

	// file not open, return failure
	return FvsFailure;
}

// Returns current position in file
uint32_t FvsFile::GetPosition(void) const
{
	// make sure file is open
	if (IsOpen())
		return ftell(fp);

	// file not open, return 0
	return 0;
}

#endif // __FVSFILE_CPP_