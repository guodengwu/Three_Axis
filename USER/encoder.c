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
    TMOD = 0x04;                                //外部计数模式
    TL0 = 0xff;
    TH0 = 0xff;
    TR0 = 1;                                    //启动定时器
    ET0 = 1;                                    //使能定时器中断
	EA = 1;
}

static void YEncoderInit(void)
{
	TMOD |= 0x40;                                //外部计数模式
    TL1 = 0xff;
    TH1 = 0xff;
    TR1 = 1;                                    //启动定时器
    ET1 = 1;                                    //使能定时器中断
	EA = 1;
}

void TM0_Isr() interrupt 1
{
	if(X_ENCODER_IN==0)	{
		if(SysMotor.motor[MOTOR_X_ID].dir==MOTOR_TO_MIN)	{
			encoder[EncoderX_ID].pluse --;
		}
		else if(SysMotor.motor[MOTOR_X_ID].dir==MOTOR_TO_MAX)		{
			encoder[EncoderX_ID].pluse ++;
		}	
	}
}

void TM1_Isr() interrupt 3
{
	if(Y_ENCODER_IN==0)	{
		if(SysMotor.motor[MOTOR_Y_ID].dir==MOTOR_TO_MIN)	{
			encoder[EncoderY_ID].pluse --;
		}
		else if(SysMotor.motor[MOTOR_Y_ID].dir==MOTOR_TO_MAX)		{
			encoder[EncoderY_ID].pluse ++;
		}
	}
}

//static u16 Time0RegBk=0xffff, Time1RegBk=0xffff;
//void ReadEncoder(TMotor *pMotor)
//{
//	u8 reghigh,reglow;
//	u16 regval;
//	s32 regdiff;
//	
//	if(pMotor->id==EncoderX_ID)		{
//		reghigh = TH0;
//		reglow = TL0;
//		regval = reghigh<<8;
//		regval |= reglow;
//		regdiff = regval - Time0RegBk;
//		if(regdiff<0)
//			regdiff += 0xffff;
//		/*if(regdiff>0)
//			SYS_PRINTF("  :%d %d %d\r\n",regdiff,regval,Time0RegBk);*/
//		Time0RegBk = regval;
//		if(pMotor->dir==MOTOR_TO_MIN)	{
//			encoder[EncoderX_ID].pluse -= regdiff;
//		}else if(pMotor->dir==MOTOR_TO_MAX)		{
//			encoder[EncoderX_ID].pluse += regdiff;
//		}	
//	}
//	else if(pMotor->id==EncoderY_ID)	{
//		reghigh = TH1;
//		reglow = TL1;
//		regval = reghigh<<8;
//		regval |= reglow;
//		regdiff = regval - Time1RegBk;
//		if(regdiff<0)
//			regdiff += 0xffff;
////		if(Time1RegBk != regval)
////			SYS_PRINTF("%ld %d\r\n",regdiff,regval);
//		Time1RegBk = regval;
//		if(pMotor->dir==MOTOR_TO_MIN)	{
//			encoder[EncoderY_ID].pluse -= regdiff;
//		}else if(pMotor->dir==MOTOR_TO_MAX)		{
//			encoder[EncoderY_ID].pluse += regdiff;
//		}
//	}	
//}


