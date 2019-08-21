#include "PWM.h"

void PWMInit(void)
{
    P_SW2 |= 0x80;
	PWMCFG = 0x00;                              //配置PWM的输出初始电平为低电平，也就是第一次翻转前输出的电平
    PWMCKS = 0x00;                              // PWM时钟为系统时钟

	PWM4T1 = 0x0010;
	PWM4T2 = 0x0000;
	PWM5T1 = 0x0010;
	PWM5T2 = 0x0000;
	PWM4CR &= ~0x80;
    PWM5CR &= ~0x80;                               //关闭PWM5输出
    P_SW2 &= ~0x80;

    PWMCR = 0x00;                               //启动PWM模块
}

void StartPWM(u8 pwm, u16 freq, u8 duty)
{
	u16 cycle;
	u32 dat;
	    
	PWMCR &= ~0x80;//关闭PWM模块
	P_SW2 |= 0x80;
	PWMCFG = 0x00;                              //配置PWM的输出初始电平为低电平，也就是第一次翻转前输出的电平
	PWMCKS = 0x00; 
	cycle = (MAIN_Fosc / freq);
	PWMC = cycle;
	if(pwm==PWM4)	{
		if(duty==0)	{
			PWMCR &=~0x04;
			IO_PWM4 = 0; 
		}
		else if(duty==100)	{		
			PWMCR &=~0x04;
			IO_PWM4=1; 
		}  
		else	{
			PWM4T1 = 0x0000;                //设置PWM2第1次反转的PWM计数
			dat = (cycle * duty) / 100;    //设置PWM2第2次反转的PWM计数
										//占空比为(PWM2T2-PWM2T1)/PWMC
			PWM4T2 = dat;
			PWM4CR |= 0x80;                               //使能PWM4输出
			PWMCR |= 0x04;
		}
	}
	else if(pwm==PWM5)	{
		if(duty==0)	{		
			PWMCR &=~0x08;
			IO_PWM5 = 0; 
		}
		else if(duty==100)	{		
			PWMCR &=~0x08;
			IO_PWM5 = 1; 
		} 
		else	{
			PWM5T1 = 0x0000;                //设置PWM2第1次反转的PWM计数
			dat = (cycle * duty) / 100;    //设置PWM2第2次反转的PWM计数
										//占空比为(PWM2T2-PWM2T1)/PWMC
			PWM5T2 = dat;    //设置PWM2第2次反转的PWM计数
										//占空比为(PWM2T2-PWM2T1)/PWMC
			PWM5CR |= 0x80;                               //使能PWM4输出
			PWMCR |= 0x08; //使能PWM信号通道输出
		}
	}
	P_SW2 &= ~0x80;
	PWMCR |= 0x80;//启动PWM模块
}

void StopPWM(u8 pwm)
{
	if(pwm==PWM4)	{
		PWM4CR &= ~0x80;                               //使能PWM4输出
		PWMCR &=~0x04;
	}
	else if(pwm==PWM5)	{
		PWM5CR &= ~0x80;                               //使能PWM4输出
		PWMCR &=~0x08;
	}
}
