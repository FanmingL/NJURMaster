#ifndef _DATATRANSFER_H
#define _DATATRANSFER_H

#include "stm32f4xx.h"
void USART2_Receive_Prepare(u8 _temp);
void USART3_Receive_Prepare(u8 _temp);
void USART2_Receive_Anl(u8 *data, u8 len);
void USART3_Receive_Anl(u8 *data, u8 len);
void RC_Receive_Prepare(u8 _temp);
void RC_Receive_Anl(u8 *data, u8 len);
void DataExchange(void);
extern u16 rc_time_cnt; 
#endif

