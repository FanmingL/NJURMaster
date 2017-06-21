#ifndef _DUTY_H_
#define _DUTY_H_
#include "stm32f4xx.h" 

typedef struct
{
	u8 check_flag;
	u8 err_flag;
	s16 cnt_1ms;
	s16 cnt_2ms;
	s16 cnt_5ms;
	s16 cnt_10ms;
	s16 cnt_20ms;
	s16 cnt_50ms;
	u32 time;
}loop_t;
void Loop_UpDate(void);
void Duty_1ms(void);
void Duty_2ms(void);
void Duty_5ms(void);
void Duty_10ms(void);
void Duty_20ms(void);
void Duty_50ms(void);
void Duty_Loop(void);
u32 get_time_ms(void);
#endif
