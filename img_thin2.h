#ifndef __IMG_THIN2_HEADER__
#define __IMG_THIN2_HEADER__
#include "fvstypes.h"
#include "fvsimage.h"

template <class itemType>
FvsError_t ImageThin2(FvsImage<itemType> & img);

// include implementation
#include "img_thin2.cpp"

#endif
