#include "main.h"


//values for PID Calculate.
float GimbalPitchPosRef=0.0f;
float GimbalPitchGyrRef=0.0f;
float GimbalYawPosRef=0.0f;
float GimbalYawGyrRef=0.0f;
float ChassisGoToward=0.0f;
float ChassisGoLeftRight=0.0f;
float ChassisMotorSpeed1,ChassisMotorSpeed2,ChassisMotorSpeed3,ChassisMotorSpeed4;

float ChassisRotateOut=0.0f;
float CMOutput1,CMOutput2,CMOutput3,CMOutput4;
float GMYawOutput,GMPitchOutput;
unsigned int test_fire_speed = 1500;

/**
  * @brief 底盘控制程序
  * @param _T程序调用周期(s)
  * @retval None
  * @details 	根据当前的期望底盘移动速度和当前底盘功率
	*						对四个底盘电机的电流进行控制
  */
void ChassisControl(float _T)
{	
	if (SysMode!=SYS_CALISTATE&&SysMode!=SYS_STOPSTATE)
	{
		switch(ControlMode)
		{
			case MC_NORMAL:
				ChassisRotateOut=PID_calculate( _T,            //周期（单位：秒）
																				0,				//前馈值
																				0,				//期望值（设定值）
																				GMYawEncoder.ecd_value,			//反馈值（）
																				&Chassis_Rot_PID_arg, //PID参数结构体
																				&Chassis_Rot_PID_val,	//PID数据结构体
																				0.2		//integration limit，积分限幅
																				 );
				break;
			
			case MC_MODE1:
				
				break;
			
			case MC_MODE2:
				
				break;
			
			default:
				break;
		}
	}
	else
	{
		ChassisRotateOut=0.0f;
		ChassisGoToward=0.0f;
		ChassisGoLeftRight=0.0f;	
	}
	ChassisMotorSpeed1=-ChassisGoToward*0.075f+ChassisGoLeftRight*0.075f+ChassisRotateOut;
	ChassisMotorSpeed2=ChassisGoToward*0.075f+ChassisGoLeftRight*0.075f+ChassisRotateOut;
	ChassisMotorSpeed3=ChassisGoToward*0.075f-ChassisGoLeftRight*0.075f+ChassisRotateOut;
	ChassisMotorSpeed4=-ChassisGoToward*0.075f-ChassisGoLeftRight*0.075f+ChassisRotateOut;
	CMOutput1=PID_calculate( 			_T,            //周期（单位：秒）
																0,				//前馈值
																ChassisMotorSpeed1,				//期望值（设定值）
																CM1Encoder.filter_rate,			//反馈值（）
																&Chassis_Vec_PID_arg, //PID参数结构体
																&Chassis_Vec_PID_val1,	//PID数据结构体
															 	0.2		//integration limit，积分限幅
																 );
	CMOutput2=PID_calculate( 			_T,            //周期（单位：秒）
																0,				//前馈值
																ChassisMotorSpeed2,				//期望值（设定值）
																CM2Encoder.filter_rate,			//反馈值（）
																&Chassis_Vec_PID_arg, //PID参数结构体
																&Chassis_Vec_PID_val2,	//PID数据结构体
																0.2		//integration limit，积分限幅
																 );
	CMOutput3=PID_calculate( 			_T,            //周期（单位：秒）
																0,				//前馈值
																ChassisMotorSpeed3,				//期望值（设定值）
																CM3Encoder.filter_rate,			//反馈值（）
																&Chassis_Vec_PID_arg, //PID参数结构体
																&Chassis_Vec_PID_val3,	//PID数据结构体
																0.2		//integration limit，积分限幅
																 );
	CMOutput4=PID_calculate( 			_T,            //周期（单位：秒）
																0,				//前馈值
																ChassisMotorSpeed4,				//期望值（设定值）
																CM4Encoder.filter_rate,			//反馈值（）
																&Chassis_Vec_PID_arg, //PID参数结构体
																&Chassis_Vec_PID_val4,	//PID数据结构体
																0.2		//integration limit，积分限幅
																 );

	if (SysMode!=SYS_CALISTATE&&SysMode!=SYS_STOPSTATE)
	{
		ChassisSpeedSet(CAN1,CMOutput1,CMOutput2,CMOutput3,CMOutput4);
	}
	else
	{
		ChassisSpeedSet(CAN1,0,0,0,0);
	}
	//ChassisSpeedSet(CAN1,600,0,0,0);
}

