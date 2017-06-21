#include "datatransfer.h"
#include "led.h"
#include "rc.h"
#include "duty.h"
/********************************************
DataTransfer
********************************************/

void DataExchange(void)
{
	//static u8 cnt = 0;
	//static u8 mpu = 10;

}


void USART2_Receive_Prepare(u8 _temp)
{
	static u8 _step=0;
	if (_step==0 && _temp==0xff)
	{
		
	}
}

void USART3_Receive_Prepare(u8 _temp)
{
	static u8 _step=0;
	if (_temp==0xff)RED_LED_ON();
	if (_step==0 && _temp==0xff)
	{
		
	}
}
u8 rc_count=0;
u8 rc_timeout=0;
u8 rc_data[18];
u16 rc_time_cnt=0;
void RC_Receive_Prepare(u8 _temp)
{
	if (rc_time_cnt>=5&&rc_count!=0)rc_count=0;
	rc_data[rc_count++]=_temp;
	if (rc_count==18)
	{
		RemoteDataPrcess(rc_data);
		rc_time_cnt=0;
		rc_count=0;
	}
}

void USART2_Receive_Anl(u8 *data, u8 len)
{

}

void USART3_Receive_Anl(u8 *data, u8 len)
{
	
}


