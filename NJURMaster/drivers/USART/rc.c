#include "main.h"
/*----USART1----PB7-----RX */
/*----BaudRate------ 100000*/
/*----------RC-------------*/

void Usart1_Init(void)
{  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_USART1); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 100000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStructure); 
	
  USART_Cmd(USART1, ENABLE);
	
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);



}


u8 Rx1_Buf[256];
u8 count1=0;
void USART1_IRQHandler(void)
{
	u8 com_data;
	
	if(USART1->SR & USART_SR_ORE)
	{
		com_data = USART1->DR;
	}
	if( USART_GetITStatus(USART1,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);

		com_data = USART1->DR;
		RC_Receive_Prepare(com_data);
	}
}
void Rc_Init(void)
{
	Usart1_Init();
}
RC_Ctl_t RC_CtrlData;

void RemoteDataPrcess(uint8_t *pData)
{
    if(pData == 0)
    {
        return;
    }
    
    RC_CtrlData.rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF; 
    RC_CtrlData.rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF;
    RC_CtrlData.rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |
                         ((int16_t)pData[4] << 10)) & 0x07FF;
    RC_CtrlData.rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) & 0x07FF;
    
    RC_CtrlData.rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;
    RC_CtrlData.rc.s2 = ((pData[5] >> 4) & 0x0003);

    RC_CtrlData.mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
    RC_CtrlData.mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);
    RC_CtrlData.mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);    

    RC_CtrlData.mouse.press_l = pData[12];
    RC_CtrlData.mouse.press_r = pData[13];
 
    RC_CtrlData.key.v = ((int16_t)pData[14]);// | ((int16_t)pData[15] << 8);
	
		SetInputMode(&RC_CtrlData.rc);
	
		RemoteControlProcess(&(RC_CtrlData.rc));
	
	switch(GetInputMode())
	{
		case REMOTE_INPUT:
		{
			//Ò£¿ØÆ÷¿ØÖÆÄ£Ê½
			RemoteControlProcess(&(RC_CtrlData.rc));
		}break;
		case KEY_MOUSE_INPUT:
		{
			//Ò£¿ØÆ÷¿ØÖÆÄ£Ê½
			MouseKeyControlProcess(&RC_CtrlData.mouse,&RC_CtrlData.key);
		}break;
		case STOP:
		{
			//½ô¼±Í£³µ
		}break;
	}
}
