#include "motor.h"

extern TIM_HandleTypeDef htim3;


/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	uint8_t Motor_Start(void)	
									
	@brief 电机启动程序 
	
	@param  Void
								
	@retval  0 ： 全部启动成功
					1~4 ： 对应电机启动失败
			
	@PS			
				
*************************************************************************************/
uint8_t Motor_Start(void)
{
	while(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1))
	{
		return 1;
	}
	while(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2))
	{
		return 2;
	}
	while(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3))
	{
		return 3;
	}
	while(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4))
	{
		return 4;
	}
	
	return 0;
}
/****************************************************************/


/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	uint8_t Motor_Stop(void)	
									
	@brief 电机停止程序 
	
	@param  Void
								
	@retval  0 ： 全部停止成功
					1~4 ： 对应电机停止失败
			
	@PS			
				
****************************************************************/
uint8_t Motor_Stop(void)
{
	while(HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1))
	{
		return 1;
	}
	while(HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2))
	{
		return 2;
	}
	while(HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3))
	{
		return 3;
	}
	while(HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4))
	{
		return 4;
	}
	
	return 0;
}
/****************************************************************/

/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	void Motor_PWM_Config(uint16_t Motor1_PWM,uint16_t Motor2_PWM,
												            uint16_t Motor3_PWM,uint16_t Motor4_PWM,)
									
	@brief 电机配置程序
	
	@param  Motor1~4_PWM ： 对应电机的PWM
								
	@retval  void
			
	@PS			电机转速配置，仅进行范围判断，范围内则配置，否则不配置
					未进行限幅操作，限幅操作放于PID调节的程序中
				
****************************************************************/
void Motor_PWM_Config(uint16_t Motor1_PWM,uint16_t Motor2_PWM,
												 uint16_t Motor3_PWM,uint16_t Motor4_PWM)
{
	
	//if((Motor1_PWM<(Motor_PWM_Max-Motor_PWM_Death))&&(Motor1_PWM>(Motor_PWM_Min+Motor_PWM_Death)))
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Motor1_PWM);
	//if((Motor2_PWM<(Motor_PWM_Max-Motor_PWM_Death))&&(Motor2_PWM>(Motor_PWM_Min+Motor_PWM_Death)))
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, Motor2_PWM);
	//if((Motor3_PWM<(Motor_PWM_Max-Motor_PWM_Death))&&(Motor3_PWM>(Motor_PWM_Min+Motor_PWM_Death)))
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, Motor3_PWM);
	//if((Motor4_PWM<(Motor_PWM_Max-Motor_PWM_Death))&&(Motor4_PWM>(Motor_PWM_Min+Motor_PWM_Death)))
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, Motor4_PWM);
	
}

/****************************************************************/


/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	
									
	@brief 电调解锁
	
	@param  Void
								
	@retval  0 ： 成功
				   1 ： 失败
			
	@PS			
				
****************************************************************/

void ESC_Unlock(void)
{
	//航程设定
	//Highest_Thrust();
	//HAL_Delay(2000);
	
	Lowest_Thrust();
	HAL_Delay(1000);	
		
	
}


/****************************************************************/


/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	
									
	@brief 最低油门
	
	@param  Void
								
	@retval  0 ： 成功
				   1 ： 失败
			
	@PS			
				
****************************************************************/
void Lowest_Thrust(void)
{
	Motor_PWM_Config(1000-10,1000-10,1000-10,1000-10);
}

/****************************************************************/


/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	
									
	@brief 最高油门
	
	@param  Void
								
	@retval  0 ： 成功
				   1 ： 失败
			
	@PS			
				
****************************************************************/
void Highest_Thrust(void)
{
	Motor_PWM_Config(2000-10,2000-10,2000-10,2000-10);	
}





