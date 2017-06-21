#ifndef _REOMTE_TASK_H_
#define _REOMTE_TASK_H_
#include "rc.h"
void SetInputMode(Remote *rc);
void RemoteControlProcess(Remote *rc);
void MouseKeyControlProcess(Mouse* mouse, Key* key);
#endif

