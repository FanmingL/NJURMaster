#ifndef _MODE_DUTY_H_
#define _MODE_DUTY_H_
#include "stm32f4xx.h"
enum
{
	SYS_STOPSTATE=0,
	SYS_PREPARESTATE,
	SYS_REMOTESTATE,
	SYS_UPPERSTATE
};
#define SYS_PREPARETIME 5000
extern u8 SysMode;
void WorkStateFSM(u32 sys);
#endif
