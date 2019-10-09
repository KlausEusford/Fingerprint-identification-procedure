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

#ifndef __MATCHING_SHIFTING_CPP__
#define __MATCHING_SHIFTING_CPP__
#include "fvsimage.h"
#include "fvstypes.h"
#include "vector.h"
#include "matching_shifting.h"

typedef struct Shift_t
{
	uint32_t count;
	int32_t shiftval;
} Shift_t;

template <class itemType>
int AddShift(int32_t shiftval, Vector<Shift_t> & shiftstr, uint32_t & lastindex, itemType junk);

template <class itemType>
uint32_t CountMatching(FvsImage<itemType> & TemplateImg, FvsImage<itemType> & VerifyingImg, int32_t shiftx, int32_t shifty);

template <class itemType>
void ShiftImage(FvsImage<itemType> & img, int32_t shiftx, int32_t shifty);


// Matching using Shifting
// This matching algorithm shifts the verifying image left\right and
// up\down to find where the most line pixels line up (no pun intended).
// This algorithm works only when both images are the same scale and there
// is no rotation in either image.
// Think of the two images on a transpaceny, moving one on top of the other
// most, if not all, of the lines will match up. If they are not the same
// fingerprint they do not line up.
// 
// This function will return the percentage of the number of line pixels
// that line up over the total number of line pixels there are in the 
// template image. The calling function should then check this percentage
// if it is high enough then it is a correct match. If it is the same image 
// there should be no shift and 100% should be returned. 
//
// --SP
template <class itemType>
float_t Match_Shifting(FvsImage<itemType> & TemplateImg, FvsImage<itemType> & VerifyingImg)
{
	uint32_t linecount = 0;			// holds total number of line pixels in template image
	uint32_t linecount2 = 0;
	int32_t x, y;					// loop variables
	int32_t lastx, lasty;			
	Vector<Shift_t> shiftx, shifty; // vector/array of shift structs, holds the shift values
	uint32_t lastxshiftindex = 0;	// holds last index for vector above
	uint32_t lastyshiftindex = 0;	// same as above except for y axis vector
	int32_t finalxshift = 0;		// the final value we shift on the x-axis
	int32_t finalyshift = 0;		// same as above except for y-axis
	uint32_t lastcount = 0;
	uint32_t finalmatchcount = 0;	// how many line pixels line up after all shift is complet
	uint32_t temp;

	// first thing is make sure that both images are the same size otherwise return with 0
	if ((TemplateImg.width() != VerifyingImg.width()) ||
		(TemplateImg.height() != VerifyingImg.height()))
	{ 
		// Error message\output here
		return 0;
	}

	// find the total number of line pixels in template image, store in linecount variable
	for (x = 0; x < (int32_t)TemplateImg.width(); x++)
	{
		for (y = 0; y < (int32_t)TemplateImg.height(); y++)
		{
			if (TemplateImg[x][y])
				linecount++;
		} // for y
	} // for x

	for (x = 0; x < (int32_t)VerifyingImg.width(); x++)
	{
		for (y = 0; y < (int32_t)VerifyingImg.height(); y++)
		{
			if (VerifyingImg[x][y])
				linecount2++;
		}
	}

	if (lincount < linecount2) linecount = linecount2;

	// allocate enought memory for vectors
	shiftx.resize(2 * linecount);
	shifty.resize(2 * linecount);


	// If they are the exact same image, or they match really well without any shifting
	// don't bother with the extra work
	if (CountMatching(TemplateImg, VerifyingImg, 0, 0) == linecount)
		return 100.0;

	// now loop through image and find the shifts, first on the x-axis
	for (y = 0; y < (int32_t)TemplateImg.height(); y++)
	{
		for (x = 0; x < (int32_t)TemplateImg.width(); x++)
		{
			// see if the current pixel is a line pixel, otherwise continue on
			if (TemplateImg[x][y])
			{
				lastx = x;

				// move to the left and find first line pixel in verifying image
				while ((lastx >= 0) && (VerifyingImg[lastx][y] != COLOR_LINE))
					// we have not yet found a line pixel continue to look
					lastx--;

				// either we have found a line pixel or we have reached the end
				if (lastx >= 0)
					// we have found a line pixel add the difference between
					// the Temlate Image pixel and the Verifying Image pixel, 
					// this is the shift value
					AddShift(-(x - lastx), shiftx, lastxshiftindex, 0);



				// Now find the first line pixel going to the right
				lastx = x + 1;
				while ((lastx < (int32_t)VerifyingImg.width()) && (VerifyingImg[lastx][y] != COLOR_LINE))
					// we have not yet found a line pixel continue to look
					lastx++;

				// either we have found a line pixel or we have reached the end 
				if (lastx < (int32_t)VerifyingImg.width())
					// we have found a line pixel add the difference between 
					// the template image pixel and the verifying image pixel
					// this is the shift value
					AddShift((lastx - x), shiftx, lastxshiftindex, 0);
			} // if current pixel is line pixel
		} // for x
	} //for y


	// now find the shift on the y-axis
	for (x = 0; x < (int32_t)TemplateImg.width(); x++)
	{
		for (y = 0; y < (int32_t)TemplateImg.height(); y++)
		{
			// make sure the current pixel is a line piel
			if (TemplateImg[x][y])
			{
				lasty = y;
				
				// find the shift foing up
				while ((lasty >= 0) && (VerifyingImg[x][lasty] != COLOR_LINE))
					// we have not yet found a line pixel continue to look
					lasty--;

				// either we have found a line pixel or we have reached the end
				if (lasty >= 0)
					// we have found a line pixel add the difference between 
					// the template image pixel and the verifying image pixel
					// this is the shift value
					AddShift(-(y - lasty), shifty, lastyshiftindex, 0);

				// now find the shift going down
				lasty = y + 1;
				while ((lasty < (int32_t)VerifyingImg.height()) && (VerifyingImg[x][lasty] != COLOR_LINE))
					// we have not yet found a line pixel continue to look
					lasty++;

				// either we have found a line pixel or we have reached the end
				if (lasty < (int32_t)VerifyingImg.height())
					// we have found a line pixel add the difference between
					// the template image pixel and the verifying image pixel
					// this is the shift value
					AddShift((lasty - y), shifty, lastyshiftindex, 0);
			} // current pixel a line pixel ?
		} // for y
	} // for x


	// find best shift values
	for (x = 0; x < (int32_t)lastxshiftindex; x++)
	{
		if ((shiftx[x].shiftval != 0) && (shiftx[x].count > 20) &&
			((temp = CountMatching(TemplateImg, VerifyingImg, shiftx[x].shiftval, 0)) > lastcount))
		{
			finalxshift = shiftx[x].shiftval;
			lastcount = temp;
		}
	}

	
	// now find best shift on y-axis
	//lastcount = 0;
	for (y = 0; y < (int32_t)lastyshiftindex; y++)
	{
		if ((shifty[y].shiftval != 0) && (shifty[y].count > 20) && 
			((temp = CountMatching(TemplateImg, VerifyingImg, finalxshift, shifty[y].shiftval)) > lastcount))
		{
			finalyshift = shifty[y].shiftval;
			lastcount = temp;
		}
	}

	
	
	// now shift image according to the shift values we found above
	ShiftImage(VerifyingImg, finalxshift, finalyshift);
	// calculate how many line pixels match up
	finalmatchcount = CountMatching(TemplateImg, VerifyingImg, 0, 0);
		
	// print exact values, this is more for testing purposes
	printf("Shift Matching: Exact values: number matching: %d out of: %d\n", finalmatchcount, linecount);
	// return the % of line pixels that match up over the total number of line pixels in the template image
	return finalmatchcount * 100.0/linecount;
}



