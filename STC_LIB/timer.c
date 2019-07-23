#include	"timer.h"

struct SYS_TIM  SysTim;
/********************* Timer0中断函数************************/
void TM0_Isr() interrupt 1 using 1
{
    SysTim.SumMs++;
	if(SysTim.SumMs>=1000)	{//1s
		SysTim.SumMs = 0;
		SysTim.SumSec ++;
	}
}

//timer0作为系统tick 1ms定时
void SystickInit_Tmer0(void)
{
	u32 timedata;
	
    TMOD = 0x00;                                //模式0
	timedata = MAIN_Fosc/12/1000;
	timedata -= 65536;
    TL0 = timedata&0xff;                                 //65536-11.0592M/12/1000
    TH0 = (timedata>>8)&0xff;
    TR0 = 1;                                    //启动定时器
    ET0 = 1;                                    //使能定时器中断
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
