/*########################################################################

  The contents of this file are subject to the Mozilla Public License
  Version 1.0(the "License");   You  may  NOT  use this file except in
  compliance with the License. You may obtain a copy of the License at
                http:// www.mozilla.org/MPL/
  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
  the License for the specific language governing rights and limitations
  under the License.

  The Initial Developer of the Original Code is Shivang Patel.

  Copyright(C) 2002. All Rights Reserved.

  Authors: Shivang Patel
           Jaap de Haan(jdh)
  
########################################################################*/
#ifndef __MINUTIA_CPP__
#define __MINUTIA_CPP__


#include "fvstypes.h"
#include "fvsimage.h"
#include "minutia.h"



// default constructor creates an empty set and assigns 0 to variables
FvsMinutiaSet::FvsMinutiaSet(void)
{
	nbminutia = 0;
	tablesize = 0;
	// set automatically initialized to 0 size when FvsMinutiaSet class is created
	// thanks to Vector class default contructor
}


// Constructor with a specific size
FvsMinutiaSet::FvsMinutiaSet(int32_t newsize)
{
	tablesize = newsize;
	nbminutia = 0;
	set.resize(newsize);
}

// copy constructor, copys from another FvsMinutiaSet class
FvsMinutiaSet::FvsMinutiaSet(const FvsMinutiaSet & src)
{
	// copy variables
	nbminutia = src.nbminutia;
	tablesize = src.tablesize;
	set.resize(tablesize);
	set = src.set;
}

// Default destructor, onlyed here if something needs to be done in the 
// future, otherwise variables destructors will take care of the rest
FvsMinutiaSet::~FvsMinutiaSet(void)
{
}

// Return the max size allowed by the set
int32_t FvsMinutiaSet::Size(void) const
{
	return tablesize;
}

// Return number of minutia currently in set
int32_t FvsMinutiaSet::Count(void) const
{
	return nbminutia;
}

// Clear set
FvsError_t FvsMinutiaSet::Empty(void) 
{
	nbminutia = 0;
	// resize to 0 then to back will clear all values
	set.resize(0);
	set.resize(tablesize);

	return FvsOk;
}

// Add minutia to set
FvsError_t FvsMinutiaSet::Add(int32_t x, int32_t y, MinutiaType_t type, float_t angle)
{

	// make sure there is room
	if (nbminutia < tablesize)
	{
		// add new values
		set[nbminutia].x = x;
		set[nbminutia].y = y;
		set[nbminutia].type = type;
		set[nbminutia].angle = angle;
		// increment counter
		nbminutia++;
	} else 
		return FvsMemory; // no rooom left
	return FvsOk;
}


// Resizes set
// NOTE: when resizing to a lesser size, items will be lost 
FvsError_t FvsMinutiaSet::Resize(int32_t newsize)
{
	tablesize = newsize;
	set.resize(newsize);
	if (newsize < nbminutia) nbminutia = newsize;

	return FvsOk;
}


// Assignment orperator works the same way as copy constructor
const FvsMinutiaSet & FvsMinutiaSet::operator= (const FvsMinutiaSet & src)
{
	tablesize = src.tablesize;
	nbminutia = src.nbminutia;
	set.resize(tablesize);
	set = src.set;
	return *this;
}

// Returns minutia struct 
Minutia_t FvsMinutiaSet::GetMinutia(int32_t index)
{
	return set[index];
}

#endif // __MINUTIA_CPP__