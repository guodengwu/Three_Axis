#include "sys.h"
#include "motor.h"

//_ship_t Ship_t;
_sys_status_t Sys;
_sys_io_state_t IOState;
_dev_state_t DevState;
_sys_error_t SysHDError;
_sys_logic_error_t SysLogicErr;
static u8 MotorStuckMonitor(void);
u8 HuoWuNearDetectFlag=0;//�������־
u8 HuoWuDetectFlag = 0;
u8 LvDaiMotorFlag = 0;

void SysDataInit(void)
{
	Sys.state = SYSSTATE_NONE;
	Sys.DevAction = ActionState_Idle;//����״̬ ����
	
	IOState.state1.ubyte = DEF_False;
	IOState.state2.ubyte = DEF_False;
	IOState.HongWaiState.ubyte = DEF_False;
	
	DevState.ubyte = DEV_STATE_IDLE;//����״̬
	
	SysHDError.E1.ubyte = DEF_False;
	SysHDError.E2.ubyte = DEF_False;
	
	SysLogicErr.logic = 0;
	
//	Ship_t.state = DEV_ShipStateIDLE;
}
u8 JiaShouCnt = 0,JiaShouFlag=0;

void JiaShouProcess(void)
{
	if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)
		SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Open;
	else 
		SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
	QuHuoMenMotorStart();
}

