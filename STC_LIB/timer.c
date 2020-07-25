#include <STC8.H>
#include	"timer.h"
#include "motor.h"

u8 _1ms_EVENT,_3ms_EVENT,_10ms_EVENT,_30ms_EVENT;
u8 _1s_EVENT;
struct SYS_TIM  SysTim;
_softtimer_t TimerSoft[7];
//_softtimer_t Timer2Soft;
_softtimer_t Timer3Soft;

static void SysTimDataInit(void)
{
	u8 i;
	SysTim.SumMs = 0;
	SysTim.SumSec = 0;
	SysTim.SumMinute = 0;
	_10ms_EVENT = 0;
	_1s_EVENT = 0;
	for(i=0;i<7;i++)	{
		TimerSoft[i].TIM = Timer1;
		TimerSoft[i].state = STOP;
	}
//	Timer1Soft.TIM = Timer1;
//	Timer1Soft.state = STOP;
//	Timer2Soft.TIM = Timer2;
//	Timer2Soft.state = STOP;
	Timer3Soft.TIM = Timer3;
	Timer3Soft.state = STOP;
}
/********************* Timer中断函数************************/
void TM3_Isr() interrupt 19
{
    SysTim.SumMs++;
	if((SysTim.SumMs%1000)==0)	{//1s
//		SysTim.SumMs = 0;
		SysTim.SumSec ++;
		_1s_EVENT = 1;
	}
	if((SysTim.SumMs%10)==0)	{//1s
		_10ms_EVENT = 1;
	}
	if((SysTim.SumMs%30)==0)	{//1s
		_30ms_EVENT = 1;
	}
	if((SysTim.SumMs%3)==0)	{//1s
		_3ms_EVENT = 1;
	}
	_1ms_EVENT = 1;
//	CalXEncode();
	CalYEncode();
}
//timer4作为电机运行时间控制 基准时间10ms
void TM4_Isr() interrupt 20 //using 1
{
	u8 i;
	for(i=0;i<7;i++)	{
		if(TimerSoft[i].state == USING)	{//电机运行时间控制
			TimerSoft[i].cnt ++;
//			if(i==6)	{
//				if(TimerSoft[i].cnt==1500)
//					StartPWM(QUHUOMEN_PWM, MOTOR_PWM_FREQ, 50);//低速关门
//			}
			if(TimerSoft[i].cnt>=TimerSoft[i].period)	{
				TimerSoft[i].cnt = 0;
				SoftTimerStop(&TimerSoft[i]);
				SysMotor.motor[i].status.abort_type = MotorAbort_Timeout;	
				if(TimerSoft[i].pCallBack!=NULL)	{
					(*TimerSoft[i].pCallBack)();
				}							
			}
		}
	}
	if(Timer3Soft.state == USING)	{
		Timer3Soft.cnt ++;
		if(Timer3Soft.cnt>=Timer3Soft.period)	{
			Timer3Soft.cnt = 0;
			SoftTimerStop(&Timer3Soft);
			if(Timer3Soft.pCallBack!=NULL)	{
				(*Timer3Soft.pCallBack)();
			}
		}
	}
}

//timer3作为系统tick 1ms定时
void SystickInit_Tmer(void)
{
	T4T3M |= 0x02;		//定时器时钟1T模式
	T3L = 0x88;		//设置定时初值
	T3H = 0x96;		//设置定时初值
	T4T3M |= 0x08;		//定时器3开始计时
	IE2 |= ET3;          //使能定时器中断
	SysTimDataInit();
}

u32 GetSysTick(void)
{
	return SysTim.SumMs;
}

void Timer4Init(void)		//10毫秒@27.000MHz
{
	T4T3M &= 0xDF;		//定时器时钟12T模式
	T4L = 0x36;		//设置定时初值
	T4H = 0xf7;		//设置定时初值
	T4T3M |= 0x80;		//定时器4开始计时
	IE2 |= ET4;          //使能定时器中断
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
	if(TIM == Timer4)
	{
		T4T3M |= 0x80;		//定时器4开始计时
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
	if(TIM == Timer4)
	{
		T4T3M &= ~0x80;		//定时器4停止计时
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
	psofttimer->state = USING;
	TimerStart(Timer4);
}

void SoftTimerStop(_softtimer_t *psofttimer)
{
	psofttimer->cnt = 0;
	psofttimer->state = STOP;
	//TimerStop(Timer4);
}

