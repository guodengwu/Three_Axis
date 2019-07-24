#include "motor.h"

SysMotor_t SysMotor;

void MotorInit(void)
{
	u8 i;
	for(i=MOTOR_X;i<MOTOR_QuHuoMen;i++)	{
		SysMotor.motor[i].status.is_run = MotorState_Stop;
		SysMotor.motor[i].status.action = ActionState_Idle;
		SysMotor.motor[i].CurPos = 0;
	}
	SysMotor.ALLMotorState.ubyte = 0;
}

void MotorSetDir(void)
{

}

void MotorEnable(void)
{

}

void MotorDisable(void)
{

}

void MotorTest(void)
{
	if(DevState.bits.State != DEV_STATE_TEST)	{
		return;
	}
	if(SysMotor.ALLMotorState.bits.b0 == DEF_Busy)	{//测试x电机
		
	}else	if(SysMotor.ALLMotorState.bits.b1 == DEF_Busy)	{//测试y电机
		
	}
	else	if(SysMotor.ALLMotorState.bits.b3 == DEF_Busy)	{//测试推杆电机
		
	}
	else	if(SysMotor.ALLMotorState.bits.b4 == DEF_Busy)	{//测试侧门电机
		
	}
	else	if(SysMotor.ALLMotorState.bits.b5 == DEF_Busy)	{//测试取货口电机
		
	}else	if(SysMotor.ALLMotorState.bits.b6 == DEF_Busy)	{//测试取货门电机
		
	}
}	

