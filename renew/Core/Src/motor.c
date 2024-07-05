#include "motor.h"

extern TIM_HandleTypeDef htim3;


/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	uint8_t Motor_Start(void)	
									
	@brief ����������� 
	
	@param  Void
								
	@retval  0 �� ȫ�������ɹ�
					1~4 �� ��Ӧ�������ʧ��
			
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
									
	@brief ���ֹͣ���� 
	
	@param  Void
								
	@retval  0 �� ȫ��ֹͣ�ɹ�
					1~4 �� ��Ӧ���ֹͣʧ��
			
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
									
	@brief ������ó���
	
	@param  Motor1~4_PWM �� ��Ӧ�����PWM
								
	@retval  void
			
	@PS			���ת�����ã������з�Χ�жϣ���Χ�������ã���������
					δ�����޷��������޷���������PID���ڵĳ�����
				
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
									
	@brief �������
	
	@param  Void
								
	@retval  0 �� �ɹ�
				   1 �� ʧ��
			
	@PS			
				
****************************************************************/

void ESC_Unlock(void)
{
	//�����趨
	//Highest_Thrust();
	//HAL_Delay(2000);
	
	Lowest_Thrust();
	HAL_Delay(1000);	
		
	
}


/****************************************************************/


/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	
									
	@brief �������
	
	@param  Void
								
	@retval  0 �� �ɹ�
				   1 �� ʧ��
			
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
									
	@brief �������
	
	@param  Void
								
	@retval  0 �� �ɹ�
				   1 �� ʧ��
			
	@PS			
				
****************************************************************/
void Highest_Thrust(void)
{
	Motor_PWM_Config(2000-10,2000-10,2000-10,2000-10);	
}





