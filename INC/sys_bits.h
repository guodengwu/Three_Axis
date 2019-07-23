/*
********************************************************************************
* novosys_bits.h
*
*   Copyright (C) 2010-2016 ACEA Biosciences, Inc. All rights reserved.
*   Author: AlexShi <shiweining123@163.com>
*
********************************************************************************
*/

#ifndef __SYS_BITS_H
#define __SYS_BITS_H

/*
********************************************************************************
* Included Files
********************************************************************************
*/

#include "sys_types.h"


/*
********************************************************************************
* Pre-processor Definitions
********************************************************************************
*/

#define FIELD_BIT00_07  \
INT8U   b0  :1;         \
INT8U   b1  :1;         \
INT8U   b2  :1;         \
INT8U   b3  :1;         \
INT8U   b4  :1;         \
INT8U   b5  :1;         \
INT8U   b6  :1;         \
INT8U   b7  :1;
#define FIELD_BIT08_15  \
INT8U   b8  :1;         \
INT8U   b9  :1;         \
INT8U   b10 :1;         \
INT8U   b11 :1;         \
INT8U   b12 :1;         \
INT8U   b13 :1;         \
INT8U   b14 :1;         \
INT8U   b15 :1;
#define FIELD_BIT16_31  \
INT8U   b16 :1;         \
INT8U   b17 :1;         \
INT8U   b18 :1;         \
INT8U   b19 :1;         \
INT8U   b20 :1;         \
INT8U   b21 :1;         \
INT8U   b22 :1;         \
INT8U   b23 :1;         \
INT8U   b24 :1;         \
INT8U   b25 :1;         \
INT8U   b26 :1;         \
INT8U   b27 :1;         \
INT8U   b28 :1;         \
INT8U   b29 :1;         \
INT8U   b30 :1;         \
INT8U   b31 :1;
#define FIELD_BIT32_63  \
INT8U   b32 :1;         \
INT8U   b33 :1;         \
INT8U   b34 :1;         \
INT8U   b35 :1;         \
INT8U   b36 :1;         \
INT8U   b37 :1;         \
INT8U   b38 :1;         \
INT8U   b39 :1;         \
INT8U   b40 :1;         \
INT8U   b41 :1;         \
INT8U   b42 :1;         \
INT8U   b43 :1;         \
INT8U   b44 :1;         \
INT8U   b45 :1;         \
INT8U   b46 :1;         \
INT8U   b47 :1;         \
INT8U   b48 :1;         \
INT8U   b49 :1;         \
INT8U   b50 :1;         \
INT8U   b51 :1;         \
INT8U   b52 :1;         \
INT8U   b53 :1;         \
INT8U   b54 :1;         \
INT8U   b55 :1;         \
INT8U   b56 :1;         \
INT8U   b57 :1;         \
INT8U   b58 :1;         \
INT8U   b59 :1;         \
INT8U   b60 :1;         \
INT8U   b61 :1;         \
INT8U   b62 :1;         \
INT8U   b63 :1;

#define FIELD_BITS08    \
        FIELD_BIT00_07
#define FIELD_BITS16    \
        FIELD_BIT00_07  \
        FIELD_BIT08_15
#define FIELD_BITS32    \
        FIELD_BIT00_07  \
        FIELD_BIT08_15  \
        FIELD_BIT16_31

/*
********************************************************************************
* Types
********************************************************************************
*/

typedef union _BIT8 {
    INT8S   sbyte;
    INT8U   ubyte;
    struct {
        FIELD_BITS08
    } bits;
} BIT8, CPU_BIT08, bit8_t;

typedef union _BIT16 {
    INT16S  shw;
    INT16U  uhw;
    INT8S   sbyte[2];
    INT8U   ubyte[2];
    struct {
        FIELD_BITS16
    } bits;
} BIT16, CPU_BIT16, bit16_t;

typedef union _BIT32 {
    INT32S  sword;
    INT32U  uword;
    INT16S    shw[2];
    INT16U    uhw[2];
    INT8S   sbyte[4];
    INT8U   ubyte[4];
    FP32    fp32;
    struct {
        FIELD_BITS32
    } bits;
} BIT32, CPU_BIT32, bit32_t;

/*
********************************************************************************
* Compiler
********************************************************************************
*/



/*
********************************************************************************
* Micro Funcion
********************************************************************************
*/


#undef FIELD_BIT00_07
#undef FIELD_BIT08_15
#undef FIELD_BIT16_31
#undef FIELD_BITS08
#undef FIELD_BITS16
#undef FIELD_BITS32
#undef FIELD_BITS64


#endif      /* __NOVOSYS_BITS_H */

/*
********************************************************************************
* No More!
********************************************************************************
*/
