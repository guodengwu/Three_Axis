#include "includes.h"

COMx_Define COM3,COM4;
#define BRT_115200             (65536 - MAIN_Fosc / 115200 / 4)
#define BRT_9600             (65536 - MAIN_Fosc / 9600 / 4)

u8 	xdata UART4_RXBuff[UART4_RXLEN];	//接收缓冲
RINGBUFF_T uart4_rxring;
//用于调试 使用定时器3做波特率发生器
void UART3_config(void)
{
//    P_SW2 = 0x00;                               // RXD3/P0.0, TXD3/P0.1
	P_SW2 = 0x02;                               // RXD3_2/P5.0, TXD3_2/P5.1
	S3CON = 0x50;
    T3L = BRT_115200;
    T3H = BRT_115200 >> 8;
    T4T3M = 0x0a;
    COM3.TX_busy = DEF_Idle;
}
//用于485通讯 使用定时器4做波特率发生器
void UART4_config(void)
{
//    P_SW2 = 0x00;                               //RXD4/P0.2, TXD4/P0.3
	P_SW2 = 0x04;                               //RXD4_2/P5.2, TXD4_2/P5.3
	S4CON = 0x50;
    T4L = BRT_9600;
    T4H = BRT_9600 >> 8;
    T4T3M = 0xA0;
    COM4.TX_busy = DEF_Idle;
	RingBuffer_Init(&uart4_rxring, UART4_RXBuff, 1, UART4_RXLEN);
}

void UART4_SendByte(char dat) 	//写入发送缓冲，指针+1
{
	//while(COM4.TX_busy == DEF_Busy);
	//if(COM3.TX_busy == 0)		//空闲
	{  
		S4BUF = dat;
		//COM4.TX_busy = DEF_Busy;		//标志忙
		//SET_TI3();				
	}
}

void UART3_SendByte(char dat) 	//写入发送缓冲，指针+1
{
	while(COM3.TX_busy == DEF_Busy);
	//if(COM3.TX_busy == 0)		//空闲
	{  
		S3BUF = dat;
		COM3.TX_busy = DEF_Busy;		//标志忙
		//SET_TI3();				
	}
}

void Uart3Isr() interrupt 17 using 1
{
    if (S3CON & 0x02)//发送中断
    {
        S3CON &= ~0x02;
        COM3.TX_busy = DEF_Idle;
    }
  /*  if (S4CON & 0x01)//接收中断
    {
        S4CON &= ~0x01;
        buffer[wptr++] = S4BUF;
        wptr &= 0x0f;
    }*/
}
#include "protocol.h"
void Uart4Isr() interrupt 18 using 1
{
	u8 rxdat;
	
    if (S4CON & 0x02)
    {
        S4CON &= ~0x02;
       // COM4.TX_busy = DEF_Idle;
		if (usart.tx_complete != NULL) usart.tx_complete(&usart);
    }
    if (S4CON & 0x01)
    {
        S4CON &= ~0x01;
		rxdat = S4BUF;
		//RingBuffer_Insert(&uart4_rxring, &rxdat);
		if (usart.rx_indicate != NULL) usart.rx_indicate(&usart, rxdat);
    }
}