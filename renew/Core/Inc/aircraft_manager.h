#ifndef   _AIRCRAFT_MANAGER_H
#define   _AIRCRAFT_MANAGER_H

#include "main.h"
#include "motor.h"
#include "24l01.h"
#include "pid.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include "stm32f411_lee_conf.h"
#include "control.h"
#include "cmsis_os2.h"





void Aircraft_Task_Manage(void);
void HardWare_Init(void);


#endif


