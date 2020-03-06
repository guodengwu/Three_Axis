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
	Sys.DevAction = ActionState_Idle;
	
	IOState.state1.ubyte = DEF_False;
	IOState.state2.ubyte = DEF_False;
	IOState.HongWaiState.ubyte = DEF_False;
	
	DevState.ubyte = DEV_STATE_IDLE;//机器状态
	
	SysHDError.E1.ubyte = DEF_False;
	SysHDError.E2.ubyte = DEF_False;
	
	SysLogicErr.logic = 0;
	
//	Ship_t.state = DEV_ShipStateIDLE;
}
u8 GetHuoWuTimeCnt = 0;
u8 NoHuoWuTimeCnt = 0;
void CheckIOState(void)
{
	IOState.state1.bits.b0 = !X_MOTOR_RightLimit_IN;	
	IOState.state1.bits.b1 = !X_MOTOR_LeftLimit_IN;
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{
		if((X_MOTOR_LeftLimit_IN==0&&SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MIN) || \
		   (X_MOTOR_RightLimit_IN==0&&SysMotor.motor[MOTOR_X_ID].dir == MOTOR_TO_MAX))	{//x碰到上限/下限 强制停止电机
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
		if(CeMenMinLimit_IN==0&&SysMotor.motor[MOTOR_D_ID].Param==DEF_Close|| \
			CeMenMaxLimit_IN==0&&SysMotor.motor[MOTOR_D_ID].Param==DEF_Open)	{//门电机限位 限位失效时超时20s停止
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
	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{
		if((QuHuoKouOpenLimit_IN == 0&&SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Open) || \
			(QuHuoKouCloseLimit_IN == 0&&SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close))	{//取货口电机限位  限位失效时超时10s停止
			StopQuHuoMenMotor();
			SoftTimerStop(&Timer2Soft);
			SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_OK;
			if(QuHuoKouOpenLimit_IN==0)	{
				SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Min_LimitOpt;//SYS_PRINTF("QuHuoKouOpenLimit_IN\r\n");
			}else if(QuHuoKouCloseLimit_IN==0)	{
				SysMotor.motor[MOTOR_QuHuoMen_ID].status.abort_type = MotorAbort_Max_LimitOpt;//SYS_PRINTF("QuHuoKouCloseLimit_IN\r\n");
			}
		}
	}
	IOState.state1.bits.b2 = !JiaShouLimit_IN;
	if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==DEF_Close)	{//放夹手处理
		if(JiaShouLimit_IN==0)	{
			StopQuHuoMenMotor();
			SoftTimerStop(&Timer2Soft);
			SysMotor.motor[MOTOR_QuHuoMen_ID].status.action = ActionState_Fail;
			SysMotor.ALLMotorState.bits.QuHuoMenMotor = DEF_Run;
			SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Open;
			QuHuoMenMotorStart();
		}
	}
	IOState.state2.bits.b0 = !Y_MOTOR_MinLimit_IN;
	IOState.state2.bits.b1 = !Y_MOTOR_MaxLimit_IN;
	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{
		if((Y_MOTOR_MinLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MIN) || \
			(Y_MOTOR_MaxLimit_IN==0&&SysMotor.motor[MOTOR_Y_ID].dir == MOTOR_TO_MAX))	{//y碰到上限/下限 强制停止电机
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
	if(HuoWuNearSwitch_IN==0)	{//货物接近出口，启动推杆电机
//		SysMotor.motor[MOTOR_T_ID].Param = 1;//前推
//		TMotorStart();
		HuoWuNearDetectFlag = 1;//货物接近标志有效
	}
	if(MotorStuckMonitor())	{
		MotorStop(DEF_Fail);SYS_PRINTF("MotorStuck\r\n");
		if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//推杆电机根据该信号停止
			SysMotor.motor[MOTOR_T_ID].status.action = ActionState_OK;
			Sys.DevAction = ActionState_OK;
		}
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
/*void CheckDevAction(void)
{
	u8 runing_id;
	
	runing_id = SysMotor.MotorIDRunning;
	if(DevState.bits.SubState == 1)
		Sys.DevAction == ActionState_Busy;
	else 
		Sys.DevAction == ActionState_Idle;
}*/
#define X_QuHuoKouPos	0
#define Y_QuHuoKouPos	10
void ShipProcess(void)
{
	static u8 timecnt = 0;
	static u8 ReqShipTimeCnt = 0;
	u8 id;
	
	if(DevState.bits.State == DEV_STATE_SHIPING)	{	
		if(DevState.bits.SubState == DEV_ShipSubStateMotorUp)	{//等待x y移动到位
			if(SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{
				DevState.bits.SubState = DEV_ShipSubStateStartZmotor;//启动z电机
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateStartZmotor)	{//启动z
			ZMotorStart();
			DevState.bits.SubState = DEV_ShipSubStateReqShip;//请求出货 等待08指令
			timecnt = 0;
			ReqShipTimeCnt = 0;
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateReqShip)	{//请求出货30s超时
			ReqShipTimeCnt ++;
			if(ReqShipTimeCnt>30)	{//出货失败
				ReqShipTimeCnt = 0;
				DevState.bits.State = DEV_STATE_SHIP_Failed;
			}
			HuoWuNearDetectFlag = 0;//清货物接近检测标志
		}
		else if(DevState.bits.SubState == DEV_ShipSubStateCeMenOpening)	{			
			if(timecnt>=2)	{
//				timecnt = 0;
				StopZMotor();
				SysMotor.motor[MOTOR_X_ID].ObjPos = X_QuHuoKouPos;
				SysMotor.motor[MOTOR_Y_ID].ObjPos = Y_QuHuoKouPos;
				XMotorStart();
				YMotorStart();
				SysMotor.motor[MOTOR_D_ID].Param = DEF_Open;
				DMotorStart();//打开侧门				
			}
			else
				timecnt ++;
			if(SysMotor.motor[MOTOR_X_ID].status.action == ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK&& \
				SysMotor.motor[MOTOR_D_ID].status.action == ActionState_OK&&HuoWuNearDetectFlag == 1)	{//x y电机到位 & 检测到货物 & 侧门开到位					
				SysMotor.motor[MOTOR_T_ID].Param = 1;
				TMotorStart();//启动推杆 前推
				HuoWuNearDetectFlag = 0;//清货物接近检测标志
				DevState.bits.SubState = DEV_ShipSubState_TuiGanMove;
			}
		else if(DevState.bits.SubState == DEV_ShipSubState_TuiGanMove)	{
			if(SysMotor.motor[MOTOR_T_ID].status.action == ActionState_OK)	{//推杆到位
				SysMotor.motor[MOTOR_T_ID].Param = 0;
				TMotorStart();//启动推杆 后推				
				SysMotor.motor[MOTOR_X_ID].ObjPos = 0;//x y复位
				SysMotor.motor[MOTOR_Y_ID].ObjPos = 0;
				XMotorStart();
				YMotorStart();
				if(SysMotor.motor[MOTOR_X_ID].status.action = ActionState_OK && SysMotor.motor[MOTOR_Y_ID].status.action == ActionState_OK)	{
					DevState.bits.SubState = DEV_ShipSubState_CeMenClosing;
					SysMotor.motor[MOTOR_D_ID].Param=DEF_Close;
					DMotorStart();//关闭侧门	
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
			}
			if(timecnt>5)	{//5s 超时货物检测失败
				DevState.bits.State = DEV_STATE_SHIP_Failed;
			}
			else
				timecnt ++;
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_QuHuoKouOpening)	{
			if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_OK)	{//取货门开门到位
				timecnt ++;
				if(HuoWuDetectFlag == 0||timecnt > 30)	{//货物被取走 or 30s后 关门
					SysMotor.motor[MOTOR_QuHuoMen_ID].Param = DEF_Close;
					QuHuoMenMotorStart();
					DevState.bits.SubState = DEV_ShipSubState_QuHuoKouCloseing;
				}
			}
		}
		else if(DevState.bits.SubState == DEV_ShipSubState_QuHuoKouCloseing)	{
			if(SysMotor.motor[MOTOR_QuHuoMen_ID].status.action == ActionState_OK)	{//取货门关门到位
				DevState.bits.State = DEV_STATE_IDLE;//取货流程结束
				DevState.bits.SubState = DEV_ShipSubStateIDLE;
			}
		}
		id = SysMotor.RunningID;
		if(SysMotor.motor[id].status.action == ActionState_Fail)
//		if(Sys.DevAction == ActionState_Fail)	{//电机动作失败
			DevState.bits.State = DEV_STATE_SHIP_Failed;
			DevState.bits.SubState = DEV_ShipSubStateIDLE;
		}
	}
}

