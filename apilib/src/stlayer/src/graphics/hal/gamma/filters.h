/*******************************************************************************

File name   : filters.h

Description : Contains filters tables

COPYRIGHT (C) STMicroelectronics 2001.

Date               Modification                                  Name
----               ------------                                  ----
28 Nov 2001       Creation                                       Michel Bruant
02 Nov 2006       Add Flicker Fillter and H & V resize coefs     Slim Ben Ezzeddine Ben Ayed

*******************************************************************************/

/* Define to prevent recursive inclusion */

#ifndef __FILTERS_H
#define __FILTERS_H

/* Includes ----------------------------------------------------------------- */
#include "stddefs.h"

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif

/* Exported Constants ------------------------------------------------------- */

#define NB_HSRC_FILTERS         10
#define NB_HSRC_PHASES          8
#define NB_HSRC_TAPS            5
#define NB_HSRC_COEFFS          (NB_HSRC_TAPS * NB_HSRC_PHASES)
#define FILTERS_ALIGN           (16 - (NB_HSRC_COEFFS % 16))



#define NB_VSRC_FILTERS         7
#define NB_VSRC_PHASES          8
#define NB_VSRC_TAPS            3
#define NB_VSRC_COEFFS          (NB_VSRC_TAPS * NB_VSRC_PHASES)
#define VFILTERS_ALIGN           (16 - (NB_VSRC_COEFFS % 16))



#define NB_FF_FILTERS         1
#define NB_FF_PHASES          8
#define NB_FF_TAPS            3
#define NB_FF_COEFFS          (NB_FF_TAPS * NB_FF_PHASES)
#define FF_FILTERS_ALIGN           (16 - (NB_FF_COEFFS % 16))



/* Exported Variables ------------------------------------------------------- */

#ifdef VARIABLE_STLAYER_RESERVATION
#define VARIABLE_STLAYER           /* real reservation   */
#else
#define VARIABLE_STLAYER extern    /* extern reservation */
#endif

VARIABLE_STLAYER const U8 stlayer_HSRC_Coeffs[  NB_HSRC_FILTERS
                                             * (NB_HSRC_COEFFS + FILTERS_ALIGN)]
