#include "sys.h"
#include "motor.h"

//_ship_t Ship_t;
_sys_status_t Sys;
_sys_io_state_t IOState;
_dev_state_t DevState;
_sys_error_t SysHDError;
//_sys_logic_error_t SysLogicErr;
u8 SysLogicErr;
static u8 MotorStuckMonitor(void);
u8 HuoWuNearDetectFlag=0;//货物检测标志
u8 HuoWuDetectFlag = 0;
u8 LvDaiMotorFlag = 0;

void SysDataInit(void)
{
	Sys.state = SYSSTATE_NONE;
	Sys.DevAction = ActionState_Idle;//出货状态 空闲
	
	IOState.state1.ubyte = DEF_False;
	IOState.state2.ubyte = DEF_False;
	IOState.HongWaiState.ubyte = DEF_False;
	
	DevState.ubyte = DEV_STATE_IDLE;//机器状态
	
	SysHDError.E1.ubyte = DEF_False;
	SysHDError.E2.ubyte = DEF_False;
	
	SysLogicErr = 0;
	
//	Ship_t.state = DEV_ShipStateIDLE;
}
u8 JiaShouCnt = 0,JiaShouFlag=0;
void JiaShouProcessCallback(void)
{
//	StopQuHuoMenMotor();
	QuHuoMenMotorStart(DEF_False);
}
//由int3中断调用
void JiaShouProcess(void)
{
	if(JiaShouLimit_IN==0)	{
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{
			StopQuHuoMenMotor();//停止关门			
			SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Busy;
			SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Normal;
			SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Open;
			Timer3Soft.pCallBack = &JiaShouProcessCallback;
			SoftTimerStart(&Timer3Soft, 1000);//电机停1s消除反向电动势后开门
			JiaShouCnt ++;
			if(JiaShouCnt>3)	{//大于3次 
				JiaShouFlag = 2;
			}
			else	{//夹手次数小于3次 重新尝试					
				JiaShouFlag = 1;
			}
//			SYS_PRINTF("Stop, 1s open\r\n");
		}		
   }
}

void ClearJiaShouFlag(void)
{
	JiaShouCnt = 0;
	JiaShouFlag = 0;
}
extern u8 QuHuoMenMotorState;
void QuHuoKouProcess(void)
{
	static u8 QuHuoKouOpenLimitCnt = 0, QuHuoKouCloseLimitCnt = 0, JiaShouLimitCnt = 0;
	
//	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	
	if(QuHuoMenMotorState == DEF_Run)
	{
		if(QuHuoKouOpenLimit_IN==0)	{
			QuHuoKouOpenLimitCnt++;
			if(QuHuoKouOpenLimitCnt>5)	{
				if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Open)	{//开门到位
					StopQuHuoMenMotor();
					SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Min_LimitOpt;
					if(JiaShouFlag==1)	{//夹手未超过3次 重新关门
						SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
						Timer3Soft.pCallBack = &JiaShouProcessCallback;
						SoftTimerStart(&Timer3Soft, 1500);
						SYS_PRINTF("reclose\r\n");
					}
					else if(JiaShouFlag==2)	{//夹手超过3次 保持开门状态
						ClearJiaShouFlag();
						SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Fail;	
						SysLogicErr = LE_QuHuoMenCloseFaileByJiashou;//上报因夹手导致关门失败
						Sys.DevAction = DevActionState_QuHuoKouCloseFailed;
						SYS_PRINTF("Close fail by JiaShou\r\n");
					}
					else
						Sys.DevAction = DevActionState_QuHuoKouOpenOk;
				}
			}
		}
		else
			QuHuoKouOpenLimitCnt = 0;
		if(QuHuoKouCloseLimit_IN == 0)	{
			QuHuoKouCloseLimitCnt++;
			if(QuHuoKouCloseLimitCnt>5)	{
				if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{//关门到位
					StopQuHuoMenMotor();
					SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Max_LimitOpt;
					Sys.DevAction = DevActionState_QuHuoKouCloseOk;
					ClearJiaShouFlag();
				}
			}
		}
		else
			QuHuoKouCloseLimitCnt = 0;
	}
	else	{
		QuHuoKouOpenLimitCnt = 0;
		QuHuoKouCloseLimitCnt = 0;
//		JiaShouLimitCnt = 0;
	}
