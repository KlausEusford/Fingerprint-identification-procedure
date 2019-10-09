#ifndef __FMTBITMAP_HEADER__
#define __FMTBITMAP_HEADER__

#include <stdlib.h>
#include <stdio.h>
#include "fvstypes.h"
#include "fvsfile.h"
#include "fvsimage.h"

FvsError_t BitmapImport(FvsFile in, FvsImage<FVS_IMAGE_TYPE> & img);
FvsError_t BitmapExport(FvsFile out, FvsImage<FVS_IMAGE_TYPE> & img);


#endif // __FMTBITMAP_HEADER__

