#include "motor.h"
#include "encoder.h"
#include "velocity_profile.h"
#include "bsp.h"

SysMotor_t xdata SysMotor;
typedef struct {
    INT16S   AccPos;
    INT16S   DecPos;
}AccDecPos;

AccDecPos XAccDecPos;
AccDecPos YAccDecPos;

void MotorInit(void)
{
	u8 i;
	for(i=MOTOR_X_ID;i<=MOTOR_QuHuoMen_ID;i++)	{
		SysMotor.motor[i].id = i;
		SysMotor.motor[i].status.is_run = MotorState_Stop;
		SysMotor.motor[i].status.action = ActionState_Idle;
		SysMotor.motor[i].status.abort_type = MotorAbort_Normal;
		SysMotor.motor[i].CurPos = 0;
		SysMotor.motor[i].ObjPos = -20000;
		SysMotor.pTimer[i] = &TimerSoft[i];
	}
	SysMotor.pTimer[MOTOR_X_ID]->pCallBack = &StopXMotor;
	SysMotor.pTimer[MOTOR_Y_ID]->pCallBack = &StopYMotor;
	SysMotor.pTimer[MOTOR_Z_ID]->pCallBack = &StopZMotor;
	SysMotor.pTimer[MOTOR_T_ID]->pCallBack = &StopTMotor;
	SysMotor.pTimer[MOTOR_D_ID]->pCallBack = &StopDMotor;
	SysMotor.pTimer[MOTOR_L_ID]->pCallBack = &StopLMotor;
	SysMotor.pTimer[MOTOR_QuHuoMen_ID]->pCallBack = &StopQuHuoMenMotor;
	
	XAccDecPos.DecPos = 0;
	YAccDecPos.DecPos = 0;
	SysMotor.RunningID = 0;
	SysMotor.ALLMotorState.ubyte = 0;
}

void XMotorSetDir(void)
{
	if(SysMotor.motor[MOTOR_X_ID].CurPos >= SysMotor.motor[MOTOR_X_ID].ObjPos)	{
		SysMotor.motor[MOTOR_X_ID].dir = MOTOR_TO_MIN;
	}
	else
		SysMotor.motor[MOTOR_X_ID].dir = MOTOR_TO_MAX;
}
void YMotorSetDir(void)
{
	if(SysMotor.motor[MOTOR_Y_ID].CurPos >= SysMotor.motor[MOTOR_Y_ID].ObjPos)	{
		SysMotor.motor[MOTOR_Y_ID].dir = MOTOR_TO_MIN;
	}
	else
		SysMotor.motor[MOTOR_Y_ID].dir = MOTOR_TO_MAX;
}

