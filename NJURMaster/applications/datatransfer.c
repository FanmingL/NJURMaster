#include "main.h"
u8 data_to_send[50];
extern u32 SelfCheckErrorFlag;

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
/**
  * @brief 串口通讯task
  * @param 系统从开机到现在运行的毫秒数
  * @retval None
  * @details 在这里向上位机或者上层硬件发送当前机器人的状态或传感器的数据
  */
void DatatransferTask(u32 sys_time)
{
//	int j=0;
//	char buff[200];
//	
//	if(sys_time%60==0)
//	{
//	//j=sprintf(buff,"%.2f  %.2f  %.2f  %.2f  %.2f  %.2f\n",MPU6500_Acc.x,MPU6500_Acc.y,MPU6500_Acc.z,MPU6500_Gyro.x,MPU6500_Gyro.y,MPU6500_Gyro.z);
//	//	j+=sprintf(j+buff,"%d\t%d\t%d\t%d\t%d\t%d\t\n",temp_ist_buff[0],temp_ist_buff[1],temp_ist_buff[2],temp_ist_buff[3],temp_ist_buff[4],temp_ist_buff[5]);
//	//	j=sprintf(j+buff,"%.2f\t%.2f\t%.2f\n",Pitch,Roll,Yaw);
//	//	j+=sprintf(j+buff,"0x%x\n",MPU6500_Read_Reg(MPU6500_I2C_SLV0_CTRL));
//	//	Usart2_Send((u8*)buff,j);
//	}
	if (sys_time%10==0)
	{
		ANO_DT_Send_Status(Roll,Pitch,Yaw,0,0,0);
	}
	else if((sys_time+1)%10==0)
	{
		ANO_DT_Send_Senser((vs16)MPU6500_Acc.x,(vs16)MPU6500_Acc.y,(vs16)MPU6500_Acc.z,
												(vs16)MPU6500_Gyro.x,(vs16)MPU6500_Gyro.y,(vs16)MPU6500_Gyro.z,
											(vs16)MagValue.x,(vs16)MagValue.y,(vs16)MagValue.z);
	}
	else if((sys_time+2)%10==0)
	{

	}
	else if((sys_time+3)%10==0)
	{
		Self_Check_Send_Status(SelfCheckErrorFlag);//放在不同的时间段上传数据，以免串口冲突
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


/**
  * @brief 向上位机发送三个欧拉角
  * @param 三个欧拉角，
  * @retval None
  */
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
	u8 _cnt=0;
	vs16 _temp;
	vs32 _temp2 = alt;
	u8 sum = 0;
	u8 i;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(angle_rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++] = fly_model;
	
	data_to_send[_cnt++] = armed;
	
	data_to_send[3] = _cnt-4;
	

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	Usart2_Send(data_to_send, _cnt);
}

void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
	u8 _cnt=0;
	vs16 _temp;
		u8 sum = 0;
	u8 i;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	_temp = a_x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
/////////////////////////////////////////
	_temp = 0;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	
	
	data_to_send[3] = _cnt-4;
	

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	Usart2_Send(data_to_send, _cnt);
}

void Self_Check_Send_Status(u32 Error_flag){
	u8 _cnt=0;
	u8 sum=0;
	u8 i;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;//Frame Type
	data_to_send[_cnt++]=5;
	data_to_send[_cnt++]=NET_ID;
	data_to_send[_cnt++]=BYTE3(Error_flag);
	data_to_send[_cnt++]=BYTE2(Error_flag);
	data_to_send[_cnt++]=BYTE1(Error_flag);
	data_to_send[_cnt++]=BYTE0(Error_flag);
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	Usart2_Send(data_to_send, _cnt);
}

