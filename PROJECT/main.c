/**************************************************************************************************************************************************************/
#include "includes.h"
#include "protocol.h"
#include "motor.h"

/******************************************************************
 - ʵ��ƽ̨��SW1A_51&ARM������
 - �������ƣ�main()
 - ����˵����������
 - ����˵������

 - ����˵������
 - ��ע˵����ʹ��51��Ƭ�����ⲿ����Ƶ��:22.1184MHZ-1Tģʽ
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
		UsartCmdProcess();//����ָ�����
		UsartCmdReply();//����ָ��ظ�
		soft_reset();
	}
} 