#ifndef	__VELOCITY_PROFILE_H
#define	__VELOCITY_PROFILE_H

#include	"includes.h"

#define DEF_VEL_MAX             32000//(float)(0.04)//Ĭ������ٶ� mm/s
#define DEF_VEL_MIN             1000//(float)(0.00125)//Ĭ����С�ٶ� mm/s

#define CURVE_BUF_MAX           300//���ٵ�300��  ���ټ��100ms

typedef struct {
    //INT8U   flexible;
//    u32 acc;//���ٶ�,m/s^2
//    u32 Vmax;//����ٶ�,m/s
//    u32 Vmin;//��С�ٶ�
    //INT16U Sa;//����·��
    //INT16U Step;//����������
    INT16U   freq_max;//���Ƶ��
    INT16U   freq_min;//��СƵ��
    u16 VelCurveBuf[CURVE_BUF_MAX];
    //INT8U   abort_type;
} velocity_profile;

extern velocity_profile X_VelCurve;
extern velocity_profile Y_VelCurve;

void VelocityProfileInit(void);
void CalculateTModelCurve(void);
#endif

