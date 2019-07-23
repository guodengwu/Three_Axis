#include "misc.h"


//自己写的log函数
float MYLOG(float a)
{
   int N = 10;//取5+1项计算
   int k,nk;
   float x,xx,y;
   x = (a-1)/(a+1);
   xx = x*x;
   nk = 2*N+1;
   y = 1.0/nk;
   for(k=N;k>0;k--)
   {
     nk = nk - 2;
     y = 1.0/nk+xx*y;
   }
	 x *= (float)2.0;
   return (float)(x*y);
}
//热敏电阻和温度转换公式 1/T1 =ln(Rt/Rp)/Bx+1/T2
/*#define Rp				120
#define Bx				3910*/
#define	Ka				273.15f
#define TEMP_25			25.0f
#define T2_cent			(1/(Ka+TEMP_25))//0.003354f//298.15f
float CalculateTemperature(u32 Rt,const u32 Rp,const u32 Bx)
{
	float temp;

	temp = (Rt*1.0)/Rp;
	temp = MYLOG(temp);//ln(Rt/Rp)
	temp/=Bx;//ln(Rt/Rp)/B
	temp+=T2_cent;
	temp = 1/(temp);
	temp-=Ka;

	return temp;
}

INT16U CRC16_Sum(INT8U *dat, INT8U len)
{
	INT16U crc=0,j;
	for(j=0; j<len; j++)
	{
		crc = crc + dat[j];
	}
	return crc;
}

INT8U CRC8_XOR(INT8U *dat, INT8U len)
{
	INT16U crc=0,j;
	for(j=0; j<len; j++)
	{
		crc = crc + dat[j];
	}
	return crc;
}
/*********************************************************************
 *  FUNCTION: FloatTo4uint8Format							          *
 *																	  *
 *  PARAMETERS:  							                          *
 *																	  *
 *  DESCRIPTION: ?Float?????char??              			  *
 *																 	  *
 *  RETURNS: ?													   	  *
 *																	  *
 *********************************************************************/
u16 FloatTo4uint8Format(u8* Dest, const float Source)
{
    u16 Len,i;
    u32 temp = Source;
    Len = sizeof(float);
    //temp = *(u32*)&Source;

    for (i = 0; i < Len; i++)
    {
        *Dest++ = (u8)(temp >> (8*(Len-i-1)));
    }

    return Len;
}

s32 floatToInt(float f)
{
    s32 i = 0;
    if(f>0) //
      i = (s32)((f*10 + 5)/10);
    else if(f<0) //
      i = (s32)((f*10 - 5)/10);
    else i = 0;

    return i;
}
//冒泡排序
u8 BubbleSort(u8 *pbuf,u32 size)
{
	u8 i,j,flag;
	u8 x,y;

	if(size==0)	{
			return 0;
	}
	i=0;
	/*---------------- 冒泡排序,由小到大排序 -----------------*/
	do{
		flag=0;
		for (j=0;j<size-i-1;j++)
		{
			if (pbuf[j] > pbuf[j+1])
			{
				x = pbuf[j];
				pbuf[j]   = pbuf[j+1];
				pbuf[j+1] = x;
				flag = 1;
			}
			if (pbuf[j] > pbuf[j+1])
			{
				y = pbuf[j];
				pbuf[j] = pbuf[j+1];
				pbuf[j+1] = y;
				flag = 1;
			}
		}
		i++;
	}while((i<size) && flag);
	//////////////////////////////////////////////////////
	return 1;
}

//BCD码转十进制函数，输入BCD，返回十进制
u8 BCD_Decimal(u8 bcd)
{
	u8 Decimal;

	Decimal=bcd>>4;
	return(Decimal=Decimal*10+(bcd&=0x0F));
}
	