void CalcXYMotorUpDownPos(u8 id)
{
	s16 len;
	u16 totalLen,temp;
	
	if(id == MOTOR_X_ID)	{
		len = SysMotor.motor[MOTOR_X_ID].ObjPos - SysMotor.motor[MOTOR_X_ID].CurPos;
		totalLen = abs(len);
		if(totalLen<=MOTOR_LEN_RANG)	{//移动距离在运行控制精度内 不启动电机
			SYS_PRINTF("X not need run ");
			XAccDecPos.DecPos = -1;
			return;
		}
		else if(totalLen <= MOTOR_CONSTANT_LEN)	{//移动距离在加减速距离内 匀速运行
			SYS_PRINTF("X constant run ");
			XAccDecPos.DecPos = 0;
			return;
		}
		else if(totalLen > XMOTOR_AccDec_LEN*2)	{
			temp = XMOTOR_AccDec_LEN;
		}
		else {
			temp = totalLen/3;
		}
		if(len>=0)	{
			XAccDecPos.DecPos = SysMotor.motor[MOTOR_X_ID].ObjPos - temp;
			SYS_PRINTF("X MAX ");
		}
		else	{
			XAccDecPos.DecPos = SysMotor.motor[MOTOR_X_ID].ObjPos + temp;
			SYS_PRINTF("X MIN ");
		}
		SYS_PRINTF("C:%ld O:%ld D:%d\r\n",SysMotor.motor[MOTOR_X_ID].CurPos,SysMotor.motor[MOTOR_X_ID].ObjPos,XAccDecPos.DecPos);
	}
	if(id == MOTOR_Y_ID)	{
		len = SysMotor.motor[MOTOR_Y_ID].ObjPos - SysMotor.motor[MOTOR_Y_ID].CurPos;
		totalLen = abs(len);
		if(totalLen<=MOTOR_LEN_RANG)	{
			SYS_PRINTF("Y not need run ");
			YAccDecPos.DecPos = -1;
			return;
		}
		else if(totalLen <= MOTOR_CONSTANT_LEN)	{
			SYS_PRINTF("Y constant run ");
			YAccDecPos.DecPos = 0;
			return;
		}
		if(len>=0)	{//上行
			if(totalLen > YMOTOR_AccDec_UpLen*2)	{
				temp = YMOTOR_AccDec_UpLen;
			}
			else {
				temp = totalLen/2;
			}
			YAccDecPos.DecPos = SysMotor.motor[MOTOR_Y_ID].ObjPos - temp;
			SYS_PRINTF("Y MAX ");
		}
		else	{//下行
			if(totalLen > YMOTOR_AccDec_DnLen*2)	{
				temp = YMOTOR_AccDec_DnLen;
			}
			else {
				temp = totalLen/2;
			}
			YAccDecPos.DecPos = SysMotor.motor[MOTOR_Y_ID].ObjPos + temp;
			SYS_PRINTF("Y MIN ");
		}
		SYS_PRINTF("C:%ld O:%ld D:%d\r\n",SysMotor.motor[MOTOR_Y_ID].CurPos,SysMotor.motor[MOTOR_Y_ID].ObjPos,YAccDecPos.DecPos);
	}
}
#define XMotorMAXSpeedIdx	10//x电机最大速度PWM占空比80%
void MotorReset(u8 id)
{
	if(id == MOTOR_X_ID)	{
		if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)
			return;
		Sys.state |= SYSSTATE_XMOTORRESET;
		SysMotor.motor[MOTOR_X_ID].dir = MOTOR_TO_MIN;
		SysMotor.ALLMotorState.bits.XMotor = DEF_Run;
		if(X_MOTOR_LeftLimit_IN==0)	{//已经在零点位置
			_nop_();_nop_();
			if(X_MOTOR_LeftLimit_IN==0)
				return;
		}
		X_VelCurve.index = 0;
		if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN)	{
			X_MOTOR_PWM2 = 0;
			X_MOTOR_PWM1 = 1;
			StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index++]);			
		}
		else if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX)	{
			X_MOTOR_PWM1 = 0;
			X_MOTOR_PWM2 = 1;
			StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index++]);
		}
		XAccDecPos.DecPos = 0;
		SysMotor.RunningID = MOTOR_X_ID;		
		SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Doing;	
//		SoftTimerStart(SysMotor.pTimer[MOTOR_X_ID], 30000);
	}else 	if(id == MOTOR_Y_ID)	{
		if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)
			return;
		Sys.state |= SYSSTATE_YMOTORRESET;
		SysMotor.motor[MOTOR_Y_ID].dir = MOTOR_TO_MIN;
		SysMotor.ALLMotorState.bits.YMotor = DEF_Run;
		if(Y_MOTOR_MinLimit_IN==0)	{//已经在零点位置
			_nop_();_nop_();
			if(Y_MOTOR_MinLimit_IN==0)
				return;
		}
		Y_VelCurve.index = 0;
		if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN)	{
			Y_MOTOR_PWM2 = 0;
			StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index++]);		
		}
		else if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX)	{
			Y_MOTOR_PWM1 = 0;
			StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index++]);
		}
		YAccDecPos.DecPos = 0;
		SysMotor.RunningID = MOTOR_Y_ID;		
		SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Doing;	
//		SoftTimerStart(SysMotor.pTimer[MOTOR_Y_ID], 30000);
	}
	if(Sys.DevAction != ActionState_Doing)	{//非出货期间的复位
		DevState.bits.State = DEV_STATE_RESET;//复位中
		DevState.bits.SubState = 0x04;//表示三轴板电机正在复位
	}
//	SoftTimerStart(&Timer2Soft, 30000);//电机复位超时控制
}

void XMotorResetCheck()
{
	if(Sys.state & SYSSTATE_XMOTORRESET)	{
		if(SysMotor.motor[MOTOR_X_ID].status.abort_type == MotorAbort_Min_LimitOpt)	{
			Sys.state &= ~SYSSTATE_XMOTORRESET;
			//StopXMotor();
			encoder[EncoderX_ID].pluse = 0;
			SysMotor.motor[MOTOR_X_ID].CurPos = 0;
			SYS_PRINTF("x motor reset ok.\r\n");
			MotorReset(MOTOR_Y_ID);
			SysMotor.motor[MOTOR_D_ID].Param=DEF_Close;
			DMotorStart();
		}
	}
}
void YMotorResetCheck()
{
	if(Sys.state & SYSSTATE_YMOTORRESET)	{
		if(SysMotor.motor[MOTOR_Y_ID].status.abort_type == MotorAbort_Min_LimitOpt)	{
			Sys.state &= ~SYSSTATE_YMOTORRESET;
			//StopYMotor();
			encoder[EncoderY_ID].pluse = 0;
			SysMotor.motor[MOTOR_Y_ID].CurPos = 0;
			SYS_PRINTF("y motor reset ok.\r\n");
			if(Sys.DevAction != ActionState_Doing)	{//非出货期间的复位
				DevState.bits.State = DEV_STATE_IDLE;//
				DevState.bits.SubState = DEV_ShipSubStateIDLE;
			}
		}
	}
}

