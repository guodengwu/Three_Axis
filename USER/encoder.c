#include "encoder.h"

Encoder_t encoder[ENCODER_NUMS];

void EncoderInit(void)
{
	encoder[EncoderX_ID].pluse = 0;
	encoder[EncoderY_ID].pluse = 0;
}

void encoder_reset(u8 id)
{
	encoder[id].pluse = 0;
}

s32 ReadEncoder(u8 id)
{
	return encoder[id].pluse;
}
