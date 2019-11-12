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
	IOState.state1.bits.b0 = !X_MOTOR_RightLimit_IN;	
	IOState.state1.bits.b1 = !X_MOTOR_LeftLimit_IN;
	if(X_MOTOR_RightLimit_IN==0 || X_MOTOR_LeftLimit_IN==0)	{//x碰到上限/下限 强制停止电机
		StopXMotor();
		SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Fail;
		Sys.DevAction = ActionState_Fail;
		if(X_MOTOR_RightLimit_IN==0)	{
			SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Min_LimitOpt;
			XMotorResetCheck();
		}else if(X_MOTOR_LeftLimit_IN==0)	{
			SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Max_LimitOpt;
		}
	}
//	IOState.state1.bits.b2 = !CeMenCloseLimit_IN;
//	IOState.state1.bits.b3 = !CeMenOpenLimit_IN;
//	if(CeMenCloseLimit_IN==0 || CeMenOpenLimit_IN==0)	{//侧门开门/关门到位 强制停止电机
//		StopDMotor();
//		SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
//		if(CeMenCloseLimit_IN==0)	{
//			SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_CloseOpt;
//		}else 		if(CeMenOpenLimit_IN==0)	{
//			SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_OpenOpt;
//		}
//	}
	IOState.state1.bits.b4 = !CeMenMaxLimit_IN;
	IOState.state1.bits.b5 = !CeMenMinLimit_IN;
	if(CeMenMinLimit_IN==0||CeMenMaxLimit_IN==0)	{//门电机限位 限位失效时超时20s停止
		StopDMotor();
		SoftTimerStop(&Timer2Soft);
		SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
		if(CeMenMinLimit_IN==0)	{
			SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Min_LimitOpt;
		}else if(CeMenMaxLimit_IN==0)	{
			SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Max_LimitOpt;
		}
	}
	IOState.state1.bits.b6 = !QuHuoKouOpenLimit_IN;
	IOState.state1.bits.b7 = !QuHuoKouCloseLimit_IN;
	if(QuHuoKouOpenLimit_IN == 0||QuHuoKouCloseLimit_IN == 0)	{//取货口电机限位  限位失效时超时10s停止
		StopQuHuoMenMotor();
		SoftTimerStop(&Timer2Soft);
		SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_OK;
		if(QuHuoKouOpenLimit_IN==0)	{
			SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Min_LimitOpt;
		}else if(QuHuoKouCloseLimit_IN==0)	{
			SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Max_LimitOpt;
		}
	}
	
	IOState.state2.bits.b0 = !Y_MOTOR_MinLimit_IN;
	IOState.state2.bits.b1 = !Y_MOTOR_MaxLimit_IN;
	if(Y_MOTOR_MinLimit_IN==0 || Y_MOTOR_MaxLimit_IN==0)	{//y碰到上限/下限 强制停止电机
		StopYMotor();
		SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Fail;
		Sys.DevAction = ActionState_Fail;
		if(Y_MOTOR_MinLimit_IN==0)	{
			SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Min_LimitOpt;
			YMotorResetCheck();
		}else if(Y_MOTOR_MaxLimit_IN==0)	{
			SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Max_LimitOpt;
		}
	}
	if(HuoWuNearSwitch_IN==0)	{//货物接近出口，启动推杆电机
		SysMotor.motor[MOTOR_T_ID].Param = 1;//前推
		TMotorStart();
	}
	if(ALLMOTOR_STUCK_IN == 1)	{//有电机堵转，所有电机堵转信号共用
		MotorStop(DEF_Fail);
		if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//推杆电机根据该信号停止
			SysMotor.motor[MOTOR_T_ID].status.action = ActionState_OK;
			Sys.DevAction = ActionState_OK;
		}
	}
}


/*void CheckDevAction(void)
{
	u8 runing_id;
	
	runing_id = SysMotor.MotorIDRunning;
	if(DevState.bits.SubState == 1)
		Sys.DevAction == ActionState_Busy;
	else 
		Sys.DevAction == ActionState_Idle;
}*/
