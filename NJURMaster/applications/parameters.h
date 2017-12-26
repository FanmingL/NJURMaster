#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_
#include "stm32f4xx.h"
#include "mymath.h"
#include "pid.h"
#define PIDGROUPLEN 6
#define Chassis_Pos_PID_arg ( *PID_arg )
#define Chassis_Vec_PID_arg ( *(PID_arg+1) )
#define Gimbal_Pos_PID_arg ( *(PID_arg+2) )
#define Gimbal_Vec_PID_arg ( *(PID_arg+3) )
#define Slibing_Pos_PID_arg ( *(PID_arg+4) )
#define Slibing_Vec_PID_arg ( *(PID_arg+5) )


void ParametersInit(void);
void SensorOffsetInit(void);
void ParametersSave(void);
void SensorsOffsetSave(void);
void IMU_GYRODataCali(void);
void IMU_ACCERDataCali(void);
void IMU_MAGDataCali(void);
void GimbalDataCali(void);

extern u8 CALIFLAG;
#define IMU_GYROCALING 0x01
#define IMU_ACCERCALING 0x02
#define IMU_MAGCALING 0x04
#define GIMBALCALING 0x08



typedef __packed struct 
{
	xyz_f_t GYRO_Offset;
	xyz_f_t ACCER_Offset;
	xyz_f_t MAG_Offset;
}IMUSensor_OffSet__;

typedef __packed struct
{
	u8 savedflag;
	IMUSensor_OffSet__ imu_offset;
	_PID_arg_st PID_ARG[PIDGROUPLEN];
}AllDataOffset__;
typedef union
{
	u8 data[sizeof(AllDataOffset__)];
	AllDataOffset__ AllData;

}AllDataUnion__;
extern u8 ParaSavingFlag;
extern u8 CALIFLAG;
extern IMUSensor_OffSet__ IMUSensor_Offset;
extern AllDataUnion__ AllDataUnion; 
extern u8 ParamSavedFlag;
extern _PID_arg_st PID_arg[PIDGROUPLEN];
#endif