// this adds a shift value to the vector, but first makes sure that the value does
// not already exist. If the value does exist it increments the count value 
// of the shift value instead of adding the shift value
//
// Side note: the junk variable is there because I have to make the function a template function
// the reason is because Match_Shifting needs to be template class because it uses FvsImage class,
// and since I want to keep similar functions together in the same file, this function must be a
// template function. The same problems occurs for the BitmapWriteHeader() function in fmt_bitmap.cpp
// If anyone knows how to fix this please let me know 
// --SP
template <class itemType>
int AddShift(int32_t shiftval, Vector<Shift_t> & shiftstr, uint32_t & lastindex, itemType junk)
{
	for (uint32_t i = 0; i < lastindex; i++)
	{
		// see if the value already exists
		if (shiftstr[i].shiftval == shiftval)
		{
			shiftstr[i].count++;
			return 0; // 0 means nothing was added to the vector
		}
	} // for i

	// we have not yet exited so the value must not already exist in the vector
	// so add it. First make sure we don't go beyond the size of vector 
	if (lastindex >= shiftstr.size())
		// Add 10 slots to the vector so we don't take up too much memory
		shiftstr.resize(shiftstr.size() + 10);
	shiftstr[lastindex].shiftval = shiftval;
	shiftstr[lastindex].count = 0;
	lastindex++;
	return 1; // one item added to the vector	
}


// This function loops through the two images and finds how many line pixels match up 
// with the new shift values
template <class itemType>
uint32_t CountMatching(FvsImage<itemType> & TemplateImg, FvsImage<itemType> & VerifyingImg, int32_t shiftx, int32_t shifty)
{
	int32_t MatchesFound = 0;
	uint32_t x, y, startx, starty, endx, endy;

	startx = ((shiftx < 0)?0:shiftx);
	starty = ((shifty < 0)?0:shifty);
	endx = TemplateImg.width() + ((shiftx < 0)?shiftx:0);
	endy = TemplateImg.height() + ((shifty < 0)?shifty:0);

	for (x = startx; x < endx; x++)
	{
		for (y = starty; y < endy; y++)
		{
			// don't do anything unless it is a line pixel
			if (((x + shiftx) >= 0) && ((x + shiftx) < VerifyingImg.width()) &&
				((y + shifty) >= 0) && ((y + shifty) < VerifyingImg.height()) &&
				(TemplateImg[x][y]) && (VerifyingImg[x + shiftx][y + shifty]))
				MatchesFound++;
		} // for y
	} // for x

	return MatchesFound;
}


// Works better with a seperate function then in the code especially since 
// we are going to make a copy of the image, once the function exits the
// variables destory itself, freeing up memory. 
template <class itemType>
void ShiftImage(FvsImage<itemType> & img, int32_t shiftx, int32_t shifty)
{
	uint32_t x, y;
	FvsImage<itemType> imgcopy = img;

	for (x = 0; x < imgcopy.width(); x++)
	{
		for (y = 0; y < imgcopy.height(); y++)
		{
			// don't do anything unless it is a line pixel
			if (((x + shiftx) >= 0) && ((x + shiftx) < imgcopy.width()) &&
				((y + shifty) >= 0) && ((y + shifty) < imgcopy.height()))
				img[x][y] = imgcopy[x + shiftx][y + shifty];
			else
				img[x][y] = COLOR_BACKGROUND;
		} // for y
	} // for x
}

#endif // __MATCHING_SHIFTING_CPP__

