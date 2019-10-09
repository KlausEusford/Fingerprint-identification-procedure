#include "fvsimage.h"
#include "fvstypes.h"
#include "fvsedgedetect.h"

FvsError_t FVS_EdgeDetect(FvsImage<FVS_IMAGE_TYPE> & img)
{
	uint32_t x, y;

	// loop through image pixel by pixel
	for (y = 0; y < img.height(); y++)
	{
		for (x = 0; x < img.width(); x++)
		{
			if (img[x][y] < EDGETHRESHOLD)
                img[x][y] = COLOR_LINE;
            else 
				img[x][y] = COLOR_BACKGROUND;

		}
	}
	return FvsOk;
}