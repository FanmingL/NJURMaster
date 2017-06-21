#include "main.h"

//Timer 3 16-bit counter  
//Timer Clock is 180MHz / 4 * 2 = 90M
u32 time_ms=0;

void TIM3_Configuration(void)
{
		TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    nvic.NVIC_IRQChannel = TIM3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 90-1;        
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 1000;  //1ms,1000Hz
    TIM_TimeBaseInit(TIM3,&tim);

}
void TIM3_Start(void)
{
    TIM_Cmd(TIM3, ENABLE);	 
    TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);	
}
void TIM3_IRQHandler(void)
{
		if (TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET) 
	  {
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		Loop_UpDate();
    }

}



