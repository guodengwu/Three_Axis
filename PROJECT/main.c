/**************************************************************************************************************************************************************/
#include "includes.h"
#include "protocol.h"
#include "motor.h"

/******************************************************************
 - 实验平台：SW1A_51&ARM开发板
 - 函数名称：main()
 - 功能说明：主程序
 - 参数说明：无

 - 返回说明：无
 - 备注说明：使用51单片机，外部晶体频率:22.1184MHZ-1T模式
 ******************************************************************/
void timer_event(void)
{
	if(_10ms_EVENT)	{
		_10ms_EVENT = 0;
		CheckIOState();
	}
	if(_1s_EVENT)	{
		_1s_EVENT = 0;
		//usart.tx_cmd = _CMD_TX_GET_VERSION;
	}
}

void main(void)
{
	SystickInit_Tmer0();
	bsp();	
	SysDataInit();
	MotorInit();
	ProDataInit();	
	ES = 1;
    EA = 1;
	SYS_PRINTF("Sys Startup.\r\n");
	while(1)
	{
		timer_event();
		UsartCmdProcess();//串口指令处理函数
		UsartCmdReply();//串口指令回复
		soft_reset();
	}
} 