void CalcXYMotorPos(void)
{
//	ReadEncoder(&SysMotor.motor[MOTOR_X_ID]);	
//	ReadEncoder(&SysMotor.motor[MOTOR_Y_ID]);

//	SysMotor.motor[MOTOR_X_ID].CurPos = (encoder[EncoderX_ID].pluse*53)/10;	
	SysMotor.motor[MOTOR_X_ID].CurPos = encoder[EncoderX_ID].pluse*XMaPan_NumPerStep;
	SysMotor.motor[MOTOR_Y_ID].CurPos = encoder[EncoderY_ID].pluse*YMaPan_NumPerStep;	
}
//码盘异常检测 在运动状态下 持续3s位置无变化
void CheckMaPan(void)
{
	static s32 XCurPos=0,YCurPos=0;
	static u16 XPosChangeCnt=0,YPosChangeCnt=0;
	
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if(XCurPos==SysMotor.motor[MOTOR_X_ID].CurPos)	{
			XPosChangeCnt++;
			if(XPosChangeCnt>100)	{
				SysHDError.E2.bits.b2 = 1;//X 码盘异常
				StopXMotor();
				SYS_PRINTF("x mapan error\r\n");
				SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_MaPanError;
				SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Fail;
//				Sys.DevAction = ActionState_Fail;
			}
		}
		else	{
			XCurPos = SysMotor.motor[MOTOR_X_ID].CurPos;
			XPosChangeCnt = 0;
			SysHDError.E2.bits.b2 = 0;
		}
	}
	else 
		XPosChangeCnt = 0;
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if(YCurPos==SysMotor.motor[MOTOR_Y_ID].CurPos)	{
			YPosChangeCnt++;
			if(YPosChangeCnt>100)	{
				SysHDError.E2.bits.b3 = 1;//Y 码盘异常
				StopYMotor();
				SYS_PRINTF("y mapan error\r\n");
				SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_MaPanError;
				SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Fail;
//				Sys.DevAction = ActionState_Fail;
			}
		}
		else	{
			YCurPos = SysMotor.motor[MOTOR_Y_ID].CurPos;
			YPosChangeCnt = 0;
			SysHDError.E2.bits.b3 = 0;
		}
	}
	else
		YPosChangeCnt = 0;
}

void XYMotorArrived(void)
{
	//s32 max_limit,min_limit;
	
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if(Sys.state & SYSSTATE_XMOTORRESET)
			return;
		if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN&&SysMotor.motor[MOTOR_X_ID].CurPos <= SysMotor.motor[MOTOR_X_ID].ObjPos)	{
			StopXMotor();
			SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Normal;
			SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK;
			SYS_PRINTF("MIN x motor arrived.%ld\r\n",SysMotor.motor[MOTOR_X_ID].CurPos);
		}
		else if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX&&SysMotor.motor[MOTOR_X_ID].CurPos >= SysMotor.motor[MOTOR_X_ID].ObjPos)	{
			StopXMotor();
			SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Normal;
			SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK;
			SYS_PRINTF("MAX x motor arrived.%ld\r\n",SysMotor.motor[MOTOR_X_ID].CurPos);
		}
	}
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if(Sys.state & SYSSTATE_YMOTORRESET)
			return;
		if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN&&SysMotor.motor[MOTOR_Y_ID].CurPos <= SysMotor.motor[MOTOR_Y_ID].ObjPos)	{
			StopYMotor();
			SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Normal;
			SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
			SYS_PRINTF("y motor arrived.%ld,%ld\r\n",SysMotor.motor[MOTOR_Y_ID].CurPos, encoder[EncoderY_ID].pluse);
		}
		else if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX&&SysMotor.motor[MOTOR_Y_ID].CurPos >= SysMotor.motor[MOTOR_Y_ID].ObjPos)	{
			StopYMotor();
			SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Normal;
			SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
			SYS_PRINTF("y motor arrived.%ld,%ld\r\n",SysMotor.motor[MOTOR_Y_ID].CurPos, encoder[EncoderY_ID].pluse);
		}
	}
