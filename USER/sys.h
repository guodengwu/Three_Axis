#ifndef __SYS_H
#define __SYS_H

#include "includes.h"

#define	VERSION		"V0.1.D"

#define SYSSTATE_NONE					0
#define SYSSTATE_RESET					DEF_BIT00_MASK
#define SYSSTATE_XMOTORRESET				DEF_BIT01_MASK
#define SYSSTATE_YMOTORRESET				DEF_BIT02_MASK

typedef struct _sys_io {
	BIT8 state1;
	BIT8 state2;
	BIT8 HongWaiState;
} _sys_io_state_t;

typedef union _dev_state {
	 struct {
		CPU_INT08U  	SubState		:5;
		CPU_INT08U	State 			:3;
	 } bits;
    CPU_INT08U  ubyte;
} _dev_state_t;

typedef struct _sys_status	{
	u32 state;
	u32 DevAction;
}_sys_status_t;

typedef struct _sys_error	{
	BIT8 E1;
	BIT8 E2;
}_sys_error_t;

typedef struct _sys_logic_error	{
	u32 logic;
}_sys_logic_error_t;

enum dev_state {
	DEV_STATE_IDLE = 0,//空闲
	DEV_STATE_BUSY,//忙碌
	DEV_STATE_SHIPING,//出货中
	DEV_STATE_TEST,//测试中
	DEV_STATE_RESET,//复位中
};

enum dev_ship_state {//出货状态
	DEV_ShipStateIDLE = 0,//
	DEV_ShipStateMotorUp,//升降机上升
	DEV_ShipStateReqShip,//请求出货中
	DEV_ShipStateCeMenOpening,//侧门打开中
	DEV_ShipState_TuiGanMove,////推杆动作
	DEV_ShipState_CeMenClosing,//侧门关闭
	DEV_ShipState_QuHuoKouOpening,//取货口开门中
	DEV_ShipState_PleaseTakeoutCargo,//请求取货
	DEV_ShipState_QuHuoKouCloseReady,
	DEV_ShipState_QuHuoKouClosing,
	DEV_ShipState_QuHuoKouClosed,
};

enum dev_logic_err {
	LE_NONE = 0,
	LE_XMOTOR_DuZhuan = 1,//堵转
	LE_YMOTOR_DuZhuan = 20,
	LE_ZMOTOR = 30,
	LE_TuiGan = 40,
	LE_CeMen = 50,
	LE_QuHuoKou = 60,
	LE_QuHuoMen = 70,
	LE_HuoPanHongWai = 80,
	LE_QuHuoKouWai = 85,
};


extern _sys_logic_error_t SysLogicErr;
extern _sys_io_state_t IOState;
extern _dev_state_t DevState;
extern _sys_status_t Sys;
extern _sys_error_t SysHDError;

void CheckIOState(void);
void SysDataInit(void);
void ResetMotorStuckMonitorCnt(void);
void MotorStuckMonitorEnable(u8 flag);
#endif

