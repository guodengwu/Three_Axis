#ifndef __MOTOR_H
#define __MOTOR_H

#include "includes.h"

#define MOTOR_TO_MAX         DEF_TRUE        // To Max
#define MOTOR_TO_MIN         DEF_FALSE       // To Min

#define	MOTOR_RUN_TIMEOUT		600000 //6s

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
	motor_state_t       status;
	//Position_t CurPos;
	INT16S CurPos;
	INT16U ObjPos;
	INT16U Param;
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

extern SysMotor_t SysMotor;

void MotorInit(void);
void MotorStart(void);
void MotorStop(u8 stop_type);
void MotorTest(void);
void SoftTimerStop(_softtimer_t *psofttimer);

#endif
