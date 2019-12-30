#include "motor.h"
#include "encoder.h"
#include "velocity_profile.h"

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
	for(i=MOTOR_X_ID;i<MOTOR_QuHuoMen_ID;i++)	{
		SysMotor.motor[i].id = i;
		SysMotor.motor[i].status.is_run = MotorState_Stop;
		SysMotor.motor[i].status.action = ActionState_Idle;
		SysMotor.motor[i].status.abort_type = MotorAbort_Normal;
		SysMotor.motor[i].CurPos = 0;
		SysMotor.motor[i].ObjPos = -20000;
	}
	XAccDecPos.DecPos = 0;
	YAccDecPos.DecPos = 0;
	SysMotor.MotorIDRunning = 0xff;
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
		if(totalLen<=MOTOR_LEN_RANG)	{//�ƶ����������п��ƾ����� ���������
			XAccDecPos.DecPos = -1;
			return;
		}
		else if(totalLen <= MOTOR_CONSTANT_LEN)	{//�ƶ������ڼӼ��پ����� ��������
			//XAccDecPos.AccPos = 0;
			XAccDecPos.DecPos = 0;
			return;
		}
//		else if(totalLen <= (MOTOR_CONSTANT_LEN))	{//������ǰ���پ���
//			temp = totalLen/2+MOTOR_CONSTANT_LEN;
//		}
		else	{
			temp = MOTOR_CONSTANT_LEN;
		}
		if(len>=0)	{
			XAccDecPos.DecPos = SysMotor.motor[MOTOR_X_ID].ObjPos - temp;
			SYS_PRINTF("MAX ");
		}
		else	{
			XAccDecPos.DecPos = SysMotor.motor[MOTOR_X_ID].ObjPos + temp;
			SYS_PRINTF("MIN ");
		}
		SYS_PRINTF("C:%ld O:%ld D:%d\r\n",SysMotor.motor[MOTOR_X_ID].CurPos,SysMotor.motor[MOTOR_X_ID].ObjPos,XAccDecPos.DecPos);
	}
	if(id == MOTOR_Y_ID)	{
		len = SysMotor.motor[MOTOR_Y_ID].ObjPos - SysMotor.motor[MOTOR_Y_ID].CurPos;
		totalLen = abs(len);
		if(totalLen<=MOTOR_LEN_RANG)	{
			YAccDecPos.DecPos = -1;
			return;
		}
		else if(totalLen <= MOTOR_CONSTANT_LEN)	{
			//XAccDecPos.AccPos = 0;
			YAccDecPos.DecPos = 0;
			return;
		}
//		else if(totalLen <= (YMOTOR_AccDec_LEN+MOTOR_CONSTANT_LEN))	{
//			temp = totalLen/2+MOTOR_CONSTANT_LEN;
//		}
		else	{
			temp = MOTOR_CONSTANT_LEN;
		}
		if(len>=0)
			YAccDecPos.DecPos = SysMotor.motor[MOTOR_Y_ID].ObjPos - temp;
		else
			YAccDecPos.DecPos = SysMotor.motor[MOTOR_Y_ID].ObjPos + temp;
		SYS_PRINTF("C:%ld O:%ld D:%d\r\n",SysMotor.motor[MOTOR_Y_ID].CurPos,SysMotor.motor[MOTOR_Y_ID].ObjPos,YAccDecPos.DecPos);
	}
}

