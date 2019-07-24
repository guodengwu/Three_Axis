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
	
	DevState.ubyte = DEV_STATE_IDLE;
	
	SysHDError.E1.ubyte = DEF_False;
	SysHDError.E2.ubyte = DEF_False;
	
	SysLogicErr.logic = 0;
}

void CheckIOState(void)
{
	IOState.state1.bits.b0 = X_MOTOR_RightLimit;
	IOState.state1.bits.b1 = X_MOTOR_LeftLimit;
	IOState.state1.bits.b2 = CeMenJinHuoKou;
	IOState.state1.bits.b3 = CeMenMaxLimit;
	IOState.state1.bits.b4 = CeMenMinLimit;
	IOState.state1.bits.b5 = ChuHuoKouOpen;
	IOState.state1.bits.b6 = ChuHuoKouClose;
	IOState.state1.bits.b7 = 0;
	
	IOState.state2.bits.b0 = Y_MOTOR_MinLimit;
	IOState.state2.bits.b1 = Y_MOTOR_MaxLimit;
}
