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
#ifndef __MINUTIA_HEADER__
#define __MINUTIA_HEADER__
//#include "fvsimage.h"
#include "fvstypes.h"
#include "fvsimage.h"

// just a definition so you don't have to remember the values
typedef enum
{
	MINUTIA_ENDING = 0,
	MINUTIA_BRANCHING = 1,
	NO_MINUTIA = 100,
} MinutiaType_t;

// Minutia structure
// x, y are position values with respect to the image
// angle is the angle in radians (0 means to the right, pi/2 up, pi left, 3*pi/2 bottom)
// type is the type of minutia it is
typedef struct Minutia_t
{
	MinutiaType_t type;
	int32_t x;
	int32_t y;
	float_t angle;
} Minutia_t;

// FvsMinutiaSet Class
class FvsMinutiaSet
{
public:
	// contructors/destructor
	FvsMinutiaSet(void);						// default contructor
	FvsMinutiaSet(int32_t newsize);				// constructor with specific size
	FvsMinutiaSet(const FvsMinutiaSet & src);	// copy constructor
	~FvsMinutiaSet(void);						// Destructor

	// accessors
	int32_t Size() const;						// Max size of minutias in set
	int32_t Count() const;						// Current number of minutias in set

	// modifers 
	FvsError_t Empty(void);						// Clears entire set
												// Add minutia to set
	FvsError_t Add(int32_t x, int32_t y, MinutiaType_t type, float_t angle);
	FvsError_t Resize(int32_t newsize);			// resize set
												// Assignment operator 
	const FvsMinutiaSet & operator= (const FvsMinutiaSet & src);
	Minutia_t GetMinutia(int32_t index);			// Get minutia struct

private:
	int32_t nbminutia;							// minutia counter
	int32_t tablesize;							// max size of set
	Vector<Minutia_t> set;						// actual set
};

// include implementation
//#include "minutia.cpp"


#endif // __MINUTIA_HEADER__