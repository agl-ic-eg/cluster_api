#ifndef CLUSTER_API_SPEED_H
#define CLUSTER_API_SPEED_H

typedef enum
{
    IC_HMI_SP_KM_H = 0,
    IC_HMI_SP_MPH = 1,
} IC_HMI_SP_UNIT_VAL;

unsigned long getSpAnalogVal(void);
IC_HMI_SP_UNIT_VAL getSpAnaDigUnitVal(void);

#endif // CLUSTER_API_SPEED_H
