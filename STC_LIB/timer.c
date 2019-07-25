#include	"timer.h"

u8 _100ms_EVENT;
u8 _1s_EVENT;
struct SYS_TIM  SysTim;

static void SysTimDataInit(void)
{
	SysTim.SumMs = 0;
	SysTim.SumSec = 0;
	SysTim.SumMinute = 0;
	_100ms_EVENT = 0;
	_1s_EVENT = 0;
}
/********************* Timer0�жϺ���************************/
void TM0_Isr() interrupt 1 using 1
{
    SysTim.SumMs++;
	if(SysTim.SumMs>=1000)	{//1s
		SysTim.SumMs = 0;
		SysTim.SumSec ++;
		_1s_EVENT = 1;
	}
	if((SysTim.SumMs%100)==0)	{//1s
		_100ms_EVENT = 1;
	}
}

//timer0��Ϊϵͳtick 1ms��ʱ
void SystickInit_Tmer0(void)
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x88;		//���ö�ʱ��ֵ
	TH0 = 0x96;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;        //ʹ�ܶ�ʱ���ж�
	SysTimDataInit();
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
