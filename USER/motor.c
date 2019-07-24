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
