#include "main.h"
u8 data_to_send[50];
/**
  * @brief 串口通讯task
  * @param 系统从开机到现在运行的毫秒数
  * @retval None
  * @details 在这里向上位机或者上层硬件发送当前机器人的状态或传感器的数据
  */
void DatatransferTask(u32 sys_time)
{
	int j=0;
	char buff[200];
	
	if(sys_time%200==0){
	j=sprintf(buff,"%.2f  %.2f  %.2f  %.2f  %.2f  %.2f\n",MPU6500_Acc.x,MPU6500_Acc.y,MPU6500_Acc.z,MPU6500_Gyro.x,MPU6500_Gyro.y,MPU6500_Gyro.z);
	Usart2_Send((u8*)buff,j);
	}
	if (sys_time%10==0)
	{
		
	}
	else if((sys_time+1)%10==0)
	{
	
	}
	else if((sys_time+2)%10==0)
	{

	}
	else if ((sys_time+5)%10==0)
	{
		
	}
	else
	{
		if (send_check)
		{
			send_check = 0;
			ANO_DT_Send_Check(checkdata_to_send,checksum_to_send);
		}
	}
}

/**
  * @brief 串口2数据预解析
  * @param data	从DR寄存器中读取到的数据
  * @retval None
  * @details 若解析成功则跳转到BasicProtocolAnalysis进行处理
  */
void Usart2_DataPrepare(u8 data)
{
	static u8 RxBuffer[50];
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;
	
	if(state==0&&data==0xAA)						//判断帧头
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)			//帧头
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)			//帧类型
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)				//帧长
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)		//数据帧
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)								//校验帧
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		BasicProtocolAnalysis(RxBuffer,_data_cnt+5);
		
	}
	else
		state = 0;

}

/**
  * @brief 串口3数据预解析
  * @param data	从DR寄存器中读取到的数据
  * @retval None
  * @details 若解析成功则跳转到BasicProtocolAnalysis进行处理
  */
void Usart3_DataPrepare(u8 data)
{
	static u8 RxBuffer[50];
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;
	
	if(state==0&&data==0xAA)						//判断帧头
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)			//帧头
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)			//帧类型
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)				//帧长
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)		//数据帧
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)								//校验帧
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		BasicProtocolAnalysis(RxBuffer,_data_cnt+5);
		
	}
	else
		state = 0;


}

/**
  * @brief 串口6数据预解析
  * @param data	从DR寄存器中读取到的数据
  * @retval None
  * @details 若解析成功则跳转到BasicProtocolAnalysis进行处理
  */
void Usart6_DataPrepare(u8 data)
{
	static u8 RxBuffer[50];
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;
	
	if(state==0&&data==0xAA)						//判断帧头
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)			//帧头
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)			//帧类型
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)				//帧长
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)		//数据帧
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)								//校验帧
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		BasicProtocolAnalysis(RxBuffer,_data_cnt+5);
		
	}
	else
		state = 0;


}

/**
  * @brief 发送attack
  * @param head 帧类型
  * @param check_sum 校验位
  * @retval None
  */
void ANO_DT_Send_Check(u8 head, u8 check_sum)
{
	u8 sum = 0;
	u8 i;
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	
	

	for(i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	Usart2_Send(data_to_send, 7);
}
