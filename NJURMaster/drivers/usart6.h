#ifndef _USART6_H_
#define _USART6_H_
#include "stm32f4xx.h"

void Usart6_Init(u32 br_num);
void Usart6_Send(unsigned char *DataToSend ,u8 data_num);


#endif
