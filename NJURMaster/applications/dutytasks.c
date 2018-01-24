#include "main.h"

/**
  * @brief 1ms����
  * @param ϵͳ�ӿ��������ھ����ĺ�����
  * @retval None
  */
static void Duty_1ms(u32 _time)
{
	u32 loop_time = GetInnerLoop(Task_1ms_Time);
	DatatransferTask(_time);
	WorkStateFSM(_time);
	MPU6500_Data_Prepare();
#if 0
	IMUupdate((float)(loop_time/2000000.0f),MPU6500_Gyro.x,MPU6500_Gyro.y,MPU6500_Gyro.z,
						MPU6500_Acc.x,MPU6500_Acc.y,MPU6500_Acc.z,
						&Roll,&Pitch,&Yaw);
#else
	AHRSUpdate((float)(loop_time/1000000.0f),
					MPU6500_Acc.x,MPU6500_Acc.y,MPU6500_Acc.z,
					MPU6500_Gyro.x,MPU6500_Gyro.y,MPU6500_Gyro.z,
					MagValue.x,MagValue.y,MagValue.z,
					&Roll,&Pitch,&Yaw,
					0.12f,0.0f);
#endif
}

/**
  * @brief 2ms����
  * @param ϵͳ�ӿ��������ھ����ĺ�����
  * @retval None
  */
static void Duty_2ms(u32 _time)
{
	u32 loop_time = GetInnerLoop(Task_2ms_Time);

	
}

/**
  * @brief 5ms����
  * @param ϵͳ�ӿ��������ھ����ĺ�����
  * @retval None
  */
static void Duty_5ms(u32 _time)
{
	u32 loop_time = GetInnerLoop(Task_5ms_Time);
	IST8310_Data_Prepare();
	CheckDog ();
	ChassisControl(loop_time/1000000.0f);
	GimbalControl(loop_time/1000000.0f);
}

/**
  * @brief 10ms����
  * @param ϵͳ�ӿ��������ھ����ĺ�����
  * @retval None
  */
static void Duty_10ms(u32 _time)
{
	u32 loop_time = GetInnerLoop(Task_10ms_Time);
	
	FireControl(loop_time/1000000.0f);
	
}

/**
  * @brief 20ms����
  * @param ϵͳ�ӿ��������ھ����ĺ�����
  * @retval None
  */
static void Duty_20ms(u32 _time)
{
	u32 loop_time = GetInnerLoop(Task_20ms_Time);

}

/**
  * @brief 50ms����
  * @param ϵͳ�ӿ��������ھ����ĺ�����
  * @retval None
  */
static void Duty_50ms(u32 _time)
{
	//int j=0;
	//char buff[200];

	if (SysMode==SYS_PREPARESTATE&&(ParamSavedFlag==1))
	{
		BOTH_LED_TOGGLE();
	}
	//	j+=sprintf(j+buff,"%d\n",CM1Encoder.filter_rate);
	//	Usart2_Send((u8*)buff,j);
}

/**
  * @brief ϵͳ������ѭ��
  * @param None
  * @retval None
  * @details �ɶ�ʱ��ÿ����׼ʱ���ã�������������task
  */
void Duty_loop(void)
{
	static u32 systime_ms=0;
	(GetInnerLoop(DutyLoop_Time));
	systime_ms++;
	Duty_1ms(systime_ms);
	if (systime_ms%2==0)Duty_2ms(systime_ms);
	if (systime_ms%5==0)Duty_5ms(systime_ms);
	if (systime_ms%10==0)Duty_10ms(systime_ms);
	if (systime_ms%20==0)Duty_20ms(systime_ms);
	if (systime_ms%50==0)Duty_50ms(systime_ms);
	if (GetInnerLoop(DutyLoop_Time)<1000)FeedDog(DEVICE_INDEX_TIMEOUT);
	
}
	
	
