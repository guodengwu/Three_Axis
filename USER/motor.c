#include "motor.h"

SysMotor_t xdata SysMotor;

void MotorInit(void)
{
	u8 i;
	for(i=MOTOR_X_ID;i<MOTOR_QuHuoMen_ID;i++)	{
		SysMotor.motor[i].status.is_run = MotorState_Stop;
		SysMotor.motor[i].status.action = ActionState_Idle;
		SysMotor.motor[i].CurPos = 0;
	}
	SysMotor.MotorIDRunning = 0xff;
	SysMotor.ALLMotorState.ubyte = 0;
}
/*
void MotorSetDir(void)
{

}

void MotorEnable(void)
{

}

void MotorDisable(void)
{

}*/

void MotorStart(void)
{
	u8 runing_id;
	u32 motor_timeout;
	
	if(SysMotor.ALLMotorState.ubyte == 0)	{//û�е����Ҫ����
		return;
	}
	runing_id = SysMotor.MotorIDRunning;
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{//����x���
		X_MOTOR_PWM1 = 1;
		X_MOTOR_PWM2 = 0;
		X_MOTOR_ENABLE1 = 0;
		X_MOTOR_ENABLE2 = 1;
		motor_timeout = 3000;//30s
		//BSP_PRINTF("x motor testing.\r\n");
		//SysMotor.motor[MOTOR_X_ID].status.action = ActionState_Doing;
		
	}else	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{//����y���
		Y_MOTOR_PWM1 = 1;
		Y_MOTOR_PWM2 = 0;
		Y_MOTOR_ENABLE1 = 0;
		Y_MOTOR_ENABLE2 = 1;
		motor_timeout = 3000;
		//SysMotor.motor[MOTOR_Y_ID].status.action = ActionState_Doing;
	}
	else	if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{//����z���
		Z_MOTOR_PWM1 = 1;
		Z_MOTOR_PWM2 = 0;
		//SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_Z_ID].Param*10);	//����ʱ��	
		Z_MOTOR_ENABLE = 1;
		motor_timeout = 3000;
		//SysMotor.motor[MOTOR_Z_ID].status.action = ActionState_Doing;
		//return;
	}
	else	if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//�����Ƹ˵��
		if(SysMotor.motor[MOTOR_T_ID].Param==0)	{//0���� �� 1����
			T_MOTOR_PWM1 = 1;
			T_MOTOR_PWM2 = 0;			
		}
		else if(SysMotor.motor[MOTOR_T_ID].Param==1)	{
			T_MOTOR_PWM1 = 0;
			T_MOTOR_PWM2 = 1;
		}
		T_MOTOR_ENABLE = 1;
		motor_timeout = 1000;//10s
		//SysMotor.motor[MOTOR_T_ID].status.action = ActionState_Doing;
	}
	else	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{//���Բ��ŵ��
		if(SysMotor.motor[MOTOR_D_ID].Param==0)	{//0���� �� 1����
			D_MOTOR_PWM1 = 1;
			D_MOTOR_PWM2 = 0;
		}
		else if(SysMotor.motor[MOTOR_D_ID].Param==1)	{
			D_MOTOR_PWM1 = 0;
			D_MOTOR_PWM2= 1;
		}
		D_MOTOR_ENABLE = 1;
		motor_timeout = 1000;
		//SysMotor.motor[MOTOR_D_ID].status.action = ActionState_Doing;
	}
	else	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{//�����Ĵ����
		L_MOTOR_PWM1 = 1;
		L_MOTOR_PWM2 = 0;
		L_MOTOR_ENABLE = 1;
		motor_timeout = 3000;
		//SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_L_ID].Param*10);//����ʱ��
		//SysMotor.motor[MOTOR_L_ID].status.action = ActionState_Doing;
		//return;
	}
	else	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{//����ȡ���ŵ��
		if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==0)	{//0���� �� 1����
			QuHuoMen_MOTOR_PWM1 = 1;
			QuHuoMen_MOTOR_PWM1 = 0;
		}
		else if(SysMotor.motor[MOTOR_QuHuoMen_ID].Param==1)	{
			QuHuoMen_MOTOR_PWM1 = 0;
			QuHuoMen_MOTOR_PWM1= 1;
		}
		QuHuoMen_MOTOR_ENABLE = 1;
		motor_timeout = 1000;
	}
	SysMotor.motor[runing_id].status.action = ActionState_Doing;
	SoftTimerStart(&Timer2Soft, motor_timeout);//������г�ʱ����
}
//
void MotorStop(u8 stop_type)
{
	u8 runing_id;
	
	if(SysMotor.ALLMotorState.ubyte == 0)	{//û�е����Ҫ����
		return;
	}
	runing_id = SysMotor.MotorIDRunning;
	if(DevState.bits.State == DEV_STATE_TEST)	{
		DevState.bits.SubState = 0;//û�е������
	}
	
	if(SysMotor.ALLMotorState.bits.XMotor == DEF_Run)	{//ֹͣx���
		X_MOTOR_ENABLE1 = 0;
		X_MOTOR_ENABLE2 = 0;
		SysMotor.ALLMotorState.bits.XMotor = DEF_Stop;

	}else	if(SysMotor.ALLMotorState.bits.YMotor == DEF_Run)	{//ֹͣy���
		Y_MOTOR_ENABLE1 = 0;
		Y_MOTOR_ENABLE2 = 0;
		SysMotor.ALLMotorState.bits.YMotor = DEF_Stop;

	}
	else	if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{//ֹͣz���
		Z_MOTOR_PWM1 = 0;
		Z_MOTOR_PWM2 = 0;		
		Z_MOTOR_ENABLE = 0;
		SysMotor.ALLMotorState.bits.ZMotor = DEF_Stop;

	}
	else	if(SysMotor.ALLMotorState.bits.TMotor == DEF_Run)	{//ֹͣ�Ƹ˵��
		T_MOTOR_PWM1 = 0;
		T_MOTOR_PWM2 = 0;
		T_MOTOR_ENABLE = 0;		
		SysMotor.ALLMotorState.bits.TMotor = DEF_Stop;

	}
	else	if(SysMotor.ALLMotorState.bits.DMotor == DEF_Run)	{//ֹͣ���ŵ��
		D_MOTOR_PWM1 = 0;
		D_MOTOR_PWM2 = 0;
		D_MOTOR_ENABLE = 0;
		SysMotor.ALLMotorState.bits.DMotor = DEF_Stop;

	}
	else	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{//ֹͣ�Ĵ����
		L_MOTOR_ENABLE = 0;
		SysMotor.ALLMotorState.bits.LMotor = DEF_Stop;
	}
	else	if(SysMotor.ALLMotorState.bits.QuHuoMenMotor == DEF_Run)	{//ֹͣȡ���ŵ��
		QuHuoMen_MOTOR_ENABLE = 0;
		SysMotor.ALLMotorState.bits.QuHuoMenMotor =  DEF_Stop;
	}
	if(stop_type==DEF_Success)	{
		SysMotor.motor[runing_id].status.action = ActionState_OK;
	}else if(stop_type==DEF_Fail)	{
		SysMotor.motor[runing_id].status.action = ActionState_Fail;
	}
}

void MotorTest(void)
{
	if(DevState.bits.State != DEV_STATE_TEST)	{
		return;
	}
	
	DevState.bits.SubState = 1;//���������
	MotorStart();
	if(SysMotor.ALLMotorState.bits.LMotor == DEF_Run)	{
		SoftTimerStart(Timer1, SysMotor.motor[MOTOR_L_ID].Param*10);
	}
	else if(SysMotor.ALLMotorState.bits.ZMotor == DEF_Run)	{
		SoftTimerStart(&Timer1Soft, SysMotor.motor[MOTOR_Z_ID].Param*10);
	}
}	

