#ifndef __CPU_BSP_H__
#define __CPU_BSP_H__

////////////////////////////
//��ͷ�ļ�
#include "includes.h"

/////////////////////////////////
#define     X_MOTOR_LeftLimit    			P17
#define     X_MOTOR_RightLimit    			P07
#define     Y_MOTOR_MaxLimit    			P31
#define     Y_MOTOR_MinLimit    			P65

#define     CeMenJinHuoKou	    			P17	//���Ž�����
#define     CeMenMaxLimit    			P07	//��������λ
#define     CeMenMinLimit    			P31 //��������λ
#define     ChuHuoKouOpen    				P65 //�����ڿ���
#define     ChuHuoKouClose    				P65 //�����ڹ���

#define     LIGHT_CTRL1    			P71
#define     LIGHT_CTRL2    			P72

void bsp(void);
void soft_reset(void);

#endif
