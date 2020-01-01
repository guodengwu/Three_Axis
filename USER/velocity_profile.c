#include "velocity_profile.h"
#include "motor.h"

velocity_profile X_VelCurve;
velocity_profile Y_VelCurve;

void VelocityProfileInit(void)
{
	X_VelCurve.freq_max = DEF_VEL_MAX;
	X_VelCurve.freq_min = DEF_VEL_MIN;
	CalculateTModelCurve();
}
//�����ٶ����� 
//���ٵ�300��  ���ټ��100ms
void CalculateTModelCurve(void)
{
	u16 i;
	
	velocity_profile *curve = &X_VelCurve;
	for(i=0;i<CURVE_BUF_MAX;i++)	{//x ����
		curve->Curve[i] = 5*i+20;
	}
	curve->index = 0;
	curve = &Y_VelCurve;	
	for(i=0;i<CURVE_BUF_MAX;i++)	{//y ����
		curve->Curve[i] = 5*i+20;
	}
	curve->index = 0;
}