//	if(JiaShouLimit_IN==0&&SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{
//		JiaShouLimitCnt ++;
//		if(JiaShouLimitCnt>5)	{
//			SYS_PRINTF("JS,");
////			if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{
////				if(Sys.DevAction == DevActionState_QuHuoKouCloseing)	{
//				StopQuHuoMenMotor();//停止关门			
//				SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Busy;
//				SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Normal;
//				SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Open;
//				Timer3Soft.pCallBack = &JiaShouProcessCallback;
//				SoftTimerStart(&Timer3Soft, 1000);//电机停1s消除反向电动势后开门
//				JiaShouCnt ++;
//				if(JiaShouCnt>3)	{//大于3次 
//					JiaShouFlag = 2;
//				}
//				else	{//夹手次数小于3次 重新尝试					
//					JiaShouFlag = 1;
//				}
//				SYS_PRINTF("Stop, 1s open\r\n");
////			}
//		}
//	}
//	else
//		JiaShouLimitCnt = 0;
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
		   (X_MOTOR_RightLimit_IN==0&&SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX))	{//x碰到上限/下限 强制停止电机
			StopXMotor();
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
	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{
		if(CeMenMinLimit_IN==0)	{
			CeMenMinCnt ++;
			if(CeMenMinCnt>=2)	{
				if(SysMotor.motor[MOTOR_D_ID].Param==DEF_Close)	{
					StopDMotor();					
//					SYS_PRINTF("d stop, limit\r\n");
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
//			CeMenMaxLimit_IN==0&&SysMotor.motor[MOTOR_D_ID].Param==DEF_Open)	{//门电机限位 限位失效时超时20s停止
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
//	QuHuoKouProcess();//取货门开关门处理
	IOState.state2.bits.b0 = !Y_MOTOR_MinLimit_IN;
	IOState.state2.bits.b1 = !Y_MOTOR_MaxLimit_IN;
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if(Y_MOTOR_MinLimit_IN==0||Y_MOTOR_MaxLimit_IN==0)	{
			_nop_();_nop_();_nop_();_nop_();
			if(Y_MOTOR_MinLimit_IN==0||Y_MOTOR_MaxLimit_IN==0)
			if((Y_MOTOR_MinLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN) || \
				(Y_MOTOR_MaxLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX))	{//y碰到上限/下限 强制停止电机
				StopYMotor();
				SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
	//			Sys.DevAction = ActionState_OK;
				if(Y_MOTOR_MinLimit_IN==0)	{
//					SYS_PRINTF("y stop, min limit  \r\n");
//					SYS_PRINTF("arrived.%ld,%ld\r\n",SysMotor.motor[MOTOR_Y_ID].CurPos, encoder[MOTOR_Y_ID].pluse);
					encoder[MOTOR_Y_ID].pluse = 0;
					SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Min_LimitOpt;
					YMotorResetCheck();
				}else if(Y_MOTOR_MaxLimit_IN==0)	{
//					SYS_PRINTF("y stop, max limit\r\n");
					SysMotor.motor[MOTOR_Y_ID].status.abort_type = MotorAbort_Max_LimitOpt;
				}
			}
		}
	}
	IOState.state1.bits.b3 = !HuoWuCheck_IN;
	if(HuoWuCheck_IN==0)	{//低电平检测到货物
		NoHuoWuTimeCnt = 0;
		GetHuoWuTimeCnt ++;
		if(GetHuoWuTimeCnt>=5)	{
			if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{
				StopLMotor();//SYS_PRINTF("HuoWuCheck_IN\r\n");
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
//	if(HuoWuNearSwitch_IN==0)	{//货物接近出口，启动推杆电机
////		SysMotor.motor[MOTOR_T_ID].Param = 1;//前推
////		TMotorStart();
//		HuoWuNearDetectFlag = 1;//货物接近标志有效
//	}
//	else
//		HuoWuNearDetectFlag = 0;
//	if(MotorStuckMonitor())	{		
//		if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//推杆电机根据该信号停止
//			SysMotor.motor[MOTOR_T_ID].status.action = ActionState_OK;
////			SoftTimerStop(&Timer2Soft);
//			SoftTimerStop(SysMotor.pTimer[MOTOR_T_ID]);
//		}
//		MotorStop(DEF_Fail);
//		SYS_PRINTF("MotorStuck\r\n");
//		SysLogicErr = LE_MotorCurrentOver;
//	}
}
u8 MotorStuckMonitorCnt = 0;
static u8 MotorStuckMonitor(void)
{
	if(ALLMOTOR_STUCK_IN == 1)	{//有电机堵转，所有电机堵转信号共用
		MotorStuckMonitorCnt++;
		if(MotorStuckMonitorCnt>=20)	{//连续200ms堵转信号有效
			return 1;
		}
	}
	else {
		MotorStuckMonitorCnt = 0;
	}
	return 0;
}
u8 GetHuoWuFlag=0;
void ShipResult(u8 result)
{
	DevState.bits.State = DEV_STATE_IDLE;			
	DevState.bits.SubState = DEV_ShipSubStateIDLE;
	Sys.DevAction = result;//出货结果
	GetHuoWuFlag = 0;
	SYS_PRINTF("ship result %ld.\r\n", Sys.DevAction);
//	SoftTimerStop(&Timer1Soft);
//	SoftTimerStop(&Timer2Soft);
}
u8 ShipStateFlag;
u8 HuoWuTakeawayCnt;
void ShipProcess(void)
{
	static u16 timecnt = 0;
	static u16 ReqShipTimeCnt = 0;
	u8 id;
	
	if(DevState.bits.State == DEV_STATE_SHIPING)	{	
		if(DevState.bits.SubState == DEV_ShipSubStateMotorUp)	{//等待x y移动到位
			if(ShipStateFlag==0)	{//等待复位结束
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
					SysMotor.motor[MOTOR_Z_ID].Param = 15000;//启动z电机
					ZMotorStart();
					DevState.bits.SubState = DEV_ShipSubStateReqShip;//请求出货 等待08指令
					timecnt = 0;
					ReqShipTimeCnt = 0;
				}
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateReqShip)	{//请求出货15s超时
			ReqShipTimeCnt ++;
			if(ReqShipTimeCnt>15)	{//出货失败
				ReqShipTimeCnt = 0;
				StopZMotor();
				ShipResult(ActionState_Fail);
				SysLogicErr = LE_ReqShipTimeout;
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
				DMotorStart();//打开侧门	
				timecnt	++;		
				HuoWuNearDetectFlag = 0;
			}
			else if(timecnt<5)	{//收到08指令 z继续转2s
				timecnt ++;	
				return;
			}
			else if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK	\
				&& SysMotor.motor[MOTOR_D_ID].status.action == ActionState_OK)	{// x y电机到位 & 侧门开到位					
				if(HuoWuNearDetectFlag == 0)	{
					SysMotor.motor[MOTOR_X_ID].ObjPos = X_QuHuoKouPos;
					XMotorStart();
					HuoWuNearDetectFlag = 1;
				}
				else if(HuoWuNearDetectFlag == 1)	{
					SysMotor.motor[MOTOR_T_ID].Param = 1;
					TMotorStart();//启动推杆 前推
					HuoWuNearDetectFlag = 0;//清货物接近检测标志
					DevState.bits.SubState = DEV_ShipSubState_TuiGanMove;
					ShipStateFlag = 0;
				}
			}
		}
		else /*if(DevState.bits.SubState == DEV_ShipSubState_TuiGanMove||DevState.bits.SubState == DEV_ShipSubState_CeMenClosing)*/	{
			if(ShipStateFlag == 0)	{
				SysMotor.motor[MOTOR_L_ID].Param = 6000;//10000;//10s 履带电机先正转4s
				SysMotor.motor[MOTOR_L_ID].dir = DEF_Up;
				LMotorStart();
				ShipStateFlag = 1;
				HuoWuDetectFlag = 0;
				GetHuoWuFlag = 0;//获取货物标志
				LvDaiMotorFlag = 0;//履带电机运转标志
				timecnt = 0;
			}
			else if(ShipStateFlag==1)	{
				if(SysMotor.motor[MOTOR_T_ID].status.action == ActionState_OK)	{//推杆前进到位
					SysMotor.motor[MOTOR_T_ID].Param = 0;
					TMotorStart();//启动推杆 后推
					ShipStateFlag = 2;
				}
			}
			else if(ShipStateFlag == 2)	{
				if(SysMotor.motor[MOTOR_T_ID].status.action == ActionState_OK)	{//推杆后退到位
					SysMotor.motor[MOTOR_X_ID].ObjPos = 0;//x 先复位
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
			if(GetHuoWuFlag==0)
			{
				if(HuoWuDetectFlag == 1)	{//货物检测ok
					StopLMotor();
					timecnt = 0;
					DevState.bits.SubState = DEV_ShipSubState_CeMenClosing;
//					Sys.DevAction = ActionState_OK;//出货结果
					GetHuoWuFlag = 1;
					LvDaiMotorFlag = 2;
				}
				else {//未检测到货物
					timecnt ++;
					if(timecnt>18)	{//15s 超时货物检测失败
						SysMotor.motor[MOTOR_D_ID].Param=DEF_Close;
						DMotorStart();//关闭侧门	
						ShipResult(ActionState_Fail);
						SysLogicErr = LE_HuoWuDetectTimeout;
					}
					if(SysMotor.motor[MOTOR_L_ID].status.action == ActionState_OK)	{
						if(LvDaiMotorFlag == 0)	{
							LvDaiMotorFlag = 1;
						}
						else if(LvDaiMotorFlag == 1)	{
							SysMotor.motor[MOTOR_L_ID].Param = 2000;//10000;//10s 未检测到货物情况下 履带电机反转2s
							SysMotor.motor[MOTOR_L_ID].dir = DEF_Dn;
							LMotorStart();
							LvDaiMotorFlag = 2;
						}
						else if(LvDaiMotorFlag == 2)	{
							LvDaiMotorFlag = 3;
						}
						else if(LvDaiMotorFlag == 3)	{
							SysMotor.motor[MOTOR_L_ID].Param = 8000;//10000;//10s 未检测到货物情况下 履带电机正转8s
							SysMotor.motor[MOTOR_L_ID].dir = DEF_Up;
							LMotorStart();
							LvDaiMotorFlag = 4;
						}
					}
				}
			}
			if(SysMotor.motor[MOTOR_X_ID].CurPos <= XMOTOR_LEN_MAX/2)	{//检测到货物且x在零点附近 或者货物检测超时 
				SysMotor.motor[MOTOR_D_ID].Param=DEF_Close;
				DMotorStart();//关闭侧门
				if(SysMotor.motor[MOTOR_D_ID].status.action == ActionState_OK&&GetHuoWuFlag == 1)	{
//					Sys.DevAction = ActionState_OK;//上报出货ok
					ShipResult(ActionState_OK);
				}
			}
		}
		id = SysMotor.RunningID;
		if(SysMotor.motor[id].status.action == ActionState_Fail)	{//电机动作失败
			ShipResult(ActionState_Fail);
			SYS_PRINTF("ship failed %d.\r\n", id);
		}
	}
	if(Sys.DevAction == DevActionState_QuHuoKouOpening)	{
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_Fail)
			Sys.DevAction = DevActionState_QuHuoKouOpenFailed;
//		timecnt = 0;
	}
//	else if(Sys.DevAction == DevActionState_QuHuoKouOpenOk)	{				
//		if(HuoWuDetectFlag == 0)	{//货物被取走 5s后关门
//			HuoWuTakeawayCnt++;
//			if(HuoWuTakeawayCnt>5)	{
////				SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
////				QuHuoMenMotorStart(DEF_True);
//				Sys.DevAction = DevActionState_QuHuoKouCloseing;
//			}
//		}
//		else 
//			HuoWuTakeawayCnt = 0;
//		timecnt ++;
//		if(timecnt > 30)	{//开门状态30s后关门
//			SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
//			QuHuoMenMotorStart(DEF_True);
//			if(HuoWuDetectFlag == 1)//若有货 报取货超时
//				SysLogicErr = LE_HuoWuTakeawayTimeout;
//		}
//	}
	else if(Sys.DevAction == DevActionState_QuHuoKouCloseing)	{
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_Fail)
			Sys.DevAction = DevActionState_QuHuoKouCloseFailed;
	}
}

