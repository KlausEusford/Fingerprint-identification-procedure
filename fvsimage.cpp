// This file contains the implementation for FvsImage class


#ifndef __FVSIMAGE_IMPLEMENTATION__
#define __FVSIMAGE_IMPLEMENTATION__

#include <stdlib.h>
#include "fvsimage.h"
#include "vector.h"

// default constuctor creates a data matrix of 0x0
template <class itemType>
FvsImage<itemType>::FvsImage(void)
{
	FvsImage<itemType>::cols = 0;
	FvsImage<itemType>::rows = 0;
	FvsImage<itemType>::data = 0;
}

// constructor that makes data matrix a specfic width and height
template <class itemType>
FvsImage<itemType>::FvsImage(uint32_t width, uint32_t height)
{
	uint32_t i;

	// Change variables
	FvsImage<itemType>::rows = height;
	FvsImage<itemType>::cols = width;
	FvsImage<itemType>::data.resize(width);

	// resize each vector 
	for (i = 0; i < width; i++)
	{
		FvsImage<itemType>::data[i].resize(height);
	}
}

// constructor that makes matrix a spefic width and height and fills
// entire matrix with specific value
template <class itemType>
FvsImage<itemType>::FvsImage(uint32_t width, uint32_t height, itemType fillValue)
{
	uint32_t i, j;

	// changes variables
	FvsImage<itemType>::rows = height;
	FvsImage<itemType>::cols = width;
	FvsImage<itemType>::data.resize(width);

	// loop throught, resize and fill matrix
	for (i = 0; i < width; i++)
	{
		FvsImage<itemType>::data[i].resize(height);

		for (j = 0; j < width; j++)
		{
			FvsImage<itemType>::data[i][j] = fillValue;
		} // for j
	} // for i
}

// copy contructor
template <class itemType>
FvsImage<itemType>::FvsImage(const FvsImage<itemType> & src)
{
	uint32_t i, j;

	// Change variables 
	FvsImage<itemType>::rows = src.rows;
	FvsImage<itemType>::cols = src.cols;
	FvsImage<itemType>::data.resize(src.cols);

	// loop through and copy matrix
	for (i = 0; i < src.cols; i++)
	{
		// resize current row 
		FvsImage<itemType>::data[i].resize(src.rows);
		for (j = 0; j < src.rows; j++)
		{
			FvsImage<itemType>::data[i][j] = src.data[i][j];
		} // for j
	} // for i
}


// destructor, don't need to do anything vector destructor will take care
// of freeing up memory. This is here for expandablity. 
template <class itemType>
FvsImage<itemType>::~FvsImage(void)
{
}

// indexing with range checking
// this is used to retrieve imformation (heance the consts)
template <class itemType>
const Vector<itemType> & FvsImage<itemType>::operator [] (uint32_t k) const
{
	// make sure we aren't out of bounds
	if (k < 0 || k > cols)
	{
		printf("Out of bounds\n");
		exit(1);
		// **********************Error code here*************************
	} else {
		return data[k];
	}
}

// this allows for assignment
template <class itemType>
Vector<itemType> & FvsImage<itemType>::operator [] (uint32_t k) 
{
	// make sure we are not out of bounds
	if (k < 0 || k > cols)
	{
		printf("Out of bounds\n");
		exit(1);
		// **********************Error code here*************************	
	} else {
		return data[k];
	}
}

// assignment operator
template <class itemType>
const FvsImage<itemType> & FvsImage<itemType>::operator = (const FvsImage<itemType> & src)
{
	uint32_t i;

	// don't assign to self
	if (this != & src)
	{
		data.resize(src.cols); 
		rows = src.rows;
		cols = src.cols;

		for (i = 0; i < src.rows; i++)
		{
			data[i] = src.data[i];
		}
	}

	return *this; // returns copy not address reference
}

// resizes data member (resize vectors)
template <class itemType>
void FvsImage<itemType>::resize(uint32_t NewWidth, uint32_t NewHeight)
{
	uint32_t k;

	data.resize(NewWidth);

	// loop through and resize each vector 
	for (k = 0; k < NewWidth; k++)
	{
		data[k].resize(NewHeight);
	}

	cols = NewWidth;
	rows = NewHeight;
}


// returns width (cols) of current data matrix 
template <class itemType>
uint32_t FvsImage<itemType>::width() const
{
	return cols;
}

// returns height (rows) of current data matrix
template <class itemType>
uint32_t FvsImage<itemType>::height() const
{
	return rows;
}


#endif __FVSIMAGE_IMPLEMENTATION__