//	if(SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{
//		if(DevState.bits.State == DEV_STATE_SHIPING && DevState.bits.SubState == DEV_ShipSubStateIDLE)	{
//			DevState.bits.SubState = DEV_ShipSubStateStartZmotor;//出货流程 x y到位后 启动z
//		}
//	}
}

u8 XMotorAccDec(void)
{
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if(XAccDecPos.DecPos == 0)	{//匀速
			if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN)	{//后退
				X_MOTOR_PWM2 = 0;
				X_MOTOR_PWM1 = 1;
				StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[1]);
			}
			else /*if(SysMotor.motor[MOTOR_X_ID].CurPos<=XAccDecPos.DecPos)*/	{//前进
				X_MOTOR_PWM1 = 0;
				X_MOTOR_PWM2 = 1;
				StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[3]);
			}
		}
		else	{
			if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN)	{
				if(SysMotor.motor[MOTOR_X_ID].CurPos>XAccDecPos.DecPos)	{//加速
					if(X_VelCurve.index < XMotorMAXSpeedIdx)	{
						StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index]);						
//						SYS_PRINTF("%d ",X_VelCurve.Curve[X_VelCurve.index]);
						X_VelCurve.index++;	
						return 1;
					}
				}else if(SysMotor.motor[MOTOR_X_ID].CurPos<=XAccDecPos.DecPos)	{//减速
					if(X_VelCurve.index < 0)
						X_VelCurve.index = 0;
					else if(X_VelCurve.index > 0)	{
						if(X_VelCurve.index > 3)
							X_VelCurve.index -= 3;
						else
							X_VelCurve.index--;
					}
					StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index]);			
					SYS_PRINTF("%d-%ld ",X_VelCurve.Curve[X_VelCurve.index],SysMotor.motor[MOTOR_X_ID].CurPos);					
				}
			}else if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX){
				if(SysMotor.motor[MOTOR_X_ID].CurPos<XAccDecPos.DecPos)	{//加速
					if(X_VelCurve.index < XMotorMAXSpeedIdx)	{
						StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index]);						
//						SYS_PRINTF("%d ",X_VelCurve.Curve[X_VelCurve.index]);
						X_VelCurve.index++;	
						return 1;
					}
				}else if(SysMotor.motor[MOTOR_X_ID].CurPos>=XAccDecPos.DecPos)	{//减速
					if(X_VelCurve.index < 1)
						X_VelCurve.index = 1;
					else if(X_VelCurve.index > 1)	{
						X_VelCurve.index--;
					}
					StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index]);			
//					SYS_PRINTF("%d-%ld ",X_VelCurve.Curve[X_VelCurve.index],SysMotor.motor[MOTOR_X_ID].CurPos);					
				}
			}
		}
	}
	return 0;
}
#define YMotorMAXSpeedIdx	9//y电机下降最大速度PWM占空比15%
u8 YMotorAccDec(void)
{
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if(YAccDecPos.DecPos == 0)	{//匀速
			if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN)	{//后退
				Y_MOTOR_PWM2 = 0;
				StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[1]);
			}
			else /*if(SysMotor.motor[MOTOR_Y_ID].CurPos<=YAccDecPos.DecPos)*/	{//前进
				Y_MOTOR_PWM1 = 0;
				StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[2]);
			}
		}
		else	{
			if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN)	{
				if(SysMotor.motor[MOTOR_Y_ID].CurPos>YAccDecPos.DecPos)	{//加速 y电机下降过程最大速度控制在10
					if(Y_VelCurve.index < YMotorMAXSpeedIdx)	{
						StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index]);						
//						SYS_PRINTF("%d ",Y_VelCurve.Curve[Y_VelCurve.index]);
						Y_VelCurve.index++;	
						return 1;
					}
				}else if(SysMotor.motor[MOTOR_Y_ID].CurPos<=YAccDecPos.DecPos)	{//减速
					if(Y_VelCurve.index < 0)
						Y_VelCurve.index = 0;
					else if(Y_VelCurve.index > 0)		{
						if(Y_VelCurve.index > 4)
							Y_VelCurve.index -= 2;
						else
							Y_VelCurve.index--;
					}
					StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index]);			
					SYS_PRINTF("%d-%ld ",Y_VelCurve.Curve[Y_VelCurve.index],SysMotor.motor[MOTOR_Y_ID].CurPos);				
				}
			}else if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX){
				if(SysMotor.motor[MOTOR_Y_ID].CurPos<YAccDecPos.DecPos)	{//加速
					if(Y_VelCurve.index < CURVE_BUF_MAX)	{
						StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index]);						
//						SYS_PRINTF("%d ",Y_VelCurve.Curve[Y_VelCurve.index]);
						Y_VelCurve.index++;	
						return 1;
					}
				}else if(SysMotor.motor[MOTOR_Y_ID].CurPos>=YAccDecPos.DecPos)	{//减速
					if(Y_VelCurve.index < 0)
						Y_VelCurve.index = 0;
					else if(Y_VelCurve.index > 0)		{
						if(Y_VelCurve.index > 5)
							Y_VelCurve.index -= 2;
						else
							Y_VelCurve.index--;
					}
					StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index]);			
					SYS_PRINTF("%d-%ld ",Y_VelCurve.Curve[Y_VelCurve.index],SysMotor.motor[MOTOR_Y_ID].CurPos);					
				}
			}
		}
	}
	return 0;
}

