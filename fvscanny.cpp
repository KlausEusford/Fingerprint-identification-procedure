// Taken from the canny.c file
// Robot Vision Group
// Dept. of Artifical Intelligence
// University of Edinburgh
//
// Authors: Bob Fisher, Dave Craft, A Fitzgibbion
// orginal date September 86
// program: canny.c
//
// purpose: to apply a simple symmetric canny operator to any size image
// 
// Revised September 2002
// By Shivang Patel
// Revisions made to work with Fingerprint Verification System (FVS)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "fvsimage.h"
#include "vector.h"
#include "fvstypes.h"


/*
double hypotenuse(double x, double y);
double gaussian(double x, double s);
FvsError_t canny_core(double stddev, FvsImage<FVS_IMAGE_TYPE> & img, Vector<unsigned char> & derivative_mag, 
					  Vector<unsigned char> & magnitude, Vector <unsigned char> & orientation);
int follow(int i, int j, int low, FvsImage<FVS_IMAGE_TYPE> & img, Vector<unsigned char> & magnitude, Vector<unsigned char> & orientation);
*/

// Scales angles in radians to fit within the 0-255 range of unsigned
// char variables
#define ORIENT_SCALE 40.0

// hypot can't cope when both it's arguments are zero, hence this hack...
double hypotenuse(double x, double y)
{
	if (x == 0.0 && y == 0.0) return 0.0;
	else return hypot(x,y);
}

// gaussian function (centered at the origin and ignoring the factor of 
// 1/(s * sqrt(2 *PI))) 
double gaussian (double x, double s)
{
	return (exp((-x*x)/(2*s*s)));
}

