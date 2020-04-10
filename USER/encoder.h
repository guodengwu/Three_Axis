#ifndef __ENCODER_H
#define __ENCODER_H

#include "includes.h"
#include "motor.h"

#define	XMaPanSubdivide				25//码盘细分
#define	YMaPanSubdivide				50//码盘细分
#define XMaPan_NumPerRound			100//x电机一圈100mm
#define YMaPan_NumPerRound			200//y电机一圈200mm
#define XMaPan_NumPerStep 		(XMaPan_NumPerRound/XMaPanSubdivide)//5 mm/step
#define YMaPan_NumPerStep 		(YMaPan_NumPerRound/YMaPanSubdivide)//10 mm/step


enum EncoderNUM{
    EncoderX_ID      = MOTOR_X_ID,
    EncoderY_ID,
};

#define ENCODER_ID_MIN 			EncoderX_ID
#define ENCODER_ID_MAX 			EncoderY_ID
#define ENCODER_NUMS       		(ENCODER_ID_MAX - ENCODER_ID_MIN + 1)

typedef struct _Encoder  {
	s32 pluse;
	//u16 diff;
}Encoder_t;

extern Encoder_t encoder[ENCODER_NUMS];

void ReadEncoder(TMotor *pMotor);
void EncoderDataInit(void);
void EncoderReset(u8 id);

#endif

