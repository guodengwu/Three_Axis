#include "includes.h"

COMx_Define COM3;
#define BRT_9600             (65536 - (MAIN_Fosc / 9600 / 4))
#define BRT_57600             (65536 - (MAIN_Fosc / 57600 / 4))

u8 	xdata UART4_RXBuff[UART4_RXLEN];	//接收缓冲
RINGBUFF_T uart4_rxring;
//用于调试 使用定时器3做波特率发生器 57600
void UART3_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	//u16 baud;

	GPIO_InitStructure.Pin  = GPIO_Pin_1;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);
	//    P_SW2 = 0x00;                               // RXD3/P0.0, TXD3/P0.1
	P_SW2 |= 0x02;                               // RXD3_2/P5.0, TXD3_2/P5.1
	
	/*baud = BRT_57600;
	S3CON = 0x10;		//8位数据,可变波特率
	S3CON |= 0x40;		//串口3选择定时器3为波特率发生器
	T4T3M |= 0x02;		//定时器3时钟为Fosc,即1T
	T3L = baud;		//设定定时初值
	T3H = baud>>8;		//设定定时初值
	T4T3M |= 0x08;		//启动定时器3
	UART3_INT_ENABLE();//允许中断
    COM3.TX_busy = DEF_Idle;*/
	S3CON |= 0x10;		//8位数据,可变波特率 9600
	S3CON &= 0xBF;		//串口3选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x41;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	UART3_INT_ENABLE();//允许中断
	COM3.TX_busy = DEF_Idle;
}

//用于485通讯 使用定时器4做波特率发生器 9600
void UART4_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	//u16 baud;
	    
	GPIO_InitStructure.Pin  = GPIO_Pin_3;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);
	//	P_SW2 = 0x04;                               //RXD4_2/P5.2, TXD4_2/P5.3
	P_SW2 &= ~0x04;                               //RXD4/P0.2, TXD4/P0.3

	/*baud = BRT_9600;
	S4CON = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	T4T3M |= 0x20;		//定时器4时钟为Fosc,即1T
	T4L = baud;//(baud&0xff);		//设定定时初值
	T4H = baud>>8;//baud>>8;		//设定定时初值
	T4T3M |= 0x80;		//启动定时器4
	UART4_INT_ENABLE();//允许中断*/
	S4CON |= 0x10;		//8位数据,可变波特率 9600
	S4CON &= 0xBF;		//串口4选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x41;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	UART4_INT_ENABLE();//允许中断
	RingBuffer_Init(&uart4_rxring, UART4_RXBuff, 1, UART4_RXLEN);
}

void UART4_SendByte(u8 dat) 	//写入发送缓冲，指针+1
{
	S4BUF = dat;
}

void UART3_SendByte(char dat) 	//写入发送缓冲，指针+1
{
	while(COM3.TX_busy == DEF_Busy);	
	COM3.TX_busy = DEF_Busy;		//标志忙
	S3BUF = dat;
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
void Uart4Isr() interrupt 18// using 1
{
	u8 rxdat;
	
	//UART4_INT_DISABLE();
    if (S4CON & 0x02)
    {
        S4CON &= ~0x02;
		if (usart.tx_complete != NULL) usart.tx_complete(&usart);
    }
    if (S4CON & 0x01)
    {
        S4CON &= ~0x01;
		rxdat = S4BUF;
		//if (usart.rx_indicate != NULL) usart.rx_indicate(&usart, rxdat);
		RingBuffer_Insert(&uart4_rxring, (const void *)&rxdat);
    }
	//UART4_INT_ENABLE();
}