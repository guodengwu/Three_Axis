/*
********************************************************************************
* novosys_defs.h
*
*   Copyright (C) 2010-2016 ACEA Biosciences, Inc. All rights reserved.
*   Author: AlexShi <shiweining123@163.com>
*
********************************************************************************
*/

#ifndef __SYS_DEFS_H
#define __SYS_DEFS_H

/*
********************************************************************************
* Included Files
********************************************************************************
*/



/*
********************************************************************************
* Pre-processor Boolean Definitions
********************************************************************************
*/

#ifndef DEF_False
#define DEF_False       0u
#endif
#ifndef DEF_True
#define DEF_True        1u
#endif

#ifndef DEF_Fail
#define DEF_Fail        0u
#endif
#ifndef DEF_Success
#define DEF_Success     1u
#endif

#ifndef DEF_No
#define DEF_No          0u
#endif
#ifndef DEF_Yes
#define DEF_Yes         1u
#endif

#ifndef DEF_Disable
#define DEF_Disable     0u
#endif
#ifndef DEF_Enable
#define DEF_Enable      1u
#endif

#ifndef DEF_Active
#define DEF_Active      0u
#endif
#ifndef DEF_Inactive
#define DEF_Inactive    1u
#endif

#ifndef DEF_Invalid
#define DEF_Invalid     0u
#endif
#ifndef DEF_Valid
#define DEF_Valid       1u
#endif

#ifndef DEF_Idle
#define DEF_Idle        0u
#endif
#ifndef DEF_Busy
#define DEF_Busy        1u
#endif

#ifndef DEF_OFF
#define DEF_OFF         0u
#endif
#ifndef DEF_ON
#define DEF_ON          1u
#endif

#ifndef DEF_Reset
#define DEF_Reset       0u
#endif
#ifndef DEF_Set
#define DEF_Set         1u
#endif

#ifndef DEF_Low
#define DEF_Low         0u
#endif
#ifndef DEF_High
#define DEF_High        1u
#endif

#ifndef DEF_Dn
#define DEF_Dn          0u
#endif
#ifndef DEF_Up
#define DEF_Up          1u
#endif

#ifndef DEF_Unlock
#define DEF_Unlock      0u
#endif
#ifndef DEF_Lock
#define DEF_Lock        1u
#endif

#ifndef DEF_Stop
#define DEF_Stop        0u
#endif
#ifndef DEF_Run
#define DEF_Run         1u
#endif

#ifndef DEF_IN
#define DEF_IN          0u
#endif
#ifndef DEF_OUT
#define DEF_OUT         1u
#endif

/*
********************************************************************************
* Pre-processor Definitions
********************************************************************************
*/


#define SYS_IS_RTCA_IG             0       // 鐎规矮绠熸稉绡榦voCyte

#define CPU_CFG_DATA_SIZE               4

#define DEF_SYSPWR_OFF                  DEF_High
#define DEF_SYSPWR_ON                    DEF_Low
#define DEF_SYSPWR_HIT                  DEF_High
#define DEF_SYSPWR_UNHIT                 DEF_Low
#define DEF_COVER_OPEN                  DEF_High
#define DEF_COVER_CLOSE                  DEF_Low
#define DEF_FLOATER_ACTIVE              DEF_High
#define DEF_FLOATER_INACTIVE             DEF_Low

#define DEF_UART_BUFF_MIN_SIZE               64u
#define DEF_BAUDRATE_4800bps              4800uL        // 4800bps
#define DEF_BAUDRATE_9600bps              9600uL        // 9600bps
#define DEF_BAUDRATE_19200bps            19200uL        // 19200bps
#define DEF_BAUDRATE_38400bps            38400uL        // 38400bps
#define DEF_BAUDRATE_57600bps            57600uL        // 57600bps
#define DEF_BAUDRATE_115200bps          115200uL        // 115200bps
#define DEF_BAUDRATE_921600bps          921600uL        // 921600bps
#define DEF_BAUDRATE_8Mbps             8000000uL        //8000000bps

#define DEF_RUN_AT_SRAM						__attribute__((section(".SRAM")))
#define DEF_RUN_AT_DDR2_DATA				__attribute__((section(".DDR2_DATA")))
/*
********************************************************************************
* Shared Types
********************************************************************************
*/



/*
********************************************************************************
* Public Types
********************************************************************************
*/



/*
********************************************************************************
* Inline Functions
********************************************************************************
*/



/*
********************************************************************************
* Shared Data
********************************************************************************
*/



/*
********************************************************************************
* Public Data
********************************************************************************
*/



/*
********************************************************************************
* Public Function Prototypes
********************************************************************************
*/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif      /* __NOVOSYS_DEFS_H */

/*
********************************************************************************
* No More!
********************************************************************************
*/
