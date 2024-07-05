#include "radiolink.h"

extern osMessageQueueId_t txQueueHandle;
extern osSemaphoreId_t nrfITHandle;
extern osEventFlagsId_t REM_StatusFlagHandle;


static uint8_t statusCount;
static uint8_t CMD_TxNub;
static uint8_t failTxCount;
static uint8_t failTX_Num;
static uint8_t rx_buf[12];
static uint8_t tx_buf[12];

/*nrf外部中断回调函数*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  
  if(GPIO_Pin==NRF_IRQ_Pin)
	{
		osSemaphoreRelease(nrfITHandle);
	}		
	
	if(GPIO_Pin == KEY3_Pin )
	{
		HAL_Delay(10);		
		LED2_Tog();
		if(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin )==GPIO_PIN_RESET)	
		{
			LED2_Tog();
			Packdata(PACK_KEY,KEY3_Value);
		}
	}
	if(GPIO_Pin == KEY2_Pin)
	{
		HAL_Delay(10);		
		LED2_Tog();
		if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==GPIO_PIN_RESET)	
		{
			LED1_Tog();
			Packdata(PACK_KEY,KEY2_Value);
		}
	}
	if(GPIO_Pin == KEY1_Pin)
	{
		HAL_Delay(10);		
		LED2_Tog();
		if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_RESET)	
		{
			LED1_Tog();
			Packdata(PACK_KEY,KEY1_Value);
		}
	}
	if(GPIO_Pin == Joystick_L_Pin)
	{
		HAL_Delay(10);	
		LED2_Tog();		
		if(HAL_GPIO_ReadPin(Joystick_L_GPIO_Port,Joystick_L_Pin)==GPIO_PIN_SET)	
		{
			//LED1_Tog();
			Packdata(PACK_KEY,Joystick_L_Value);
		}
	}
	if(GPIO_Pin == Joystick_R_Pin)
	{
		HAL_Delay(10);		
		LED2_Tog();
		if(HAL_GPIO_ReadPin(Joystick_R_GPIO_Port,Joystick_R_Pin)==GPIO_PIN_SET)	
		{
			LED1_Tog();
			Packdata(PACK_KEY,Joystick_R_Value);
		}
	}
	
}

uint8_t SendTestSignal(void)
{
	tx_buf[0] = PACK_TEST;
	tx_buf[11] = PACK_END;
	
	if(NRF24L01_TxPacket(tx_buf) == 0)   //测试包发送完成
	{
		//多发几次？
		// 连接状态标志位 
		osEventFlagsSet(REM_StatusFlagHandle,LINK_Status_Flag);
		osDelay(500);
		return 0;
	}
	else
		return 1;
}


uint8_t WaitResultofTest(void)
{
	
	if(NRF24L01_RxPacket(rx_buf) == 0) //接收到数据
	{				
		//解析
		 if((rx_buf[0]==Test_Res_Initing)&&(rx_buf[11]==PACK_END))
			{
				osDelay(100);
				return 1;
			}
			
			else if((rx_buf[0]==Test_Res_Error)&&(rx_buf[11]==PACK_END))
			{
				//显示飞控自检失败
				//给标志位？
				return 2;
			}
			
			else if((rx_buf[0]==Test_Res_OK)&&(rx_buf[11]==PACK_END))
			{
				osEventFlagsSet(REM_StatusFlagHandle,TASK_Canrun_Flag);
				osDelay(500);
				//界面显示可以起飞
				//释放可以起飞的信号量或者flag				
				return 0;
			}					//数据解析完
		else
			return 3;
	}	
	else
		return 4;
}



/*无线连接初始化*/

void radiolinkInit(void)
{	
	while(SendTestSignal());
	
	NRF24L01_RX_Mode();
	
	while(WaitResultofTest());
	
	NRF24L01_TX_Mode();
	
}	


/*无线发送atkpPacket*/
osStatus_t radiolinkSendPacket(const uint8_t *p)
{
	ASSERT(p);
	return osMessageQueuePut(txQueueHandle, p,2,0);
}
osStatus_t radiolinkSendPacketBlocking(const uint8_t *p)
{
	ASSERT(p);
	return osMessageQueuePut(txQueueHandle, p,1,osWaitForever);
}



/*数据发送任务*/
void radiolinkTask(void)
{		
		if(NRF24L01_TxPacket(tx_buf))
			{
				failTxCount++;
				statusCount++;
				if(statusCount > 10)//连续10次发送失败
				{
					statusCount = 0;
					osEventFlagsClear(REM_StatusFlagHandle,LINK_Status_Flag);
					//超过十次 停止重连
					//界面显示连接失败超过十次，停止连接 显示连接失败1
				}		
			}
			else  //发送成功
			{
			if(osMessageQueueGet(txQueueHandle,tx_buf,NULL,10))
				{
					//编辑失败信息下次发送给飞控
					tx_buf[0] = PACK_ERROR;
					tx_buf[11] = PACK_END;
					//界面显示发送队列取出失败
				}
		}
		CMD_TxNub++;
		/*1000ms统计一次收发失败次数   如何实现呀！V2版本没有获取节拍功能*/
		//现在是每100次统计一次失败次数
		if(CMD_TxNub>100)
		{
			failTX_Num = failTxCount;
			failTxCount = 0;
			CMD_TxNub=0;
		}				
			
}

/*获取丢包个数*/
uint16_t radioinkfailTxCount(void)
{
	return failTX_Num;
}











