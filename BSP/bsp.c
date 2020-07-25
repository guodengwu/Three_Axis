#include "bsp.h"
//#include "encoder.h"
#include "PWM.h"

////////////////////////////////////
//IO配置函数
static void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		        //结构定义
//输入
	GPIO_InitStructure.Pin  = /*GPIO_Pin_4|*/GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);
		GPIO_InitStructure.Pin  = /*GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|*/GPIO_Pin_7;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);
		GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	
		GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_2;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);
			GPIO_InitStructure.Pin  = GPIO_Pin_3;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);
		GPIO_InitStructure.Pin  = GPIO_Pin_5|GPIO_Pin_6/*|GPIO_Pin_2|GPIO_Pin_3*/;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P6,&GPIO_InitStructure);
		GPIO_InitStructure.Pin  = GPIO_Pin_0/*|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3*/;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P7,&GPIO_InitStructure);
	
	/*GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);*/

//输出
	
	GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_0;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	
//	
	GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	
//	
//	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;    //4 5 管脚
//	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
//	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	        //初始化 P3
//	
	GPIO_InitStructure.Pin  = GPIO_Pin_0|/*GPIO_Pin_1|*/GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4/*|GPIO_Pin_7*/;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);
//	
	GPIO_InitStructure.Pin  = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	

	GPIO_InitStructure.Pin  = GPIO_Pin_4|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;    //4 5 管脚
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P6,&GPIO_InitStructure);	        //初始化 P3
	
 	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P7,&GPIO_InitStructure);

	LIGHT_CTRL1_OUT = 0;
	LIGHT_CTRL2_OUT = 0;
	IO_PWM2 = 0;
	IO_PWM3 = 0;
	IO_PWM4 = 0;
//	IO_PWM5 = 0;
	IO_PWM6 = 0;
	Z_MOTOR_PWM1 = 0;
	Z_MOTOR_PWM2 = 0;
	T_MOTOR_ENABLE = 0;	
	D_MOTOR_ENABLE = 0;
	L_MOTOR_ENABLE = 0;
	QuHuoMen_MOTOR_PWM1 = 0;
	QuHuoMen_MOTOR_PWM2 = 0;
	QuHuoMen_MOTOR_ENABLE = 0;
	BK_MOTOR_ENABLE = 0;
	RS485_CTRL = 0;//默认接收模式
}
#if 0
#if 1
/*************  串口1初始化函数 *****************/
static void	UART1_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义	
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			    //波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = DISABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityHigh;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P36_P37;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	UART1_Init(&COMx_InitStructure);		//初始化串口1 USART1,USART2
}
#endif
/*************  串口3初始化函数 *****************/
void	UART3_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART3_SW_P00_P01;	//切换端口,   UART3_SW_P00_P01,UART3_SW_P50_P51
	UART3_Init(&COMx_InitStructure);
}
void Timer1_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;
	
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//指定时钟源, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	//TIM_InitStructure.TIM_Value     = 65536UL - ((MAIN_Fosc/12) / 50);		//初值,20ms
	TIM_InitStructure.TIM_Value     = 25536UL;
	TIM_InitStructure.TIM_Run       = DISABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer1,&TIM_InitStructure);				//初始化Timer1	  Timer0,Timer1,Timer2
}

void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//结构定义

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = DISABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
	
	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_Fall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = DISABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT1,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//结构定义
	ADC_InitStructure.ADC_Px        = ADC_P16 | ADC_P17 | ADC_P14 | ADC_P13;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//初始化
	ADC_PowerControl(ENABLE);							//单独的ADC电源操作函数, ENABLE或DISABLE
}
#endif

///////////////////////////////////////////////////////////
//函数名:   bsp
//功能:    板载硬件初始化
void bsp(void)
{
    GPIO_config();          //IO配置函数
	UART3_config();//用于串口调试打印
	UART4_config();
	//Timer1Init();
	Timer4Init();
	PWMInit();
}
//软件复位分2种：
//从用户程序区软件复位并切换到用户程序 ; -- 0x20
//从用户程序区软件复位并切换到ISP程序下载监控区  --  0x60 （会导致取货门开门）
void soft_reset(void)
{
	if(Sys.state&SYSSTATE_RESET)	{
		QuHuoMen_MOTOR_PWM1 = 0;
		QuHuoMen_MOTOR_PWM2 = 0;
		QuHuoMen_MOTOR_ENABLE = 0;
		delay_us(100);
		IAP_CONTR = 0x20;//0x60;
	}
}