void XMotorStart(void)
{
	if(SysMotor.motor[MOTOR_X_ID].status.action==ActionState_Doing||SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		SYS_PRINTF("x running.\r\n");
		return;
	}
	if(SysMotor.motor[MOTOR_X_ID].ObjPos>X_QuHuoKouPos)	{
		SysMotor.motor[MOTOR_X_ID].ObjPos = X_QuHuoKouPos;
//		return;
	}
	XMotorSetDir();
	CalcXYMotorUpDownPos(MOTOR_X_ID);
//	if(SysMotor.ALLMotorState.bits.XMotor != DEF_Run)	
	{
		X_VelCurve.index = 0;
		if(XAccDecPos.DecPos == -1)	{//运行距离在控制精度内
			StopXMotor();
			SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK;
//			Sys.DevAction = ActionState_OK;
			return;
		}		
		if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN)	{
			X_MOTOR_PWM2 = 0;//P25
			X_MOTOR_PWM1 = 1;
			StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index++]);		
		}
		else if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX)	{
			X_MOTOR_PWM1 = 0;//P24
			X_MOTOR_PWM2 = 1;
			StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index++]);
		}
		SYS_PRINTF("x motor start.\r\n");
		SysMotor.RunningID = MOTOR_X_ID;
		SysMotor.ALLMotorState.bits.XMotor = DEF_Run;
		SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Doing;	
//		Sys.DevAction = ActionState_Doing;
//		ResetMotorStuckMonitorCnt();
	}
}

void YMotorStart(void)
{
	if(SysMotor.motor[MOTOR_Y_ID].status.action==ActionState_Doing||SysMotor.ALLMotorState.bits.YMotor == DEF_Run)		{
		SYS_PRINTF("y running.\r\n");
		return;
	}
	if(SysMotor.motor[MOTOR_Y_ID].ObjPos>YMOTOR_LEN_MAX)	{
		SysMotor.motor[MOTOR_Y_ID].ObjPos = YMOTOR_LEN_MAX;
//		return;
	}
	YMotorSetDir();
	CalcXYMotorUpDownPos(MOTOR_Y_ID);
//	if(SysMotor.ALLMotorState.bits.YMotor != DEF_Run)	
	{//测试y电机
		Y_VelCurve.index = 0;
		if(YAccDecPos.DecPos == -1)	{//运行距离在控制精度内
			StopYMotor();
			SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
//			Sys.DevAction = ActionState_OK;
			return;
		}	
		if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN)	{
			Y_MOTOR_PWM2 = 0;
			StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index++]);	
		}
		else if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX)	{
			Y_MOTOR_PWM1 = 0;
			StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index++]);
		}
		SYS_PRINTF("y motor start.\r\n");
		SysMotor.RunningID = MOTOR_Y_ID;
		SysMotor.ALLMotorState.bits.YMotor = DEF_Run;
		SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Doing;
//		Sys.DevAction = ActionState_Doing;
//		ResetMotorStuckMonitorCnt();
	}
}

