#include <stdlib.h>
#include <stdio.h>
// memory.h only for win32
#include <memory.h>
#include "fvstypes.h"
#include "fvsfile.h"
#include "fvsimage.h"
#include "fmt_bitmap.h"

// Definition for Bitmap header
typedef struct BitmapFileHeader_t
{
	uint16_t	bfType;
	uint32_t	bfSize;
	uint16_t	bfReserved1;
	uint16_t	bfReserved2;
	uint32_t	bfOffBits;
	uint32_t	biSize;
	int32_t		biWidth;
	int32_t		biHeight;
	uint16_t	biPlanes;
	uint16_t	biBitCount;
	uint32_t	biCompression;
	uint32_t	biSizeImage;
	int32_t		biXPelsPerMeter;
	int32_t		biYPelsPerMeter;
	uint32_t	biClrUsed;
	uint32_t	biClrImportant;
} BitmapFileHeader_t;

#define TYPE_BM	0x4d42

// import functions
FvsError_t Import32BMP			(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & out, FVS_IMAGE_TYPE* pal);
FvsError_t Import24BMP			(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & out, FVS_IMAGE_TYPE* pal);
FvsError_t Import16BMP			(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & out, FVS_IMAGE_TYPE* pal);
FvsError_t Import8BMP			(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & out, FVS_IMAGE_TYPE* pal);
FvsError_t Import4RLE			(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & out, FVS_IMAGE_TYPE* pal);
FvsError_t Import8RLE			(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & out, FVS_IMAGE_TYPE* pal);
FvsError_t BitmapReadPalette	(FvsFile & in, FVS_IMAGE_TYPE* pal, BitmapFileHeader_t* bmpheader);
FvsError_t BitmapReadHeader		(FvsFile & in, BitmapFileHeader_t* bmpheader);

// export functions
FvsError_t Export8BMP			(FvsFile & out, FvsImage<FVS_IMAGE_TYPE> & img);
FvsError_t BitmapWriteHeader	(FvsFile & out, BitmapFileHeader_t* bmpheader);
FvsError_t BitmapWritePalette	(FvsFile & out);

// Reads bitmap file and stores in FvsImage object
FvsError_t BitmapImport(FvsFile in, FvsImage<FVS_IMAGE_TYPE> & img)
{
	FvsError_t nRet = FvsOk;
	BitmapFileHeader_t bmpheader;
	FVS_IMAGE_TYPE pal[256];

	// Make sure file is open
	if (in.IsOpen() == TRUE)
	{
		// move to beginning of file
		in.SeekToBegin();

		// Read bitmapheader
		nRet = BitmapReadHeader(in, &bmpheader);
		// make sure there was no problems reading the header
		if (nRet == FvsOk)
		{
			// make sure its bitmap if the first two bytes are not BM or if 
			// the header info is not 40 bytes, its not a bitmap
			if (bmpheader.bfType != TYPE_BM || bmpheader.biSize != 40)
				nRet = FvsBadFormat;
			else
			{
				// Read palette from file
				nRet = BitmapReadPalette(in, pal, &bmpheader);
				if (nRet == FvsOk)
				{
					// jump to the bitmap bits
					nRet = in.Seek(bmpheader.bfOffBits);
					if (nRet == FvsOk)
					{
						img.resize(bmpheader.biWidth, bmpheader.biHeight);		
						// determine how to read the image according to the 
						// format it is saved in
						switch (bmpheader.biBitCount)
						{
						case 4:
							if (bmpheader.biCompression == 0)
								nRet = Import4RLE(in, img, pal);
							else
								nRet = Import4RLE(in, img, pal);
							break;
						case 8:
							if (bmpheader.biCompression == 0)
								nRet = Import8BMP(in, img, pal);
							else
								nRet = Import8RLE(in, img, pal);
							break;
						case 16: nRet = Import16BMP(in, img, pal); break;
						case 24: nRet = Import24BMP(in, img, pal); break;
						case 32: nRet = Import32BMP(in, img, pal); break;

						case 1:
						default:
							nRet = FvsBadFormat;
							break;
						} // switch
					} // File seek
				} // ReadPalette
			else
				nRet = FvsBadFormat;
			} // if correct bitmap file
		} // if ReadHeader
	else
		nRet = FvsIoError; // file was not open before function was called
	} // if IsOpen
	else
		nRet = FvsBadParameter;
	return nRet;
} // BitmapImport