#ifdef VARIABLE_STLAYER_RESERVATION
= {
 /* Incr <= 1024 (single coeff set for any upsize) */
 0x00,  0x00,  0x40,  0x00,  0x00,
 0xfd,  0x09,  0x3c,  0xfa,  0x04,
 0xf9,  0x13,  0x39,  0xf5,  0x06,
 0xf5,  0x1f,  0x31,  0xf3,  0x08,
 0xf3,  0x2a,  0x28,  0xf3,  0x08,
 0xf3,  0x34,  0x1d,  0xf5,  0x07,
 0xf5,  0x3b,  0x12,  0xf9,  0x05,
 0xfa,  0x3f,  0x07,  0xfd,  0x03,
 0,0,0,0,0,0,0,0, /* align next table on 16 bytes */


/*   1024 < Incr <= 1331 */
 0xfc,  0x05,  0x3e,  0x05,  0xfc,
 0xf8,  0x0e,  0x3b,  0xff,  0x00,
 0xf5,  0x18,  0x38,  0xf9,  0x02,
 0xf4,  0x21,  0x31,  0xf5,  0x05,
 0xf4,  0x2a,  0x27,  0xf4,  0x07,
 0xf6,  0x30,  0x1e,  0xf4,  0x08,
 0xf9,  0x35,  0x15,  0xf6,  0x07,
 0xff,  0x37,  0x0b,  0xf9,  0x06,
 0,0,0,0,0,0,0,0, /* align next table on 16 bytes */


/*  1331 < Incr <= 1433 */
 0xf8,  0x0a,  0x3c,  0x0a,  0xf8,
 0xf6,  0x12,  0x3b,  0x02,  0xfb,
 0xf4,  0x1b,  0x35,  0xfd,  0xff,
 0xf4,  0x23,  0x30,  0xf8,  0x01,
 0xf6,  0x29,  0x27,  0xf6,  0x04,
 0xf9,  0x2e,  0x1e,  0xf5,  0x06,
 0xfd,  0x31,  0x16,  0xf6,  0x06,
 0x02,  0x32,  0x0d,  0xf8,  0x07,
 0,0,0,0,0,0,0,0, /* align next table on 16 bytes */


/* 1433 < Incr <= 1536 */
 0xf6,  0x0e,  0x38,  0x0e,  0xf6,
 0xf5,  0x15,  0x38,  0x06,  0xf8,
 0xf5,  0x1d,  0x33,  0x00,  0xfb,
 0xf6,  0x23,  0x2d,  0xfc,  0xfe,
 0xf9,  0x28,  0x26,  0xf9,  0x00,
 0xfc,  0x2c,  0x1e,  0xf7,  0x03,
 0x00,  0x2e,  0x18,  0xf6,  0x04,
 0x05,  0x2e,  0x11,  0xf7,  0x05,
 0,0,0,0,0,0,0,0, /* align next table on 16 bytes */


/* 1536 < Incr <= 2048 */
 0xfb,  0x13,  0x24,  0x13,  0xfb,
 0xfd,  0x17,  0x23,  0x0f,  0xfa,
 0xff,  0x1a,  0x23,  0x0b,  0xf9,
 0x01,  0x1d,  0x22,  0x07,  0xf9,
 0x04,  0x20,  0x1f,  0x04,  0xf9,
 0x07,  0x22,  0x1c,  0x01,  0xfa,
 0x0b,  0x24,  0x17,  0xff,  0xfb,
 0x0f,  0x24,  0x14,  0xfd,  0xfc,
 0,0,0,0,0,0,0,0, /* align next table on 16 bytes */


/* 2048 < Incr <= 3072 */
 0x05,  0x10,  0x16,  0x10,  0x05,
 0x06,  0x11,  0x16,  0x0f,  0x04,
 0x08,  0x13,  0x15,  0x0e,  0x02,
 0x09,  0x14,  0x16,  0x0c,  0x01,
 0x0b,  0x15,  0x15,  0x0b,  0x00,
 0x0d,  0x16,  0x13,  0x0a,  0x00,
 0x0f,  0x17,  0x13,  0x08,  0xff,
 0x11,  0x18,  0x12,  0x07,  0xfe,
 0,0,0,0,0,0,0,0, /* align next table on 16 bytes */


/* 3072 < Incr <= 4096 */
 0x09,  0x0f,  0x10,  0x0f,  0x09,
 0x09,  0x0f,  0x12,  0x0e,  0x08,
 0x0a,  0x10,  0x11,  0x0e,  0x07,
 0x0b,  0x11,  0x11,  0x0d,  0x06,
 0x0c,  0x11,  0x12,  0x0c,  0x05,
 0x0d,  0x12,  0x11,  0x0c,  0x04,
 0x0e,  0x12,  0x11,  0x0b,  0x04,
 0x0f,  0x13,  0x11,  0x0a,  0x03,
 0,0,0,0,0,0,0,0, /* align next table on 16 bytes */


/* 4096 < Incr <= 5120 */
 0x0a,  0x0e,  0x10,  0x0e,  0x0a,
 0x0b,  0x0e,  0x0f,  0x0e,  0x0a,
 0x0b,  0x0f,  0x10,  0x0d,  0x09,
 0x0c,  0x0f,  0x10,  0x0d,  0x08,
 0x0d,  0x0f,  0x0f,  0x0d,  0x08,
 0x0d,  0x10,  0x10,  0x0c,  0x07,
 0x0e,  0x10,  0x0f,  0x0c,  0x07,
 0x0f,  0x10,  0x10,  0x0b,  0x06,
 0,0,0,0,0,0,0,0, /* align next table on 16 bytes */


/* 5120 < Incr */
 0x0b,  0x0e,  0x0e,  0x0e,  0x0b,
 0x0b,  0x0e,  0x0f,  0x0d,  0x0b,
 0x0c,  0x0e,  0x0f,  0x0d,  0x0a,
 0x0c,  0x0e,  0x0f,  0x0d,  0x0a,
 0x0d,  0x0f,  0x0e,  0x0d,  0x09,
 0x0d,  0x0f,  0x0f,  0x0c,  0x09,
 0x0e,  0x0f,  0x0e,  0x0c,  0x09,
 0x0e,  0x0f,  0x0f,  0x0c,  0x08,
 0,0,0,0,0,0,0,0 /* align next table on 16 bytes */
}
#endif
;




