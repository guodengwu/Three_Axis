/*
*********************************************************************************************************
*	                                  
*	模块名称 : printf模块    
*	文件名称 : bsp_printf.c
*	版    本 : V2.0
*	说    明 : 实现printf和scanf函数重定向到串口1，即支持printf信息到USART1
*				实现重定向，只需要添加2个函数:
*				int fputc(int ch, FILE *f);
*				int fgetc(FILE *f);
*				对于KEIL MDK编译器，编译选项中需要在MicorLib前面打钩，否则不会有数据打印到USART1。
*				
*				这个c模块无对应的h文件。
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2013-11-24 armfly  首发
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "includes.h"
#include <stdio.h>
#include  "USART.h"

char putchar(char c)
{
    //USART_SendByte(USART3, (u8)c);
	UART3_SendByte(c);
    return c;
}
#if 0
/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	/* 写一个字节到USART1 */
	USART_SendData(&USART2_Handler, (uint8_t) ch);
	/* 等待发送结束 */
	//while (__HAL_UART_GET_FLAG(&USART2_Handler, USART_FLAG_TC) == RESET)
	//{}
     
	return ch;
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用scanff函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	//while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	//return (int)USART_ReceiveData(USART2_Handler);
}
#endif
