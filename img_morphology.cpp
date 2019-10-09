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
#ifndef __IMG_MORPHOLOGY_CPP__
#define __IMG_MORPHOLOGY_CPP__
#include "fvstypes.h"
#include "fvsimage.h"
#include "img_morphology.h"

/* Use a structural operator to dilate the image 
**    1
**  1 1 1
**    1
*/
template <class itemType>
FvsError_t ImageDilate(FvsImage<itemType> & img)
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



#endif // __IMG_MORPHOLOGY_CPP__