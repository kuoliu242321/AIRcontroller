
#include "joystick.h"
#include <stdio.h>

__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0};
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc;


//摇杆中间软件死区值（ADC值）
#define MID_DB_THRUST		150	
#define MID_DB_YAW			300	
#define MID_DB_PITCH		150
#define MID_DB_ROLL			150

//摇杆上下量程死区值（ADC值）
#define DB_RANGE			80

//获取摇杆方向时定义在中间的范围值（ADC值）
#define DIR_MID_THRUST		800
#define DIR_MID_YAW			800
#define DIR_MID_PITCH		800
#define DIR_MID_ROLL		800


/*去除死区函数*/
int16_t deadband(int16_t value, const int16_t threshold)
{
	if (abs(value) < threshold)
	{
		value = 0;
	}
	else if (value > 0)
	{
		value -= threshold;
	}
	else if (value < 0)
	{
		value += threshold;
	}
	return value;
}

/*摇杆初始化*/
void joystickInit(void)
{

	
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_ConvertedValue, NOFCHANEL);
	
}


/*获取摇杆ADC值*/
void getFlyDataADCValue(joystickFlyui16_t *adcValue)
{
	adcValue->thrust =  ADC_ConvertedValue[ADC_THRUST];
	adcValue->roll =  ADC_ConvertedValue[ADC_ROLL];
	adcValue->pitch =  ADC_ConvertedValue[ADC_PITCH];
	adcValue->yaw = ADC_ConvertedValue[ADC_YAW];
	
	
//	
//	printf("thrust = %d\r\n",adcValue->thrust);
//	printf("roll = %d\r\n",adcValue->roll);
//	printf("pitch = %d\r\n",adcValue->pitch);
//	printf("yaw = %d\r\n",adcValue->yaw);
	
}

//ADC值转换成飞控数据百分比
void ADCtoFlyDataPercent(joystickFlyf_t *percent)
{
	int16_t adcValue;
	
	//THRUST   左Y 上+ 下-
	adcValue = (int16_t)ADC_ConvertedValue[ADC_THRUST] - THRUST_MID;
	adcValue = -adcValue;  //左Y 遥感上顶 0  下顶 4096
	adcValue = deadband(adcValue,MID_DB_THRUST);
	if(adcValue>=0)
		percent->thrust = (float)adcValue/(float)(THRUST_RANGE_POS-MID_DB_THRUST+DB_RANGE);
	else
		percent->thrust = (float)adcValue/(float)(THRUST_RANGE_NEG-MID_DB_THRUST+DB_RANGE);
	//-((0~4096)-2000)/(2000-150+80)=(-1~1)
	//ROLL  左X  左— 右+
	adcValue = (int16_t)ADC_ConvertedValue[ADC_ROLL] - ROLL_MID;
	adcValue = deadband(adcValue, MID_DB_ROLL);
	if(adcValue >= 0)
		percent->roll = (float)adcValue/(float)(ROLL_RANGE_POS-MID_DB_ROLL+DB_RANGE);
	else
		percent->roll = (float)adcValue/(float)(ROLL_RANGE_NEG-MID_DB_ROLL+DB_RANGE);
	//((0~4096)-2000)/(2000-150+80)=(-1~1)
	//PITCH  右Y 上+ 下-
	adcValue = (int16_t)ADC_ConvertedValue[ADC_PITCH] - PITCH_MID;
	adcValue = -adcValue;  //右Y 遥感上顶 0  下顶 5096
	adcValue = deadband(adcValue, MID_DB_PITCH);
	if(adcValue >= 0)
		percent->pitch = (float)adcValue/(float)(PITCH_RANGE_POS-MID_DB_PITCH+DB_RANGE);
	else
		percent->pitch = (float)adcValue/(float)(PITCH_RANGE_NEG-MID_DB_PITCH+DB_RANGE);
	//-((0~4096)-2000)/(2000-150+80)=(-1~1)
	//YAW			右X  左— 右+
	adcValue = (int16_t)ADC_ConvertedValue[ADC_YAW] - YAW_MID;
	adcValue = deadband(adcValue, MID_DB_YAW);
	if(adcValue >= 0)
		percent->yaw = (float)adcValue/(float)(YAW_RANGE_POS-MID_DB_YAW+DB_RANGE);
	else
		percent->yaw = (float)adcValue/(float)(YAW_RANGE_NEG-MID_DB_YAW+DB_RANGE);
    //((0~4096)-2000)/(2000-150+80)=(-1~1)
}


