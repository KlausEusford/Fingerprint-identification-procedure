#ifndef __FVSEDGEDETECT_HEADER__
#define __FVSEDGEDETECT_HEADER__

// This value may be changed if needed, play around with it if you like
#define EDGETHRESHOLD 100

// Logarithmic form of edge dectection
FvsError_t FVS_EdgeDetect(FvsImage<FVS_IMAGE_TYPE> & img);

#endif // __FVSEDGEDETECT_HEADER__