/**
  * @brief 浜戝彴鎺у埗绋嬪簭
  * @param _T绋嬪簭璋冪敤鍛ㄦ湡
  * @retval None
  * @details 鏍规嵁褰撳墠浜戝彴鐨勬湡鏈涜�掑害鏉ユ帶鍒朵袱涓�浜戝彴鐢垫満鐨勮緭鍑虹數娴�
  */
void GimbalControl(float _T)
{
	float yaw_speed,pitch_speed;
   if (SysMode!=SYS_CALISTATE&&SysMode!=SYS_STOPSTATE)
	 {
		 yaw_speed = PID_calculate( 			_T,            //���ڣ���λ���룩
																0,				//ǰ��ֵ
																GimbalYawPosRef,				//����ֵ���趨ֵ��
																Yaw,			//����ֵ����
																&GimbalYaw_Pos_PID_arg, //PID�����ṹ��
																&GimbalYaw_Pos_PID_val,	//PID���ݽṹ��
																0.2		//integration limit�������޷�
																 );
		 GMYawOutput = PID_calculate( 			_T,            //���ڣ���λ���룩
																0,				//ǰ��ֵ
																yaw_speed,				//����ֵ���趨ֵ��
																MPU6500_Gyro.z,			//����ֵ����
																&GimbalYaw_Vec_PID_arg, //PID�����ṹ��
																&GimbalYaw_Vec_PID_val,	//PID���ݽṹ��
																0.2		//integration limit�������޷�
																 );
		 pitch_speed = PID_calculate( 			_T,            //���ڣ���λ���룩
																0,				//ǰ��ֵ
																GimbalPitchPosRef,				//����ֵ���趨ֵ��
																GMPitchEncoder.ecd_value,			//����ֵ����
																&GimbalPitch_Pos_PID_arg, //PID�����ṹ��
																&GimbalPitch_Pos_PID_val,	//PID���ݽṹ��
																0.2		//integration limit�������޷�
																 );
		 GMPitchOutput = PID_calculate( 			_T,            //���ڣ���λ���룩
																0,				//ǰ��ֵ
																pitch_speed,				//����ֵ���趨ֵ��
																MPU6500_Gyro.y,			//����ֵ����
																&GimbalPitch_Vec_PID_arg, //PID�����ṹ��
																&GimbalPitch_Vec_PID_val,	//PID���ݽṹ��
																0.2		//integration limit�������޷�
																 );
		 GimbalCurrentSet(CAN1,GMYawOutput,GMPitchOutput);
	 }
	 else
	 {
		 GimbalCurrentSet(CAN1,0,0);
	 }
}

/**
  * @brief 鍙戝皠鏈烘瀯鎺у埗绋嬪簭
  * @param _T绋嬪簭璋冪敤鍛ㄦ湡(s)
  * @retval None
  * @details 鏍规嵁缁欑殑閬ユ帶鍣ㄦ垨鑰呭叾浠栦俊鍙锋帶鍒舵懇鎿﹁疆鍜屾嫧鍗曠數鏈虹殑杞�閫�
  */
void FireControl(float _T)
{
	if (SysMode!=SYS_CALISTATE&&SysMode!=SYS_STOPSTATE)
	{
		 if(RC_CtrlData.rc.s1 == 3)
		 { 
			 SetFrictionWheelSpeed(1500);  
		 }
		 else
		 {
			 SetFrictionWheelSpeed(1000);
		 }
	 }
}

