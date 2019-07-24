#ifndef __MOTOR_H
#define __MOTOR_H

#include "includes.h"

typedef enum {
    MOTOR_X      = 0,
    MOTOR_Y,
	MOTOR_Z,
	MOTOR_TuiGan,
	MOTOR_CeMen,
	MOTOR_QuHuoKou,
	MOTOR_QuHuoMen,
} MOTOR_ID;
#define MOTOR_ID_MIN 			MOTOR_X
#define MOTOR_ID_MAX 			MOTOR_QuHuoMen
#define MOTOR_NUMS       	(MOTOR_QuHuoMen - MOTOR_X + 1)

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
}TMotor;

typedef struct _SysMotor  {
	BIT8 ALLMotorState;
	TMotor motor[MOTOR_NUMS];
}SysMotor_t;

enum eMotorState {
    MotorState_Stop         = 0,    // Motor State:stop
    MotorState_Run          = 1,    // Motor State:run
    MotorState_Stuck		= 2,	//µç»ú¿¨ËÀ
    
    MotorState_Unkown = 0xff,
};

enum eActionState {
    ActionState_Idle     = 0,    // Action State:IDLE
    ActionState_Doing       = 1,    // Action State:Doing
    ActionState_OK          = 2,    // Action State:OK
    ActionState_Fail        = 3,     // Action State:Fail
	ActionState_DoReady		= 4,
};

extern SysMotor_t SysMotor;

void MotorInit(void);

#endif
