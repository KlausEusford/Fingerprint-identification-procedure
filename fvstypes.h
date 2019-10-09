
/* Some of the following types may already be defines in stddef.h on some
** systems. The code here will certainly be improved here (when we will
** provide a configure script). The typedefs provided here are only valid
** on some systems. Modify them according to your systems till we provide
** sufficient self detection.
*/

#ifndef __FVSTYPES_HEADER__
#define __FVSTYPES_HEADER__

// Tried using template class functions, but they are too much of a headache
#define FVS_IMAGE_TYPE unsigned char

// Color definitions you may switch the value around if you want diffent
// colors
#define COLOR_BACKGROUND 0xFF
#define COLOR_LINE 0x00

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef float float_t;

#ifndef __int8_t_defined

typedef signed char    int8_t;
typedef signed short   int16_t;
typedef signed int     int32_t;

#endif


typedef enum bool_t
{
    FALSE = 0,
    TRUE  = 1
} bool_t;

typedef void* pointer_t;
typedef char* String_t;
/* Error codes. An error code should be returned by each function from the
** interface. The user of the API should always know when an operation did
** not end up successfully.
*/
typedef enum FvsError_t
{
    FvsFailure          = -1,   /* an undefined error, use with parcimony */
    FvsOk               = 0,    /* no error */
    FvsMemory,                  /* not enough memory */
    FvsBadParameter,            /* an invalid parameter was used */
    FvsBadFormat,               /* bad file format */
    FvsIoError,                 /* input / output error */
} FvsError_t;


#endif /* __FVSTYPES_HEADER__ */