
#include "stm32f411_lee_conf.h"



extern UART_HandleTypeDef huart1;

/*------------------------------------------------------------------------*/

/* ================      USART1 重定向       =============        */
/***************************************************************************************/
//重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);	
	
	return (ch);
}

//重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{		
	int ch;
	HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 100);	
	return (ch);
}
/*************************************************************************************/








/*-------------------------------------------------------------------*/

 
/*
while(MPU_Init())
	{
		printf("error");
		HAL_Delay(5000);
	}
	while(mpu_dmp_init())
	{
		temp = mpu_dmp_init();
		printf("MPU Error : %d",temp);
		HAL_Delay(500);
	}	
	
	
if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
        {
          temp=MPU_Get_Temperature();	//得到温度值
					MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
					MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
					printf("pitch = %f,  roll = %f,   yaw = %f\r\n",pitch,roll,yaw);
					printf("aacx = %d,  aacy = %d,   aacy = %d\r\n",aacx,aacy,aacz);
					printf("gyrox = %d,  gyroy = %d,   gyroz = %d\r\n",gyrox,gyroy,gyroz);
					printf("temp = %d\r\n",temp);
					HAL_Delay(1000);
				}
				
*/

