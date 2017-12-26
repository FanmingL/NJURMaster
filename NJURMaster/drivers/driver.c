#include "main.h"
void All_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	Rc_Init();
	Usart2_Init(115200);

	DogInit();

	TIM6_Configuration();
	TIM6_Start();
	InnerLoopInit();
	Led_Configuration();
	PWM_Init();
	
	TIM5_Configuration();
}
