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
/********************* Timer0中断函数************************/
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
//timer1作为电机运行时间控制 基准时间10ms
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
//timer2作为动作超时计时 基准时间10ms
void TM2_Isr() interrupt 12 //using 1
{
    Timer2Soft.cnt ++;
	if(Timer2Soft.cnt>=Timer2Soft.period)	{
		Timer2Soft.cnt = 0;
		MotorStop(DEF_Fail);		//电机运行超时
		TimerStop(Timer2);
		asld = 1;
	}
}

//timer0作为系统tick 1ms定时
void SystickInit_Tmer0(void)
{
	u16 timedata;
	
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	timedata = MAIN_Fosc/12/1000;
	timedata = 65536 - timedata;
	TL0 = timedata;//0x88;		//设置定时初值
	TH0 = timedata>>8;//0x96;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;        //使能定时器中断
	SysTimDataInit();
}
//timer1作为电机运行时间控制 基准时间10ms
void Timer1Init(void)		//10毫秒@27.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0x1C;		//设置定时初值
	TH1 = 0xA8;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	ET1 = 1;        //使能定时器1中断
	//TR1 = 1;		//定时器1开始计时
}
//timer2作为动作超时计时 基准时间10ms
void Timer2Init(void)		//10毫秒@27.000MHz
{
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0x1C;		//设置定时初值
	T2H = 0xA8;		//设置定时初值
	IE2 |= ET2;                                  //使能定时器中断
	//AUXR |= 0x10;		//定时器2开始计时
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

