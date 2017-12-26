#include "main.h"

/**
  * @brief 串口3初始化 
  * @param BaudRate
  * @retval None
  * @details	PD8	Tx
	*						PD9	Rx
	*						BaudRate	115200
	*						使能DMA发送，RXNE中断
  */
void Usart3_Init(u32 br_num)
{ 
	
}

/**
  * @brief 串口3中断
  * @param None
  * @retval None
  * @details RXNE中断，由此进入Usart3通讯协议解析
  */
void USART3_IRQHandler(void)
{
	u8 com_data;
	
	if(USART3->SR & USART_SR_ORE)
	{
		com_data = USART3->DR;
	}
	if( USART_GetITStatus(USART3,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);

		com_data = USART3->DR;
		Usart3_DataPrepare(com_data);
	}
}

/**
  * @brief 串口3的DMA发送函数，发送一组数据
  * @param DataToSend 要发送数据的数组的指针
	* @param data_num 要发送的数据的个数
  * @retval None
  */
void Usart3_Send(unsigned char *DataToSend ,u8 data_num)
{
  

}


