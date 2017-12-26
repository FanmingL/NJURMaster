#include "main.h"

/**
  * @brief µ×²ãÇý¶¯³õÊ¼»¯
  * @param None
  * @retval None
  */
void All_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
		
	TIM6_Configuration();
	TIM5_Configuration();
	InnerLoopInit();
	Led_Configuration();
	
	Rc_Init();
	Usart2_Init(115200);
	Usart3_Init(115200);
	Usart6_Init(115200);

	PWM_Init();
	EncoderInit();
	
	delay_ms(100);
	SPI5_Init();
	delay_ms(100);
	while(MPU6500_Init())
	{
		delay_ms(100);
	}



	
	
	TIM6_Start();
}
