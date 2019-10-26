/**************************************************************************************************************************************************************/
#include "includes.h"
#include "protocol.h"
#include "motor.h"
#include "encoder.h"
#include "velocity_profile.h"

/******************************************************************
 - ʵ��ƽ̨��SW1A_51&ARM������
 - �������ƣ�main()
 - ����˵����������
 - ����˵������

 - ����˵������
 - ��ע˵����ʹ��51��Ƭ�����ⲿ����Ƶ��:22.1184MHZ-1Tģʽ
 ******************************************************************/
u8 xdata printfbuf[100];
void timer_event(void)
{
	if(_3ms_EVENT)	{
		_3ms_EVENT = 0;
		RS485TxToRx();
		CalcXYMotorPos();
	}
	if(_10ms_EVENT)	{
		_10ms_EVENT = 0;
		CheckIOState();		
		CheckMaPan();
		MotorStuck();		
	}
	if(_50ms_EVENT)	{
		_50ms_EVENT = 0;
		XMotorAccDec();
		YMotorAccDec();
	}
	if(_1s_EVENT)	{
		_1s_EVENT = 0;
		sprintf(printfbuf,"%d %ld",encoder[EncoderX_ID].pluse, SysMotor.motor[MOTOR_X_ID].CurPos);
		SYS_PRINTF("x pos:%s\r\n",printfbuf);
		sprintf(printfbuf,"%d %ld",encoder[EncoderY_ID].pluse, SysMotor.motor[MOTOR_Y_ID].CurPos);
		SYS_PRINTF("y pos:%s\r\n",printfbuf);
		soft_reset();
	}
}

void main(void)
{
	SystickInit_Tmer();
	bsp();	
	SysDataInit();
	MotorInit();
	ProDataInit();	
	VelocityProfileInit();
	ES = 1;
    EA = 1;
	//delay_us(30000);
	SYS_PRINTF("Sys Startup.\r\n");
	EncoderDataInit();
	MotorReset(MOTOR_X_ID);//X Y�����λ
	MotorReset(MOTOR_Y_ID);
	
	while(1)
	{
		timer_event();
		UsartCmdProcess();//����ָ�����
		UsartCmdReply();//����ָ��ظ�	
		XYMotorArrived();		
	}
} 