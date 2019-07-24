/*
*********************************************************************************************************
*                                               uC/OS-II
*                                               ʵʱ�ں�
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                               ��Ȩ����
*
*                                            MCU-51 ר�ô���
*                                           KEIL C51��ģʽ����
*
* �ļ��� : INCLUDES.H
*********************************************************************************************************
*/

//uC/OS-II����
#ifndef __INCLUDES__
#define __INCLUDES__

#include    <string.h> 
#include "intrins.h"
//�û�ͷ�ļ����������
//Your head file add here.
#include	"STC8.h"
#include	"STCConfig.h"
#include "sys_bits.h"
#include "sys_defs.h"
#include "sys_debug.h"
#include    "bsp.h"
#include    "globalvariable.h"

#include	"GPIO.h"
#include  "USART.h"
#include "ring_buffer.h"
#include "bsp.h"
#include "sys.h"
#include	"timer.h"
#include  "misc.h"
/*#include "timer.h"
#include	"Exti.h"
#include "protocol.h"
#include "misc.h"
#include "ADC.h"*/
//#include "eeprom.h"
//#include "save_data.h"

#define BSP_PRINTF		printf
#endif