FvsError_t canny_core(double stddev, FvsImage<FVS_IMAGE_TYPE> & img, Vector<unsigned char> & derivative_mag, 
					  Vector<unsigned char> & magnitude, Vector <unsigned char> & orientation)
{
	int filter_width; // length of 1-D gaussian mask
	Vector<float> gsmooth_x, gsmooth_y;
	Vector<float> derivative_x, derivative_y;
	int i, j, k, n; // counter
	int t; // temp grad magnitude variable
	double a, b, c, d, g0; // mask generation intermediate vars
	double ux, uy;
	double t1, t2;
	double G[20], dG[20], D2G[20]; // gaussian & derivative filter masks
	double gc, gn, gs, gw, ge, gnw, gne, gsw, gse; 
	int jlimit, jstart;
	int ilimit;
	int jfactor;
	int kfactor1, kfactor2;
	int kfactor;
	int cindex, nindex, sindex, windex, eindex, nwindex, neindex, swindex, seindex;
	int low = 1, high = 255; // tracker hysteresis parameters
	int col_plus, col_minus; // col + 1 and col-1 respectiably
	int picsize;
	int mag_overflow_count = 0; // used to measure how often mag array overflows
	int man_underflow_count = 0; // used to measure how often mag array underflow

	picsize = img.width() * img.height();

	// calculate coefficents for 1-dimensional G, dG/dn and delta squared G filters
	for (n = 0; n < 20; n++)
	{
		a = gaussian(((double)n), stddev);
		if (a > 0.005 || n<2)
		{
			b = gaussian((double)n-0.5, stddev);
			c = gaussian((double)n+0.5, stddev);
			d = gaussian((double)n, stddev);

			G[n] = (a + b + c)/3/(6.283185*stddev*stddev);
			dG[n] = c - b;
			D2G[n] = 1.6 * d - a; // DOG
		}
		else break;
	} // for n

	filter_width = n;
	
	// allocate space for gaussian smoothing arrays
	gsmooth_x.resize(picsize);
	gsmooth_y.resize(picsize);

	// produce x and y convolutions with gaussian
	ilimit = img.width() - (filter_width - 1);
	jstart = img.width() - (filter_width - 1);
	jlimit = img.width() - (img.height() - (filter_width - 1));

	for (i = filter_width - 1; i < ilimit; ++i)
	{
		for (jfactor = jstart; jstart < jlimit; jfactor += img.width())
		{
			cindex = i + jfactor;
			t1 = img[i][cindex] * G[0];
			t2 = t1;

			for (k = 1, kfactor = cindex - img.width(), kfactor2 = cindex + img.width();
			k < filter_width; k++, kfactor1 -= img.width(), kfactor2 += img.width())
			{
				t1 += G[k] * (img[kfactor1 - (kfactor1/img.width())][kfactor1/img.width()] +
					img[kfactor2 - (kfactor2/img.width())][kfactor2/img.width()]);
				t2 += G[k] * (img[(cindex - k) - ((cindex - k)/img.width())][((cindex - k)/
					img.width())] + img[(cindex + k) - ((cindex + k)/img.width())][((cindex +k)/img.width())]);
			} // for k

			gsmooth_x[cindex] = t1;
			gsmooth_y[cindex] = t2;

		} // for jfactor
	} // for i

	// allocate space for gradient arrays 
	derivative_x.resize(picsize);
	derivative_y.resize(picsize);

	// produce x and y convolutions with derivative of gaussian
	for (i = filter_width - 1; i < ilimit; ++i)
	{
		for (jfactor = jstart; jfactor < jlimit; jfactor += img.width())
		{
			t1 = 0;
			cindex = i + jfactor;
			for (k = 1; k < filter_width; ++k)
				t1 += dG[k] * (gsmooth_x[cindex - k] - gsmooth_x[cindex + k]);
			derivative_x[cindex] = t1;
		} // for jfactor
	} // for i

	for (i = n; i < img.width() - n; ++i)
	{
		for (jfactor = jstart; jfactor < jlimit; jfactor += img.width())
		{
			t2 = 0; 
			cindex = i + jfactor;
			for (k = 1, kfactor = img.width(); k < filter_width; k++, kfactor += img.width())
				t2 += dG[k] * (gsmooth_y[cindex - kfactor] - gsmooth_y[cindex + kfactor]);
			derivative_y[cindex] = t2;
		} // for jfactor 
	} // for i

	// non-maximum suppression (4 cases for orientation of line of max slope)
	ilimit = img.width() - filter_width;
	jstart = img.width() - filter_width;
	jlimit = img.width() * (img.height() - filter_width);
	
	for (i = filter_width; i < ilimit; ++i)
	{
		for (jfactor = jstart; jfactor < jlimit; jfactor += img.width())
		{
			// calculate current indeces
			cindex = i + jfactor;
			nindex = cindex - img.width();
			sindex = cindex + img.width();
			windex = cindex - 1;
			eindex = cindex + 1;
			nwindex = nindex - 1;
			neindex = nindex + 1;
			swindex = sindex - 1;
			seindex = sindex + 1;
			ux = derivative_x[cindex];
			uy = derivative_y[cindex];
			gc = hypotenuse(ux, uy);
			
			nwindex = (nwindex<0)?0:nwindex;

			// scale fv to fit into an unsighed change arry
			t = gc * 20.0;
			
			derivative_mag[cindex] = (t < 256?t:255);
			gn = hypotenuse(derivative_x[nindex], derivative_y[nindex]);
			gs = hypotenuse(derivative_x[sindex], derivative_y[sindex]);
			gw = hypotenuse(derivative_x[windex], derivative_y[windex]);
			ge = hypotenuse(derivative_x[eindex], derivative_y[eindex]);
			gne = hypotenuse(derivative_x[neindex], derivative_y[neindex]);
			gnw = hypotenuse(derivative_x[nwindex], derivative_y[nwindex]);
			gsw = hypotenuse(derivative_x[swindex], derivative_y[swindex]);
			gse = hypotenuse(derivative_x[seindex], derivative_y[seindex]);

			if (ux * uy > 0)
			{
				if (fabs(ux) < fabs(uy))
				{
					if ((g0 = fabs(uy * gc)) < fabs(ux * gse + (uy - ux) * gs) || 
						g0 <= fabs(ux * gnw + (uy - ux)*gn))
						continue;
				} else {
					if ((g0 = fabs(ux * gc)) < fabs(uy * gse + (ux - uy) * gc) || 
						g0 <= fabs(uy * gnw + (ux - uy) * gw))
						continue;
				} 
			} else {
				if (fabs(ux) < fabs(uy))
				{
					if ((g0 = fabs(uy * gc)) < fabs(ux * gne - (uy + ux) * gn) || 
						g0 <= fabs(ux * gsw - (uy + ux) * gs))
						continue;
				} else {
					if ((g0 = fabs(ux * gc)) < fabs(uy * gne - (ux + uy) * gc) ||
						g0 <= fabs(uy * gsw - (ux + uy) * gw))
						continue;
				}
			}

			// seems to be a good scale factor
			magnitude [cindex] = derivative_mag[cindex];

			// pi * 40 ~= 128 - direction is (thought of as ) a sign byte
			orientation[cindex] = atan2(ux, uy) * ORIENT_SCALE;
		}
	}
	
	//SP: vector class destructor will free all memory when function ends
	return FvsOk;
}

