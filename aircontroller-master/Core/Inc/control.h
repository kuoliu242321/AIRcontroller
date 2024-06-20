#ifndef   _CONTROL_H
#define   _CONTROL_H

#include "pid.h"
#include "motor.h"
#include "dataprocess.h"
#include "main.h"
#include "dataconfiguration.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"




bool PID_Init(void);

void Control(FLOAT_POSTURE *att_in,FLOAT_XYZ *gyr_in, REMOTE_DATA_TYPEDEF *RemData_in);
void KEY_Process_Task(uint8_t KEY_Value);
uint8_t AirCraft_TestSelf(void);
void AirCraft_Control_Task(void);

#endif



