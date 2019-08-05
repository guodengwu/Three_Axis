#ifndef __ENCODER_H
#define __ENCODER_H

#include "includes.h"

enum EncoderNUM{
    EncoderX_ID      = 0,
    EncoderY_ID,
};

#define ENCODER_ID_MIN 			EncoderX_ID
#define ENCODER_ID_MAX 			EncoderY_ID
#define ENCODER_NUMS       		(ENCODER_ID_MAX - ENCODER_ID_MIN + 1)

typedef struct _Encoder  {
	s32 pluse;
	
}Encoder_t;

extern Encoder_t encoder[ENCODER_NUMS];

s32 ReadEncoder(u8 id);

#endif

