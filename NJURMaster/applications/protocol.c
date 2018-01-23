#include "main.h"


RC_Ctrl_t RC_CtrlData;   //remote control data
u8 checkdata_to_send,checksum_to_send,send_check=0;
u8 send_pid1=0,send_pid2=0,send_pid3=0;
/**
  * @brief 基本串口通讯协议解析
  * @param data_buf	包含完整一帧数据的数组的指针
	* @param _len		帧总长
  * @retval None
  * @details 上层硬件发来的信号或是地面站发来的信号的解析函数
  */
void BasicProtocolAnalysis(u8 *data_buf,int _len)
{
	u8 sum = 0;
	u8 i;
	for(i=0;i<(_len-1);i++)														//求和校验
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+_len-1)))		return;						//校验不成功则return
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;	//帧头校验不成功则return
	if(*(data_buf+2)==0X01)														//分析帧类型
	{
		if(*(data_buf+4)==0X01)
		{
			IMU_ACCERDataCali();
			
		}
		else if(*(data_buf+4)==0X02)
		{
			IMU_GYRODataCali();
		}
		else if(*(data_buf+4)==0X03)
		{
			IMU_ACCERDataCali();	
			IMU_GYRODataCali();	
		}
		else if(*(data_buf+4)==0X04)
		{
			IMU_MAGDataCali();

		}
		else if (*(data_buf+4)==0X05)
		{
			ParaSavingFlag=1;
		}
		else if((*(data_buf+4)>=0X021)&&(*(data_buf+4)<=0X26))
		{
		}
		else if(*(data_buf+4)==0X20)
		{
		}
	}
	if(*(data_buf+2)==0X22){
			//NS=(enum PendulumMode)(*(data_buf+4));
		}
			if(*(data_buf+2)==0X21){
			//NS=Stop;
		}
if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{
			send_pid1 = 1;
			send_pid2 = 1;
			send_pid3 = 1;
		}
		if(*(data_buf+4)==0XA1)	
		{
			ParametersInit();
		}
	}
		if(*(data_buf+2)==0X10)								//PID1
    {
        PID_arg[0].kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        PID_arg[0].ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        PID_arg[0].kd  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        PID_arg[1].kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        PID_arg[1].ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        PID_arg[1].kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        PID_arg[2].kp 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        PID_arg[2].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        PID_arg[2].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
				if(send_check == 0)
				{
					send_check = 1;
					checkdata_to_send = *(data_buf+2);
					checksum_to_send = sum;
				}
    }
		 if(*(data_buf+2)==0X11)								//PID2
    {
        PID_arg[3].kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        PID_arg[3].ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        PID_arg[3].kd  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        PID_arg[4].kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        PID_arg[4].ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        PID_arg[4].kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
				PID_arg[5].kp 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        PID_arg[5].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        PID_arg[5].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
						if(send_check == 0)
				{
					send_check = 1;
					checkdata_to_send = *(data_buf+2);
					checksum_to_send = sum;
				}
    }
	
    	if(*(data_buf+2)==0X03)
	{

		
	}
    if(*(data_buf+2)==0X12)								//PID3
    {	
			  PID_arg[6].kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        PID_arg[6].ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        PID_arg[6].kd  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        PID_arg[7].kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        PID_arg[7].ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        PID_arg[7].kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
       	if(send_check == 0)
				{
					send_check = 1;
					checkdata_to_send = *(data_buf+2);
					checksum_to_send = sum;
				}
    }
	if(*(data_buf+2)==0X13)								//PID4
	{
		   	if(send_check == 0)
				{
					send_check = 1;
					checkdata_to_send = *(data_buf+2);
					checksum_to_send = sum;
				}
	}
	if(*(data_buf+2)==0X14)								//PID5
	{
						if(send_check == 0)
				{
					send_check = 1;
					checkdata_to_send = *(data_buf+2);
					checksum_to_send = sum;
				}
	}
	if(*(data_buf+2)==0X15)								//PID6
	{
						if(send_check == 0)
				{
					send_check = 1;
					checkdata_to_send = *(data_buf+2);
					checksum_to_send = sum;
				}
	}


}

#define PITCH_MAX (10.0f)
#define YAW_MAX   (0.1f)
#define CHANNELMIDDLE	(1024)
#define RC_TOWARD_SCALE (0.5f)
#define RC_LEFTRIGHT_SCALE (0.5f)

/**
  * @brief 对遥控器解析结果进行反应
  * @param None
  * @retval None
  */
void RcDataAnalysis(RC_Ctrl_t *rc)
{
	float __temp;
	if (GetRcMode()==RC_KEY_RCMODE)
	{
		__temp=GimbalPitchPosRef+rc->rc.ch1;
		GimbalPitchPosRef=LIMIT(__temp,-PITCH_MAX,PITCH_MAX);
		__temp=GimbalYawPosRef+rc->rc.ch0;
		GimbalYawPosRef=LIMIT(__temp,-YAW_MAX+Yaw,YAW_MAX+Yaw);
		
		ChassisGoToward=(rc->rc.ch3-CHANNELMIDDLE)*RC_TOWARD_SCALE;
		ChassisGoLeftRight=(rc->rc.ch2-CHANNELMIDDLE)*RC_LEFTRIGHT_SCALE;
		
	}
	else if (GetRcMode()==RC_KEY_RCMODE)
	{
		
	}
	else if (GetRcMode()==RC_KEY_STOP)
	{
	
	
	}
}

/**
  * @brief 遥控器解析函数
  * @param pData	完整一帧接收机发来的数据
  * @param _len		帧总长
  * @retval None
  */
void RcProtocolAnalysis(u8 *pData,int _len)
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
 
    RC_CtrlData.key.v = ((int16_t)pData[15] << 8)|((int16_t)pData[14]);// | ((int16_t)pData[15] << 8);
		
}

/**
  * @brief 裁判系统解析
  * @param _item	完整一帧裁判系统的数据
	* @param _len		帧总长
  * @retval None
  */
void RefereeProtocolAnalysis(u8 *_item,int _len)
{


}

/**
  * @brief CAN数据解析
  * @param None
  * @retval None
  */
void CanProtocolAnalysis(CanRxMsg * msg)
{



}
