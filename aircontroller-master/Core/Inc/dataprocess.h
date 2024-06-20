#ifndef   _DATAPROCESS_H
#define   _DATAPROCESS_H

#include "control.h"
#include "link2remote.h"
#include "main.h"
#include "mpu6050.h"



void Remote_Data_Process(void);
void Get_Float_Gyroscope(float *gx,float *gy,float *gz);
uint16_t thrust_limit(uint16_t x_in);
uint16_t thrust_limit2(uint16_t x_in);

#endif /* _DATAPROCESS_H */

