#include "main.h"

/**
  * @brief 串口6初始化 
  * @param BaudRate
  * @retval None
  * @details DMA发送，RXNE接收中断
  */
void Usart6_Init(u32 br_num)
{ 
	
}

/**
  * @brief 串口6中断
  * @param None
  * @retval None
  * @details RXNE中断，由此进入Usart6通讯协议解析
  */
void USART6_IRQHandler(void)
{
	u8 com_data;
	
	if(USART6->SR & USART_SR_ORE)
	{
		com_data = USART6->DR;
	}
	if( USART_GetITStatus(USART6,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);

		com_data = USART6->DR;
		Usart6_DataPrepare(com_data);
	}
}

/**
  * @brief 串口6的DMA发送函数，发送一组数据
  * @param DataToSend 要发送数据的数组的指针
	* @param data_num 要发送的数据的个数
  * @retval None
  */
void Usart6_Send(unsigned char *DataToSend ,u8 data_num)
{
  

}


