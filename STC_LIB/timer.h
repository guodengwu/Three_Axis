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

typedef struct _softtimer
{
	u8 TIM;
	u32 cnt;
	u32 period;
  	u8 state;
}_softtimer_t;

typedef enum {
	NONE = 0,
    STOP,
    USING,
} TimerState;

extern struct SYS_TIM  SysTim;
extern _softtimer_t Timer1Soft;
extern _softtimer_t Timer2Soft;
extern u8 _10ms_EVENT,_50ms_EVENT;
extern u8 _1s_EVENT;
extern u8 _3ms_EVENT;

void	SystickInit_Tmer(void);
void Timer4Init(void);
//void Timer2Init(void);
void TimerStart(u8 TIM);
void TimerStop(u8 TIM);
void TimerSetValue(u8 TIM, u16 value);
void SoftTimerStart(_softtimer_t *psofttimer, u32 value);
void SoftTimerStop(_softtimer_t *psofttimer);

#endif
