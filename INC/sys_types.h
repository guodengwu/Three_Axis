/*
********************************************************************************
* novosys_types.h
*
*   Copyright (C) 2010-2016 ACEA Biosciences, Inc. All rights reserved.
*   Author: AlexShi <shiweining123@163.com>
*
********************************************************************************
*/

#ifndef __SYS_TYPES_H
#define __SYS_TYPES_H

/*
********************************************************************************
* Included Files
********************************************************************************
*/

//#include "includes.h"
#include <stdio.h>

/*
********************************************************************************
* Pre-processor Definitions
********************************************************************************
*/

#ifndef FALSE
#define FALSE                                       0u
#endif

#ifndef TRUE
#define TRUE                                        1u
#endif

/*Bit Mask Macros**************************************************************/
#define DEF_BIT_NONE_MASK                           0uL
#define DEF_BIT_ALL_MASK                           ~0uL
#define DEF_BIT00_MASK                              0x00000001uL    // 1 << 0
#define DEF_BIT01_MASK                              0x00000002uL    // 1 << 1
#define DEF_BIT02_MASK                              0x00000004uL    // 1 << 2
#define DEF_BIT03_MASK                              0x00000008uL    // 1 << 3
#define DEF_BIT04_MASK                              0x00000010uL    // 1 << 4
#define DEF_BIT05_MASK                              0x00000020uL    // 1 << 5
#define DEF_BIT06_MASK                              0x00000040uL    // 1 << 6
#define DEF_BIT07_MASK                              0x00000080uL    // 1 << 7
#define DEF_BIT08_MASK                              0x00000100uL    // 1 << 8
#define DEF_BIT09_MASK                              0x00000200uL    // 1 << 9
#define DEF_BIT10_MASK                              0x00000400uL    // 1 << 10
#define DEF_BIT11_MASK                              0x00000800uL    // 1 << 11
#define DEF_BIT12_MASK                              0x00001000uL    // 1 << 12
#define DEF_BIT13_MASK                              0x00002000uL    // 1 << 13
#define DEF_BIT14_MASK                              0x00004000uL    // 1 << 14
#define DEF_BIT15_MASK                              0x00008000uL    // 1 << 15
#define DEF_BIT16_MASK                              0x00010000uL    // 1 << 16
#define DEF_BIT17_MASK                              0x00020000uL    // 1 << 17
#define DEF_BIT18_MASK                              0x00040000uL    // 1 << 18
#define DEF_BIT19_MASK                              0x00080000uL    // 1 << 19
#define DEF_BIT20_MASK                              0x00100000uL    // 1 << 20
#define DEF_BIT21_MASK                              0x00200000uL    // 1 << 21
#define DEF_BIT22_MASK                              0x00400000uL    // 1 << 22
#define DEF_BIT23_MASK                              0x00800000uL    // 1 << 23
#define DEF_BIT24_MASK                              0x01000000uL    // 1 << 24
#define DEF_BIT25_MASK                              0x02000000uL    // 1 << 25
#define DEF_BIT26_MASK                              0x04000000uL    // 1 << 26
#define DEF_BIT27_MASK                              0x08000000uL    // 1 << 27
#define DEF_BIT28_MASK                              0x10000000uL    // 1 << 28
#define DEF_BIT29_MASK                              0x20000000uL    // 1 << 29
#define DEF_BIT30_MASK                              0x40000000uL    // 1 << 30
#define DEF_BIT31_MASK                              0x80000000uL    // 1 << 31

/*Size Macros******************************************************************/
#define DEF_SIZE_1                                  0x00000001      // Size:   1
#define DEF_SIZE_2                                  0x00000002      // Size:   2
#define DEF_SIZE_4                                  0x00000004      // Size:   4
#define DEF_SIZE_8                                  0x00000008      // Size:   8
#define DEF_SIZE_16                                 0x00000010      // Size:  16
#define DEF_SIZE_32                                 0x00000020      // Size:  32
#define DEF_SIZE_64                                 0x00000040      // Size:  64
#define DEF_SIZE_128                                0x00000080      // Size: 128
#define DEF_SIZE_256                                0x00000100      // Size: 256
#define DEF_SIZE_512                                0x00000200      // Size: 512
#define DEF_SIZE_1K                                 0x00000400      // Size:  1K
#define DEF_SIZE_2K                                 0x00000800      // Size:  2K
#define DEF_SIZE_4K                                 0x00001000      // Size:  4K
#define DEF_SIZE_8K                                 0x00002000      // Size:  8K
#define DEF_SIZE_16K                                0x00004000      // Size: 16K
#define DEF_SIZE_32K                                0x00008000      // Size: 32K
#define DEF_SIZE_64K                                0x00010000      // Size: 64K
#define DEF_SIZE_128K                               0x00020000      // Size:128K
#define DEF_SIZE_256K                               0x00040000      // Size:256K
#define DEF_SIZE_512K                               0x00080000      // Size:512K
#define DEF_SIZE_1M                                 0x00100000      // Size:  1M
#define DEF_SIZE_2M                                 0x00200000      // Size:  2M
#define DEF_SIZE_4M                                 0x00400000      // Size:  4M
#define DEF_SIZE_8M                                 0x00800000      // Size:  8M
#define DEF_SIZE_16M                                0x01000000      // Size: 16M
#define DEF_SIZE_32M                                0x02000000      // Size: 32M
#define DEF_SIZE_64M                                0x04000000      // Size: 64M
#define DEF_SIZE_128M                               0x08000000      // Size:128M
#define DEF_SIZE_256M                               0x10000000      // Size:256M
#define DEF_SIZE_512M                               0x20000000      // Size:512M
#define DEF_SIZE_1G                                 0x40000000      // Size:  1G
#define DEF_SIZE_2G                                 0x80000000      // Size:  2G

