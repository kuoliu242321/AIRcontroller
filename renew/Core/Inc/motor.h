#ifndef   _MOTOR_H
#define   _MOTOR_H

#include "tim.h"
#include "main.h"





#define Motor_PWM_Min  990
#define Motor_PWM_Max  1500  //1990



typedef struct MotorPwm
{
	uint16_t Motor1_Compare;
	uint16_t Motor2_Compare;
	uint16_t Motor3_Compare;
	uint16_t Motor4_Compare;
}Motor_PWM_Typedef;



uint8_t Motor_Start(void);

void Motor_PWM_Config(uint16_t Motor1_PWM,uint16_t Motor2_PWM,
												 uint16_t Motor3_PWM,uint16_t Motor4_PWM);		
												 
uint8_t Motor_Stop(void);


void Lowest_Thrust(void);
void ESC_Unlock(void);
void Highest_Thrust(void);

















#endif /* _MOTOR_H */
