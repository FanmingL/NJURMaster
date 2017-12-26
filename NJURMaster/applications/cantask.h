#ifndef _CANTASK_H_
#define _CANTASK_H_

#include "stm32f4xx.h"
void ChassisSpeedSet(CAN_TypeDef *CANx, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);
void GimbalCurrentSet(CAN_TypeDef *CANx, int16_t gimbal_yaw_iq, int16_t gimbal_pitch_iq);

#endif
