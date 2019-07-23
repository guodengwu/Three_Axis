/*
*********************************************************************************************************
*                                               uC/OS-II
*                                               实时内核
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                               版权所有
*
*                                            MCU-51 专用代码
*                                           KEIL C51大模式编译
*
* 文件名 : INCLUDES.H
*********************************************************************************************************
*/

//uC/OS-II核心
#ifndef __INCLUDES__
#define __INCLUDES__

#include    <string.h> 
#include "intrins.h"
//用户头文件加在最后面
//Your head file add here.
#include	"STC8.h"
#include	"STCConfig.h"
#include "sys_bits.h"
#include "sys_defs.h"
//#include "sys_debug.h"
#include    "bsp.h"
#include    "globalvariable.h"

#include	"GPIO.h"
#include  "USART.h"
#include "ring_buffer.h"
#include "sys.h"
#include	"timer.h"
/*#include  "msg.h"
#include "timer.h"
#include	"Exti.h"
#include "protocol.h"
#include "misc.h"
#include "ADC.h"*/
//#include "eeprom.h"
//#include "save_data.h"

#define BSP_PRINTF		printf
#endif