void MotorReset(u8 id)
{
	if(id == MOTOR_X_ID)	{
		Sys.state |= SYSSTATE_XMOTORRESET;
		SysMotor.motor[MOTOR_X_ID].dir = MOTOR_TO_MIN;
		SysMotor.ALLMotorState.bits.XMotor = DEF_Run;
		X_VelCurve.index = 0;
		if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN)	{
			X_MOTOR_PWM2 = 0;
			StartPWM(XMOTOR_MIN_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index++]);			
		}
		else if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX)	{
			X_MOTOR_PWM1 = 0;
			StartPWM(XMOTOR_MAX_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index++]);
		}
		SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Doing;	
		SYS_PRINTF("x motor reset.\r\n");
	}else 	if(id == MOTOR_Y_ID)	{
		Sys.state |= SYSSTATE_YMOTORRESET;
		SysMotor.motor[MOTOR_Y_ID].dir = MOTOR_TO_MIN;
		SysMotor.ALLMotorState.bits.YMotor = DEF_Run;
		Y_VelCurve.index = 0;
		if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN)	{
			Y_MOTOR_PWM2 = 0;
			StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index++]);		
		}
		else if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX)	{
			Y_MOTOR_PWM1 = 0;
			StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index++]);
		}
		SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Doing;	
		SYS_PRINTF("y motor reset.\r\n");
	}
	Sys.DevAction = ActionState_Doing;
	DevState.bits.State = DEV_STATE_RESET;//��λ��
	DevState.bits.SubState = 0x04;//��ʾ����������ڸ�λ
	SoftTimerStart(&Timer2Soft, 5000);//�����λ��ʱ����
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

	SysMotor.motor[MOTOR_X_ID].CurPos = encoder[EncoderX_ID].pluse*XMaPan_NumPerStep;	
	SysMotor.motor[MOTOR_Y_ID].CurPos = encoder[EncoderY_ID].pluse*YMaPan_NumPerStep;	
}
//�����쳣��� ���˶�״̬�� ����10s���̶����ޱ仯
void CheckMaPan(void)
{
	static XCurPos=0,YCurPos=0;
	static XPosChangeCnt=0,YPosChangeCnt=0;
	
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if(XCurPos==SysMotor.motor[MOTOR_X_ID].CurPos)	{
			XPosChangeCnt++;
			if(XPosChangeCnt>10)	{
				SysHDError.E2.bits.b2 = 1;//X �����쳣
				StopXMotor();
				SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_MaPanError;
				SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Fail;
				Sys.DevAction = ActionState_Fail;
			}
		}
		else	{
			XPosChangeCnt = 0;
			SysHDError.E2.bits.b2 = 0;
		}
	}
	else 
		XPosChangeCnt = 0;
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if(YCurPos==SysMotor.motor[MOTOR_Y_ID].CurPos)	{
			YPosChangeCnt++;
			if(YPosChangeCnt>10)	{
				SysHDError.E2.bits.b3 = 1;//Y �����쳣
				StopYMotor();
				SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_MaPanError;
				SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Fail;
				Sys.DevAction = ActionState_Fail;
			}
		}
		else	{
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
		if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN&&SysMotor.motor[MOTOR_Y_ID].CurPos <= SysMotor.motor[MOTOR_Y_ID].ObjPos)	{
			StopYMotor();
			SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Normal;
			SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
			SYS_PRINTF("MIN y motor arrived.%ld\r\n",SysMotor.motor[MOTOR_Y_ID].CurPos);
		}
		else if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX&&SysMotor.motor[MOTOR_Y_ID].CurPos >= SysMotor.motor[MOTOR_Y_ID].ObjPos)	{
			StopYMotor();
			SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Normal;
			SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
			SYS_PRINTF("MAX y motor arrived.%ld\r\n",SysMotor.motor[MOTOR_Y_ID].CurPos);
		}
	}
	if(SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{
		if(DevState.bits.State == DEV_STATE_SHIPING)	{
			DevState.bits.SubState = DEV_ShipStateReqShip;
		}
		//arriveflag = 0;
	}
}
#define XMotorMAXSpeedIdx	15//x�������ٶ�PWMռ�ձ�80%
void XMotorAccDec(void)
{
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if(XAccDecPos.DecPos == 0)	{
			if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN)	{//����
				X_MOTOR_PWM2 = 0;
				StartPWM(XMOTOR_MIN_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[3]);
			}
			else if(SysMotor.motor[MOTOR_X_ID].CurPos<=XAccDecPos.DecPos)	{//����
				X_MOTOR_PWM1 = 0;
				StartPWM(XMOTOR_MAX_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[3]);
			}
			return;
		}
		else	{
			if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN)	{
				if(SysMotor.motor[MOTOR_X_ID].CurPos>XAccDecPos.DecPos)	{//����
					if(X_VelCurve.index < XMotorMAXSpeedIdx)	{
						StartPWM(XMOTOR_MIN_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index]);						
						SYS_PRINTF("%d ",X_VelCurve.Curve[X_VelCurve.index]);
						X_VelCurve.index++;	
					}
				}else if(SysMotor.motor[MOTOR_X_ID].CurPos<=XAccDecPos.DecPos)	{//����
					if(X_VelCurve.index > 0)	{
						X_VelCurve.index--;
						StartPWM(XMOTOR_MIN_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index]);			
						SYS_PRINTF("%d ",X_VelCurve.Curve[X_VelCurve.index]);				
					}
				}
			}else if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX){
				if(SysMotor.motor[MOTOR_X_ID].CurPos<XAccDecPos.DecPos)	{//����
					if(X_VelCurve.index < XMotorMAXSpeedIdx)	{
						StartPWM(XMOTOR_MAX_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index]);						
						SYS_PRINTF("%d ",X_VelCurve.Curve[X_VelCurve.index]);
						X_VelCurve.index++;	
					}
				}else if(SysMotor.motor[MOTOR_X_ID].CurPos>=XAccDecPos.DecPos)	{//����
					if(X_VelCurve.index > 0)	{
						X_VelCurve.index--;
						StartPWM(XMOTOR_MAX_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index]);			
						SYS_PRINTF("%d ",X_VelCurve.Curve[X_VelCurve.index]);				
					}
				}
			}
		}
	}
}
#define YMotorMAXSpeedIdx	2//y����½�����ٶ�PWMռ�ձ�15%
void YMotorAccDec(void)
{
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if(YAccDecPos.DecPos == 0)	{
			if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN)	{//����
				Y_MOTOR_PWM2 = 0;
				StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[1]);
			}
			else if(SysMotor.motor[MOTOR_Y_ID].CurPos<=YAccDecPos.DecPos)	{//����
				Y_MOTOR_PWM1 = 0;
				StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[3]);
			}
			return;
		}
		else	{
			if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN)	{
				if(SysMotor.motor[MOTOR_Y_ID].CurPos>YAccDecPos.DecPos)	{//���� y����½���������ٶȿ�����10
					if(Y_VelCurve.index < YMotorMAXSpeedIdx)	{
						StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index]);						
						SYS_PRINTF("%d ",Y_VelCurve.Curve[Y_VelCurve.index]);
						Y_VelCurve.index++;	
					}
				}else if(SysMotor.motor[MOTOR_Y_ID].CurPos<=YAccDecPos.DecPos)	{//����
					if(Y_VelCurve.index > 0)	{
						Y_VelCurve.index--;
						StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index]);			
						SYS_PRINTF("%d ",Y_VelCurve.Curve[Y_VelCurve.index]);				
					}
				}
			}else if(SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX){
				if(SysMotor.motor[MOTOR_Y_ID].CurPos<XAccDecPos.DecPos)	{//����
					if(Y_VelCurve.index < CURVE_BUF_MAX)	{
						StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index]);						
						SYS_PRINTF("%d ",Y_VelCurve.Curve[Y_VelCurve.index]);
						Y_VelCurve.index++;	
					}
				}else if(SysMotor.motor[MOTOR_Y_ID].CurPos>=YAccDecPos.DecPos)	{//����
					if(Y_VelCurve.index > 0)	{
						Y_VelCurve.index--;
						StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, Y_VelCurve.Curve[Y_VelCurve.index]);			
						SYS_PRINTF("%d ",Y_VelCurve.Curve[Y_VelCurve.index]);				
					}
				}
			}
		}
	}
}

