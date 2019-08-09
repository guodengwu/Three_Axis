#include "motor.h"
#include "encoder.h"

SysMotor_t xdata SysMotor;

void MotorInit(void)
{
	u8 i;
	for(i=MOTOR_X_ID;i<MOTOR_QuHuoMen_ID;i++)	{
		SysMotor.motor[i].id = i;
		SysMotor.motor[i].status.is_run = MotorState_Stop;
		SysMotor.motor[i].status.action = ActionState_Idle;
		SysMotor.motor[i].CurPos = 0;
	}
	SysMotor.MotorIDRunning = 0xff;
	SysMotor.ALLMotorState.ubyte = 0;
}
/*
void MotorSetDir(void)
{

}

void MotorEnable(void)
{

}

void MotorDisable(void)
{

}*/
void MotorReset(u8 id)
{
	if(id == MOTOR_X_ID)	{
		Sys.state |= SYSSTATE_XMOTORRESET;
		SysMotor.motor[MOTOR_X_ID].dir = MOTOR_TO_MIN;
		SysMotor.ALLMotorState.bits.XMotor = DEF_Run;
	}else 	if(id == MOTOR_Y_ID)	{
		Sys.state |= SYSSTATE_YMOTORRESET;
		SysMotor.motor[MOTOR_Y_ID].dir = MOTOR_TO_MIN;
		SysMotor.ALLMotorState.bits.YMotor = DEF_Run;
	}
	MotorStart();
}

void XYMotorResetCheck()
{
	if(Sys.state & SYSSTATE_XMOTORRESET)	{
		if(SysMotor.motor[MOTOR_X_ID].status.abort_type == MotorAbort_Min_LimitOpt)	{
			Sys.state &= ~SYSSTATE_XMOTORRESET;
			//StopXMotor();
			SysMotor.motor[MOTOR_X_ID].CurPos = 0;
			SYS_PRINTF("x motor reset ok.\r\n");
		}
	}
	if(Sys.state & SYSSTATE_YMOTORRESET)	{
		if(SysMotor.motor[MOTOR_Y_ID].status.abort_type == MotorAbort_Min_LimitOpt)	{
			Sys.state &= ~SYSSTATE_YMOTORRESET;
			//StopYMotor();
			SysMotor.motor[MOTOR_Y_ID].CurPos = 0;
			SYS_PRINTF("y motor reset ok.\r\n");
		}
	}
}

void CalcXYMotorPos(void)
{
	/*if((SysMotor.ALLMotorState.bits.XMotor != DEF_Run) || (SysMotor.ALLMotorState.bits.YMotor != DEF_Run))	{
		return;
	}*/
	ReadEncoder(&SysMotor.motor[MOTOR_X_ID]);	
	ReadEncoder(&SysMotor.motor[MOTOR_Y_ID]);

	SysMotor.motor[MOTOR_X_ID].CurPos = encoder[EncoderX_ID].pluse*XMotor_StepsPerum;	
	SysMotor.motor[MOTOR_Y_ID].CurPos = encoder[EncoderY_ID].pluse*YMotor_StepsPerum;	
}

void MotorStart(void)
{
	u8 runing_id;
	u32 motor_timeout;
	
	if(SysMotor.ALLMotorState.ubyte == 0)	{//没有电机需要运行
		return;
	}
	runing_id = SysMotor.MotorIDRunning;
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{//测试x电机
		if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN)	{
			X_MOTOR_PWM1 = 1;
			X_MOTOR_PWM2 = 0;
		}
		else if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX)	{
			X_MOTOR_PWM1 = 0;
			X_MOTOR_PWM2 = 1;
		}
		X_MOTOR_ENABLE1 = 0;
		X_MOTOR_ENABLE2 = 1;
		motor_timeout = 3000;//30s
		//BSP_PRINTF("x motor testing.\r\n");
		//SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Doing;
		
	}else	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{//测试y电机
		if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN)	{
			Y_MOTOR_PWM1 = 1;
			Y_MOTOR_PWM2 = 0;
		}
		else if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX)	{
			Y_MOTOR_PWM1 = 0;
			Y_MOTOR_PWM2 = 1;
		}
		Y_MOTOR_ENABLE1 = 0;
		Y_MOTOR_ENABLE2 = 1;
		motor_timeout = 3000;
		//SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Doing;
	}
	else	if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{//测试z电机
		Z_MOTOR_PWM1 = 1;
		Z_MOTOR_PWM2 = 0;
		//SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_Z_ID].Param*10);	//运行时间	
		Z_MOTOR_ENABLE = 1;
		motor_timeout = 3000;
		//SysMotor.motor[MOTOR_Z_ID].status.action = ActionState_Doing;
		//return;
	}
	else	if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//测试推杆电机
		if(SysMotor.motor[MOTOR_T_ID].Param==0)	{//0收缩 ， 1延伸
			T_MOTOR_PWM1 = 1;
			T_MOTOR_PWM2 = 0;			
		}
		else if(SysMotor.motor[MOTOR_T_ID].Param==1)	{
			T_MOTOR_PWM1 = 0;
			T_MOTOR_PWM2 = 1;
		}
		T_MOTOR_ENABLE = 1;
		motor_timeout = 1000;//10s
		//SysMotor.motor[MOTOR_T_ID].status.action = ActionState_Doing;
	}
	else	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{//测试侧门电机
		if(SysMotor.motor[MOTOR_D_ID].Param==0)	{//0关门 ， 1开门
			D_MOTOR_PWM1 = 1;
			D_MOTOR_PWM2 = 0;
		}
		else if(SysMotor.motor[MOTOR_D_ID].Param==1)	{
			D_MOTOR_PWM1 = 0;
			D_MOTOR_PWM2= 1;
		}
		D_MOTOR_ENABLE = 1;
		motor_timeout = 1000;
		//SysMotor.motor[MOTOR_D_ID].status.action = ActionState_Doing;
	}
	else	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{//测试履带电机
		L_MOTOR_PWM1 = 1;
		L_MOTOR_PWM2 = 0;
		L_MOTOR_ENABLE = 1;
		motor_timeout = 3000;
		//SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_L_ID].Param*10);//运行时间
		//SysMotor.motor[MOTOR_L_ID].status.action = ActionState_Doing;
		//return;
	}
	else	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{//测试取货门电机
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==0)	{//0关门 ， 1开门
			QuHuoMen_MOTOR_PWM1 = 1;
			QuHuoMen_MOTOR_PWM1 = 0;
		}
		else if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==1)	{
			QuHuoMen_MOTOR_PWM1 = 0;
			QuHuoMen_MOTOR_PWM1= 1;
		}
		QuHuoMen_MOTOR_ENABLE = 1;
		motor_timeout = 1000;
	}
	SysMotor.motor[runing_id].status.action = ActionState_Doing;
	SoftTimerStart(&Timer2Soft, motor_timeout);//电机运行超时控制
}

