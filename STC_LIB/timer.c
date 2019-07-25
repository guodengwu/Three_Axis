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
/********************* Timer0中断函数************************/
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

//timer0作为系统tick 1ms定时
void SystickInit_Tmer0(void)
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x88;		//设置定时初值
	TH0 = 0x96;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;        //使能定时器中断
	SysTimDataInit();
}

void TimerStart(u8 TIM)
{
	if(TIM == Timer0)
	{
			TR0 = 1;	//开始运行
	}
	if(TIM == Timer1)
	{
			TR1 = 1;	//开始运行
	}
	if(TIM == Timer2)
	{
			AUXR |=  (1<<4);	//开始运行
	}
}

void TimerStop(u8 TIM)
{
	if(TIM == Timer0)
	{
			TR0 = 0;	//停止运行
	}
	if(TIM == Timer1)
	{
			TR1 = 0;	//停止运行
	}
	if(TIM == Timer2)
	{
			AUXR &= ~(1<<4);	//停止运行
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
