#ifndef _DATATRANSFER_H_
#define _DATATRANSFER_H_
#include "stm32f4xx.h"

#define NET_ID 1//Net的0号节点是手持终端，此后按照战车编号

void DatatransferTask(u32 _time_sys);
void Usart2_DataPrepare(u8 data);
void Usart3_DataPrepare(u8 data);
void Usart6_DataPrepare(u8 data);
void ANO_DT_Send_Check(u8 head, u8 check_sum);
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);
void Self_Check_Send_Status(u32 Error_flag);
void Zigbee_Send(u32 target,u8 *data,u8 num);
#endif
