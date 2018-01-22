#include "main.h"

/*******����6����*********************/
u8 Rx_6_Buf[256];	
u8 Tx6Buffer[256];
u8 Tx6Counter=0;
u8 count6=0; 
u8 Tx6DMABuffer[256]={0};
/***********************************/

/**
  * @brief ����6��ʼ�� 
  * @param BaudRate
  * @retval None
  * @details DMA���ͣ�RXNE�����ж�
  *          BaudRate 115200
  *          TX  DMA2_Stream6_Ch5  PC6
  *          RX  DMA2_Stream1_Ch5  PC7
  */
void Usart6_Init(u32 br_num)
{
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef USART_ClockInitStruct;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

  //�����ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //�������Ÿ�����ӳ��
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

  //GPIO�˿�ģʽ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //���ڲ�����ʼ��
  USART_InitStructure.USART_BaudRate = br_num;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(USART6, &USART_InitStructure);

  USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;
  USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;
  USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable;
  USART_ClockInit(USART6, &USART_ClockInitStruct);

  //���� DMA Stream
  DMA_DeInit(DMA2_Stream6);  //��DMAͨ��6�Ĵ�������Ϊȱʡֵ
  while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){}  //�ȴ�DMA������
  DMA_InitStructure.DMA_Channel = DMA_Channel_5;       //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART6->DR;//DMA�����ַ������Ϊ���ڽ��շ������ݵĵ�ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Tx6DMABuffer;//���DMA�������ݵĴ洢����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = 0;  //��ʼ�����ݴ�����
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA2_Stream6, &DMA_InitStructure);//��ʼ��DMA Stream

  USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);  //RXNE �����ݼĴ����ǿ�
  USART_Cmd(USART6, ENABLE); 
  USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);    //ʹ��USART6��DMA����

}

/**
  * @brief ����6�ж�
  * @param None
  * @retval None
  * @details RXNE�жϣ��ɴ˽���Usart6ͨѶЭ�����
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
  * @brief ����6��DMA���ͺ���������һ������
  * @param DataToSend Ҫ�������ݵ������ָ��
	* @param data_num Ҫ���͵����ݵĸ���
  * @retval None
  */
void Usart6_Send(unsigned char *DataToSend ,u8 data_num)
{
  

}