// Writes to a file, represented by FvsFile object which should have alread been set before
// the function was called. The image should have been stored in the FvsImage object.
// This function will always export a 8 bit (256 colors) bitmap file
FvsError_t BitmapExport(FvsFile out, FvsImage<FVS_IMAGE_TYPE> & img)
{
	uint32_t len;
	FvsError_t nRet = FvsOk;
	BitmapFileHeader_t bmpheader;


	// make sure file is open
	if (out.IsOpen() == TRUE)
	{
		// move to beginning of file
		out.SeekToBegin();
		len = sizeof(bmpheader);
		memset(&bmpheader, 0, len);

		// Setup bmpheader
		bmpheader.bfType			= TYPE_BM;
		bmpheader.biSize			= 40;
		bmpheader.biWidth			= img.width();
		bmpheader.biHeight			= img.height();
		bmpheader.biPlanes			= 1;
		bmpheader.biBitCount		= 8;
		bmpheader.biClrImportant	= 256;
		bmpheader.biClrUsed			= 256;
		bmpheader.biSizeImage		= img.width() * img.height();

		// write header info to file
		nRet = BitmapWriteHeader(out, &bmpheader);

		if (nRet == FvsOk)
		{
			// write palette
			nRet = BitmapWritePalette(out);
			if (nRet == FvsOk)
			{
				// Get what byte we are at this is the size of the header
				// info. That information needs to be part of the header information
				bmpheader.bfOffBits = out.GetPosition();

				// wrete image to file
				nRet = Export8BMP(out, img);

				// This is where the file ends (EOF) this also needs to be part of the header
				bmpheader.bfSize = out.GetPosition();

				if (nRet == FvsOk)
				{
					// move to beginning of file
					out.SeekToBegin();
					// rewrite header so there's no discrepency 
					nRet = BitmapWriteHeader(out, &bmpheader);
				} // if get total size
			} // if write palette 

		} // if if write header
		else
			nRet = FvsIoError;
	} // if file is open
	else
		nRet = FvsBadParameter;
	return nRet;
}

// import a 32 bit image
FvsError_t Import32BMP(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & img, FVS_IMAGE_TYPE* pal)
{
	FvsError_t nRet = FvsOk;
	FVS_IMAGE_TYPE r, g, b;
	uint32_t y, x;

	for (y = 0; y < img.height(); y++)
	{
		for (x = 0; x < img.width(); x++)
		{
			r = in.GetByte();
			g = in.GetByte();
			b = in.GetByte();
			in.GetByte(); // skip
			// write to FvsImage object in 8 bit format
			img[x][y] = pal[((r+(g<<1)+b)>>2)&0xFF];
		}
	}
	return nRet;
}

// import 24 bit image
FvsError_t Import24BMP(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & img, FVS_IMAGE_TYPE* pal)
{
	FvsError_t nRet = FvsOk;
	FVS_IMAGE_TYPE r, g, b;
	uint32_t x, y;

	for (y = 0; y < img.height(); y++)
	{
		for (x = 0; x < img.width(); x++)
		{
			r = in.GetByte();
			g = in.GetByte();
			b = in.GetByte();
			img[x][y] = pal[((r + (g << 1) + b) >> 2) & 0xFF];
		}
	}
	return nRet;
}

// import 16 bit image
FvsError_t Import16BMP(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & img, FVS_IMAGE_TYPE* pal)
{
	FvsError_t nRet = FvsOk;
	uint16_t w;
	uint32_t x, y;

	for (y = 0; y < img.height(); y++)
	{
		for (x = 0; x < img.width(); x++)
		{
			w = in.GetWord();
			img[x][y] = pal[(((w&0xF800)>>11)+((w&0x07E0)>>5)+(w&0x001F))&0xFF];

		}
	}
	return nRet;
}

// import 8 bit image
FvsError_t Import8BMP(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & img, FVS_IMAGE_TYPE* pal)
{
	FvsError_t nRet = FvsOk;
	FVS_IMAGE_TYPE b;
	uint32_t x, y;

	for (y = 0; y < img.height(); y++)
	{
		for (x = 0; x < img.width(); x++)
		{
			b = in.GetByte();
			img[x][y] = b;
		}
	}
	
	return nRet;
}