void ZMotorStart(void)
{
	if(SysMotor.motor[MOTOR_Z_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.ALLMotorState.bits.ZMotor != DEF_Run)	{//测试z电机
		if(SysMotor.motor[MOTOR_Z_ID].Param>30000)//z电机最大运行时间控制
			return;
		Z_MOTOR_PWM1 = 1;
		Z_MOTOR_PWM2 = 0;
		SysMotor.ALLMotorState.bits.ZMotor = DEF_Run;
		SysMotor.RunningID = MOTOR_Z_ID;
		SysMotor.motor[MOTOR_Z_ID].status.action = ActionState_Doing;
//		SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_Z_ID].Param);//电机运行时间控制
		SoftTimerStart(SysMotor.pTimer[MOTOR_Z_ID], SysMotor.motor[MOTOR_Z_ID].Param);
//		SYS_PRINTF("z motor T:%ld\r\n",SysMotor.motor[MOTOR_Z_ID].Param);
	}
}
void TMotorStart(void)
{
	if(SysMotor.motor[MOTOR_T_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.ALLMotorState.bits.TMotor != DEF_Run)	{//测试推杆电机
		if(SysMotor.motor[MOTOR_T_ID].Param==0)	{//0收缩 ， 1延伸
			T_MOTOR_PWM1 = 1;
			T_MOTOR_PWM2 = 0;			
		}
		else if(SysMotor.motor[MOTOR_T_ID].Param==1)	{
			T_MOTOR_PWM1 = 0;
			T_MOTOR_PWM2 = 1;
		}
		T_MOTOR_ENABLE = 1;
		SysMotor.RunningID = MOTOR_T_ID;
		SysMotor.ALLMotorState.bits.TMotor = DEF_Run;
		SysMotor.motor[MOTOR_T_ID].status.action = ActionState_Doing;
//		SoftTimerStart(&Timer2Soft, 5000);//电机超时控制
		SoftTimerStart(SysMotor.pTimer[MOTOR_T_ID], 2500);
//		SYS_PRINTF("T motor start, %d", SysMotor.motor[MOTOR_T_ID].Param);
	}
}
void DMotorStart(void)
{
	if(SysMotor.motor[MOTOR_D_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.ALLMotorState.bits.DMotor != DEF_Run)	{//测试侧门电机
		if(SysMotor.motor[MOTOR_D_ID].Param==DEF_Close)	{//0关门 ， 1开门
			if(CeMenMinLimit_IN==0)	{
				if(CeMenMinLimit_IN==0)	{//已经关门到位
					SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
					SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Min_LimitOpt;
					return;
				}
			}
			D_MOTOR_PWM1 = 1;
			D_MOTOR_PWM2 = 0;
		}
		else if(SysMotor.motor[MOTOR_D_ID].Param==DEF_Open)	{
			D_MOTOR_PWM1 = 0;
			D_MOTOR_PWM2= 1;
		}
		D_MOTOR_ENABLE = 1;
		SysMotor.RunningID = MOTOR_D_ID;
		SysMotor.ALLMotorState.bits.DMotor = DEF_Run;
		SysMotor.motor[MOTOR_D_ID].status.action = ActionState_Doing;
//		SoftTimerStart(&Timer2Soft, 10000);//电机超时控制
		SoftTimerStart(SysMotor.pTimer[MOTOR_D_ID], 3200);
//		SYS_PRINTF("d motor start");
	}
}
void LMotorStart(void)
{
	if(SysMotor.motor[MOTOR_L_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.ALLMotorState.bits.LMotor != DEF_Run)	{//测试履带电机
		if(SysMotor.motor[MOTOR_L_ID].dir == DEF_Up)	{//正转
			L_MOTOR_PWM1 = 1;
			L_MOTOR_PWM2 = 0;
			BK_MOTOR_PWM1 = 1;
			BK_MOTOR_PWM2 = 0;
			L_MOTOR_ENABLE = 1;
			BK_MOTOR_ENABLE = 1;
		}
		else if(SysMotor.motor[MOTOR_L_ID].dir == DEF_Dn)	{//反转
			L_MOTOR_PWM1 = 0;
			L_MOTOR_PWM2 = 1;
			BK_MOTOR_PWM1 = 0;
			BK_MOTOR_PWM2 = 1;
			L_MOTOR_ENABLE = 1;
			BK_MOTOR_ENABLE = 1;			
		}
		SysMotor.RunningID = MOTOR_L_ID;
		SysMotor.ALLMotorState.bits.LMotor = DEF_Run;
		SysMotor.motor[MOTOR_L_ID].status.action = ActionState_Doing;
//		SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_L_ID].Param);//电机运行时间控制
		SoftTimerStart(SysMotor.pTimer[MOTOR_L_ID], SysMotor.motor[MOTOR_L_ID].Param);
	}
}
void QuHuoMenMotorStart(u8 flag)
{
	if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action==ActionState_Doing)	{
//		SYS_PRINTF("quhuo motor runnig");
		return;
	}
	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor != DEF_Run)	{//测试取货门电机
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{//0关门 ， 1开门
			if(QuHuoKouCloseLimit_IN==0)	{//门已关 
				if(QuHuoKouCloseLimit_IN==0)	{
					SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_OK;
					SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Min_LimitOpt;
					return;
				}
			}
			QuHuoMen_MOTOR_PWM1 = 1;
			QuHuoMen_MOTOR_PWM2 = 0;
			Sys.DevAction = DevActionState_QuHuoKouCloseing;
		}
		else if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Open)	{
			QuHuoMen_MOTOR_PWM1 = 0;
			QuHuoMen_MOTOR_PWM2 = 1;
			Sys.DevAction = DevActionState_QuHuoKouOpening;
		}
		else	{
			return;
		}
//		QuHuoMen_MOTOR_ENABLE = 1;
		StartPWM(QUHUOMEN_PWM, MOTOR_PWM_FREQ, 100);//全速关门
		SysMotor.RunningID = MOTOR_QuHuoMen_ID;
		SysMotor.ALLMotorState.bits.QuHuoMenMotor = DEF_Run;
		SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Doing;	
		SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Normal;
//		SysMotor.pTimer[MOTOR_QuHuoMen_ID]->pCallBack = &QuHuoMenMotorCallback;
//		SoftTimerStart(SysMotor.pTimer[MOTOR_QuHuoMen_ID], 1500);//1.5s后 关门速度减半 防夹手
		SysMotor.pTimer[MOTOR_QuHuoMen_ID]->pCallBack = &StopQuHuoMenMotor;
		SoftTimerStart(SysMotor.pTimer[MOTOR_QuHuoMen_ID], 6000);
		if(flag)
			ClearJiaShouFlag();
	}
}
void MotorStopTypeSet(u8 id, u8 stop_type)
{
	if(stop_type==DEF_Success)	{
		SysMotor.motor[id].status.action = ActionState_OK;
	}else if(stop_type==DEF_Fail)	{
		SysMotor.motor[id].status.action = ActionState_Fail;
	}
	//SysMotor.motor[id].status.abort_type = stop_type;
}

void MotorStop(u8 stop_type)
{
//	u8 runing_id;
	
//	if(SysMotor.ALLMotorState.ubyte == 0)	{//没有电机需要运行
//		return;
//	}
//	runing_id = SysMotor.MotorIDRunning;
	if(DevState.bits.State == DEV_STATE_TEST)	{
		DevState.bits.SubState = 0;//没有电机运行
	}
//	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{//停止x电机
//		StopXMotor();
//		MotorStopTypeSet(MOTOR_X_ID, stop_type);
//	}
//	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{//停止y电机
//		StopYMotor();
//		MotorStopTypeSet(MOTOR_Y_ID, stop_type);	
//	}
	if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{//停止z电机
		StopZMotor();
		MotorStopTypeSet(MOTOR_Z_ID, stop_type);
	}
	if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//停止推杆电机 使用卡死信号停止
		StopTMotor();
		MotorStopTypeSet(MOTOR_T_ID, DEF_Success);
	}
	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{//停止侧门电机
		StopDMotor();
		MotorStopTypeSet(MOTOR_D_ID, stop_type);
	}
	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{//停止履带电机
		StopLMotor();
		MotorStopTypeSet(MOTOR_L_ID, stop_type);
	}
	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{//停止取货门电机
		StopQuHuoMenMotor();
		MotorStopTypeSet(MOTOR_QuHuoMen_ID, stop_type);
	}
//	if(stop_type==DEF_Success)	{
//		Sys.DevAction = ActionState_OK;
//	}else if(stop_type==DEF_Fail)	{
//		Sys.DevAction = ActionState_Fail;
//	}
}

