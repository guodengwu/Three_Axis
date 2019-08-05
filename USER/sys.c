#include "sys.h"
#include "motor.h"

_sys_status_t Sys;
_sys_io_state_t IOState;
_dev_state_t DevState;
_sys_error_t SysHDError;
_sys_logic_error_t SysLogicErr;

void SysDataInit(void)
{
	Sys.state = SYSSTATE_NONE;
	Sys.DevAction = ActionState_Idle;
	
	IOState.state1.ubyte = DEF_False;
	IOState.state2.ubyte = DEF_False;
	IOState.HongWaiState.ubyte = DEF_False;
	
	DevState.ubyte = DEV_STATE_IDLE;//机器状态
	
	SysHDError.E1.ubyte = DEF_False;
	SysHDError.E2.ubyte = DEF_False;
	
	SysLogicErr.logic = 0;
}

void CheckIOState(void)
{
	IOState.state1.bits.b0 = X_MOTOR_RightLimit_IN;	
	IOState.state1.bits.b1 = X_MOTOR_LeftLimit_IN;
	if(X_MOTOR_RightLimit_IN==1 || X_MOTOR_LeftLimit_IN==1)	{//x碰到上限/下限 强制停止电机
		StopXMotor();
		SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK;
		if(X_MOTOR_RightLimit_IN==1)	{
			SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Min_LimitOpt;
		}else if(X_MOTOR_LeftLimit_IN==1)	{
			SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Max_LimitOpt;
		}
	}
	IOState.state1.bits.b2 = CeMenCloseLimit_IN;
	IOState.state1.bits.b3 = CeMenOpenLimit_IN;
	if(CeMenCloseLimit_IN==1 || CeMenOpenLimit_IN==1)	{//侧门开门/关门到位 强制停止电机
		StopDMotor();
		SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
		if(CeMenCloseLimit_IN==1)	{
			SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_CloseOpt;
		}else 		if(CeMenOpenLimit_IN==1)	{
			SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_OpenOpt;
		}
	}
	IOState.state1.bits.b4 = CeMenMaxLimit_IN;
	IOState.state1.bits.b5 = CeMenMinLimit_IN;
	if(CeMenMinLimit_IN==1||CeMenMaxLimit_IN==1)	{
		StopDMotor();
		SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
		if(CeMenMinLimit_IN==1)	{
			SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Min_LimitOpt;
		}else if(CeMenMaxLimit_IN==1)	{
			SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Max_LimitOpt;
		}
	}
	IOState.state1.bits.b6 = ChuHuoKouOpenLimit_IN;
	IOState.state1.bits.b7 = ChuHuoKouCloseLimit_IN;
	
	IOState.state2.bits.b0 = Y_MOTOR_MinLimit_IN;
	IOState.state2.bits.b1 = Y_MOTOR_MaxLimit_IN;
	if(Y_MOTOR_MinLimit_IN==1 || Y_MOTOR_MaxLimit_IN==1)	{//y碰到上限/下限 强制停止电机
		StopYMotor();
		SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
		if(Y_MOTOR_MinLimit_IN==1)	{
			SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Min_LimitOpt;
		}else if(Y_MOTOR_MaxLimit_IN==1)	{
			SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Max_LimitOpt;
		}
	}
}