void QuHuoKouProcess(void)
{
//	static u16 timercnt = 0;
	
	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{
		if((QuHuoKouOpenLimit_IN == 0&&SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Open) || \
			(QuHuoKouCloseLimit_IN == 0&&SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close))	{//ȡ���ڵ����λ  ��λʧЧʱ��ʱ10sֹͣ
			StopQuHuoMenMotor();		
			SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_OK;
			if(QuHuoKouOpenLimit_IN==0)	{//���ŵ�λ
				SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Min_LimitOpt;
				if(JiaShouFlag==1)	{//�м������ ���¹���
					Timer3Soft.pCallBack = &JiaShouProcess;
					SoftTimerStart(&Timer3Soft, 500);
				}
				else if(JiaShouFlag==2)	{//����ʧ�� ���濪��״̬
					JiaShouCnt = 0;
					JiaShouFlag = 0;
					SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Fail;					
				}
			}else if(QuHuoKouCloseLimit_IN==0)	{//���ŵ�λ
				SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Max_LimitOpt;
				JiaShouFlag = 0;
				JiaShouCnt = 0;
			}
		}
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{//�ڹ��Ź����м����ź�
			if(JiaShouLimit_IN==0)	{//�м����ź�	
			if(JiaShouLimit_IN==0)	{
	//			SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Fail;
				JiaShouCnt ++;
				if(JiaShouCnt>3)	{//����3�μ�������
					JiaShouFlag = 2;
//					JiaShouCnt = 0;
					SoftTimerStart(&Timer3Soft, 500);
				}
				else	{//ֹͣ����
					StopQuHuoMenMotor();
					SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Busy;
					Timer3Soft.pCallBack = &JiaShouProcess;
					SoftTimerStart(&Timer3Soft, 500);//�м����¼����ȴ�2s����
					JiaShouFlag = 1;
				}
			}
			}
		}
	}
//	if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close&&SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_Fail)
//	{//����ʧ��
//		JiaShouFlag = 0;
//		JiaShouCnt = 0;
//	}
}
#include "encoder.h"
u8 GetHuoWuTimeCnt = 0;
u8 NoHuoWuTimeCnt = 0;
u8 CeMenMinCnt=0,CeMenMaxCnt=0;
void CheckIOState(void)
{
	IOState.state1.bits.b0 = !X_MOTOR_RightLimit_IN;	
	IOState.state1.bits.b1 = !X_MOTOR_LeftLimit_IN;
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if((X_MOTOR_LeftLimit_IN==0&&SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN) || \
		   (X_MOTOR_RightLimit_IN==0&&SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX))	{//x��������/���� ǿ��ֹͣ���
			StopXMotor();SYS_PRINTF("x stop, limit\r\n");
			SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK;
//			Sys.DevAction = ActionState_OK;
			if(X_MOTOR_LeftLimit_IN==0)	{
				SysMotor.motor[MOTOR_X_ID].status.abort_type = MotorAbort_Min_LimitOpt;
				encoder[MOTOR_X_ID].pluse = 0;
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
	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{
		if(CeMenMinLimit_IN==0)	{
			CeMenMinCnt ++;
			if(CeMenMinCnt>=2)	{
				if(SysMotor.motor[MOTOR_D_ID].Param==DEF_Close)	{
					StopDMotor();					
					SYS_PRINTF("d stop, limit\r\n");
					SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
					SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Min_LimitOpt;
				}
			}		
		}else
				CeMenMinCnt = 0;
		if(CeMenMaxLimit_IN==0)	{
			CeMenMaxCnt ++;
			if(CeMenMaxCnt>=2)	{
				if(SysMotor.motor[MOTOR_D_ID].Param==DEF_Open)	{
					StopDMotor();
					SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
					SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Max_LimitOpt;
				}
			}
		}else
				CeMenMaxCnt = 0;
	}
	else {
		CeMenMinCnt = 0;
		CeMenMaxCnt = 0;
	}
//	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{
//		if(CeMenMinLimit_IN==0&&SysMotor.motor[MOTOR_D_ID].Param==DEF_Close|| 
//			CeMenMaxLimit_IN==0&&SysMotor.motor[MOTOR_D_ID].Param==DEF_Open)	{//�ŵ����λ ��λʧЧʱ��ʱ20sֹͣ
//			StopDMotor();
//			SoftTimerStop(&Timer2Soft);
//			SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
//			if(CeMenMinLimit_IN==0)	{
//				SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Min_LimitOpt;
//			}else if(CeMenMaxLimit_IN==0)	{
//				SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Max_LimitOpt;
//			}
//		}
//	}
	IOState.state1.bits.b6 = !QuHuoKouOpenLimit_IN;
	IOState.state1.bits.b7 = !QuHuoKouCloseLimit_IN;
	IOState.state1.bits.b2 = !JiaShouLimit_IN;
	QuHuoKouProcess();//ȡ���ſ����Ŵ���
	IOState.state2.bits.b0 = !Y_MOTOR_MinLimit_IN;
	IOState.state2.bits.b1 = !Y_MOTOR_MaxLimit_IN;
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if(Y_MOTOR_MinLimit_IN==0||Y_MOTOR_MaxLimit_IN==0)	{
			_nop_();_nop_();_nop_();_nop_();
			if(Y_MOTOR_MinLimit_IN==0||Y_MOTOR_MaxLimit_IN==0)
			if((Y_MOTOR_MinLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN) || \
				(Y_MOTOR_MaxLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX))	{//y��������/���� ǿ��ֹͣ���
				StopYMotor();
				SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
	//			Sys.DevAction = ActionState_OK;
				if(Y_MOTOR_MinLimit_IN==0)	{
					SYS_PRINTF("y stop, min limit  \r\n");
					SYS_PRINTF("arrived.%ld,%ld\r\n",SysMotor.motor[MOTOR_Y_ID].CurPos, encoder[MOTOR_Y_ID].pluse);
					encoder[MOTOR_Y_ID].pluse = 0;
					SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Min_LimitOpt;
					YMotorResetCheck();
				}else if(Y_MOTOR_MaxLimit_IN==0)	{
					SYS_PRINTF("y stop, max limit\r\n");
					SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Max_LimitOpt;
				}
			}
		}
	}
	IOState.state1.bits.b3 = !HuoWuCheck_IN;
	if(HuoWuCheck_IN==0)	{//�͵�ƽ��⵽����
		NoHuoWuTimeCnt = 0;
		GetHuoWuTimeCnt ++;
		if(GetHuoWuTimeCnt>=5)	{
			if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{
				StopLMotor();SYS_PRINTF("HuoWuCheck_IN\r\n");
				SysMotor.motor[MOTOR_L_ID].status.action = ActionState_OK;
//				Sys.DevAction = ActionState_OK;
			}
			HuoWuDetectFlag = 1;
		}
	}
	else if(HuoWuCheck_IN==1)	{
		GetHuoWuTimeCnt = 0;
		NoHuoWuTimeCnt ++;
		if(NoHuoWuTimeCnt>=5)	{
			HuoWuDetectFlag = 0;
		}
	}
//	if(HuoWuNearSwitch_IN==0)	{//����ӽ����ڣ������Ƹ˵��
////		SysMotor.motor[MOTOR_T_ID].Param = 1;//ǰ��
////		TMotorStart();
//		HuoWuNearDetectFlag = 1;//����ӽ���־��Ч
//	}
//	else
//		HuoWuNearDetectFlag = 0;
	if(MotorStuckMonitor())	{		
//		if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//�Ƹ˵�����ݸ��ź�ֹͣ
//			SysMotor.motor[MOTOR_T_ID].status.action = ActionState_OK;
////			SoftTimerStop(&Timer2Soft);
//			SoftTimerStop(SysMotor.pTimer[MOTOR_T_ID]);
//		}
		MotorStop(DEF_Fail);
		SYS_PRINTF("MotorStuck\r\n");
	}
}
u8 MotorStuckMonitorCnt = 0;
static u8 MotorStuckMonitor(void)
{
	if(ALLMOTOR_STUCK_IN == 1)	{//�е����ת�����е����ת�źŹ���
		MotorStuckMonitorCnt++;
		if(MotorStuckMonitorCnt>=20)	{//����200ms��ת�ź���Ч
			return 1;
		}
	}
	else {
		MotorStuckMonitorCnt = 0;
	}
	return 0;
}
void ShipResult(u8 result)
{
	DevState.bits.State = DEV_STATE_IDLE;			
	DevState.bits.SubState = DEV_ShipSubStateIDLE;
	Sys.DevAction = result;//�������
	SYS_PRINTF("ship result %ld.\r\n", Sys.DevAction);
//	SoftTimerStop(&Timer1Soft);
//	SoftTimerStop(&Timer2Soft);
}
u8 ShipStateFlag;
void ShipProcess(void)
{
	static u16 timecnt = 0;
	static u16 ReqShipTimeCnt = 0;
	u8 id;
	
	if(DevState.bits.State == DEV_STATE_SHIPING)	{	
		if(DevState.bits.SubState == DEV_ShipSubStateMotorUp)	{//�ȴ�x y�ƶ���λ
			if(ShipStateFlag==0)	{//�ȴ���λ����
				if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK && \
					SysMotor.motor[MOTOR_D_ID].status.action == ActionState_OK)	{			
					ShipStateFlag = 1;
					timecnt = 0;
				}
			}
			else if(ShipStateFlag==1)	{
				ShipStateFlag = 2;
				XMotorStart();
				YMotorStart();
			}
			else if(ShipStateFlag==2)	{
				if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{
					SysMotor.motor[MOTOR_Z_ID].Param = 15000;//����z���
					ZMotorStart();
					DevState.bits.SubState = DEV_ShipSubStateReqShip;//������� �ȴ�08ָ��
					timecnt = 0;
					ReqShipTimeCnt = 0;
				}
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateReqShip)	{//�������15s��ʱ
			ReqShipTimeCnt ++;
			if(ReqShipTimeCnt>15)	{//����ʧ��
				ReqShipTimeCnt = 0;
				ShipResult(ActionState_Fail);
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateCeMenOpening)	{
			ReqShipTimeCnt = 0;					
			if(timecnt==5)	{
				StopZMotor();				
				SysMotor.motor[MOTOR_Z_ID].status.action = ActionState_OK;
				SysMotor.motor[MOTOR_X_ID].ObjPos = XMOTOR_LEN_MAX;//X_QuHuoKouPos;
				SysMotor.motor[MOTOR_Y_ID].ObjPos = Y_QuHuoKouPos;
				XMotorStart();
				YMotorStart();
				SysMotor.motor[MOTOR_D_ID].Param = DEF_Open;
				DMotorStart();//�򿪲���	
				timecnt	++;		
				HuoWuNearDetectFlag = 0;
			}
			else if(timecnt<5)	{//�յ�08ָ�� z����ת2s
				timecnt ++;	
				return;
			}
			else if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK	\
				&& SysMotor.motor[MOTOR_D_ID].status.action == ActionState_OK)	{// x y�����λ & ���ſ���λ					
				if(HuoWuNearDetectFlag == 0)	{
					SysMotor.motor[MOTOR_X_ID].ObjPos = X_QuHuoKouPos;
					XMotorStart();
					HuoWuNearDetectFlag = 1;
				}
				else if(HuoWuNearDetectFlag == 1)	{
					SysMotor.motor[MOTOR_T_ID].Param = 1;
					TMotorStart();//�����Ƹ� ǰ��
					HuoWuNearDetectFlag = 0;//�����ӽ�����־
					DevState.bits.SubState = DEV_ShipSubState_TuiGanMove;
					ShipStateFlag = 0;
				}
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_TuiGanMove||DevState.bits.SubState == DEV_ShipSubState_CeMenClosing)	{
			if(ShipStateFlag == 0)	{
				SysMotor.motor[MOTOR_L_ID].Param = 6000;//10000;//10s �Ĵ��������ת4s
				SysMotor.motor[MOTOR_L_ID].dir = DEF_Up;
				LMotorStart();
				ShipStateFlag = 1;
				HuoWuDetectFlag = 0;
				LvDaiMotorFlag = 0;//�Ĵ������ת��־
				timecnt = 0;
			}
			else if(ShipStateFlag==1)	{
				if(SysMotor.motor[MOTOR_T_ID].status.action == ActionState_OK)	{//�Ƹ�ǰ����λ
					SysMotor.motor[MOTOR_T_ID].Param = 0;
					TMotorStart();//�����Ƹ� ����
					ShipStateFlag = 2;
				}
			}
			else if(ShipStateFlag == 2)	{
				if(SysMotor.motor[MOTOR_T_ID].status.action == ActionState_OK)	{//�Ƹ˺��˵�λ
					SysMotor.motor[MOTOR_X_ID].ObjPos = 0;//x �ȸ�λ
					XMotorStart();
					ShipStateFlag = 3;
				}
			}
			else if(ShipStateFlag == 3)	{
				if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK)	
				{
					SysMotor.motor[MOTOR_Y_ID].ObjPos = 0;	
					YMotorStart();
					ShipStateFlag = 4;
				}
			}
			if(HuoWuDetectFlag == 1)	{//������ok
				StopLMotor();
				timecnt = 0;
				DevState.bits.SubState = DEV_ShipSubState_CeMenClosing;
				Sys.DevAction = ActionState_OK;//�������
			}
			else if(SysMotor.motor[MOTOR_L_ID].status.action == ActionState_OK)	{
				if(LvDaiMotorFlag == 0)	{
					SysMotor.motor[MOTOR_L_ID].Param = 2000;//10000;//10s δ��⵽��������� �Ĵ������ת2s
					SysMotor.motor[MOTOR_L_ID].dir = DEF_Dn;
					LMotorStart();
					LvDaiMotorFlag = 1;
				}else if(LvDaiMotorFlag == 1)	{
					SysMotor.motor[MOTOR_L_ID].Param = 8000;//10000;//10s δ��⵽��������� �Ĵ������ת8s
					SysMotor.motor[MOTOR_L_ID].dir = DEF_Up;
					LMotorStart();
					LvDaiMotorFlag = 2;
				}
			}
			if(DevState.bits.SubState == DEV_ShipSubState_CeMenClosing&&SysMotor.motor[MOTOR_X_ID].CurPos <= XMOTOR_LEN_MAX/2)	{//��⵽������x����㸽�� ���߻����ⳬʱ 
				SysMotor.motor[MOTOR_D_ID].Param=DEF_Close;
				DMotorStart();//�رղ���	
//				SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Open;
//				QuHuoMenMotorStart();//��ȡ���� �ȴ�ȡ�߻���
//				DevState.bits.SubState = DEV_ShipSubState_QuHuoKouOpening;				
			}
			if(timecnt>18)	{//15s ��ʱ������ʧ��
				SysMotor.motor[MOTOR_D_ID].Param=DEF_Close;
				DMotorStart();//�رղ���	
				ShipResult(ActionState_Fail);
			}
			else	{
				timecnt ++;
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_QuHuoKouOpening)	{
			if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_OK)	{//ȡ���ſ��ŵ�λ
				DevState.bits.SubState = DEV_ShipSubState_QuHuoKouOpenOk;
			}
			else if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_Fail)
				DevState.bits.SubState = DEV_ShipSubState_QuHuoKouOpenFailed;
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_QuHuoKouOpenOk)	{
			timecnt ++;				
			if(HuoWuDetectFlag == 0)	{//���ﱻȡ�� 5s�����
				if(timecnt>5)	{
					SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
					QuHuoMenMotorStart();
					DevState.bits.SubState = DEV_ShipSubState_QuHuoKouCloseing;
				}
			}
			if(timecnt > 30)	{//����û��ȡ�� 30s�� ����
				SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
				QuHuoMenMotorStart();
				DevState.bits.SubState = DEV_ShipSubState_QuHuoKouCloseing;
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_QuHuoKouCloseing)	{
			if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_OK)	{//ȡ���Ź��ŵ�λ
				ShipResult(ActionState_OK);//�������
				DevState.bits.SubState = DEV_ShipSubState_QuHuoKouCloseOk;
			}
			else if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_Fail)
				DevState.bits.SubState = DEV_ShipSubState_QuHuoKouCloseFailed;
		}
		id = SysMotor.RunningID;
		if(SysMotor.motor[id].status.action == ActionState_Fail)	{//�������ʧ��
			ShipResult(ActionState_Fail);
			SYS_PRINTF("ship failed %d.\r\n", id);
		}
	}
}