//void MotorStuck(void)
//{
//	static u16 XMotorStuckCnt=0,YMotorStuckCnt=0;
//	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
//		if(X_MotorDuZhuan_IN==0)	{
//			XMotorStuckCnt ++;
//			if(XMotorStuckCnt>100)	{
//				StopXMotor();
//				SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Stuck;
//				SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Fail;
//				SysHDError.E1.bits.b0 = 1;
//				SysLogicErr = LE_XMOTOR_DuZhuan;
//				XMotorStuckCnt = 0;
//			}
//		}
//		else
//			XMotorStuckCnt = 0;
//	}
//	else
//		XMotorStuckCnt = 0;
//	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
//		if(Y_MotorDuZhuan_IN==0)	{
//			YMotorStuckCnt ++;
//			if(YMotorStuckCnt>100)	{
//				StopYMotor();
//				SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Stuck;
//				SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Fail;
//				SysHDError.E1.bits.b1 = 1;
//				SysLogicErr = LE_YMOTOR_DuZhuan;	
//				YMotorStuckCnt = 0;
//			}
//		}	
//		else 
//			YMotorStuckCnt = 0;
//	}
//	else
//		YMotorStuckCnt = 0;
//}

void MotorTest(void)
{
//	if(DevState.bits.State != DEV_STATE_TEST)	{
//		return;
//	}
//	
//	DevState.bits.SubState = 1;//电机运行中
	//MotorStart();
//	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{
//		SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_L_ID].Param);
//	}
//	else if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{
//		SoftTimerStart(&Timer2Soft, 10000);//10s超时控制
//	}
}	

