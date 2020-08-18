#include <STC8.H>
#include	"exti.h"

extern void JiaShouProcess(void);
void INT3_Isr() interrupt 11
{
	JiaShouProcess();
}

void INT3_INIT(void)
{
	INTCLKO = EX3;                              //Ê¹ÄÜINT3ÖÐ¶Ï
}