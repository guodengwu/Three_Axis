#include "bsp.h"

////////////////////////////////////
//IO���ú���
static void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		        //�ṹ����
//����
	GPIO_InitStructure.Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);
		GPIO_InitStructure.Pin  = /*GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|*/GPIO_Pin_7;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);
		GPIO_InitStructure.Pin  = GPIO_Pin_1/*|GPIO_Pin_5|GPIO_Pin_6*/|GPIO_Pin_7;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	
		GPIO_InitStructure.Pin  = GPIO_Pin_1;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);
		GPIO_InitStructure.Pin  = GPIO_Pin_5/*|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3*/;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P6,&GPIO_InitStructure);
		GPIO_InitStructure.Pin  = GPIO_Pin_0/*|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3*/;
	//GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P7,&GPIO_InitStructure);
	
	/*GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.Mode = GPIO_HighZ;
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);*/

//���
	
//	GPIO_InitStructure.Pin  = GPIO_Pin_4;
//	GPIO_InitStructure.Mode = GPIO_OUT_PP;
//	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	
//	
//	GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_1/*|GPIO_Pin_4*/|GPIO_Pin_5/*|GPIO_Pin_7*/;
//	GPIO_InitStructure.Mode = GPIO_OUT_PP;
//	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	
//	
//	GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.Mode = GPIO_OUT_PP;
//	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	
//	
//	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;    //4 5 �ܽ�
//	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
//	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	        //��ʼ�� P3
//	
//	GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_1/*|GPIO_Pin_2*/|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7;
//	GPIO_InitStructure.Mode = GPIO_OUT_PP;
//	GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5;
//	GPIO_InitStructure.Mode = GPIO_OUT_PP;
//	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	

//	GPIO_InitStructure.Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6||GPIO_Pin_6|GPIO_Pin_7;    //4 5 �ܽ�
//	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
//	GPIO_Inilize(GPIO_P6,&GPIO_InitStructure);	        //��ʼ�� P3
	
 	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_2/*|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7*/;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P7,&GPIO_InitStructure);
	
	LIGHT_CTRL1=0;
	LIGHT_CTRL2=0;
}
#if 0
#if 1
/*************  ����1��ʼ������ *****************/
static void	UART1_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����	
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			    //������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = DISABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityHigh;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P36_P37;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	UART1_Init(&COMx_InitStructure);		//��ʼ������1 USART1,USART2
}
#endif
/*************  ����3��ʼ������ *****************/
void	UART3_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART3_SW_P00_P01;	//�л��˿�,   UART3_SW_P00_P01,UART3_SW_P50_P51
	UART3_Init(&COMx_InitStructure);
}
void Timer1_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;
	
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//ָ��ʱ��Դ, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	//TIM_InitStructure.TIM_Value     = 65536UL - ((MAIN_Fosc/12) / 50);		//��ֵ,20ms
	TIM_InitStructure.TIM_Value     = 25536UL;
	TIM_InitStructure.TIM_Run       = DISABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer1,&TIM_InitStructure);				//��ʼ��Timer1	  Timer0,Timer1,Timer2
}

void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//�ṹ����

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//�ж�ģʽ,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//�ж����ȼ�,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = DISABLE;				//�ж�����,     ENABLE��DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//��ʼ��INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
	
	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_Fall;	//�ж�ģʽ,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//�ж����ȼ�,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = DISABLE;				//�ж�����,     ENABLE��DISABLE
	Ext_Inilize(EXT_INT1,&EXTI_InitStructure);				//��ʼ��INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P16 | ADC_P17 | ADC_P14 | ADC_P13;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE
}
#endif
///////////////////////////////////////////////////////////
//������:   bsp
//����:    ����Ӳ����ʼ��
void bsp(void)
{
    GPIO_config();          //IO���ú���
	UART3_config();//���ڴ��ڵ��Դ�ӡ
	UART4_config();
	/*	UART1_config();//���Ͽ�
    UART3_config();
		Timer1_config();//����˶���ʱ
		Init_Display();
		motor_init();		
		EXTI_config();
		ADC_config();
      */
}

void soft_reset(void)
{
	if(Sys.state&SYSSTATE_RESET)	{
		delay_us(3000);
		IAP_CONTR = 0x60;
	}
}