// import 4 bit image
FvsError_t Import4BMP(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & img, FVS_IMAGE_TYPE* pal)
{
	FvsError_t nRet = FvsOk;
	FVS_IMAGE_TYPE b;
	uint32_t x, y;

	for (y = 0; y < img.height(); y++)
	{ 
		for (x = 0; x < img.width(); x+=2)
		{
			b = in.GetByte();
			img[x][y] = pal[(b>>4)&0x0F];
			img[x + 1][y] = pal[b&0xF];
		}
	}
	
	return nRet;
}


// import 4 bit RLE compressed image
FvsError_t Import4RLE(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & img, FVS_IMAGE_TYPE* pal)
{
	FvsError_t nRet = FvsOk;
	uint32_t nMax = img.width() * img.height();
	bool_t bEOL = FALSE;
	bool_t bEOF = FALSE;
	uint8_t nLen;
	uint32_t y = 0;
	uint32_t j = 0;

	for (y = 0; (y < img.height()) && !bEOF; y++)
	{
		bEOL = FALSE;

		while (!bEOL && !bEOF && !in.IsAtEOF() && nMax > 0)
		{
			nLen = in.GetByte();
			if (nLen == 0) 
			{
				nLen = in.GetByte();
				switch (nLen)
				{
				case 1: // end of file escape (0x00 0x01)
				case 2: // treate seldom used delta escape the same way...
					bEOF = TRUE;
					// fall through
				case 0: // end of line escape (0x00 0x00)
					bEOL = TRUE;
					break;
				default:
					nMax -= nLen;

					if (nMax >= 0)
					{
						int i; 

						// literal
						for (i = 0; i<nLen;)
						{
							uint8_t b = in.GetByte();
							img[j - (j/img.width())][j/img.width()] = pal[b>>4];
							j++;
							i++;
							if (i < nLen)
							{
								img[j - (j/img.width())][j/img.width()] = pal[b&0xF];
								j++;
								i++;
							}
						}
						// word alignment at end of literal packet
						if (nLen & 1)
							in.GetByte();
					}
					break;
				}
			}
			else
			{
				nMax -= nLen;
				if (nMax >= 0)
				{
					// RunLength pixels, all with the same value
					int val = pal[in.GetByte()];
					// keep j from where it left off and file image 
					// with same value
					for (;j<(j + nLen); j++)
					{
						img[j - (j/img.width())][j/img.width()] = val;
					}
				}
			}
		}
	}
	return nRet;
}

// import 8 bit RLE comporess image
FvsError_t Import8RLE(FvsFile & in, FvsImage<FVS_IMAGE_TYPE> & img, FVS_IMAGE_TYPE* pal)
{
	FvsError_t nRet = FvsOk;
	bool_t bEOL = FALSE;
	bool_t bEOF = FALSE;
	uint8_t nLen;
	int32_t nMax = img.width() * img.height();
	uint8_t val = 0;
	uint32_t y = 0, i, j = 0;

	for (y = 0; (y < img.height()) && bEOF; y++)
	{
		bEOL = FALSE;

		// start of line
		while (!bEOL && !bEOF && !in.IsAtEOF() && nMax > 0)
		{
			nLen = in.GetByte();
			if (nLen == 0)
			{
				nLen = in.GetByte();
				switch (nLen)
				{
				case 1: // End of file escape (0x00 0x01)
				case 2: // treate seldon used delta escape the same way...
					bEOF = TRUE;
					// fallthrough
				case 0: // end of line escape (0x00 0x00)
					bEOL = TRUE;
					break;
				default:
					nMax -= nLen;
					if (nMax >= 0)
					{
						// literal 
						for (i = 0; i < nLen; i++)
						{
							img[j - (j/img.width())][j/img.width()] = pal[in.GetByte()];
							j++;
						}

						// word alignment at end of literal packet
						if (nLen & 1)
							in.GetByte();

					}
					break;
				}
			}
			else
			{
				nMax -= nLen;
				if (nMax >= 0)
				{
					// RunLength pixels, all with the same value
					val = pal[in.GetByte()];
					for (;j < (j + nLen); j++)
					{
						img[j - (j/img.width())][j/img.width()] = val;
						j++;
					}
				}
			}
		}
	}

	return nRet;
}


