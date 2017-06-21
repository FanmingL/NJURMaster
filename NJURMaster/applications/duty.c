#include "main.h"
loop_t loop;

void Loop_UpDate(void)  //TIME INTTERRUPT
{
	loop.time++; 
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;
	loop.check_flag = 1;
}


u32 get_time_ms(void)
{
	return loop.time;
}
void Duty_1ms(void)
{
	rc_time_cnt++;
}

void Duty_2ms(void)
{
	
}
void Duty_5ms(void)
{
	ControlTask();         //底盘、云台控制任务
}
void Duty_10ms(void)
{

}
void Duty_20ms(void)
{
	MPU6500_ReadValue();
}
void Duty_50ms(void)
{
	static u8 _step = 0;
	if (_step == 0){RED_LED_ON();_step++;}
	else if (_step == 1){GREEN_LED_ON();_step++;}
	else if (_step == 2){RED_LED_OFF();_step++;}
	else if (_step == 3){GREEN_LED_OFF();_step=0;}
	printf("ax = %d ay = %d z = %d\n",MPU6500_Acc.X,MPU6500_Acc.Y,MPU6500_Acc.Z);
	printf("temprture = %f\n", mpu6500_tempreature);
	printf("gx = %d gy = %d gz = %d\n\n",MPU6500_Gyro.X,MPU6500_Gyro.Y,MPU6500_Gyro.Z);
}


void Duty_Loop() 
{
	if( loop.check_flag == 1 )
	{
		
		Duty_1ms();				
		
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			Duty_2ms();					
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();					
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();		
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();			
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();		
			
		}
		
		loop.check_flag = 0;	
	}
}



