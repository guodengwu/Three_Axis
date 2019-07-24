#ifndef	__TIMER_H
#define	__TIMER_H

#include	"includes.h"

#define	Timer0						0
#define	Timer1						1
#define	Timer2						2
#define	Timer3						3
#define	Timer4						4

struct SYS_TIM
{
    uint32_t  SumNumber;       //累加计数
    uint32_t  SumMs;          //累加ms数
    uint32_t  SumSec;         //累加秒数
    uint32_t  SumMinute;      //累加分钟数
};
extern struct SYS_TIM  SysTim;
extern u8 _100ms_EVENT;
extern u8 _1s_EVENT;

void	SystickInit_Tmer0(void);
void TimerStart(u8 TIM);
void TimerStop(u8 TIM);
void TimerSetValue(u8 TIM, u16 value);

#endif
