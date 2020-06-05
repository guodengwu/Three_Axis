/**************************************************************************************************************************************************************/
#include "includes.h"
#include "protocol.h"
#include "motor.h"
#include "encoder.h"
#include "velocity_profile.h"

/******************************************************************
 - 实验平台：SW1A_51&ARM开发板
 - 函数名称：main()
 - 功能说明：主程序
 - 参数说明：无

 - 返回说明：无
 - 备注说明：使用51单片机，外部晶体频率:22.1184MHZ-1T模式
 ******************************************************************/
//u8 xdata printfbuf[100];
void timer_event(void)
{
	if(_1ms_EVENT)	{
		_1ms_EVENT = 0;
		CalcXYMotorPos();
	}
	if(_3ms_EVENT)	{
		_3ms_EVENT = 0;
		RS485TxToRx();	
//		CalcXYMotorPos();		
	}
	if(_10ms_EVENT)	{
		_10ms_EVENT = 0;
		CheckIOState();		
		CheckMaPan();
		//MotorStuck();
//		JiaShouProcess();
	}
	if(_30ms_EVENT)	{
		_30ms_EVENT = 0;
//		XMotorAccDec();//加速过程返回1 不检测堵转
		YMotorAccDec();
	}
	if(_1s_EVENT)	{
		_1s_EVENT = 0;
//		sprintf(printfbuf,"%d %ld",encoder[EncoderX_ID].pluse, SysMotor.motor[MOTOR_X_ID].CurPos);
//		SYS_PRINTF("x pos:%s\r\n",printfbuf);
//		sprintf(printfbuf,"%d %ld",encoder[EncoderY_ID].pluse, SysMotor.motor[MOTOR_Y_ID].CurPos);
//		SYS_PRINTF("y pos:%s\r\n",printfbuf);
		soft_reset();
		ShipProcess();
	}
}

//void CheckDevAction(void)
//{
//	u8 i,id;
//	
//	for(i=0;i<;i++)	{
//		if(SysMotor.motor[i].status.action == ActionState_Doing)	{
//			Sys.DevAction = ActionState_Doing;
//			return;
//		}
//	}
////	id = SysMotor.RunningID;
////	Sys.DevAction = SysMotor.motor[id].status.action;
//} 

void main(void)
{
	delay_us(2000);
	SystickInit_Tmer();
	bsp();	
	SysDataInit();
	MotorInit();
	ProDataInit();	
	VelocityProfileInit();
	ES = 1;
    EA = 1;	
	SYS_PRINTF("Sys Startup.\r\n");
	EncoderDataInit();
	SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
	QuHuoMenMotorStart();
	MotorReset(MOTOR_X_ID);//X 电机先复位
//	for(;;)	{
//		if(!(Sys.state & SYSSTATE_XMOTORRESET))	{//x 复位成功后才能动y
//			MotorReset(MOTOR_Y_ID);
//			break;
//		}
//	}
	
	while(1)
	{
		timer_event();
		UsartCmdProcess();//串口指令处理函数
		UsartCmdReply();//串口指令回复	
		XYMotorArrived();
//		CheckIOState();			
	}
} 