/*获取摇杆1方向*/
/*mode:0,不支持连续按;1,支持连续按*/

enum dir_e getJoystick1Dir(uint8_t mode)
{
	enum dir_e ret=CENTER;
	joystickFlyui16_t adcValue;
	static bool havebackToCenter = true;
	
	getFlyDataADCValue(&adcValue);
	if(mode) havebackToCenter = true;
	if(havebackToCenter == true)//摇杆回到过中间位置
	{
		if(adcValue.thrust > (THRUST_MID+DIR_MID_THRUST))
			ret = FORWARD;
		else if(adcValue.thrust < (THRUST_MID-DIR_MID_THRUST))
			ret = BACK;
		
		if(ret==BACK && adcValue.yaw>(YAW_MID+DIR_MID_YAW))
			ret = BACK_RIGHT;
		else if(ret==BACK && adcValue.yaw<(YAW_MID-DIR_MID_YAW))
			ret = BACK_LEFT;
		else if(adcValue.yaw > (YAW_MID+DIR_MID_YAW))
			ret = RIGHT;
		else if(adcValue.yaw < (YAW_MID-DIR_MID_YAW))
			ret = LEFT;
		
		havebackToCenter = false;//摇杆离开了中间位置
		if(ret == CENTER)//摇杆依然在中间位置
			havebackToCenter = true;
	}
	else if( adcValue.thrust >= (THRUST_MID-DIR_MID_THRUST) &&
			 adcValue.thrust <= (THRUST_MID+DIR_MID_THRUST) &&
			 adcValue.yaw >= (YAW_MID-DIR_MID_YAW) &&
			 adcValue.yaw <= (YAW_MID+DIR_MID_YAW) 
		   )//摇杆离开了中间位置，现在查询摇杆是否回中
	{
		havebackToCenter = true;
		ret = CENTER;
	}
	
	return ret;
}


/*获取摇杆2方向*/
/*mode:0,不支持连续按;1,支持连续按*/

enum dir_e getJoystick2Dir(uint8_t mode)
{
	enum dir_e ret = CENTER;
	joystickFlyui16_t adcValue;
	static bool havebackToCenter = true;
	
	getFlyDataADCValue(&adcValue);
	if(mode) havebackToCenter = true;
	if(havebackToCenter == true)//摇杆回到过中间位置
	{	
		if(adcValue.pitch > (PITCH_MID+DIR_MID_PITCH))
			ret = FORWARD;
		else if(adcValue.pitch < (PITCH_MID-DIR_MID_PITCH))
			ret = BACK;
		
		if(ret==BACK && adcValue.roll>(ROLL_MID+DIR_MID_ROLL))
			ret = BACK_RIGHT;
		else if(ret==BACK && adcValue.roll<(ROLL_MID-DIR_MID_ROLL))
			ret = BACK_LEFT;
		else if(adcValue.roll>(ROLL_MID+DIR_MID_ROLL))
			ret = RIGHT;
		else if(adcValue.roll<(ROLL_MID-DIR_MID_ROLL))
			ret = LEFT;

		havebackToCenter = false;//摇杆离开了中间位置
		if(ret == CENTER)//摇杆依然在中间位置
			havebackToCenter = true;
	}
	else if( adcValue.pitch >= (PITCH_MID-DIR_MID_PITCH) &&
			 adcValue.pitch <= (PITCH_MID+DIR_MID_PITCH) &&
			 adcValue.roll >= (ROLL_MID-DIR_MID_ROLL) &&
			 adcValue.roll <= (ROLL_MID+DIR_MID_ROLL) 
		   )//摇杆离开了中间位置，现在查询摇杆是否回中
	{
		havebackToCenter = true;
		ret = CENTER;
	}
	
	return ret;
}