VARIABLE_STLAYER const U8 stlayer_VSRC_Coeffs[  NB_VSRC_FILTERS
                                             * (NB_VSRC_COEFFS + VFILTERS_ALIGN)]
#ifdef VARIABLE_STLAYER_RESERVATION
= {
    /*filter coefs computed with : inc <=256 */
    0x00  ,  0x40 ,  0x00,
    0x09  ,  0x3D ,  0xFA,
    0x13  ,  0x37,   0xF6,
    0x1D  ,  0x2F,   0xF4,
    0x26  ,  0x26,   0xF4,
    0x2F  ,  0x1B,   0xF6,
    0x36  ,  0x11,   0xF9,
    0x3B  ,  0x08,   0xFD ,
    0,0,0,0,0,0,0,0, /* align next table on 16 bytes */

    /* filter coefs computed with :256<inc <=384 */
    0x0E  ,    0x24 ,  0x0E,
    0x13 ,     0x22 ,  0x0B,
    0x17 ,     0x22,   0x07,
    0x1B,      0x22,   0x03,
    0x20,      0x20,   0x00,
    0x25,      0x1D,   0xFE,
    0x2A,      0x1B,   0xFB,
    0x2F,      0x19,   0xF8 ,
    0,0,0,0,0,0,0,0, /* align next table on 16 bytes */
    /* filter coefs computed with :384<inc <=512 */
    0x12  ,    0x1C ,  0x12,
    0x14 ,     0x1C ,  0x10,
    0x17 ,     0x1B,   0x0E,
    0x19,      0x1C,   0x0B,
    0x1B,      0x1C,   0x09,
    0x1E,      0x1B,   0x07,
    0x21,      0x1A,   0x05,
    0x24,      0x1A,   0x02 ,
    0,0,0,0,0,0,0,0, /* align next table on 16 bytes */
    /* filter coefs computed with :512<inc <=768 */
    0x14  ,    0x18 ,  0x14,
    0x15 ,     0x18 ,  0x13,
    0x16 ,     0x18,   0x12,
    0x17,      0x18,   0x11,
    0x18,      0x18,   0x10,
    0x19,      0x18,   0x0F,
    0x1A,      0x18,   0x0E,
    0x1B,      0x18,   0x0D ,
    0,0,0,0,0,0,0,0, /* align next table on 16 bytes */
    /* filter coefs computed with :768<inc <=1024 */
    0x15  ,    0x16 ,  0x15,
    0x15 ,     0x17 ,  0x14,
    0x16 ,     0x17,   0x13,
    0x17,      0x17,   0x13,
    0x17,      0x17,   0x12,
    0x17,      0x17,   0x12,
    0x18,      0x17,   0x11,
    0x19,      0x16,   0x11 ,
    0,0,0,0,0,0,0,0, /* align next table on 16 bytes */
    /* filter coefs computed with :1024<inc  */
    0x15  ,    0x16 ,  0x15,
    0x15 ,     0x16 ,  0x15,
    0x16 ,     0x16,   0x14,
    0x16,      0x16,   0x14,
    0x17,      0x17,   0x13,
    0x17,      0x16,   0x13,
    0x17,      0x16,   0x13,
    0x17,      0x17,   0x12 ,
    0,0,0,0,0,0,0,0 /* align next table on 16 bytes */

   }
#endif
;



VARIABLE_STLAYER const U8 stlayer_FF_Coeffs[  NB_FF_FILTERS
                                             * (NB_FF_COEFFS + FF_FILTERS_ALIGN)]
#ifdef VARIABLE_STLAYER_RESERVATION
= {
    /*filter coefs computed with : x8 */
    0x00  ,  0x80 ,    0x00,
    0x04  ,  0x10 ,    0x60,
    0x10 ,    0x06,    0x18,
    0x50 ,    0x18,    0x08,
    0x20 ,    0x40,    0x20,
    0x00 ,    0x15,    0x01,
    0x34 ,    0x0b,    0x01,
    0x3b ,    0x04,    0x01
    }


#endif
;



#undef VARIABLE_STLAYER


#undef VARIABLE_STLAYER


/* C++ support */
#ifdef __cplusplus
}
#endif

#endif /* #ifndef __FILTERS_H */
