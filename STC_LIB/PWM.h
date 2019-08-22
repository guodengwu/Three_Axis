#ifndef __PWM_H__
#define __PWM_H__

#include "includes.h"

//sbit PWM2=P3^7;                                         
//sbit PWM3=P2^1;                                           
sbit PWM4	=P2^4; 
sbit PWM5	=P2^5; 

#define	PWM4						4
#define	PWM5						5
#define     IO_PWM4    				P24
#define     IO_PWM5    				P25

void PWMInit(void);
void StartPWM(u8 pwm, u16 freq, u8 duty);
//void StopPWM(u8 pwm);
#endif
