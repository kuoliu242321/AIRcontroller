#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include <stdbool.h>
#include <stdlib.h>
#include "adc.h"
#include "main.h"

#define    NOFCHANEL							4


// 0：右X  1：左Y   2：左X   3：右Y
#define  ADC_THRUST		1
#define  ADC_ROLL			0
#define  ADC_PITCH	 	3
#define  ADC_YAW			2


#define		ROLL_MID 					2000
#define   ROLL_RANGE_POS		2000
#define		ROLL_RANGE_NEG		2000

#define		PITCH_MID 				2000
#define   PITCH_RANGE_POS		2000
#define		PITCH_RANGE_NEG		2000

#define		YAW_MID 					2000
#define   YAW_RANGE_POS			2000
#define		YAW_RANGE_NEG			2000

#define		THRUST_MID 				2000
#define   THRUST_RANGE_POS	2000
#define		THRUST_RANGE_NEG	2000


//#define MAX(a,b) a>=b? a:b
//#define MIN(a,b) a<=b? a:b

enum dir_e
{
	CENTER = 0x00,
	FORWARD = 0x01,
	BACK   = 0x08,
	LEFT = 0x04,
	RIGHT = 0x02,
	BACK_LEFT = 0x0C,
	BACK_RIGHT  = 0x0A,
};


//飞控数据结构
typedef struct 
{
	float roll;
	float pitch;
	float yaw;
	float thrust;
}joystickFlyf_t;

//飞控数据结构
typedef struct 
{
	uint16_t roll;
	uint16_t pitch;
	uint16_t yaw;
	uint16_t thrust;
}joystickFlyui16_t;


void joystickInit(void);

void getFlyDataADCValue(joystickFlyui16_t *adcValue); //供显示使用

void ADCtoFlyDataPercent(joystickFlyf_t *flyDataPercent);

enum dir_e getJoystick1Dir(uint8_t mode);

enum dir_e getJoystick2Dir(uint8_t mode);


#endif /*__JOYSTICK_H*/
