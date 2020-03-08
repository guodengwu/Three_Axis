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
	
//	Ship_t.state = DEV_ShipStateIDLE;
}
u8 JiaShouCnt = 0,JiaShouFlag=0;

void JiaShouProcess(void)
{
	SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Open;
	QuHuoMenMotorStart();
}

void QuHuoKouProcess(void)
{
//	static u16 timercnt = 0;
	
	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{
		if((QuHuoKouOpenLimit_IN == 0&&SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Open) || \
			(QuHuoKouCloseLimit_IN == 0&&SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close))	{//ȡ���ڵ����λ  ��λʧЧʱ��ʱ10sֹͣ
			StopQuHuoMenMotor();
			SoftTimerStop(&Timer2Soft);
			SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_OK;
			if(QuHuoKouOpenLimit_IN==0)	{//���ųɹ�
				SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Min_LimitOpt;
				if(JiaShouFlag)	{//�м������ ���¹���
					SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
					QuHuoMenMotorStart();
				}
			}else if(QuHuoKouCloseLimit_IN==0)	{//���ųɹ�
				SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Max_LimitOpt;
				JiaShouFlag = 0;
				JiaShouCnt = 0;
			}
		}
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{//�ڹ��Ź����м����ź�
			if(JiaShouLimit_IN==0)	{//�м����ź�		
	//			SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Fail;
				JiaShouCnt ++;
				if(JiaShouCnt>3)	{//����3�μ�������
					JiaShouFlag = 0;
					JiaShouCnt = 0;
				}
				else	{//ֹͣ����
					StopQuHuoMenMotor();
					SoftTimerStop(&Timer2Soft);		
					Timer3Soft.pCallBack = &JiaShouProcess;
					SoftTimerStart(&Timer3Soft, 3000);//�м����¼����ȴ�2s����
					JiaShouFlag = 1;
				}
			}
		}
	}
}
	
