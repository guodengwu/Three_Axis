
#ifndef __USART_H
#define __USART_H	 

#include	"includes.h"

typedef struct
{ 
	u8	id;				//���ں�

	u8	TX_read;		//���Ͷ�ָ��
	u8	TX_write;		//����дָ��
	u8	TX_busy;		//æ��־
	u8 *pTXbuf;
	
	u8 	RX_Cnt;			//�����ֽڼ���
	u8	RX_TimeOut;		//���ճ�ʱ
	u8	B_RX_OK;		//���տ����
	u8 *pRXbuf;
} COMx_Define; 

#define UART4_RXLEN		20

#define		UART4_INT_ENABLE()		IE2 |=  (1<<4)	//������3�ж�
#define		UART4_INT_DISABLE()		IE2 &= ~(1<<4)	//������3�ж�

#define		UART3_INT_ENABLE()		IE2 |=  (1<<3)	//������3�ж�
#define		UART3_INT_DISABLE()		IE2 &= ~(1<<3)	//������3�ж�

#define		SET_TI4()			(S4CON |= 2)
#define usart4_tx_int_enable()     SET_TI4()

void UART3_SendByte(char dat);
void UART3_config(void);
void UART4_config(void);
void UART4_SendByte(char dat);

#endif

