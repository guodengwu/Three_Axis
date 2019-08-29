#include "PWM.h"

void PWMInit(void)
{
    P_SW2 |= 0x80;
	PWMCFG = 0x00;  //PWM �� ADC ������
    PWMCKS = 0x0F;  // PWMʱ��=ϵͳʱ��/(PS[3:0]+1) = 27M/16 = 1.6875M

	PWM4T1 = 0x0010;
	PWM4T2 = 0x0000;
	PWM5T1 = 0x0010;
	PWM5T2 = 0x0000;
	PWM4CR = 0x0;	//�ر�PWM4�ж� ʹ��P2.4
    PWM5CR = 0x0;   //�ر�PWM5�ж� ʹ��P2.5
    P_SW2 &= ~0x80;

    PWMCR = 0x00;                               //����PWMģ��
}
extern u8  printfbuf[100];
void StartPWM(u8 pwm, u16 freq, u8 duty)
{
	u32 cycle;
	u32 dat;
	u32 duty_dat;
	    
	duty_dat = 100 - duty;
	//PWMCR &= ~0x80;//�ر�PWMģ��
	P_SW2 |= 0x80;
	PWMCFG = 0x00;  //PWM �� ADC ������
	PWMCKS = 0x0F;  // PWMʱ��=ϵͳʱ��/(PS[3:0]+1) = 27M/16 = 1.6875M
	cycle = ((MAIN_Fosc / 16) / freq);
	PWMC = cycle;

	if(pwm==DEF_PWM2)	{
		if(duty==0)	{
			PWM2CR =0x0;
			IO_PWM2 = 0; 
		}
		else if(duty==100)	{
			PWM2CR =0x0;
			IO_PWM2=1; 
		}  
		else	{
			dat = cycle*duty_dat / 100;
			//dat *= duty_dat;    //����PWM2��2�η�ת��PWM���� ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
			PWM2T1 = 0;                //����PWM2��1�η�ת��PWM����
			PWM2T2 = dat;	
			//SYS_PRINTF("%d %d %d\r\n",cycle,duty_dat,dat);
			PWM2CR = 0x80;                               //ʹ��PWM4���			
		}
	}
	else if(pwm==DEF_PWM3)	{
		if(duty==0)	{
			PWM3CR =0x0;
			IO_PWM3 = 0; 
		}
		else if(duty==100)	{
			PWM3CR =0x0;
			IO_PWM3=1; 
		}  
		else	{
			dat = cycle*duty_dat / 100;
			//dat *= duty_dat;    //����PWM2��2�η�ת��PWM���� ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
			PWM3T1 = 0;                //����PWM2��1�η�ת��PWM����
			PWM3T2 = dat;	
			//SYS_PRINTF("%d %d %d\r\n",cycle,duty_dat,dat);
			PWM3CR = 0x80;                               //ʹ��PWM4���			
		}
	}
	else if(pwm==DEF_PWM4)	{
		if(duty==0)	{
			PWM4CR =0x0;
			IO_PWM4 = 0; 
		}
		else if(duty==100)	{
			PWM4CR =0x0;
			IO_PWM4=1; 
		}  
		else	{
			//dat = cycle*duty_dat / 100;
			dat = cycle*duty_dat / 100;
			//dat *= duty_dat;    //����PWM2��2�η�ת��PWM���� ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
			PWM4T1 = 0;                //����PWM2��1�η�ת��PWM����
			PWM4T2 = dat;	
			//SYS_PRINTF("%d %d %d\r\n",cycle,duty_dat,dat);
			PWM4CR = 0x80;                               //ʹ��PWM4���			
//			PWMCR |= 0x04;
		}
	}
	else if(pwm==DEF_PWM5)	{
		if(duty==0)	{		
			PWM5CR =0x0;
			IO_PWM5 = 0; 
		}
		else if(duty==100)	{		
			PWM5CR =0x0;
			IO_PWM5 = 1; 
		} 
		else	{
			//PWM5T1 = 0x0001;                //����PWM2��1�η�ת��PWM����
			dat = cycle*duty_dat / 100;    //����PWM2��2�η�ת��PWM���� ռ�ձ�Ϊ(PWM2T2-PWM2T1)/PWMC
			//dat *= duty_dat;
			PWM5T1 = 0; 
			PWM5T2 = dat;    
													
			PWM5CR = 0x80;                               //ʹ��PWM4���
//			PWMCR |= 0x08; //ʹ��PWM�ź�ͨ�����
		}
	}
	P_SW2 &= ~0x80;
	PWMCR |= 0x80;//����PWMģ��
}
/*
void StopPWM(u8 pwm)
{
	if(pwm==PWM4)	{
		PWM4CR = 0x0;                               //ʹ��PWM4���
//		PWMCR &=~0x04;
		IO_PWM4 = 0; 
	}
	else if(pwm==PWM5)	{
		PWM5CR = 0x0;                               //ʹ��PWM4���
//		PWMCR &=~0x08;
		IO_PWM5 = 0; 
	}
}
*/