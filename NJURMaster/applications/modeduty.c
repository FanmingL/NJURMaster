#include "main.h"

u8 SysMode=SYS_PREPARESTATE;
/**
  * @brief Ä£Ê½ÇÐ»»
  * @param None
  * @retval None
  */
void WorkStateFSM(u32 sys)
{
	if (sys<SYS_PREPARETIME)
	{
		SysMode=SYS_PREPARESTATE;
		return;
	}
	SysMode=SYS_REMOTESTATE;
	

}

