#ifndef _DATATRANSFER_H_
#define _DATATRANSFER_H_
#include "stm32f4xx.h"
#include "RefereeSystem.h"

void DatatransferTask(u32 _time_sys);
void Usart2_DataPrepare(u8 data);
void Usart3_DataPrepare(u8 data);
void Usart6_DataPrepare(u8* pData);
void ANO_DT_Send_Check(u8 head, u8 check_sum);
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);
void RefereeSys_Send_Data(extUserData_t userdata);

//CRC check
unsigned char Get_CRC8_Check_Sum(u8* pMessage, u32 dwLength, u8 ucCRC8);
unsigned char Verify_CRC8_Check_Sum(u8* pMessage, u32 dwLength, u8 ucCRC8);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
unsigned char Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
#endif
