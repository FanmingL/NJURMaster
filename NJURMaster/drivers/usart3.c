#include "main.h"
#include "stdio.h"
/*----USART3----PD8-----TX */
/*----USART3----PD9-----RX */
/*----BaudRate------ 115200*/


void Usart3_Init(u32 br_num)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	 NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
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
	
	
  USART_Init(USART3, &USART_InitStructure); 

  USART_Cmd(USART3, ENABLE);
	
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);


}


u8 Rx3_Buf[256];
u8 Tx3_Buf[256];
u8 count3=0;
u8 TxCounter3=0;	
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
	if( USART_GetITStatus(USART3,USART_IT_TC ) )
	{
		USART3->DR = Tx3_Buf[TxCounter3++];     
		if(TxCounter3 == count3)
		{
			USART3->CR1 &= ~USART_CR1_TCIE;		
		}
	}
}
void Usart3_Send(unsigned char *DataToSend ,u8 data_num)
{
  u8 i;
	for(i=0;i<data_num;i++)
	{
		Tx3_Buf[count3++] = *(DataToSend+i);
	}

	if(!(USART3->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART3, USART_IT_TC, ENABLE); 
	}

}
void Usart3_Send_Byte(unsigned char DataToSend)
{
	Tx3_Buf[count3++] = DataToSend;
	if(!(USART3->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART3, USART_IT_TC, ENABLE); 
	}
}

//int fputc(int ch, FILE *f)
//{
//		Usart3_Send_Byte((uint8_t)ch);
//    return ch;
//}


