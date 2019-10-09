// This file contains declaration for the Vector class
#include "fvstypes.h"


#ifndef __VECTOR_HEADER__
#define __VECTOR_HEADER__
 

template <class itemType>
class Vector
{
	public:
		// constructor/destructor
		Vector(void);			// default constructor
		Vector(uint32_t newSize);	// contructor with specific size
		Vector(uint32_t newSize, itemType fillValue);	// specific size and value
		Vector(const Vector<itemType> & src);		// copy constructor
		~Vector(void);			// desctructor

		// indexing
		// allows assignment of value
		itemType & operator[] (uint32_t k);
		// allows only for retrival
		const itemType & operator[] (uint32_t k) const;

		// asignment operator
		const Vector<itemType> & operator= (const Vector<itemType> & src);

		// modifers
		void resize (uint32_t newSize);


		// accessor
		uint32_t size() const;		// returns size of vector 

	private:
		// holds size of vector
		uint32_t VectorSize;

		// array
		itemType * data;		
};

// include vector implementation
#include "vector.cpp"
#endif
