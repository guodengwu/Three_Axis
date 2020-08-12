
#ifndef __USART_H
#define __USART_H	 

#include	"includes.h"

typedef struct
{ 
	u8	id;				//串口号

	u8	TX_read;		//发送读指针
	u8	TX_write;		//发送写指针
	u8	TX_busy;		//忙标志
	u8 *pTXbuf;
	
	u8 	RX_Cnt;			//接收字节计数
	u8	RX_TimeOut;		//接收超时
	u8	B_RX_OK;		//接收块完成
	u8 *pRXbuf;
} COMx_Define; 

#define UART4_RXLEN		20

#define		UART4_INT_ENABLE()		IE2 |=  (1<<4)	//允许串口3中断
#define		UART4_INT_DISABLE()		IE2 &= ~(1<<4)	//允许串口3中断

#define		UART3_INT_ENABLE()		IE2 |=  (1<<3)	//允许串口3中断
#define		UART3_INT_DISABLE()		IE2 &= ~(1<<3)	//允许串口3中断

#define		SET_TI4()			(S4CON |= 2)
#define usart4_tx_int_enable()     SET_TI4()

void UART3_SendByte(char dat);
void UART3_config(void);
void UART4_config(void);
void UART4_SendByte(char dat);

#endif

