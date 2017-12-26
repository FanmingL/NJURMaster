#ifndef _DATATRANSFER_H_
#define _DATATRANSFER_H_
#include "stm32f4xx.h"

void DatatransferTask(u32 _time_sys);
void Usart2_DataPrepare(u8 data);
void Usart3_DataPrepare(u8 data);
void Usart6_DataPrepare(u8 data);
#endif
