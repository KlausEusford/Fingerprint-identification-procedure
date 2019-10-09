// This file contains the declartion for the FvsImage class, 
// a templated class that hold the image data for an image
// FvsImage uses the vector class as a helper to store the image data 
// in memory. Both the vector class and the FvsImage class are able to be
// resized during execution, allowing for more flexiblity.

#include "vector.h"
#include "fvstypes.h"

#ifndef __FVSIMAGE_HEADER__
#define __FVSIMAGE_HEADER__

template <class itemType>
class FvsImage
{
	public:
		// Constructors/destructor
		FvsImage(void);						// default constructor
		FvsImage(uint32_t width, uint32_t height);	// make to certain width and height
		FvsImage(uint32_t width, uint32_t height,		// make to ceratin width and height
					itemType fillValue);	// and fill all with fillValue
		FvsImage(const FvsImage<itemType> & src); // copy constructor
		~FvsImage(void);

		// indexing
		const Vector<itemType> & operator[] (uint32_t k) const; // allows accessing
		Vector<itemType> & operator[] (uint32_t k); // allows changing

		// assignment
		const FvsImage & operator= (const FvsImage & src);

		// modifers
		void resize(uint32_t NewWidth, uint32_t NewHeight); // used to resize vector 

		// accessors
		uint32_t width() const;				// returns width
		uint32_t height() const;			// returns height

	private:
		uint32_t rows;						// number of rows
		uint32_t cols;						// number of cols		
		Vector<Vector<itemType> > data;		// holds image data
};


// Include the implementation from the cpp file
#include "fvsimage.cpp"

// img[cols][rows]

#endif // __FVSIMAGE_HEADER__