// follow a connect edge 
int follow(int i, int j, int low, FvsImage<FVS_IMAGE_TYPE> & img, Vector<unsigned char> & magnitude, Vector<unsigned char> & orientation)
{
	int k, l; // counters
	int i_plus_1, i_minus_1, j_plus_1, j_minus_1;
	long index, kindex;
	char break_flag;

	i_plus_1 = i + 1;
	i_minus_1 = i - 1;
	j_plus_1 = j + 1;
	j_minus_1 = j - 1;
	index = i + j * img.width();

	if (j_plus_1 >= img.height()) j_plus_1 = img.height() - 1;
	if (j_minus_1 < 0) j_minus_1 = 0;
	if (i_plus_1 >= img.height()) i_plus_1 = img.height() - 1;
	if (i_minus_1 < 0) i_minus_1 = 0;

	if (!img[i][j])
	{
		// current point must be added to the list of tracked points
		img[i][j] = magnitude[j * img.width() + i];

		// now we check immediately adhacent points to see if they too 
		// could be added to the track
		break_flag = 0;
		for (k = i_minus_1; k <= i_plus_1; k++)
		{
			for (l = j_minus_1; l <= j_plus_1; ++l)
			{
				kindex = k+l * img.width();
				// SP: Threshold low value should be replaced by 1
				if (!(l == j && k == i) && magnitude[kindex] >= 1) 
				{
					if (follow(k, l, low, img,magnitude,orientation)) 
					{
						break_flag = 1;
						break;
					} // if follow
				} // if (!(l==j && k==i)
			} // for l
			if (break_flag) break;
		} // for k
		return 1;
	} 
	else return 0;
}

// Track the edges in the magnitude away, starting at points that exceed the "high"
// threshold and continuing to track as long as point values don't duck below the "low"
// threshold (yes it's hystercsis... I'm sure that hyster means "womb" (as in hysterical),
// but I don't know what its doin in a common engineering term)
void thresholding_tracker(int high, int low, FvsImage<FVS_IMAGE_TYPE> & img, Vector<unsigned char> & magnitude,
						  Vector<unsigned char> & orientation)
{
	int i, j, k; // counters

	// clear data array before tracking
/*	for (i = 0; i < img.height(); i++)
	{
		for (j = 0; j < img.width(); j++)
		{
			img[i][j] = 0;
		}
	}
*/
	// threshold image with hystersis: follow from each strong edge point
	for (i = 0; i < img.width(); i++)
	{
		for (j = 0; j < img.height(); j++)
		{
			if (magnitude[i + img.width() * j] >= high)
				follow(i, j, low, img, magnitude, orientation);
		}
	}
}


int FVSCannyEdgeDect(FvsImage<FVS_IMAGE_TYPE> & img, int low=1, int high=255)
{
	double s = 1.0; // standard deviation of gaussian
	Vector<unsigned char> derivative_mag; // mag of del G before non-maximum suppression
	Vector<unsigned char> magnitude, orientation; // mag & orintation agter non-max suppression

	// resize vectors
	derivative_mag.resize(img.width() * img.height());
	magnitude.resize(img.width() * img.height());
	orientation.resize(img.width() * img.height());

	// call canny core routines - these perform a gaussian smoothing, calculate a gradient
	// array, and suppress non-maximal points in this array
	canny_core(s, img, derivative_mag, magnitude, orientation);



	// here write orientation and magnitude to a file to see how they look

	

	// track edges 
	thresholding_tracker(high, low, img, magnitude, orientation);

	// to see the image output img to a file
	return 0;
}