#include "fvstypes.h"
#include "fvsimage.h"
#include "fvs_img_morphology.h"

/* Use a structural operator to dilate the image 
**    1
**  1 1 1
**    1
*/
FvsError_t FVSImageDilate(FvsImage<FVS_IMAGE_TYPE> & img)
{
    uint32_t x,y;
    for (y=1; y<img.height()-1; y++)
    for (x=1; x<img.width()-1; x++)
    {
        if (img[x][y]==0xFF)
        {
            img[x-1][y] |= 0x80;
            img[x+1][y] |= 0x80;
            img[x][y-1] |= 0x80;
            img[x][y+1] |= 0x80;
        }
    }

    for (y=0; y<img.height(); y++)
	{
		for (x = 0; x<img.width(); x++)
		{
			if (img[x][y])
				img[x][y] = 0xFF;
		}
	}

    return FvsOk;
}

