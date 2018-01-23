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
  * @brief 云台控制程序
  * @param _T程序调用周期
  * @retval None
  * @details 根据当前云台的期望角度来控制两个云台电机的输出电流
  */
void GimbalControl(float _T)
{


}

/**
  * @brief 发射机构控制程序
  * @param _T程序调用周期(s)
  * @retval None
  * @details 根据给的遥控器或者其他信号控制摩擦轮和拨单电机的转速
  */
void FireControl(float _T)
{


}

