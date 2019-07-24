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
	if(_100ms_EVENT)	{
		_100ms_EVENT = 0;
		CheckIOState();
	}
}

void main()
{
	SystickInit_Tmer0();
	bsp();
	MotorInit();
	ProDataInit();
	SYS_PRINTF("Sys Startup.\r\n");
	while(1)
	{
		timer_event();
		UsartCmdProcess();//����ָ�����
		UsartCmdReply();//����ָ��ظ�
		soft_reset();
	}
} 