u8 GetHuoWuTimeCnt = 0;
u8 NoHuoWuTimeCnt = 0;
void CheckIOState(void)
{
	IOState.state1.bits.b0 = !X_MOTOR_RightLimit_IN;	
	IOState.state1.bits.b1 = !X_MOTOR_LeftLimit_IN;
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if((X_MOTOR_LeftLimit_IN==0&&SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN) || \
		   (X_MOTOR_RightLimit_IN==0&&SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX))	{//x��������/���� ǿ��ֹͣ���
			StopXMotor();SYS_PRINTF("x stop, limit\r\n");
			SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK;
			Sys.DevAction = ActionState_OK;
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
	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{
		if(CeMenMinLimit_IN==0&&SysMotor.motor[MOTOR_D_ID].Param==DEF_Close|| \
			CeMenMaxLimit_IN==0&&SysMotor.motor[MOTOR_D_ID].Param==DEF_Open)	{//�ŵ����λ ��λʧЧʱ��ʱ20sֹͣ
			StopDMotor();
			SoftTimerStop(&Timer2Soft);
			SysMotor.motor[MOTOR_D_ID].status.action = ActionState_OK;
			if(CeMenMinLimit_IN==0)	{
				SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Min_LimitOpt;
			}else if(CeMenMaxLimit_IN==0)	{
				SysMotor.motor[MOTOR_D_ID].status.abort_type = MotorAbort_Max_LimitOpt;
			}
		}
	}
	IOState.state1.bits.b6 = !QuHuoKouOpenLimit_IN;
	IOState.state1.bits.b7 = !QuHuoKouCloseLimit_IN;
	IOState.state1.bits.b2 = !JiaShouLimit_IN;
	QuHuoKouProcess();//ȡ���ſ����Ŵ���
	IOState.state2.bits.b0 = !Y_MOTOR_MinLimit_IN;
	IOState.state2.bits.b1 = !Y_MOTOR_MaxLimit_IN;
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if((Y_MOTOR_MinLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN) || \
			(Y_MOTOR_MaxLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX))	{//y��������/���� ǿ��ֹͣ���
			StopYMotor();SYS_PRINTF("y stop, limit\r\n");
			SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
			Sys.DevAction = ActionState_OK;
			if(Y_MOTOR_MinLimit_IN==0)	{
				SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Min_LimitOpt;
				YMotorResetCheck();
			}else if(Y_MOTOR_MaxLimit_IN==0)	{
				SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Max_LimitOpt;
			}
		}
	}
	IOState.state1.bits.b2 = HuoWuCheck_IN;
	if(HuoWuCheck_IN==1)	{
		NoHuoWuTimeCnt = 0;
		GetHuoWuTimeCnt ++;
		if(GetHuoWuTimeCnt>5)	{
			if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{
				StopLMotor();SYS_PRINTF("HuoWuCheck_IN\r\n");
				SoftTimerStop(&Timer2Soft);
				SysMotor.motor[MOTOR_L_ID].status.action = ActionState_OK;
				Sys.DevAction = ActionState_OK;
			}
			HuoWuDetectFlag = 1;
		}
	}
	else if(HuoWuCheck_IN==0)	{
		GetHuoWuTimeCnt = 0;
		NoHuoWuTimeCnt ++;
		if(NoHuoWuTimeCnt>5)	{
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
		if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//�Ƹ˵�����ݸ��ź�ֹͣ
			SysMotor.motor[MOTOR_T_ID].status.action = ActionState_OK;
			Sys.DevAction = ActionState_OK;
		}
		MotorStop(DEF_Fail);SYS_PRINTF("MotorStuck\r\n");
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
/*void CheckDevAction(void)
{
	u8 runing_id;
	
	runing_id = SysMotor.MotorIDRunning;
	if(DevState.bits.SubState == 1)
		Sys.DevAction == ActionState_Busy;
	else 
		Sys.DevAction == ActionState_Idle;
}*/
u8 ShipStateFlag;
void ShipProcess(void)
{
	static u16 timecnt = 0;
	static u16 ReqShipTimeCnt = 0;
	u8 id;
	
	if(DevState.bits.State == DEV_STATE_SHIPING)	{	
		if(DevState.bits.SubState == DEV_ShipSubStateMotorUp)	{//�ȴ�x y�ƶ���λ
			if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{
				DevState.bits.SubState = DEV_ShipSubStateStartZmotor;//����z���
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateStartZmotor)	{//����z
			SysMotor.motor[MOTOR_Z_ID].Param = 15000;
			ZMotorStart();
			DevState.bits.SubState = DEV_ShipSubStateReqShip;//������� �ȴ�08ָ��
			timecnt = 0;
			ReqShipTimeCnt = 0;
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateReqShip)	{//�������15s��ʱ
			ReqShipTimeCnt ++;
			if(ReqShipTimeCnt>15)	{//����ʧ��
				ReqShipTimeCnt = 0;
				DevState.bits.State = DEV_STATE_SHIP_Failed;
			}
			HuoWuNearDetectFlag = 0;//�����ӽ�����־
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateCeMenOpening)	{
			ReqShipTimeCnt = 0;					
			if(timecnt==2)	{
				StopZMotor();
				SysMotor.motor[MOTOR_X_ID].ObjPos = XMOTOR_LEN_MAX;//X_QuHuoKouPos;
				SysMotor.motor[MOTOR_Y_ID].ObjPos = Y_QuHuoKouPos;
				XMotorStart();
				YMotorStart();
				SysMotor.motor[MOTOR_D_ID].Param = DEF_Open;
				DMotorStart();//�򿪲���	
				timecnt	++;		
			}
			else if(timecnt<2)	{
				timecnt ++;	
				return;
			}
			if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{				
				if(HuoWuNearDetectFlag == 0)	{
					SysMotor.motor[MOTOR_X_ID].ObjPos = X_QuHuoKouPos;
					XMotorStart();
					HuoWuNearDetectFlag = 1;
				}
				else if((HuoWuNearDetectFlag == 1)&&(SysMotor.motor[MOTOR_D_ID].status.action == ActionState_OK))	{//x y�����λ & ��⵽���� & ���ſ���λ					
					SysMotor.motor[MOTOR_T_ID].Param = 1;
					TMotorStart();//�����Ƹ� ǰ��
//					HuoWuNearDetectFlag = 0;//�����ӽ�����־
					DevState.bits.SubState = DEV_ShipSubState_TuiGanMove;
					ShipStateFlag = 0;
				}
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_TuiGanMove)	{
			if(ShipStateFlag==0)	{
				if(SysMotor.motor[MOTOR_T_ID].status.action == ActionState_OK)	{//�Ƹ�ǰ����λ
					SysMotor.motor[MOTOR_T_ID].Param = 0;
					TMotorStart();//�����Ƹ� ����
					ShipStateFlag = 1;
				}
			}
			else if(ShipStateFlag == 1)	{
				if(SysMotor.motor[MOTOR_T_ID].status.action == ActionState_OK)	{//�Ƹ˺��˵�λ
					SysMotor.motor[MOTOR_X_ID].ObjPos = 0;//x �ȸ�λ
					XMotorStart();
					ShipStateFlag = 2;
				}
			}
			else if(ShipStateFlag == 2)	{
				if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK)	{//x ��λok
					SysMotor.motor[MOTOR_Y_ID].ObjPos = 0;				
					YMotorStart();
					SysMotor.motor[MOTOR_D_ID].Param=DEF_Close;
					DMotorStart();//�رղ���	
//					SysMotor.motor[MOTOR_L_ID].Param = 10000;//10s
//					LMotorStart();
					ShipStateFlag = 3;
				}
			}
			else if(ShipStateFlag == 3)	{
				if(SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{
					DevState.bits.SubState = DEV_ShipSubState_CeMenClosing;
//					SysMotor.motor[MOTOR_D_ID].Param=DEF_Close;
//					DMotorStart();//�رղ���	
					SysMotor.motor[MOTOR_L_ID].Param = 10000;//10s
					LMotorStart();
					HuoWuDetectFlag = 0;
				}
			}
			timecnt = 0;
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_CeMenClosing)	{//�ȴ����������
			if(HuoWuDetectFlag == 1)	{
				SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Open;
				QuHuoMenMotorStart();//��ȡ���� �ȴ�ȡ�߻���
				DevState.bits.SubState = DEV_ShipSubState_QuHuoKouOpening;
			}
			if(timecnt>5)	{//5s ��ʱ������ʧ��
				DevState.bits.State = DEV_STATE_SHIP_Failed;
			}
			else
				timecnt ++;
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_QuHuoKouOpening)	{
			if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_OK)	{//ȡ���ſ��ŵ�λ
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
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_QuHuoKouCloseing)	{
			if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_OK)	{//ȡ���Ź��ŵ�λ
				DevState.bits.State = DEV_STATE_IDLE;//ȡ�����̽���
				DevState.bits.SubState = DEV_ShipSubStateIDLE;
			}
		}
		id = SysMotor.RunningID;
		if(SysMotor.motor[id].status.action == ActionState_Fail)	{
//		if(Sys.DevAction == ActionState_Fail)	{//�������ʧ��
			DevState.bits.State = DEV_STATE_SHIP_Failed;
			DevState.bits.SubState = DEV_ShipSubStateIDLE;
		}
	}
}

