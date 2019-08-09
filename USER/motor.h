#ifndef __MOTOR_H
#define __MOTOR_H

#include "includes.h"

#define MOTOR_TO_MAX         DEF_True        // To Max
#define MOTOR_TO_MIN         DEF_False       // To Min

#define	MOTOR_RUN_TIMEOUT		600000 //6s

#define XMOTOR_LEN_MAX			600//mm
#define YMOTOR_LEN_MAX			1435//mm

#define	MicroSteps				20//码盘细分
#define XMotor_NumPerRound		100//x电机一圈100mm
#define YMotor_NumPerRound		200//y电机一圈200mm
#define XMotor_StepsPerum		(XMotor_NumPerRound/MicroSteps)//5mm
#define YMotor_StepsPerum		(YMotor_NumPerRound/MicroSteps)//10mm

typedef enum {
    MOTOR_X_ID      = 0,
    MOTOR_Y_ID,
	MOTOR_Z_ID,
	MOTOR_T_ID,
	MOTOR_D_ID,
	MOTOR_L_ID,
	MOTOR_QuHuoMen_ID,
} MOTOR_ID;
#define MOTOR_ID_MIN 			MOTOR_X_ID
#define MOTOR_ID_MAX 			MOTOR_QuHuoMen_ID
#define MOTOR_NUMS       	(MOTOR_ID_MAX - MOTOR_ID_MIN + 1)

typedef struct {
    INT8U   is_run;
    INT8U   action;
    INT8U   abort_type;
} motor_state_t;
/*
typedef struct {	
	INT32S		x;
    INT32S      y;
    INT32S      z;
} Position_t;
*/
typedef struct Motor_t  {
	u8 id;
	motor_state_t       status;
	INT32S CurPos;
	INT32S ObjPos;
	INT32U Param;
	u8 dir;
}TMotor;

typedef union _allmotor_state {
    struct {
		CPU_INT08U 	XMotor	:1;
		CPU_INT08U  YMotor	:1;
		CPU_INT08U  ZMotor	:1;
		CPU_INT08U  TMotor	:1;
		CPU_INT08U  DMotor	:1;
		CPU_INT08U  LMotor	:1;
		CPU_INT08U  QuHuoMenMotor	:1;
		CPU_INT08U  UNUSED	:1;
	}bits;
	CPU_INT08U  ubyte;
}allmotor_state_t;

typedef struct _SysMotor  {
	u8 MotorIDRunning;
	allmotor_state_t ALLMotorState;//所有电机状态 0-空闲 1-运行中
	TMotor motor[MOTOR_NUMS];
}SysMotor_t;

enum eMotorState {
    MotorState_Stop         = 0,    // Motor State:stop
    MotorState_Run          = 1,    // Motor State:run
    MotorState_Stuck		= 2,	//电机卡死
    
    MotorState_Unkown = 0xff,
};

enum eActionState {
    ActionState_Idle     = 0,    // Action State:IDLE
	ActionState_Busy,
    ActionState_Doing       ,    // Action State:Doing
    ActionState_OK          ,    // Action State:OK
    ActionState_Fail        ,     // Action State:Fail
	ActionState_DoReady		,
};

enum eMotorAbort {
	MotorAbort_Normal    = 0,       // Motor Abort:Normal
	MotorAbort_Timeout = 1,       // Motor Abort:Over steps
	MotorAbort_Stuck   = 2,       // Motor Abort:door opened
	MotorAbort_LimitOpt  = 3,        // Motor Abort:touch limit opt
	MotorAbort_Min_LimitOpt  = 4,
	MotorAbort_Max_LimitOpt  = 5,
	MotorAbort_OpenOpt  = 6,
	MotorAbort_CloseOpt  = 7,
};

extern SysMotor_t SysMotor;

void MotorInit(void);
void MotorStart(void);
void MotorStop(u8 stop_type);
void MotorTest(void);
void SoftTimerStop(_softtimer_t *psofttimer);
void StopXMotor(void);
void StopYMotor(void);
void StopZMotor(void);
void StopLMotor(void);
void StopTMotor(void);
void StopDMotor(void);
void StopQuHuoMenMotor(void);
void MotorReset(u8 id);
void CalcXYMotorPos(void);
void XYMotorResetCheck();
void CheckMaPan(void);
#endif
