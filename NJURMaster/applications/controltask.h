#ifndef _CONTROLTASK_H_
#define _CONTROLTASK_H_

void ChassisControl(float _T);
void GimbalControl(float _T);
void FireControl(float _T);

extern float GimbalPitchPosRef,GimbalPitchGyrRef,GimbalYawPosRef,GimbalYawGyrRef;
#endif
