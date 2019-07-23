#ifndef __CPU_BSP_H__
#define __CPU_BSP_H__

////////////////////////////
//总头文件
#include "includes.h"

/////////////////////////////////
#define     BEEP    			P31
#define     IO_IR_CHK    			P32
#define     IO_MOTOR_PLUSE_CHK    			P33
#define     IO_LIGHT_CTRL    P10  
#define     IO_PUMP    			P11  
#define     IO_IR_CTRL    		P55 
#define     IO_HUASHUANG_CTRL    	P34 //化霜控制
#define     IO_FOG_CTRL    	P35
#define     IO_RELAY    			P40 //温度压缩机控制
//LED控制器
#define 		TM1640_SCLK    P66
#define			TM1640_DIO     P67
//升降机
#define     UP_DOWN_BI    P64//后退
#define     UP_DOWN_FI    P65//前进
//拨码开关
#define     SW0    				P04
#define     SW1    				P52  
#define     SW2    				P53  
//电机检测
#define     MOTOR_DET0    P45  
#define     MOTOR_DET1    P05 
#define     MOTOR_DET2    P06 
#define     MOTOR_DET3    P07 
#define     MOTOR_DET4    P60 
#define     MOTOR_DET5    P61
#define     MOTOR_DET6    P62
#define     MOTOR_DET7    P63
#define     MOTOR_DET8    P50
#define     MOTOR_DET9    P54
//电机控制
#define     MOTOR_NPN0    P27
#define     MOTOR_NPN1    P74
#define     MOTOR_NPN2    P75
#define     MOTOR_NPN3    P76
#define     MOTOR_NPN4    P77
#define     MOTOR_NPN5    P41
#define     MOTOR_NPN6    P73
#define     MOTOR_NPN7    P72
#define     MOTOR_NPN8    P71
#define     MOTOR_NPN9    P70

#define     MOTOR_PNP0    P26
#define     MOTOR_PNP1    P25
#define     MOTOR_PNP2    P24
#define     MOTOR_PNP3    P23
#define     MOTOR_PNP4    P22
#define     MOTOR_PNP5    P21
#define     MOTOR_PNP6    P20
#define     MOTOR_PNP7    P44
#define     MOTOR_PNP8    P43
#define     MOTOR_PNP9    P42
//#define     MOTOR_AD   		P16  

#define     IO_DOOR_STATE    P42//门状态
void bsp(void);
void soft_reset(void);

#endif