void XMotorStart(void)
{
	if(SysMotor.motor[MOTOR_X_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.motor[MOTOR_X_ID].ObjPos>XMOTOR_LEN_MAX)	{
		StopXMotor();
		SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Idle;
		Sys.DevAction = ActionState_Idle;
		return;
	}
	XMotorSetDir();
	CalcXYMotorUpDownPos(MOTOR_X_ID);
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{//����x���
		X_VelCurve.index = 0;
		if(XAccDecPos.DecPos == -1)	{
			StopXMotor();
			SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Fail;
			Sys.DevAction = ActionState_Fail;
			return;
		}		
		if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN)	{
			X_MOTOR_PWM2 = 0;//P25
			StartPWM(XMOTOR_MIN_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index++]);		
		}
		else if(SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX)	{
			X_MOTOR_PWM1 = 0;//P24
			StartPWM(XMOTOR_MAX_PWM, MOTOR_PWM_FREQ, X_VelCurve.Curve[X_VelCurve.index++]);
		}
		SYS_PRINTF("x motor start.\r\n");
		SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Doing;	
		Sys.DevAction = ActionState_Doing;
	}
}

void YMotorStart(void)
{
	if(SysMotor.motor[MOTOR_Y_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.motor[MOTOR_Y_ID].ObjPos>YMOTOR_LEN_MAX)	{
		StopYMotor();
		SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Idle;
		Sys.DevAction = ActionState_Idle;
		return;
	}
	YMotorSetDir();
	CalcXYMotorUpDownPos(MOTOR_Y_ID);
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{//����y���
		Y_VelCurve.index = 0;
		if(YAccDecPos.DecPos == -1)	{
			StopYMotor();
			SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Fail;
			Sys.DevAction = ActionState_Fail;
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
		SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Doing;
		Sys.DevAction = ActionState_Doing;
	}
}

void ZMotorStart(void)
{
	if(SysMotor.motor[MOTOR_Z_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{//����z���
		Z_MOTOR_PWM1 = 1;
		Z_MOTOR_PWM2 = 0;
		//SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_Z_ID].Param*10);	//����ʱ��	
		Z_MOTOR_ENABLE = 1;
		//motor_timeout = 3000;
		SysMotor.motor[MOTOR_Z_ID].status.action = ActionState_Doing;
		SoftTimerStart(&Timer2Soft, SysMotor.motor[MOTOR_Z_ID].Param);//�����ʱ����
	}
}
void TMotorStart(void)
{
	if(SysMotor.motor[MOTOR_T_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//�����Ƹ˵��
		if(SysMotor.motor[MOTOR_T_ID].Param==0)	{//0���� �� 1����
			T_MOTOR_PWM1 = 1;
			T_MOTOR_PWM2 = 0;			
		}
		else if(SysMotor.motor[MOTOR_T_ID].Param==1)	{
			T_MOTOR_PWM1 = 0;
			T_MOTOR_PWM2 = 1;
		}
		T_MOTOR_ENABLE = 1;
		//motor_timeout = 1000;//10s
		SysMotor.motor[MOTOR_T_ID].status.action = ActionState_Doing;
		SoftTimerStart(&Timer2Soft, 10000);//�����ʱ����
	}
}
void DMotorStart(void)
{
	if(SysMotor.motor[MOTOR_D_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{//���Բ��ŵ��
		if(SysMotor.motor[MOTOR_D_ID].Param==0)	{//0���� �� 1����
			D_MOTOR_PWM1 = 1;
			D_MOTOR_PWM2 = 0;
		}
		else if(SysMotor.motor[MOTOR_D_ID].Param==1)	{
			D_MOTOR_PWM1 = 0;
			D_MOTOR_PWM2= 1;
		}
		D_MOTOR_ENABLE = 1;
		//motor_timeout = 1000;
		SysMotor.motor[MOTOR_D_ID].status.action = ActionState_Doing;
		SoftTimerStart(&Timer2Soft, 10000);//�����ʱ����
	}
}
void LMotorStart(void)
{
	if(SysMotor.motor[MOTOR_L_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{//�����Ĵ����
		L_MOTOR_PWM1 = 1;
		L_MOTOR_PWM2 = 0;
		L_MOTOR_ENABLE = 1;
		//motor_timeout = 3000;
		//SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_L_ID].Param*10);//����ʱ��
		SysMotor.motor[MOTOR_L_ID].status.action = ActionState_Doing;
		SoftTimerStart(&Timer2Soft, SysMotor.motor[MOTOR_L_ID].Param);//�����ʱ����
	}
}
void QuHuoMenMotorStart(void)
{
	if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action==ActionState_Doing)	
		return;
	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{//����ȡ���ŵ��
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==0)	{//0���� �� 1����
			QuHuoMen_MOTOR_PWM1 = 1;
			QuHuoMen_MOTOR_PWM2 = 0;
		}
		else if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==1)	{
			QuHuoMen_MOTOR_PWM1 = 0;
			QuHuoMen_MOTOR_PWM2 = 1;
		}
		QuHuoMen_MOTOR_ENABLE = 1;
		//motor_timeout = 1000;
		SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Doing;	
		SoftTimerStart(&Timer2Soft, 10000);//�����ʱ����
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
//
void MotorStop(u8 stop_type)
{
//	u8 runing_id;
	
//	if(SysMotor.ALLMotorState.ubyte == 0)	{//û�е����Ҫ����
//		return;
//	}
//	runing_id = SysMotor.MotorIDRunning;
	if(DevState.bits.State == DEV_STATE_TEST)	{
		DevState.bits.SubState = 0;//û�е������
	}
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{//ֹͣx���
		StopXMotor();
		MotorStopTypeSet(MOTOR_X_ID, stop_type);
	}
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{//ֹͣy���
		StopYMotor();
		MotorStopTypeSet(MOTOR_Y_ID, stop_type);	
	}
	if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{//ֹͣz���
		StopZMotor();
		MotorStopTypeSet(MOTOR_Z_ID, stop_type);
	}
	if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//ֹͣ�Ƹ˵��
		StopTMotor();
		MotorStopTypeSet(MOTOR_T_ID, stop_type);
	}
	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{//ֹͣ���ŵ��
		StopDMotor();
		MotorStopTypeSet(MOTOR_D_ID, stop_type);
	}
	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{//ֹͣ�Ĵ����
		StopLMotor();
		MotorStopTypeSet(MOTOR_L_ID, stop_type);
	}
	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{//ֹͣȡ���ŵ��
		StopQuHuoMenMotor();
		MotorStopTypeSet(MOTOR_QuHuoMen_ID, stop_type);
	}
	if(stop_type==DEF_Success)	{
		Sys.DevAction = ActionState_OK;
	}else if(stop_type==DEF_Fail)	{
		Sys.DevAction = ActionState_Fail;
	}
}

void MotorStuck(void)
{
	static u16 XMotorStuckCnt=0,YMotorStuckCnt=0;
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if(X_MotorDuZhuan_IN==0)	{
			XMotorStuckCnt ++;
			if(XMotorStuckCnt>100)	{
				StopXMotor();
				SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Stuck;
				SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Fail;
				SysHDError.E1.bits.b0 = 1;
				SysLogicErr.logic = LE_XMOTOR_DuZhuan;
				XMotorStuckCnt = 0;
			}
		}
		else
			XMotorStuckCnt = 0;
	}
	else
		XMotorStuckCnt = 0;
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if(Y_MotorDuZhuan_IN==0)	{
			YMotorStuckCnt ++;
			if(YMotorStuckCnt>100)	{
				StopYMotor();
				SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Stuck;
				SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Fail;
				SysHDError.E1.bits.b1 = 1;
				SysLogicErr.logic = LE_YMOTOR_DuZhuan;	
				YMotorStuckCnt = 0;
			}
		}	
		else 
			YMotorStuckCnt = 0;
	}
	else
		YMotorStuckCnt = 0;
}

void MotorTest(void)
{
	if(DevState.bits.State != DEV_STATE_TEST)	{
		return;
	}
	
	DevState.bits.SubState = 1;//���������
	//MotorStart();
	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{
		SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_L_ID].Param*10);
	}
	else if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{
		SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_Z_ID].Param*10);
	}
}	

void StopXMotor(void)
{
	StartPWM(XMOTOR_MIN_PWM, MOTOR_PWM_FREQ, 0);
	StartPWM(XMOTOR_MAX_PWM, MOTOR_PWM_FREQ, 0);
	SysMotor.ALLMotorState.bits.XMotor = DEF_Stop;
}

void StopYMotor(void)
{
	StartPWM(YMOTOR_MIN_PWM, MOTOR_PWM_FREQ, 0);
	StartPWM(YMOTOR_MAX_PWM, MOTOR_PWM_FREQ, 0);
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
