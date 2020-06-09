#ifndef __MOTOR_H
#define __MOTOR_H

#include "includes.h"
#include "PWM.h"

#define MOTOR_TO_MAX         DEF_True        // To Max
#define MOTOR_TO_MIN         DEF_False       // To Min

#define	MOTOR_RUN_TIMEOUT		600000 //6s

#define XMOTOR_LEN_MAX			540//mm
#define YMOTOR_LEN_MAX			1435//mm

#define X_QuHuoKouPos			650//mm
#define Y_QuHuoKouPos			270//450//380//mm

#define XMOTOR_AccDec_LEN			100//mm
#define YMOTOR_AccDec_LEN			100//mm
#define MOTOR_CONSTANT_LEN			60//mm

#define MOTOR_LEN_RANG			10//mm

//#define XMOTOR_MIN_PWM				DEF_PWM4
//#define XMOTOR_MAX_PWM				DEF_PWM5
#define XMOTOR_PWM				DEF_PWM6
#define YMOTOR_MIN_PWM				DEF_PWM2
#define YMOTOR_MAX_PWM				DEF_PWM3
#define MOTOR_PWM_FREQ				200

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
	u8 RunningID;
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
//	ActionState_Retry		,
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
	MotorAbort_MaPanError = 8,
};

extern SysMotor_t SysMotor;

void MotorInit(void);
//void MotorStart(void);
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
void XMotorResetCheck();
void YMotorResetCheck();
void CheckMaPan(void);
void MotorStuck(void);
void XYMotorArrived(void);
//void XMotorSetDir(void);
//void YMotorSetDir(void);
void XMotorStart(void);
void YMotorStart(void);
void ZMotorStart(void);
void TMotorStart(void);
void LMotorStart(void);
void DMotorStart(void);
void QuHuoMenMotorStart(void);
u8 XMotorAccDec(void);
u8 YMotorAccDec(void);
#endif
