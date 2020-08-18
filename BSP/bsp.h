#ifndef __CPU_BSP_H__
#define __CPU_BSP_H__

////////////////////////////
//总头文件
#include "includes.h"

/////////////////////////////////
#define     X_MOTOR_LeftLimit_IN    			P42
#define     X_MOTOR_RightLimit_IN    			P07
#define     Y_MOTOR_MaxLimit_IN    			P65
#define     Y_MOTOR_MinLimit_IN    			P31

#define     QuHuoKouOpenLimit_IN    				P70//P37 //出货口开门到位
#define     QuHuoKouCloseLimit_IN    				P41 //出货口关门到位
#define     HuoWuCheck_IN	    			P30//P66	//货物检测开关
#define     JiaShouLimit_IN	    	P37//P70	//夹手开关
//#define     HuoWuNearSwitch_IN				P30//货物接近开关
#define     CeMenMaxLimit_IN    			P53	//侧门上限位
#define     CeMenMinLimit_IN    			P06 //侧门下限位
#define		X_MotorDuZhuan_IN				P32
#define		Y_MotorDuZhuan_IN				P33

#define		X_ENCODER_IN					P34
#define		Y_ENCODER_IN					P35
#define		ALLMOTOR_STUCK_IN					P36
//////////////////////////////////
#define     X_MOTOR_PWM1    				P27//P24
#define     X_MOTOR_PWM2    				P00//P25
//#define     X_MOTOR_ENABLE    			P26
//#define     X_MOTOR_ENABLE1    			//P42
//#define     X_MOTOR_ENABLE2    			P21

#define     Y_MOTOR_PWM1    			P22
#define     Y_MOTOR_PWM2    			P23
//#define     Y_MOTOR_ENABLE1    			P73
//#define     Y_MOTOR_ENABLE2    			P20

#define     Z_MOTOR_PWM1    			P24//P27
#define     Z_MOTOR_PWM2    			P25//P00
//#define     Z_MOTOR_ENABLE    			P26
#define     T_MOTOR_PWM1    				P77
#define     T_MOTOR_PWM2    				P74
#define     T_MOTOR_ENABLE    			P01

#define     D_MOTOR_PWM1    				P76
#define     D_MOTOR_PWM2    				P75
#define     D_MOTOR_ENABLE    			P20//P10
#define     L_MOTOR_PWM1    				P64
#define     L_MOTOR_PWM2    				P40
#define     L_MOTOR_ENABLE    			P62

#define     QuHuoMen_MOTOR_PWM1    				P54
#define     QuHuoMen_MOTOR_PWM2    				P55
#define     QuHuoMen_MOTOR_ENABLE    			P61

#define     BK_MOTOR_PWM1    				P44
#define     BK_MOTOR_PWM2    				P63
#define     BK_MOTOR_ENABLE    			P60
///////////////////////////////////
#define     LIGHT_CTRL1_OUT    			P71
#define     LIGHT_CTRL2_OUT    			P72
#define     RS485_CTRL    			P43


void bsp(void);
void soft_reset(void);

#endif
