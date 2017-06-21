#include "main.h"
/*----USART2----PD5-----TX */
/*----USART2----PD6-----RX */
/*----BaudRate------ 115200*/
/*------Bluetooth----------*/

void Usart2_Init(u32 br_num)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = br_num;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure); 
	
  USART_Cmd(USART2, ENABLE);
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);



}


u8 Rx2_Buf[256];
u8 Tx2_Buf[256];
u8 count2=0;
u8 TxCounter2=0;
void USART2_IRQHandler(void)
{
	u8 com_data;
	
	if(USART2->SR & USART_SR_ORE)
	{
		com_data = USART2->DR;
	}
	if( USART_GetITStatus(USART2,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);

		com_data = USART2->DR;
		USART3_Receive_Prepare(com_data);
	}
	if( USART_GetITStatus(USART2,USART_IT_TC ) )
	{
		USART2->DR = Tx2_Buf[TxCounter2++];     
		if(TxCounter2 == count2)
		{
			USART2->CR1 &= ~USART_CR1_TCIE;		
		}
	}
}
void Usart2_Send(unsigned char *DataToSend ,u8 data_num)
{
  u8 i;
	for(i=0;i<data_num;i++)
	{
		Tx2_Buf[count2++] = *(DataToSend+i);
	}

	if(!(USART2->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART2, USART_IT_TC, ENABLE); 
	}

}

void BT_Init(void)
{
	Usart2_Init(115200);
}

