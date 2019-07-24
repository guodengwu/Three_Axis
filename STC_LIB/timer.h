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
    uint32_t  SumNumber;       //�ۼӼ���
    uint32_t  SumMs;          //�ۼ�ms��
    uint32_t  SumSec;         //�ۼ�����
    uint32_t  SumMinute;      //�ۼӷ�����
};
extern struct SYS_TIM  SysTim;
extern u8 _100ms_EVENT;
extern u8 _1s_EVENT;

void	SystickInit_Tmer0(void);
void TimerStart(u8 TIM);
void TimerStop(u8 TIM);
void TimerSetValue(u8 TIM, u16 value);

#endif