//
void MotorStop(u8 stop_type)
{
	u8 runing_id;
	
	if(SysMotor.ALLMotorState.ubyte == 0)	{//没有电机需要运行
		return;
	}
	runing_id = SysMotor.MotorIDRunning;
	if(DevState.bits.State == DEV_STATE_TEST)	{
		DevState.bits.SubState = 0;//没有电机运行
	}
	
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{//停止x电机
		StopXMotor();
	}else	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{//停止y电机
		StopYMotor();

	}
	else	if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{//停止z电机
		StopZMotor();

	}
	else	if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//停止推杆电机
		StopTMotor();

	}
	else	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{//停止侧门电机
		StopDMotor();

	}
	else	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{//停止履带电机
		StopLMotor();
	}
	else	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{//停止取货门电机
		StopQuHuoMenMotor();		
	}
	if(stop_type==DEF_Success)	{
		SysMotor.motor[runing_id].status.action = ActionState_OK;
		SysMotor.motor[runing_id].status.abort_type = MotorAbort_Normal;
	}else if(stop_type==DEF_Fail)	{
		SysMotor.motor[runing_id].status.action = ActionState_Fail;
		SysMotor.motor[runing_id].status.abort_type = MotorAbort_Timeout;
	}
}

void MotorTest(void)
{
	if(DevState.bits.State != DEV_STATE_TEST)	{
		return;
	}
	
	DevState.bits.SubState = 1;//电机运行中
	MotorStart();
	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{
		SoftTimerStart(Timer1, SysMotor.motor[MOTOR_L_ID].Param*10);
	}
	else if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{
		SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_Z_ID].Param*10);
	}
}	


void StopXMotor(void)
{
	X_MOTOR_ENABLE1 = 0;
	X_MOTOR_ENABLE2 = 0;
	SysMotor.ALLMotorState.bits.XMotor = DEF_Stop;
}

void StopYMotor(void)
{
	Y_MOTOR_ENABLE1 = 0;
	Y_MOTOR_ENABLE2 = 0;
	SysMotor.ALLMotorState.bits.YMotor = DEF_Stop;
}

void StopZMotor(void)
{
	Z_MOTOR_PWM1 = 0;
	Z_MOTOR_PWM2 = 0;		
	Z_MOTOR_ENABLE = 0;
	SysMotor.ALLMotorState.bits.ZMotor = DEF_Stop;
}

void StopTMotor(void)
{
	T_MOTOR_PWM1 = 0;
	T_MOTOR_PWM2 = 0;
	T_MOTOR_ENABLE = 0;	
	SysMotor.ALLMotorState.bits.TMotor = DEF_Stop;
}

void StopDMotor(void)
{
	D_MOTOR_PWM1 = 0;
	D_MOTOR_PWM2 = 0;
	D_MOTOR_ENABLE = 0;
	SysMotor.ALLMotorState.bits.DMotor = DEF_Stop;
}

void StopLMotor(void)
{
	L_MOTOR_ENABLE = 0;
	SysMotor.ALLMotorState.bits.LMotor = DEF_Stop;
}
void StopQuHuoMenMotor(void)
{
	QuHuoMen_MOTOR_ENABLE = 0;
	SysMotor.ALLMotorState.bits.QuHuoMenMotor =  DEF_Stop;
}
