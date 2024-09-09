#include "management.h"

extern osThreadId_t RadiolinkHandle;
extern osThreadId_t CommandersendHandle;
extern osEventFlagsId_t REM_StatusFlagHandle;



void UI_Display(void)
{
	
	OLED_Clear();
	OLED_ShowString(20,7,"test24.5.13",8);
	OLED_ShowString(0,2,"Init Over",8);
	
	OLED_ShowString(0,6,"----------------",8);
}



void HardWare_Init(void)
{
	OLED_Init();
	NRF24L01_Init();
	joystickInit();

	//默认界面显示
	UI_Display();
		
}




void Management_Prepare(void)
{
	osThreadSuspend(RadiolinkHandle);
	osThreadSuspend(CommandersendHandle);

	
	radiolinkInit();
	
	OLED_ShowString(0,0,"Prepare well",8);
	
	osThreadResume(RadiolinkHandle);
	osThreadResume(CommandersendHandle);	
	
}



void Task_Management(void)
{
	uint32_t temp_flag;
	temp_flag = osEventFlagsGet(REM_StatusFlagHandle);
	
	if(	(temp_flag&LINK_Status_Flag) == LINK_Status_Flag)
	{
		if(	(temp_flag&TASK_Canrun_Flag) == TASK_Canrun_Flag)
		{
			osThreadResume(RadiolinkHandle);
			osThreadResume(CommandersendHandle);					
		}
		else
		{
			osThreadSuspend(RadiolinkHandle);
			osThreadSuspend(CommandersendHandle);
			//显示终止数据传输
		}		
		
	}
	else
	{
		
		osThreadSuspend(RadiolinkHandle);
		osThreadSuspend(CommandersendHandle);
		//显示连接失败
	}

}












