#ifndef __PWM_H__
#define __PWM_H__

#include "includes.h"

#define	DEF_PWM1						1
#define	DEF_PWM2						2
#define	DEF_PWM3						3
#define	DEF_PWM4						4
#define	DEF_PWM5						5
#define	DEF_PWM6						6

#define     IO_PWM1    				P61
#define     IO_PWM2    				P22
#define     IO_PWM3    				P23
#define     IO_PWM4    				P24
#define     IO_PWM5    				P25
#define     IO_PWM6    				P26

void PWMInit(void);
void StartPWM(u8 pwm, u16 freq, u8 duty);
//void StopPWM(u8 pwm);
#endif
