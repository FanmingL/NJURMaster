#include "main.h"
void All_Init(void)
{
	delay_init(180);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Led_Configuration();
	Laser_Configuration();
	while(!MPU6500_Init());	
	MPU6500_Date_Offset(100);
	Rc_Init();
	BT_Init();
	Can1_Init();
	Can2_Init();
	Usart3_Init(115200);
	PWM_Init();
	TIM3_Configuration();
	TIM3_Start();
}
