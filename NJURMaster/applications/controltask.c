#include "main.h"


//values for PID Calculate.
float GimbalPitchPosRef=0.0f;
float GimbalPitchGyrRef=0.0f;
float GimbalYawPosRef=0.0f;
float GimbalYawGyrRef=0.0f;
float ChassisGoToward=0.0f;
float ChassisGoLeftRight=0.0f;


/**
  * @brief 底盘控制程序
  * @param _T程序调用周期(s)
  * @retval None
  * @details 	根据当前的期望底盘移动速度和当前底盘功率
	*						对四个底盘电机的电流进行控制
  */
void ChassisControl(float _T)
{
    ChassisSpeedSet(CAN1, 0, 0, 0, 0);
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

