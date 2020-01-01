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
//计算速度曲线 
//加速点300个  加速间隔100ms
void CalculateTModelCurve(void)
{
	u16 i;
	
	velocity_profile *curve = &X_VelCurve;
//	divide = (curve->freq_max - curve->freq_min)/CURVE_BUF_MAX;
//	for(i=0;i<CURVE_BUF_MAX;i++)	{
//		curve->VelCurveBuf[i] = curve->freq_min + divide*i;
//	}
//	curve->Curve[0] = 10;
	for(i=0;i<CURVE_BUF_MAX;i++)	{
		curve->Curve[i] = 5*i+20;
	}
	curve->index = 0;
	curve = &Y_VelCurve;	
//	divide = (curve->freq_max - curve->freq_min)/CURVE_BUF_MAX;
//	for(i=0;i<CURVE_BUF_MAX;i++)	{
//		curve->VelCurveBuf[i] = curve->freq_min + divide*i;
//	}
//	curve->Curve[0] = 10;
	for(i=0;i<CURVE_BUF_MAX;i++)	{
		curve->Curve[i] = 5*i+20;
	}
	curve->index = 0;
}
