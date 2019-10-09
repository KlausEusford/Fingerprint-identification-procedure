#ifndef __IMG_THIN_CPP__
#define __IMG_THIN_CPP__

#include "fvstypes.h"
#include "fvsimage.h"
#include "img_thin.h"

#define REMOVE_P    { img[x][y]=0x80; changed = TRUE; }


/*
From     : Nadeem Ahmed (umahmed@cc.umanitoba.ca)
Subject  : Thinning Algorithm
Newsgroup: borland.public.cppbuilder.graphics
Date :1998/02/23 

Here is that thinning algorithm:
                            9 2 3
                            8 1 4
                            7 5 6
For each pixel(#1, above), we have 8 neighbors (#'s 2-8).

Step 1:
a) Make sure pixel 1, has 2 to 6 (inclusive) neighbors
b) starting from 2, go clockwise until 9, and count the
   number of 0 to 1 transitions.  This should be equal to 1.
c) 2*4*6=0  (ie either 2,4 ,or 6 is off)
d) 4*6*8=0

if these conditions hold, remove pixel 1.
Do this for the entire image.

Step 2
a) same as above
b) same as above
c) 2*6*8=0
d) 2*4*8=0

if these hold remove pixel 1.
*/
/* defines to facilitate reading */
#define P1 img[x-1][y-1]
#define P2 img[x][y-1]
#define P3 img[x+1][y-1]
#define P4 img[x-1][y]
#define P5 img[x+1][y]
#define P6 img[x-1][y+1]
#define P7 img[x][y+1]
#define P8 img[x+1][y+1]

/* a) Make sure it has 2 to 6 neighbours */
#define STEP_A  n = 0; /* number of neighbours */ \
                if (P2) n++; if (P3) n++; if (P5) n++; if (P8) n++; \
                if (P7) n++; if (P6) n++; if (P4) n++; if (P1) n++; \
                if (n>=2 && n<=6)

/* b) count number 0 to 1 transsitions */
#define STEP_B  t = 0; /* number of transitions */ \
                if (!P1 && P2) t++; if (!P2 && P3) t++; \
				if (!P3 && P5) t++; if (!P5 && P8) t++; \
                if (!P8 && P7) t++; if (!P7 && P6) t++; \
                if (!P6 && P4) t++; if (!P4 && P1) t++; \
                if (t==1)
 
template <class itemType>
FvsError_t ImageThin(FvsImage<itemType> & img)
{
    uint32_t x, y, n, t;
    bool_t changed = TRUE;
    while (changed==TRUE)
    {
        changed = FALSE;
        for (y=1; y<img.height()-1; y++)
        for (x=1; x<img.width()-1; x++)
        {
            if (img[x][y]==0xFF)
            {
                STEP_A
                {
                    STEP_B
                    {
                        /*
                        c) 2*5*7=0  (ie either 2,5 ,or 7 is off)
                        d) 5*7*4=0
                        */
                        if (P2*P5*P7==0 && P5*P7*P4==0)
                            REMOVE_P;

                    }
                }
            }
        }

        for (y=1; y<img.height()-1; y++)
        for (x=1; x<img.width()-1; x++)
            if (img[x][y]==0x80)
                img[x][y] = 0;

        for (y=1; y<img.height()-1; y++)
        for (x=1; x<img.width()-1; x++)
        {
            if (img[x][y]==0xFF)
            {
                STEP_A
                {
                    STEP_B
                    {
                        /*
                        c) 2*7*4=0
                        d) 2*5*4=0
                        */
                        if (P2*P7*P4==0 && P2*P5*P4==0)
                            REMOVE_P;

                    }
                }
            }
        }

        for (y=1; y<img.height()-1; y++)
        for (x=1; x<img.width()-1; x++)
            if (img[x][y]==0x80)
                img[x][y] = 0;
    }

    return FvsOk;
}
#endif // __IMG_THIN_CPP__