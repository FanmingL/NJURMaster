#include "main.h"

_ramp_st RcKeyTowardRamp={1000,1};
_ramp_st RcKeyLeftRightRamp={1000,1};
_ramp_st RcKeyRotateRamp={1000,1};


void RampReset(_ramp_st * ramp)
{
	ramp->Num_Current=0;
}

float RampCalculate(_ramp_st * ramp)
{
	if (ramp->Num_Current<ramp->Num_Max)
	{
		ramp->Num_Current++;
	}
	return ramp->Num_Current/ramp->Num_Max;
}











