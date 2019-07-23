#include	"timer.h"

struct SYS_TIM  SysTim;
/********************* Timer0�жϺ���************************/
void TM0_Isr() interrupt 1 using 1
{
    SysTim.SumMs++;
	if(SysTim.SumMs>=1000)	{//1s
		SysTim.SumMs = 0;
		SysTim.SumSec ++;
	}
}

//timer0��Ϊϵͳtick 1ms��ʱ
void SystickInit_Tmer0(void)
{
	u32 timedata;
	
    TMOD = 0x00;                                //ģʽ0
	timedata = MAIN_Fosc/12/1000;
	timedata -= 65536;
    TL0 = timedata&0xff;                                 //65536-11.0592M/12/1000
    TH0 = (timedata>>8)&0xff;
    TR0 = 1;                                    //������ʱ��
    ET0 = 1;                                    //ʹ�ܶ�ʱ���ж�
}

void TimerStart(u8 TIM)
{
	if(TIM == Timer0)
	{
			TR0 = 1;	//��ʼ����
	}
	if(TIM == Timer1)
	{
			TR1 = 1;	//��ʼ����
	}
	if(TIM == Timer2)
	{
			AUXR |=  (1<<4);	//��ʼ����
	}
}

void TimerStop(u8 TIM)
{
	if(TIM == Timer0)
	{
			TR0 = 0;	//ֹͣ����
	}
	if(TIM == Timer1)
	{
			TR1 = 0;	//ֹͣ����
	}
	if(TIM == Timer2)
	{
			AUXR &= ~(1<<4);	//ֹͣ����
	}
}

void TimerSetValue(u8 TIM, u16 value)
{
	TimerStop(TIM);
	if(TIM == Timer0)
	{		
		TH0 = (u8)(value >> 8);
		TL0 = (u8)value;
	}
	if(TIM == Timer1)
	{		
		TH1 = (u8)(value >> 8);
		TL1 = (u8)value;
	}
	if(TIM == Timer2)
	{		
		TH1 = (u8)(value >> 8);
		TL1 = (u8)value;
	}
}	
