#include "main.h"
u8 data_to_send[50];

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
	else if ((sys_time+5)%10==0)
	{
		
	}
	else
	{

	}
	if ((sys_time+5)%50==0)
	{
		ANO_DT_Send_RCData(RC_CtrlData.rc.ch0,RC_CtrlData.rc.ch1,RC_CtrlData.rc.ch2,RC_CtrlData.rc.ch3,RC_CtrlData.rc.s1*300+1000,RC_CtrlData.rc.s2*300+1000,RC_CtrlData.mouse.x+1000,RC_CtrlData.mouse.y+1000,RC_CtrlData.mouse.z+1000,RC_CtrlData.key.v);
	}
	else if((sys_time+6)%50==0)
	{
		ANO_DT_Send_MotoPWM(ABS(CM1Encoder.filter_rate),ABS(CM2Encoder.filter_rate),ABS(CM3Encoder.filter_rate),
												ABS(CM4Encoder.filter_rate),ABS(GMPitchEncoder.filter_rate),ABS(GMYawEncoder.filter_rate),0,0);
	}
	if (send_check)
	{
		send_check = 0;
		ANO_DT_Send_Check(checkdata_to_send,checksum_to_send);
	}
	if (send_pid1)
	{
		send_pid1=0;
		ANO_DT_Send_PID(1,PID_arg[0].kp,PID_arg[0].ki,PID_arg[0].kd,
											PID_arg[1].kp,PID_arg[1].ki,PID_arg[1].kd,
											PID_arg[2].kp,PID_arg[2].ki,PID_arg[2].kd);
		
	}
	else if(send_pid2)
	{
		send_pid2=0;
		ANO_DT_Send_PID(2,PID_arg[3].kp,PID_arg[3].ki,PID_arg[3].kd,
											PID_arg[4].kp,PID_arg[4].ki,PID_arg[4].kd,
											PID_arg[5].kp,PID_arg[5].ki,PID_arg[5].kd);
	}
	else if (send_pid3)
	{
		send_pid3=0;
				ANO_DT_Send_PID(3,PID_arg[6].kp,PID_arg[6].ki,PID_arg[6].kd,
											PID_arg[7].kp,PID_arg[7].ki,PID_arg[7].kd,
											0,0,0);
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

/**
  * @brief 发送遥控器数据
  * @param None
  * @retval None
  */
void ANO_DT_Send_RCData(u16 ch0,u16 ch1,u16 ch2,u16 ch3,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
	u8 _cnt=0;
	u8 i=0;
	u8 sum = 0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(ch0);
	data_to_send[_cnt++]=BYTE0(ch0);
	data_to_send[_cnt++]=BYTE1(ch1);
	data_to_send[_cnt++]=BYTE0(ch1);
	data_to_send[_cnt++]=BYTE1(ch2);
	data_to_send[_cnt++]=BYTE0(ch2);
	data_to_send[_cnt++]=BYTE1(ch3);
	data_to_send[_cnt++]=BYTE0(ch3);
	data_to_send[_cnt++]=BYTE1(aux1);
	data_to_send[_cnt++]=BYTE0(aux1);
	data_to_send[_cnt++]=BYTE1(aux2);
	data_to_send[_cnt++]=BYTE0(aux2);
	data_to_send[_cnt++]=BYTE1(aux3);
	data_to_send[_cnt++]=BYTE0(aux3);
	data_to_send[_cnt++]=BYTE1(aux4);
	data_to_send[_cnt++]=BYTE0(aux4);
	data_to_send[_cnt++]=BYTE1(aux5);
	data_to_send[_cnt++]=BYTE0(aux5);
	data_to_send[_cnt++]=BYTE1(aux6);
	data_to_send[_cnt++]=BYTE0(aux6);

	data_to_send[3] = _cnt-4;
	

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Usart2_Send(data_to_send, _cnt);
}

/**
  * @brief 发送PID至上位机
  * @param 三组PID的值及其group序号
  * @retval None
  * @details None
  */
void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	u8 _cnt=0;
	vs16 _temp;
	u8 sum=0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	
	_temp = p1_p * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_i  * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_d  * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_p  * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_i  * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_d * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_p  * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_i  * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_d * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	Usart2_Send(data_to_send, _cnt);
}
/**
  * @brief 发送电机转速
  * @param None
  * @retval None
  */
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8)
{
	u8 _cnt=0;
	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE1(m_1);
	data_to_send[_cnt++]=BYTE0(m_1);
	data_to_send[_cnt++]=BYTE1(m_2);
	data_to_send[_cnt++]=BYTE0(m_2);
	data_to_send[_cnt++]=BYTE1(m_3);
	data_to_send[_cnt++]=BYTE0(m_3);
	data_to_send[_cnt++]=BYTE1(m_4);
	data_to_send[_cnt++]=BYTE0(m_4);
	data_to_send[_cnt++]=BYTE1(m_5);
	data_to_send[_cnt++]=BYTE0(m_5);
	data_to_send[_cnt++]=BYTE1(m_6);
	data_to_send[_cnt++]=BYTE0(m_6);
	data_to_send[_cnt++]=BYTE1(m_7);
	data_to_send[_cnt++]=BYTE0(m_7);
	data_to_send[_cnt++]=BYTE1(m_8);
	data_to_send[_cnt++]=BYTE0(m_8);
	
	data_to_send[3] = _cnt-4;
	
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Usart2_Send(data_to_send, _cnt);
}
