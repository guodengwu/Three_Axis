#include <STC8.H>
#include	"timer.h"
#include "motor.h"

u8 _10ms_EVENT;
u8 _1s_EVENT;
struct SYS_TIM  SysTim;
_softtimer_t Timer1Soft;
_softtimer_t Timer2Soft;

static void SysTimDataInit(void)
{
	SysTim.SumMs = 0;
	SysTim.SumSec = 0;
	SysTim.SumMinute = 0;
	_10ms_EVENT = 0;
	_1s_EVENT = 0;
	Timer1Soft.TIM = Timer1;
	Timer2Soft.TIM = Timer2;
}
/********************* Timer0�жϺ���************************/
void TM0_Isr() interrupt 1
{
    SysTim.SumMs++;
	if(SysTim.SumMs>=1000)	{//1s
		SysTim.SumMs = 0;
		SysTim.SumSec ++;
		_1s_EVENT = 1;
	}
	if((SysTim.SumMs%10)==0)	{//1s
		_10ms_EVENT = 1;
	}
}
//timer1��Ϊ�������ʱ����� ��׼ʱ��10ms
void TM1_Isr() interrupt 3 //using 1
{
    Timer1Soft.cnt ++;
	if(Timer1Soft.cnt>=Timer1Soft.period)	{
		Timer1Soft.cnt = 0;
		MotorStop(DEF_Success);
		SoftTimerStop(&Timer2Soft);
	}
}
u8 asld=0;
//timer2��Ϊ������ʱ��ʱ ��׼ʱ��10ms
void TM2_Isr() interrupt 12 //using 1
{
    Timer2Soft.cnt ++;
	if(Timer2Soft.cnt>=Timer2Soft.period)	{
		Timer2Soft.cnt = 0;
		MotorStop(DEF_Fail);		//������г�ʱ
		TimerStop(Timer2);
		asld = 1;
	}
}

//timer0��Ϊϵͳtick 1ms��ʱ
void SystickInit_Tmer0(void)
{
	u16 timedata;
	
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	timedata = MAIN_Fosc/12/1000;
	timedata = 65536 - timedata;
	TL0 = timedata;//0x88;		//���ö�ʱ��ֵ
	TH0 = timedata>>8;//0x96;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;        //ʹ�ܶ�ʱ���ж�
	SysTimDataInit();
}
//timer1��Ϊ�������ʱ����� ��׼ʱ��10ms
void Timer1Init(void)		//10����@27.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x10;		//���ö�ʱ��ģʽ
	TL1 = 0x1C;		//���ö�ʱ��ֵ
	TH1 = 0xA8;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	ET1 = 1;        //ʹ�ܶ�ʱ��1�ж�
	//TR1 = 1;		//��ʱ��1��ʼ��ʱ
}
//timer2��Ϊ������ʱ��ʱ ��׼ʱ��10ms
void Timer2Init(void)		//10����@27.000MHz
{
	AUXR &= 0xFB;		//��ʱ��ʱ��12Tģʽ
	T2L = 0x1C;		//���ö�ʱ��ֵ
	T2H = 0xA8;		//���ö�ʱ��ֵ
	IE2 |= ET2;                                  //ʹ�ܶ�ʱ���ж�
	//AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
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

void SoftTimerStart(_softtimer_t *psofttimer, u32 value)
{
	psofttimer->cnt = 0;
	psofttimer->period = value;
	TimerStart(psofttimer->TIM);
}

void SoftTimerStop(_softtimer_t *psofttimer)
{
	psofttimer->cnt = 0;
	TimerStop(psofttimer->TIM);
}

