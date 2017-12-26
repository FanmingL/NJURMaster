#include "main.h"
/**
  * @brief main函数，用于初始化执行主循环等
  * @param None
  * @retval 成功返回0，失败返回错误码
  */
int main()
{
	int j=0;
	char buffer[200];
	AppInit();
	All_Init();

	while (1)
	{
		j = sprintf(buffer," Init ok, this time is %.2f\n",Get_Time_Micros()/1000.f);
		Usart2_Send((u8*)buffer,j);
		delay_ms(1000);
	}
}

