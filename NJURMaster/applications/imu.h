#ifndef _IMU_H_
#define _IMU_H_

typedef struct 
{
	xyz_f_t err;
	xyz_f_t err_tmp;
	xyz_f_t err_lpf;
	xyz_f_t err_Int;
	xyz_f_t g;
	
}ref_t;
void IMUupdate(float half_T,float gx, float gy, float gz, float ax, float ay, float az,float *rol,float *pit,float *yaw);
extern float Roll,Pitch,Yaw;
extern float ref_q[4];
#define ANGLE_TO_RADIAN 0.01745329f
#define IMU_INTEGRAL_LIM  ( 2.0f *ANGLE_TO_RADIAN )
#endif
