#include "sys.h"
#include "motor.h"

_sys_status_t Sys;
_sys_io_state_t IOState;
_dev_state_t DevState;
_sys_error_t SysHDError;
_sys_logic_error_t SysLogicErr;
static u8 MotorStuckMonitor(void);

void SysDataInit(void)
{
	Sys.state = SYSSTATE_NONE;
	Sys.DevAction = ActionState_Idle;
	
	IOState.state1.ubyte = DEF_False;
	IOState.state2.ubyte = DEF_False;
	IOState.HongWaiState.ubyte = DEF_False;
	
	DevState.ubyte = DEV_STATE_IDLE;//����״̬
	
	SysHDError.E1.ubyte = DEF_False;
	SysHDError.E2.ubyte = DEF_False;
	
	SysLogicErr.logic = 0;
}

void CheckIOState(void)
{
	IOState.state1.bits.b0 = !X_MOTOR_RightLimit_IN;	
	IOState.state1.bits.b1 = !X_MOTOR_LeftLimit_IN;
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if((X_MOTOR_LeftLimit_IN==0&&SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN) || \
		   (X_MOTOR_RightLimit_IN==0&&SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX))	{//x��������/���� ǿ��ֹͣ���
			StopXMotor();SYS_PRINTF("x stop, limit\r\n");
			SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Fail;
			Sys.DevAction = ActionState_Fail;
			if(X_MOTOR_LeftLimit_IN==0)	{
				SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Min_LimitOpt;
				XMotorResetCheck();
			}else if(X_MOTOR_RightLimit_IN==0)	{
				SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Max_LimitOpt;
			}
		}
	}
//	IOState.state1.bits.b2 = !CeMenCloseLimit_IN;
//	IOState.state1.bits.b3 = !CeMenOpenLimit_IN;
//	if(CeMenCloseLimit_IN==0 || CeMenOpenLimit_IN==0)	{//���ſ���/���ŵ�λ ǿ��ֹͣ���
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
	if(CeMenMinLimit_IN==0||CeMenMaxLimit_IN==0)	{//�ŵ����λ ��λʧЧʱ��ʱ20sֹͣ
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
	if(QuHuoKouOpenLimit_IN == 0||QuHuoKouCloseLimit_IN == 0)	{//ȡ���ڵ����λ  ��λʧЧʱ��ʱ10sֹͣ
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
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if((Y_MOTOR_MinLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN) || \
			(Y_MOTOR_MaxLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX))	{//y��������/���� ǿ��ֹͣ���
			StopYMotor();SYS_PRINTF("y stop, limit\r\n");
			SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Fail;
			Sys.DevAction = ActionState_Fail;
			if(Y_MOTOR_MinLimit_IN==0)	{
				SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Min_LimitOpt;
				YMotorResetCheck();
			}else if(Y_MOTOR_MaxLimit_IN==0)	{
				SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Max_LimitOpt;
			}
		}
	}
	if(HuoWuNearSwitch_IN==0)	{//����ӽ����ڣ������Ƹ˵��
		SysMotor.motor[MOTOR_T_ID].Param = 1;//ǰ��
		TMotorStart();
	}
	if(MotorStuckMonitor())	{
		MotorStop(DEF_Fail);SYS_PRINTF("MotorStuck\r\n");
		if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//�Ƹ˵�����ݸ��ź�ֹͣ
			SysMotor.motor[MOTOR_T_ID].status.action = ActionState_OK;
			Sys.DevAction = ActionState_OK;
		}
	}
}
u8 MotorStuckMonitorCnt = 0;
void ResetMotorStuckMonitorCnt(void)
{
	MotorStuckMonitorCnt = 0;
}

static u8 MotorStuckMonitor(void)
{
	if(ALLMOTOR_STUCK_IN == 1)	{//�е����ת�����е����ת�źŹ���
		MotorStuckMonitorCnt++;
		if(MotorStuckMonitorCnt>=11)	{//����120ms��ת�ź���Ч
			return 1;
		}
	}
	else {
		MotorStuckMonitorCnt = 0;
	}
	return 0;
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