void StopXMotor(void)
{
	X_MOTOR_PWM1 = 0;
	X_MOTOR_PWM2 = 0;
	StartPWM(XMOTOR_PWM, MOTOR_PWM_FREQ, 0);
//	StartPWM(XMOTOR_MAX_PWM, MOTOR_PWM_FREQ, 0);
	SysMotor.ALLMotorState.bits.XMotor = DEF_Stop;
	SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK;
}

void StopYMotor(void)
{
	StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, 0);
	StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, 0);
	SysMotor.ALLMotorState.bits.YMotor = DEF_Stop;
	SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
}

void StopZMotor(void)
{
	Z_MOTOR_PWM1 = 0;
	Z_MOTOR_PWM2 = 0;		
//	Z_MOTOR_ENABLE = 0;
	SysMotor.ALLMotorState.bits.ZMotor = DEF_Stop;
	SoftTimerStop(SysMotor.pTimer[MOTOR_Z_ID]);
	if(SysMotor.motor[MOTOR_Z_ID].status.abort_type == MotorAbort_Timeout)	{
		SysMotor.motor[MOTOR_Z_ID].status.action = ActionState_OK;
	}
	else
		SysMotor.motor[MOTOR_Z_ID].status.action = ActionState_Fail;
}

void StopTMotor(void)
{
	T_MOTOR_PWM1 = 0;
	T_MOTOR_PWM2 = 0;
	T_MOTOR_ENABLE = 0;	
	SysMotor.ALLMotorState.bits.TMotor = DEF_Stop;
	if(SysMotor.motor[MOTOR_T_ID].status.abort_type == MotorAbort_Timeout)	{
		SysMotor.motor[MOTOR_T_ID].status.action = ActionState_OK;
	}
	else
		SysMotor.motor[MOTOR_T_ID].status.action = ActionState_Fail;
}

void StopDMotor(void)
{
	D_MOTOR_PWM1 = 0;
	D_MOTOR_PWM2 = 0;
	D_MOTOR_ENABLE = 0;
	SysMotor.ALLMotorState.bits.DMotor = DEF_Stop;
	SoftTimerStop(SysMotor.pTimer[MOTOR_D_ID]);
	if(SysMotor.motor[MOTOR_D_ID].status.abort_type == MotorAbort_Timeout)	{
		SysMotor.motor[MOTOR_D_ID].status.action = ActionState_Fail;
		SysHDError.E1.bits.b4 = 1;
	}
	else {
		SysHDError.E1.bits.b4 = 0;
		SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
	}
}

void StopLMotor(void)
{
	L_MOTOR_ENABLE = 0;
	BK_MOTOR_ENABLE = 0;//P6.0
	SysMotor.ALLMotorState.bits.LMotor = DEF_Stop;	
	SoftTimerStop(SysMotor.pTimer[MOTOR_L_ID]);
	if(SysMotor.motor[MOTOR_L_ID].status.abort_type == MotorAbort_Timeout)	{
		SysMotor.motor[MOTOR_L_ID].status.action = ActionState_OK;
	}
	else
		SysMotor.motor[MOTOR_L_ID].status.action = ActionState_Fail;
}

//void QuHuoMenMotorCallback(void)
//{
//	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{//测试取货门电机
//		if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{//0关门减速
//			StartPWM(QUHUOMEN_PWM, MOTOR_PWM_FREQ, 60);
//		}
//	}
//	SoftTimerStop(SysMotor.pTimer[MOTOR_QuHuoMen_ID]);
//	SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Normal;
//	SysMotor.pTimer[MOTOR_QuHuoMen_ID]->pCallBack = &StopQuHuoMenMotor;
//	SoftTimerStart(SysMotor.pTimer[MOTOR_QuHuoMen_ID], 4000);
//}
extern u8 JiaShouFlag;
void StopQuHuoMenMotor(void)
{
	QuHuoMen_MOTOR_PWM1 = 0;
	QuHuoMen_MOTOR_PWM2 = 0;
	StartPWM(QUHUOMEN_PWM, MOTOR_PWM_FREQ, 0);
	SysMotor.ALLMotorState.bits.QuHuoMenMotor =  DEF_Stop;
	SoftTimerStop(SysMotor.pTimer[MOTOR_QuHuoMen_ID]);
	if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type == MotorAbort_Timeout&&JiaShouFlag==0)	{
		SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Fail;
		SysHDError.E1.bits.b6 = 1;
	}
	else 	{
		SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_OK;
		SysHDError.E1.bits.b6 = 0;
	}
}
