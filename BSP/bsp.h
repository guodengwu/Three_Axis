#ifndef __CPU_BSP_H__
#define __CPU_BSP_H__

////////////////////////////
//总头文件
#include "includes.h"

/////////////////////////////////
#define     X_MOTOR_LeftLimit    			P17
#define     X_MOTOR_RightLimit    			P07
#define     Y_MOTOR_MaxLimit    			P31
#define     Y_MOTOR_MinLimit    			P65

#define     CeMenJinHuoKou	    			P17	//侧门进货口
#define     CeMenMaxLimit    			P07	//侧门上限位
#define     CeMenMinLimit    			P31 //侧门下限位
#define     ChuHuoKouOpen    				P65 //出货口开门
#define     ChuHuoKouClose    				P65 //出货口关门

#define     LIGHT_CTRL1    			P71
#define     LIGHT_CTRL2    			P72

void bsp(void);
void soft_reset(void);

#endif
