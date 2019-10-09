// This file contains the implementation for Vector class

#ifndef __VECTOR_CPP__
#define __VECTOR_CPP__

#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

// default constructor creates a vector array that is 0 in length
template <class itemType>
Vector<itemType>::Vector(void)
{
	VectorSize = 0;
	data = 0;
}

// constructor with specific size
template <class itemType>
Vector<itemType>::Vector(uint32_t newSize)
{
	VectorSize = newSize;
	// allocate memory for specific size
	data = new itemType[newSize];
}


// constructor with specific size and fill value, creates a vector array of the specific size
// and fills with fillValue 
template <class itemType>
Vector<itemType>::Vector(uint32_t newSize, itemType fillValue)
{
	uint32_t i;

	VectorSize = newSize;
	data = new itemType[newSize];

	// loop through and fill vector
	for (i = 0; i < newSize; i++)
	{
		data[i] = fillValue;
	}
}

// copy constructor
template <class itemType>
Vector<itemType>::Vector(const Vector<itemType> & src)
{
	uint32_t i;

	VectorSize = src.VectorSize;
	data = new itemType[VectorSize];

	// copy elements form source
	for (i = 0; i < VectorSize; i++)
	{
		data[i] = src.data[i];
	}
}

// destructor deallocates memory 
template <class itemType>
Vector<itemType>::~Vector(void)
{
	delete [] data;
}

// assignment operator 
template <class itemType>
const Vector<itemType> & Vector<itemType>::operator= (const Vector<itemType> & src)
{
	uint32_t i;
	// don't assign to self
	if (this != & src)
	{
		delete [] data;					// get rid of old storage
		VectorSize = src.VectorSize;
		data = new itemType[VectorSize];	// allocate new storage

		// copy values
		for (i = 0; i < VectorSize; i++)
		{
			data[i] = src.data[i];
		}
	}
	return *this; // copy not address
}


// indexing with bound checking
// allows for assignment
template <class itemType>
itemType & Vector<itemType>::operator [] (uint32_t k) 
{
	// make sure we do not go out of bounds
	if (k < 0 || k > VectorSize)
	{
		printf("Out of bounds\n");
		exit(1);
		//********************ERROR CODE*******************
		
	} else {
		return data[k];
	}
}


// allows for retrival
template <class itemType>
const itemType & Vector<itemType>::operator [] (uint32_t k) const
{
	if (k < 0 || k > VectorSize)
	{
		printf("Out of bounds\n");
		exit(1);
		//********************ERROR CODE*******************
	} else {
		return data[i];
	}
}


// used to resize vector array 
// NOTE: if newSize < VectorSize elements will
template <class itemType>
void Vector<itemType>::resize(uint32_t newSize)
{
	uint32_t k;
	uint32_t NumToCopy = (newSize < VectorSize)?newSize:VectorSize;

	// allocate new storage in temporary variable
	itemType * newList = new itemType[newSize];

	// copy elements
	for (k = 0; k < NumToCopy; k++)
	{
		newList[k] = data[k];
	}

	delete [] data;		// deallocate old storage
	VectorSize = newSize;
	data = newList;
}

// Returns size of vector, used so outside functions can not change size
// without using resize function
template <class itemType>
uint32_t Vector<itemType>::size() const
{
	return VectorSize;
}
#endif // __VECTOR_CPP__