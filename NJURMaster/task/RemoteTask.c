#include "main.h"
InputMode_e inputmode = STOP;
void SetInputMode(Remote *rc)
{
		if(rc->s2 == 1)
	{
	
		inputmode = REMOTE_INPUT;
	}
	else if(rc->s2 == 3)
	{
		inputmode = KEY_MOUSE_INPUT;
	}
	else if(rc->s2 == 2)
	{
		inputmode = STOP;
	}
}
void RemoteControlProcess(Remote *rc)
{
	
}

InputMode_e GetInputMode(void)
{
	return inputmode;
}

void MouseKeyControlProcess(Mouse* mouse, Key* key)
{
	
}