/*Const Macros*****************************************************************/
#define DEF_FP_Delta                                0.00001f
#define DEF_Pi                                      3.1415926535897932384626433832795f
#define DEF_2Pi                                     6.283185307179586476925286766559f
#define DEF_Radian2Degree                          (180.0f/DEF_Pi)
#define DEF_Degree2Radian                          (DEF_Pi/180.0f)
#define OS_TICKS_PER_MIN                           (DEF_TIME_NBR_SEC_PER_MIN*OS_TICKS_PER_SEC)
#define OS_TICKS_PER_HOUR                          (DEF_TIME_NBR_SEC_PER_HR*OS_TICKS_PER_SEC)

/*
********************************************************************************
* Types
********************************************************************************
*/
typedef           unsigned char           uint8_t;      /*          Unsigned  8 bit Quantity          */
typedef           unsigned short          uint16_t;     /*          Unsigned 16 bit Quantity          */
typedef           unsigned int           uint32_t;     /*          Unsigned 32 bit Quantity          */
/************************************** 8-bit DATA TYPE DEFINITION ************************************/
typedef           unsigned char           INT8U;      /*          Unsigned  8 bit Quantity          */
typedef             signed char           INT8S;      /*            Signed  8 bit Quantity          */
typedef             unsigned char   const   INT8UC;     /*          Unsigned  8 bit Constant          */
typedef               signed char   const   INT8SC;     /*            Signed  8 bit Constant          */
typedef volatile    unsigned char           VINT8U;     /* Volatile Unsigned  8 bit Quantity          */
typedef volatile      signed char           VINT8S;     /* Volatile   Signed  8 bit Quantity          */
//typedef volatile    unsigned char   const   VINT8UC;    /* Volatile Unsigned  8 bit Constant          */
//typedef volatile      signed char   const   VINT8SC;    /* Volatile   Signed  8 bit Constant          */
/************************************* 16-bit DATA TYPE DEFINITION ************************************/
typedef           unsigned short          INT16U;     /*          Unsigned 16 bit Quantity          */
typedef             signed short          INT16S;     /*            Signed 16 bit Quantity          */
typedef             unsigned short  const   INT16UC;    /*          Unsigned 16 bit Constant          */
typedef               signed short  const   INT16SC;    /*            Signed 16 bit Constant          */
typedef volatile    unsigned short          VINT16U;    /* Volatile Unsigned 16 bit Quantity          */
typedef volatile      signed short          VINT16S;    /* Volatile   Signed 16 bit Quantity          */
//typedef volatile    unsigned short  const   VINT16UC;   /* Volatile Unsigned 16 bit Constant          */
//typedef volatile      signed short  const   VINT16SC;   /* Volatile   Signed 16 bit Constant          */
/************************************* 32-bit DATA TYPE DEFINITION ************************************/
typedef           unsigned int           INT32U;     /*          Unsigned 32 bit Quantity          */
typedef             signed int           INT32S;     /*            Signed 32 bit Quantity          */
typedef             unsigned int   const   INT32UC;    /*          Unsigned 32 bit Constant          */
typedef               signed int   const   INT32SC;    /*            Signed 32 bit Constant          */
typedef volatile    unsigned int           VINT32U;    /* Volatile Unsigned 32 bit Quantity          */
typedef volatile      signed int           VINT32S;    /* Volatile   Signed 32 bit Quantity          */
//typedef volatile    unsigned int   const   VINT32UC;   /* Volatile Unsigned 32 bit Constant          */
//typedef volatile      signed int   const   VINT32SC;   /* Volatile   Signed 32 bit Constant          */
/************************************* 64-bit DATA TYPE DEFINITION ************************************/
//typedef          unsigned long long         INT64U;     /*          Unsigned 64 bit Quantity          */
//typedef            signed long long         INT64S;     /*            Signed 64 bit Quantity          */
//typedef          unsigned long long const   INT64UC;    /*          Unsigned 64 bit Constant          */
//typedef            signed long long const   INT64SC;    /*            Signed 64 bit Constant          */
//typedef volatile unsigned long long         VINT64U;    /* Volatile Unsigned 64 bit Quantity          */
//typedef volatile   signed long long         VINT64S;    /* Volatile   Signed 64 bit Quantity          */
//typedef volatile unsigned long long const   VINT64UC;   /* Volatile Unsigned 64 bit Constant          */
//typedef volatile   signed long long const   VINT64SC;   /* Volatile   Signed 64 bit Constant          */
/************************************* Float DATA TYPE DEFINITION *************************************/
typedef float                             FP32;       /* Single precision floating point            */
typedef double                            FP64;       /* Double precision floating point            */
/******************************************************************************************************/
typedef            void         CPU_VOID;
typedef            char         CPU_CHAR;               /*  8-bit character                           */
typedef  unsigned  char         CPU_BOOLEAN;            /*  8-bit boolean or logical                  */
typedef  unsigned  char         CPU_INT08U;             /*  8-bit unsigned integer                    */
typedef    signed  char         CPU_INT08S;             /*  8-bit   signed integer                    */
typedef  unsigned  short        CPU_INT16U;             /* 16-bit unsigned integer                    */
typedef    signed  short        CPU_INT16S;             /* 16-bit   signed integer                    */
typedef  unsigned  int          CPU_INT32U;             /* 32-bit unsigned integer                    */
typedef    signed  int          CPU_INT32S;             /* 32-bit   signed integer                    */
//typedef  unsigned  long  long   CPU_INT64U;             /* 64-bit unsigned integer                    */
//typedef    signed  long  long   CPU_INT64S;             /* 64-bit   signed integer                    */
typedef            float        CPU_FP32;               /* 32-bit floating point                      */
typedef            double       CPU_FP64;               /* 64-bit floating point                      */
typedef  volatile  CPU_INT08U   CPU_REG08;              /*  8-bit register                            */
typedef  volatile  CPU_INT16U   CPU_REG16;              /* 16-bit register                            */
typedef  volatile  CPU_INT32U   CPU_REG32;              /* 32-bit register                            */
//typedef  volatile  CPU_INT64U   CPU_REG64;              /* 64-bit register                            */
typedef            void       (*CPU_FNCT_VOID)(void  ); /* See Note #2a.                              */
typedef            void       (*CPU_FNCT_PTR )(void *); /* See Note #2b.                              */
/******************************************************************************************************/
typedef           CPU_INT32S    CPU_BASE_T;
typedef           CPU_INT32U    CPU_UBASE_T;
typedef           CPU_BASE_T    CPU_ERR_T;
typedef           CPU_BASE_T    CPU_FLAG_T;
typedef           CPU_BASE_T    CPU_OFF_T;
typedef          CPU_UBASE_T    CPU_DATA;
typedef          CPU_UBASE_T    CPU_SIZE_T;

