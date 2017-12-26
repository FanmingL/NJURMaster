#include "main.h"


RC_Ctrl_t RC_CtrlData;   //remote control data

/**
  * @brief 基本串口通讯协议解析
  * @param _item	包含完整一帧数据的数组的指针
	* @param _len		帧总长
  * @retval None
  * @details 上层硬件发来的信号或是地面站发来的信号的解析函数
  */
void BasicProtocolAnalysis(u8 *_item,int _len)
{


}

/**
  * @brief 遥控器解析函数
  * @param pData	完整一帧接收机发来的数据
  * @param _len		帧总长
  * @retval None
  */
void RcProtocolAnalysis(u8 *pData,int _len)
{
	//BOTH_LED_TOGGLE();
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

