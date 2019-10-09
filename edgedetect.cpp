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
#ifndef __EDGEDETECT_CPP__
#define __EDGEDETECT_CPP__
#include "fvsimage.h"


template <class itemType>
FvsError_t EdgeDetect(FvsImage<itemType> & img)
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

#endif // __EDEGEDETECT_CPP__