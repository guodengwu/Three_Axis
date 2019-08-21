#include "PWM.h"

void PWMInit(void)
{
    P_SW2 |= 0x80;
	PWMCFG = 0x00;                              //����PWM�������ʼ��ƽΪ�͵�ƽ��Ҳ���ǵ�һ�η�תǰ����ĵ�ƽ
    PWMCKS = 0x00;                              // PWMʱ��Ϊϵͳʱ��

	PWM4T1 = 0x0010;
	PWM4T2 = 0x0000;
	PWM5T1 = 0x0010;
	PWM5T2 = 0x0000;
	PWM4CR &= ~0x80;
    PWM5CR &= ~0x80;                               //�ر�PWM5���
    P_SW2 &= ~0x80;

    PWMCR = 0x00;                               //����PWMģ��
}

void StartPWM(u8 pwm, u16 freq, u8 duty)
{
	u16 cycle;
	u32 dat;
	    
	PWMCR &= ~0x80;//�ر�PWMģ��
	P_SW2 |= 0x80;
	PWMCFG = 0x00;                              //����PWM�������ʼ��ƽΪ�͵�ƽ��Ҳ���ǵ�һ�η�תǰ����ĵ�ƽ
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
			PWM4T1 = 0x0000;                //����PWM2��1�η�ת��PWM����
			dat = (cycle * duty) / 100;    //����PWM2��2�η�ת��PWM����
										//ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
			PWM4T2 = dat;
			PWM4CR |= 0x80;                               //ʹ��PWM4���
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
			PWM5T1 = 0x0000;                //����PWM2��1�η�ת��PWM����
			dat = (cycle * duty) / 100;    //����PWM2��2�η�ת��PWM����
										//ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
			PWM5T2 = dat;    //����PWM2��2�η�ת��PWM����
										//ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
			PWM5CR |= 0x80;                               //ʹ��PWM4���
			PWMCR |= 0x08; //ʹ��PWM�ź�ͨ�����
		}
	}
	P_SW2 &= ~0x80;
	PWMCR |= 0x80;//����PWMģ��
}

void StopPWM(u8 pwm)
{
	if(pwm==PWM4)	{
		PWM4CR &= ~0x80;                               //ʹ��PWM4���
		PWMCR &=~0x04;
	}
	else if(pwm==PWM5)	{
		PWM5CR &= ~0x80;                               //ʹ��PWM4���
		PWMCR &=~0x08;
	}
}
