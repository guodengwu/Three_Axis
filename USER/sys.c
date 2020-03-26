#include "sys.h"
#include "motor.h"

//_ship_t Ship_t;
_sys_status_t Sys;
_sys_io_state_t IOState;
_dev_state_t DevState;
_sys_error_t SysHDError;
_sys_logic_error_t SysLogicErr;
static u8 MotorStuckMonitor(void);
u8 HuoWuNearDetectFlag=0;//货物检测标志
u8 HuoWuDetectFlag = 0;

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
			(QuHuoKouCloseLimit_IN == 0&&SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close))	{//取货口电机限位  限位失效时超时10s停止
			StopQuHuoMenMotor();
			SoftTimerStop(&Timer2Soft);
			SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_OK;
			if(QuHuoKouOpenLimit_IN==0)	{//开门到位
				SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Min_LimitOpt;
				if(JiaShouFlag==1)	{//有夹手情况 重新关门
					Timer3Soft.pCallBack = &JiaShouProcess;
					SoftTimerStart(&Timer3Soft, 1500);
				}
				else if(JiaShouFlag==2)	{//关门失败 保存开门状态
					JiaShouCnt = 0;
					JiaShouFlag = 0;
					SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Fail;
				}
			}else if(QuHuoKouCloseLimit_IN==0)	{//关门成功
				SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Max_LimitOpt;
				JiaShouFlag = 0;
				JiaShouCnt = 0;
			}
		}
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{//在关门过程有夹手信号
			if(JiaShouLimit_IN==0)	{//有夹手信号	
			if(JiaShouLimit_IN==0)	{
	//			SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Fail;
				JiaShouCnt ++;
				if(JiaShouCnt>3)	{//大于3次继续关门
					JiaShouFlag = 2;
//					JiaShouCnt = 0;
					SoftTimerStart(&Timer3Soft, 1500);
				}
				else	{//停止关门
					StopQuHuoMenMotor();
					SoftTimerStop(&Timer2Soft);		
					SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Busy;
					Timer3Soft.pCallBack = &JiaShouProcess;
					SoftTimerStart(&Timer3Soft, 1500);//有夹手事件，等待2s开门
					JiaShouFlag = 1;
				}
			}
			}
		}
	}
//	if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close&&SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_Fail)
//	{//关门失败
//		JiaShouFlag = 0;
//		JiaShouCnt = 0;
//	}
}
	
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
			StopXMotor();SYS_PRINTF("x stop, limit\r\n");
			SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK;
//			Sys.DevAction = ActionState_OK;
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
					SoftTimerStop(&Timer2Soft);
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
					SoftTimerStop(&Timer2Soft);
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
	QuHuoKouProcess();//取货门开关门处理
	IOState.state2.bits.b0 = !Y_MOTOR_MinLimit_IN;
	IOState.state2.bits.b1 = !Y_MOTOR_MaxLimit_IN;
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if((Y_MOTOR_MinLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN) || \
			(Y_MOTOR_MaxLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX))	{//y碰到上限/下限 强制停止电机
			StopYMotor();SYS_PRINTF("y stop, limit\r\n");
			SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_OK;
//			Sys.DevAction = ActionState_OK;
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
		if(GetHuoWuTimeCnt>=5)	{
			if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{
				StopLMotor();SYS_PRINTF("HuoWuCheck_IN\r\n");
				SoftTimerStop(&Timer2Soft);
				SysMotor.motor[MOTOR_L_ID].status.action = ActionState_OK;
//				Sys.DevAction = ActionState_OK;
			}
			HuoWuDetectFlag = 1;
		}
	}
	else if(HuoWuCheck_IN==0)	{
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
	if(MotorStuckMonitor())	{		
		if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//推杆电机根据该信号停止
			SysMotor.motor[MOTOR_T_ID].status.action = ActionState_OK;
//			Sys.DevAction = ActionState_OK;
		}
		MotorStop(DEF_Fail);SYS_PRINTF("MotorStuck\r\n");
	}
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
void ShipResult(u8 result)
{
	DevState.bits.State = DEV_STATE_IDLE;			
	DevState.bits.SubState = DEV_ShipSubStateIDLE;
	Sys.DevAction = result;//出货结果
	SYS_PRINTF("ship result %ld.\r\n", Sys.DevAction);
}
u8 ShipStateFlag;
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
				timecnt++;
				if(timecnt>1)	{
					ShipStateFlag = 2;
					XMotorStart();
					YMotorStart();
				}
			}
			else if(ShipStateFlag==2)	{
				if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{
	//				DevState.bits.SubState = DEV_ShipSubStateStartZmotor;
					SysMotor.motor[MOTOR_Z_ID].Param = 15000;//启动z电机
					ZMotorStart();
					DevState.bits.SubState = DEV_ShipSubStateReqShip;//请求出货 等待08指令
					timecnt = 0;
					ReqShipTimeCnt = 0;
				}
			}
		}
