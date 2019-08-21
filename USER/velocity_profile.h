#ifndef	__VELOCITY_PROFILE_H
#define	__VELOCITY_PROFILE_H

#include	"includes.h"

#define DEF_VEL_MAX             32000//(float)(0.04)//默认最大速度 mm/s
#define DEF_VEL_MIN             1000//(float)(0.00125)//默认最小速度 mm/s

#define CURVE_BUF_MAX           300//加速点300个  加速间隔100ms

typedef struct {
    //INT8U   flexible;
//    u32 acc;//加速度,m/s^2
//    u32 Vmax;//最大速度,m/s
//    u32 Vmin;//最小速度
    //INT16U Sa;//加速路程
    //INT16U Step;//加速脉冲数
    INT16U   freq_max;//最大频率
    INT16U   freq_min;//最小频率
    u16 VelCurveBuf[CURVE_BUF_MAX];
    //INT8U   abort_type;
} velocity_profile;

extern velocity_profile X_VelCurve;
extern velocity_profile Y_VelCurve;

void VelocityProfileInit(void);
void CalculateTModelCurve(void);
#endif