// Read in the palette, in order to do this correctly for all formats, we need some 
// information from the bitmap header
FvsError_t BitmapReadPalette(FvsFile & in, FVS_IMAGE_TYPE* pal, BitmapFileHeader_t* bmpheader)
{
	FvsError_t nRet = FvsOk;

	uint32_t nColors = 0;
	uint32_t i;

	switch (bmpheader->biBitCount)
	{
	case 1:	case 4:	case 8:
		nColors = (bmpheader->biClrUsed != 0)? 
			bmpheader->biClrUsed:
			(1<<bmpheader->biBitCount);
		break;
	case 16:	case 24:	case 32:
		// default palette
		for (i = 0; i < 256; i++)
			pal[i] = (FVS_IMAGE_TYPE)i;
		break;
	default:
		nRet = FvsBadFormat;
	}

	if (nColors != 0)
	{ 
		uint32_t _pal[256];
		uint32_t nBytes = in.Read(_pal, nColors*4);
		if (nBytes ==nColors*4)
		{
			for (i = 0; i < nColors; i++)
			{
				int32_t r = (_pal[i])&0xFF;
				int32_t g = (_pal[i]>>8)&0xFF;
				int32_t b = (_pal[i]>>16)&0xFF;
				pal[i] = ((r+(g<<1)+b)>>2)&0xFF;
			} 
		} else
			nRet = FvsBadFormat;
	} 
	return nRet;
}


// adapter to read in the header without complicated packing options
// and relying on memory alightment. 
FvsError_t BitmapReadHeader(FvsFile & in, BitmapFileHeader_t* bmpheader)
{
	if (in.Read(&bmpheader->bfType, 2) != 2)
		return FvsIoError;
	if (in.Read(&bmpheader->bfSize, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->bfReserved1, 2) != 2)
		return FvsIoError;
	if (in.Read(&bmpheader->bfReserved2, 2) != 2)
		return FvsIoError;
	if (in.Read(&bmpheader->bfOffBits, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->biSize, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->biWidth, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->biHeight, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->biPlanes, 2) != 2)
		return FvsIoError;
	if (in.Read(&bmpheader->biBitCount, 2) != 2)
		return FvsIoError;
	if (in.Read(&bmpheader->biCompression, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->biSizeImage, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->biXPelsPerMeter, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->biYPelsPerMeter, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->biClrUsed, 4) != 4)
		return FvsIoError;
	if (in.Read(&bmpheader->biClrImportant, 4) != 4)
		return FvsIoError;
	return FvsOk;
}

// Export 8 bitmap image
FvsError_t Export8BMP(FvsFile & out, FvsImage<FVS_IMAGE_TYPE> & img)
{
	uint32_t x, y;

	for (y = 0; y < img.height(); y++)
	{
		for (x = 0; x < img.width(); x++)
		{
			if (out.Write(&img[x][y], 1) != 1) 
				return FvsIoError;
		}
	}
	return FvsOk;
}

// Adpater function to write out the header
FvsError_t BitmapWriteHeader(FvsFile & out, BitmapFileHeader_t* bmpheader)
{
	if (out.Write(&bmpheader->bfType, 2) != 2)
		return FvsIoError;
	if (out.Write(&bmpheader->bfSize, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->bfReserved1, 2) != 2)
		return FvsIoError;
	if (out.Write(&bmpheader->bfReserved2, 2) != 2)
		return FvsIoError;
	if (out.Write(&bmpheader->bfOffBits, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->biSize, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->biWidth, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->biHeight, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->biPlanes, 2) != 2)
		return FvsIoError;
	if (out.Write(&bmpheader->biBitCount, 2) != 2)
		return FvsIoError;
	if (out.Write(&bmpheader->biCompression, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->biSizeImage, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->biXPelsPerMeter, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->biYPelsPerMeter, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->biClrUsed, 4) != 4)
		return FvsIoError;
	if (out.Write(&bmpheader->biClrImportant, 4) != 4)
		return FvsIoError;

	out.Write(&bmpheader->bfType, 2);
	return FvsOk;
}


// Write palette into the file
FvsError_t BitmapWritePalette(FvsFile & out)
{
	FvsError_t nRet = FvsOk;

	uint32_t nBytes;
	uint32_t nColors = 256;
	uint32_t i;
	FVS_IMAGE_TYPE _pal[256];

	for (i = 0; i<nColors; i++)
		_pal[i] = (( (i<<8) + i) << 8) + i;

	nBytes = out.Write(_pal, nColors*4);
	return (nBytes==nColors*4)?FvsOk:FvsIoError;
}
