#ifndef _MISC_H
#define _MISC_H

#include "includes.h"

float CalculateTemperature(u32 Rt,const u32 Rp,const u32 Bx);

s32 floatToInt(float f);
INT16U CRC16_Sum(INT8U *dat, INT8U len);
u8 BubbleSort(u8 *pbuf,u32 size);
u8 BCD_Decimal(u8 bcd);
INT8U CRC8_XOR(INT8U *dat, INT8U len);
#endif
