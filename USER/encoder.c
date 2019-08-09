#include "encoder.h"


Encoder_t encoder[ENCODER_NUMS];
static void XEncoderInit(void);
static void YEncoderInit(void);

void EncoderDataInit(void)
{
	EncoderReset(EncoderX_ID);
	EncoderReset(EncoderY_ID);
	XEncoderInit();
	YEncoderInit();
}

void EncoderReset(u8 id)
{
	encoder[id].pluse = 0;
}

static void XEncoderInit(void)
{
    TMOD |= 0x04;                                //外部计数模式
    TL0 = 0x0;
    TH0 = 0x0;
    TR0 = 1;                                    //启动定时器
    ET0 = 0;                                    //使能定时器中断
}

static void YEncoderInit(void)
{
	TMOD |= 0x40;                                //外部计数模式
    TL1 = 0x0;
    TH1 = 0x0;
    TR1 = 1;                                    //启动定时器
    ET1 = 0;                                    //使能定时器中断
}
static u16 Time0RegBk=0, Time1RegBk=0;
void ReadEncoder(TMotor *pMotor)
{
	u8 reghigh,reglow;
	u16 regval,regdiff=0;
	
	if(pMotor->id==EncoderX_ID)		{
		reghigh = TH0;
		reglow = TL0;
//		TH0 = 0;
//		TL0 = 0;
		regval = reghigh<<8 | reglow;
		if(Time0RegBk >= regval)	{
			regdiff = regval - Time0RegBk;
		}
		else 
			regdiff = 0xffff - Time0RegBk + regval;
		Time0RegBk = regval;
	}else if(pMotor->id==EncoderY_ID)	{
		reghigh = TH1;
		reglow = TL1;
//		TH1 = 0;
//		TL1 = 0;
		regval = reghigh<<8 | reglow;
		if(Time1RegBk >= regval)	{
			regdiff = regval - Time1RegBk;
		}
		else 
			regdiff = 0xffff - Time1RegBk + regval;
		Time1RegBk = regval;
	}	
	if(pMotor->dir==MOTOR_TO_MIN)	{
		encoder[pMotor->id].pluse -= regdiff;
	}else if(pMotor->dir==MOTOR_TO_MAX)		{
		encoder[pMotor->id].pluse += regdiff;
	}	
	
}
