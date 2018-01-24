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
  * @brief åº•ç›˜æ§åˆ¶ç¨‹åº
  * @param _Tç¨‹åºè°ƒç”¨å‘¨æœŸ(s)
  * @retval None
  * @details 	æ ¹æ®å½“å‰çš„æœŸæœ›åº•ç›˜ç§»åŠ¨é€Ÿåº¦å’Œå½“å‰åº•ç›˜åŠŸç‡
	*						å¯¹å››ä¸ªåº•ç›˜ç”µæœºçš„ç”µæµè¿›è¡Œæ§åˆ¶
  */
void ChassisControl(float _T)
{	
	if (SysMode!=SYS_CALISTATE&&SysMode!=SYS_STOPSTATE)
	{
		switch(ControlMode)
		{
			case MC_NORMAL:
				ChassisRotateOut=PID_calculate( _T,            //ÖÜÆÚ£¨µ¥Î»£ºÃë£©
																				0,				//Ç°À¡Öµ
																				0,				//ÆÚÍûÖµ£¨Éè¶¨Öµ£©
																				GMYawEncoder.ecd_value,			//·´À¡Öµ£¨£©
																				&Chassis_Rot_PID_arg, //PID²ÎÊı½á¹¹Ìå
																				&Chassis_Rot_PID_val,	//PIDÊı¾İ½á¹¹Ìå
																				0.2		//integration limit£¬»ı·ÖÏŞ·ù
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
	CMOutput1=PID_calculate( 			_T,            //ÖÜÆÚ£¨µ¥Î»£ºÃë£©
																0,				//Ç°À¡Öµ
																ChassisMotorSpeed1,				//ÆÚÍûÖµ£¨Éè¶¨Öµ£©
																CM1Encoder.filter_rate,			//·´À¡Öµ£¨£©
																&Chassis_Vec_PID_arg, //PID²ÎÊı½á¹¹Ìå
																&Chassis_Vec_PID_val1,	//PIDÊı¾İ½á¹¹Ìå
															 	0.2		//integration limit£¬»ı·ÖÏŞ·ù
																 );
	CMOutput2=PID_calculate( 			_T,            //ÖÜÆÚ£¨µ¥Î»£ºÃë£©
																0,				//Ç°À¡Öµ
																ChassisMotorSpeed2,				//ÆÚÍûÖµ£¨Éè¶¨Öµ£©
																CM2Encoder.filter_rate,			//·´À¡Öµ£¨£©
																&Chassis_Vec_PID_arg, //PID²ÎÊı½á¹¹Ìå
																&Chassis_Vec_PID_val2,	//PIDÊı¾İ½á¹¹Ìå
																0.2		//integration limit£¬»ı·ÖÏŞ·ù
																 );
	CMOutput3=PID_calculate( 			_T,            //ÖÜÆÚ£¨µ¥Î»£ºÃë£©
																0,				//Ç°À¡Öµ
																ChassisMotorSpeed3,				//ÆÚÍûÖµ£¨Éè¶¨Öµ£©
																CM3Encoder.filter_rate,			//·´À¡Öµ£¨£©
																&Chassis_Vec_PID_arg, //PID²ÎÊı½á¹¹Ìå
																&Chassis_Vec_PID_val3,	//PIDÊı¾İ½á¹¹Ìå
																0.2		//integration limit£¬»ı·ÖÏŞ·ù
																 );
	CMOutput4=PID_calculate( 			_T,            //ÖÜÆÚ£¨µ¥Î»£ºÃë£©
																0,				//Ç°À¡Öµ
																ChassisMotorSpeed4,				//ÆÚÍûÖµ£¨Éè¶¨Öµ£©
																CM4Encoder.filter_rate,			//·´À¡Öµ£¨£©
																&Chassis_Vec_PID_arg, //PID²ÎÊı½á¹¹Ìå
																&Chassis_Vec_PID_val4,	//PIDÊı¾İ½á¹¹Ìå
																0.2		//integration limit£¬»ı·ÖÏŞ·ù
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
  * @brief äº‘å°æ§åˆ¶ç¨‹åº
  * @param _Tç¨‹åºè°ƒç”¨å‘¨æœŸ
  * @retval None
  * @details æ ¹æ®å½“å‰äº‘å°çš„æœŸæœ›è§’åº¦æ¥æ§åˆ¶ä¸¤ä¸ªäº‘å°ç”µæœºçš„è¾“å‡ºç”µæµ
  */
void GimbalControl(float _T)
{


}

/**
  * @brief å‘å°„æœºæ„æ§åˆ¶ç¨‹åº
  * @param _Tç¨‹åºè°ƒç”¨å‘¨æœŸ(s)
  * @retval None
  * @details æ ¹æ®ç»™çš„é¥æ§å™¨æˆ–è€…å…¶ä»–ä¿¡å·æ§åˆ¶æ‘©æ“¦è½®å’Œæ‹¨å•ç”µæœºçš„è½¬é€Ÿ
  */
void FireControl(float _T)
{


}

