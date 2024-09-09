#include "aircraft_manager.h"

extern  osEventFlagsId_t CanFlyEventHandle;
extern	osThreadId_t Link2RemTaskHandle;
extern	osThreadId_t ControlTaskHandle;
	
void HardWare_Init(void)
{
	while(NRF24L01_Init())
	{
		printf("NRF24L01_Init Failed");
		BEEP_ON();
		HAL_Delay(2000);
		BEEP_OFF();
		
	}
	while(PID_Init()!=true)
		{
		printf("PID_Init Failed");
		BEEP_ON();
		HAL_Delay(2000);
		BEEP_OFF();
	  }

	while(mpu_dmp_init())
	{
		printf("mpu_dmp_init Failed");
		HAL_Delay(500);
	}
	
	
		BEEP_ON();
		LED_ON();
		HAL_Delay(200);
		BEEP_OFF();
		LED_OFF();
		HAL_Delay(200);
		BEEP_ON();
		LED_ON();
		HAL_Delay(200);
		BEEP_OFF();
		LED_OFF();
		//printf("Init Over");
}



void Aircraft_Task_Manage(void)
{
	static uint32_t temp_flag;
	static uint8_t temp;
	temp_flag = osEventFlagsGet(CanFlyEventHandle);
		
		if(	(temp_flag&LINK_Status_Flag) == LINK_Status_Flag)
		{
			
			if(	(temp_flag&MOTOR_Status_Flag) == MOTOR_Status_Flag)
			{
				
				printf("CONTROL_TASK_Flag set\r\n");
				if((temp_flag&CONTROL_TASK_Flag) == CONTROL_TASK_Flag)
				{		
					temp = osThreadGetState(ControlTaskHandle);
						if(temp==osThreadBlocked){
							osThreadResume(ControlTaskHandle);
						}
					osEventFlagsSet(CanFlyEventHandle,CanFly_Status_Flag);
					osDelay(100);
				}
				else
				{
					do{
					osThreadResume(ControlTaskHandle);
					temp = osThreadGetState(ControlTaskHandle);
					}while(temp==osThreadBlocked);
					
					do{
					temp_flag = osEventFlagsSet(CanFlyEventHandle,CONTROL_TASK_Flag);
					temp_flag = osEventFlagsSet(CanFlyEventHandle,CanFly_Status_Flag);	
					}while((temp_flag&0x8000)==0x8000);
												
				}
			}
			else
			{
				//启动电机
				while(Motor_Start());
				ESC_Unlock();
				
				BEEP_ON();
				HAL_Delay(1000);
				BEEP_OFF();
				
				printf("Motor Start");
				osEventFlagsSet(CanFlyEventHandle,MOTOR_Status_Flag);
			}		
		}
		
		else
		{
			//连接断开 加入自动降落功能
			//AutomaticLand(); //即控制三个航向角为0度 并且不断下降推力thrust
					
			Motor_Stop();
			osThreadSuspend(ControlTaskHandle);
	
			osEventFlagsClear(CanFlyEventHandle,CONTROL_TASK_Flag);
			osEventFlagsClear(CanFlyEventHandle,CanFly_Status_Flag);
		}
		
//其他标志位检测 对应操作后续补充
}


