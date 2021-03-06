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
extern void ShipProcess(void);
u8 ssss=0;
void timer_event(void)
{
	if(_1ms_EVENT)	{
		_1ms_EVENT = 0;
		CalcXYMotorPos();
	}
	if(_3ms_EVENT)	{
		_3ms_EVENT = 0;
		RS485TxToRx();	
		QuHuoKouProcess();//取货门开关门处理
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
		XMotorAccDec();//加速过程返回1 不检测堵转
		YMotorAccDec();
	}
	if(_1s_EVENT)	{
		_1s_EVENT = 0;
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
	bsp();
	delay_us(1000);	
	SystickInit_Tmer();	
	SysDataInit();
	MotorInit();
	ProDataInit();	
	VelocityProfileInit();
	TimerStart(Timer4);
	INT3_INIT();
	ES = 1;
    EA = 1;	
	SYS_PRINTF("Sys Startup.\r\n");
	EncoderDataInit();
	SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
	QuHuoMenMotorStart(DEF_True);
	SysMotor.motor[MOTOR_T_ID].Param = 0;//0收缩
	TMotorStart();
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