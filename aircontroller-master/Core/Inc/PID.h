#ifndef __PID_H
#define __PID_H

#include "dataconfiguration.h"

typedef struct{

	float P;			//比例常数
	float I;			//积分常数
	float D;			//微分常数
	float LastError;	//上次偏差值
	float Errorsum;		//上上次偏差值
	float I_Out_MAX;
	float I_Out_MIN;
	float PID_Out; 
	float PID_Out_MAX;
	float PID_Out_MIN;  
	float ErrorRang;   //稳态误差范围 比如加热50度，想要在45~55 度之间加入积分消除稳态误差则其为 5
}PID;



#define Errorsum_MAX  80

/*       函数           */

void PID_Process(PID* sptr,float Target,float Measure);
void PID_Arg_Init(PID* roll_angle,PID* pitch_angle,
									PID* roll_rate ,PID* pitch_rate ,PID* yaw_rate);

#endif  /* PID  */ 