//		else if(DevState.bits.SubState == DEV_ShipSubStateStartZmotor)	{//请求出货 等待08指令
//			SysMotor.motor[MOTOR_Z_ID].Param = 15000;
//			ZMotorStart();
//			DevState.bits.SubState = DEV_ShipSubStateReqShip;//请求出货 等待08指令
//			timecnt = 0;
//			ReqShipTimeCnt = 0;
//		}
		else if(DevState.bits.SubState == DEV_ShipSubStateReqShip)	{//请求出货15s超时
			ReqShipTimeCnt ++;
			if(ReqShipTimeCnt>15)	{//出货失败
				ReqShipTimeCnt = 0;
				ShipResult(ActionState_Fail);
			}
			HuoWuNearDetectFlag = 0;//清货物接近检测标志
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateCeMenOpening)	{
			ReqShipTimeCnt = 0;					
			if(timecnt==2)	{
				StopZMotor();
				SysMotor.motor[MOTOR_Z_ID].status.action = ActionState_OK;
				SysMotor.motor[MOTOR_X_ID].ObjPos = XMOTOR_LEN_MAX;//X_QuHuoKouPos;
				SysMotor.motor[MOTOR_Y_ID].ObjPos = Y_QuHuoKouPos;
				XMotorStart();
				YMotorStart();
				SysMotor.motor[MOTOR_D_ID].Param = DEF_Open;
				DMotorStart();//打开侧门	
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
				else if((HuoWuNearDetectFlag == 1)&&(SysMotor.motor[MOTOR_D_ID].status.action == ActionState_OK))	{//x y电机到位 & 检测到货物 & 侧门开到位					
					SysMotor.motor[MOTOR_T_ID].Param = 1;
					TMotorStart();//启动推杆 前推
//					HuoWuNearDetectFlag = 0;//清货物接近检测标志
					DevState.bits.SubState = DEV_ShipSubState_TuiGanMove;
					ShipStateFlag = 0;
				}
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_TuiGanMove)	{
			if(ShipStateFlag==0)	{
				if(SysMotor.motor[MOTOR_T_ID].status.action == ActionState_OK)	{//推杆前进到位
					SysMotor.motor[MOTOR_T_ID].Param = 0;
					TMotorStart();//启动推杆 后推
					ShipStateFlag = 1;
				}
			}
			else if(ShipStateFlag == 1)	{
				if(SysMotor.motor[MOTOR_T_ID].status.action == ActionState_OK)	{//推杆后退到位
//					SysMotor.motor[MOTOR_X_ID].ObjPos = 0;//x 先复位
//					XMotorStart();
					MotorReset(MOTOR_X_ID);
					ShipStateFlag = 2;
				}
			}
//			else if(ShipStateFlag == 2)	{
//				if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK)	{//x 复位ok
//					SysMotor.motor[MOTOR_D_ID].Param=DEF_Close;
//					DMotorStart();//关闭侧门	
//					ShipStateFlag = 3;					
//				}
//			}
//			else if(ShipStateFlag == 3)	{	
//				if(SysMotor.motor[MOTOR_D_ID].status.action == ActionState_OK)	{
//					SysMotor.motor[MOTOR_Y_ID].ObjPos = 0;				
//					YMotorStart();
//					ShipStateFlag = 4;	
//				}
//			}				
			else if(ShipStateFlag == 2)	{
//				if(SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{
				if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK && \
					SysMotor.motor[MOTOR_D_ID].status.action == ActionState_OK)	{	
					DevState.bits.SubState = DEV_ShipSubState_CeMenClosing;
					SysMotor.motor[MOTOR_L_ID].Param = 10000;//10s
					LMotorStart();
					HuoWuDetectFlag = 0;
				}
			}
			timecnt = 0;
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_CeMenClosing)	{//等待货物检测完成
			if(HuoWuDetectFlag == 1)	{
				SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Open;
				QuHuoMenMotorStart();//打开取货门 等待取走货物
				DevState.bits.SubState = DEV_ShipSubState_QuHuoKouOpening;
				timecnt = 0;
			}
			if(timecnt>15)	{//5s 超时货物检测失败
				ShipResult(ActionState_Fail);
			}
			else
				timecnt ++;
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_QuHuoKouOpening)	{
			if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_OK)	{//取货门开门到位
				timecnt ++;
				if(HuoWuDetectFlag == 0)	{//货物被取走 5s后关门
					if(timecnt>5)	{
						SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
						QuHuoMenMotorStart();
						DevState.bits.SubState = DEV_ShipSubState_QuHuoKouCloseing;
					}
				}
				if(timecnt > 30)	{//货物没被取走 30s后 关门
					SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
					QuHuoMenMotorStart();
					DevState.bits.SubState = DEV_ShipSubState_QuHuoKouCloseing;
				}
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_QuHuoKouCloseing)	{
			if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_OK)	{//取货门关门到位
				ShipResult(ActionState_OK);//出货完成
			}
		}
		id = SysMotor.RunningID;
		if(SysMotor.motor[id].status.action == ActionState_Fail)	{//电机动作失败
			ShipResult(ActionState_Fail);
			SYS_PRINTF("ship failed %d.\r\n", id);
		}
	}
}

