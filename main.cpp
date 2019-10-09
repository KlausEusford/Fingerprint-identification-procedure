#include <stdlib.h>
#include <stdio.h>
#include "fvsimage.h"
#include "fvsfile.h"
#include "fvstypes.h"
#include "fmt_bitmap.h"
#include "fvsedgedetect.h"
#include "fvs_img_morphology.h"
#include "fvscanny.h"

int main(void)
{
	FvsImage<FVS_IMAGE_TYPE> img;
	FvsFile in;
	FvsFile out;

	in.Open("img8.bmp", FILE_READ);
	BitmapImport(in, img);
	in.Close();

	FVSCannyEdgeDect(img, 1, 255);
	//FVSEdgeDetect(img);
	//FVSImageDilate(img);

	out.Open("edonly_img8.bmp", FILE_WRITE|FILE_CREATE);
	BitmapExport(out, img);
	out.Close();

	return 0;
}
