#ifndef __IMG_THIN2_CPP__
#define __IMG_THIN2_CPP__
#include "fvstypes.h"
#include "fvsimage.h"
#include "img_thin2.h"

// defines to facilitate reading 
#define P1 img[x-1][y-1]
#define P2 img[x][y-1]
#define P3 img[x+1][y-1]
#define P4 img[x-1][y]
#define P5 img[x+1][y]
#define P6 img[x-1][y+1]
#define P7 img[x][y+1]
#define P8 img[x+1][y+1]

#define REMOVE_P_T2 {img[x][y] = COLOR_BACKGROUND; changed = TRUE;}

// Second thin algorithm, avaiable from http://www.dai.ed.ac.uk/HIPR2/thin.htm
// in this algorithm we loop through the image and at every line pixel we
// check the 8 surrounding pixels. If the 9 pixels together form any of the 
// structures below we remove the center/current pixel. We continue to loop
// through the image until looping results in no changes being made to the image.
//
// Structures to look for:
//	0 0 0		1   0		1 1 1		0   1		  0 0		  1			    1		0 0 
//	  1			1 1 0		  1			0 1 1		1 1 0		1 1 0		0 1 1		0 1 1
//	1 1 1		1   0		0 0 0		0	1		  1			  0 0		0 0			  1
//
// Once we make a pass through the image and no changes are made the image is thin. 
// We loop an additional 5 times to remove "spurs" in the image using pruning.
template <class itemType>
FvsError_t ImageThin2(FvsImage<itemType> & img)
{
	bool_t changed = TRUE;
	uint32_t x,y;
	
	while (changed == TRUE)
	{
		changed = FALSE;
		for (x = 1; x < img.width() - 1; x++)
		{
			for (y = 1; y < img.height() - 1; y++)
			{
				// check if current/center pixel is line pixel
				if (img[x][y] == COLOR_LINE)
				{
					// look for 8 structures
					if (!P1 && !P2 && !P3 && P6 && P7 && P8)
						REMOVE_P_T2;
					if (!P3 && !P5 && !P8 && P1 && P4 && P6)
						REMOVE_P_T2;
					if (!P6 && !P7 && !P8 && P1 && P2 && P2 && P3)
						REMOVE_P_T2;
					if (!P1 && !P4 && !P6 && P3 && P5 && P8)
						REMOVE_P_T2;
					if (!P2 && !P3 && !P5 && P4 && P7)
						REMOVE_P_T2;
					if (!P5 && !P8 && !P7 && P4 && P2)
						REMOVE_P_T2;
					if (!P4 && !P6 && !P7 && P2 && P5)
						REMOVE_P_T2;
					if (!P1 && !P2 && !P4 && P7 && P5)
						REMOVE_P_T2;
				} // if center pixel is line pixel
			} // for x
		} // for y
	} // while changed
	// Loop through and prune. Loop 5 times and remove spurs by looking if the current 
	// pixel and the 8 surrounding make up any of the structures below
	//
	// 1 1 1		  1			0 0 0		0 0 0		    0		0 0
	// 1 1 1		1 1 1		0 1 0		  1 0		0 1 0		0 1
	// 1 1 1		  1			0			  0 0		0 0 0		0 0 0
	//
	// 0 0 0		  0 0		0			0 0 0
	// 0 1 0		  1 0		0 1 0		0 1
	//     0		0 0 0		0 0 0		0 0
/*
	for (int i = 0; i < 4; i++)
	{
		for (x = 1; x < img.width() - 1; x++)
		{
			for (y = 1; y < img.height() - 1; y++)
			{
				// is it a line pixel
				if (img[x][y])
				{
					if (P1 && P2 && P3 && P4 && P5 && P6 && P7 && P8)
						REMOVE_P_T2;
					if (P2 && P4 && P5 && P7)
						REMOVE_P_T2;
					if (!P1 && !P2 && !P3 && !P4 && !P5 && !P6)
						REMOVE_P_T2;
					if (!P1 & !P2 && !P3 && !P5 && !P7 && !P8)
						REMOVE_P_T2;
					if (!P3 && !P4 && !P5 && !P6 && !P7 && !P8)
						REMOVE_P_T2;
					if (!P1 && !P2 && !P4 && !P6 && !P7 && !P8)
						REMOVE_P_T2;
					if (!P1 && !P2  & !P3 && !P4 && !P5 && !P8)
						REMOVE_P_T2;
					if (!P2 && !P3 && !P5 && !P6 && !P7 && !P8)
						REMOVE_P_T2;		
					if (!P1 && !P4 && !P5 && !P6 && !P7 && !P8)
						REMOVE_P_T2;
					if (!P1 && !P2 && !P3 && !P4 && !P6 && !P7)
						REMOVE_P_T2;
				} // if line pixel
			} // for y
		} // for x
	} // for i
*/
	return FvsOk;
}


#endif