typedef         signed  char    cpu_int8s_t;
typedef         signed short    cpu_int16s_t;
typedef         signed  int    cpu_int32s_t;
//typedef     signed long long    cpu_int64s_t;
typedef       unsigned  char    cpu_bool_t;
typedef       unsigned  char    cpu_int8u_t;
typedef       unsigned short    cpu_int16u_t;
typedef       unsigned  int    cpu_int32u_t;
//typedef   unsigned long long    cpu_int64u_t;
typedef                float    cpu_fp32_t;
typedef               double    cpu_fp64_t;

typedef          signed long    cpu_base_t;
typedef        unsigned long    cpu_ubase_t;
typedef           cpu_base_t    cpu_err_t;
typedef           cpu_base_t    cpu_off_t;
typedef          cpu_ubase_t    cpu_flag_t;
typedef          cpu_ubase_t    cpu_tick_t;
typedef          cpu_ubase_t    cpu_size_t;
typedef          cpu_ubase_t    cpu_addr_t;

typedef       unsigned  char    bool_t;
typedef         signed  char    int8s_t;    /*            Signed  8 bit Quantity          */
typedef       unsigned  char    int8u_t;    /*          Unsigned  8 bit Quantity          */
typedef         signed short    int16s_t;   /*            Signed 16 bit Quantity          */
typedef       unsigned short    int16u_t;   /*          Unsigned 16 bit Quantity          */
typedef         signed  int    int32s_t;   /*            Signed 32 bit Quantity          */
typedef       unsigned  int    int32u_t;   /*          Unsigned 32 bit Quantity          */
//typedef    signed long  long    int64s_t;   /*            Signed 64 bit Quantity          */
//typedef  unsigned long  long    int64u_t;   /*          Unsigned 64 bit Quantity          */
typedef                float    float32_t;  /* Single precision floating point            */
typedef               double    float64_t;  /* Double precision floating point            */

typedef        signed  char     S8;
typedef      unsigned  char     U8;
typedef        signed short     S16;
typedef      unsigned short     U16;
typedef        signed  int     S32;
typedef      unsigned  int     U32;

typedef        signed  char     s8;
typedef      unsigned  char     u8;
typedef        signed short     s16;
typedef      unsigned short     u16;
typedef        signed  int     s32;
typedef      unsigned  int     u32;

typedef int (*cmd_call_t)(void *context);

typedef struct _hexcmd_tbl {
    CPU_INT08U cmd;
    CPU_INT08U flags;
    CPU_INT16U param;
    cmd_call_t call;
} hexcmd_tbl_t;

typedef struct _strcmd_tbl {
    CPU_CHAR *name;
} strcmd_tbl_t;

#endif      /* __NOVOSYS_TYPES_H */

/*
********************************************************************************
* No More!
********************************************************************************
*/
