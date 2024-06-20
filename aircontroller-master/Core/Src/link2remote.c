#include "link2remote.h"


extern osSemaphoreId_t NrfIT_BinaryHandle;
extern osMessageQueueId_t rxQueueHandle;
extern osEventFlagsId_t CanFlyEventHandle;

static	uint8_t rx_buf[12]; 
static  uint8_t RxTaskFailedCount = 0;
/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)		
									
	@brief GPIO外部中断回调函数 
	
	@param  GPIO_Pin 哪个IO
								
	@retval  none
			
	@PS			
					**key0 1 2 up 皆为上升沿触发 0 1 2 为上拉 up为上拉 理清消抖思路 
					**0 1 2 触发后 IO 恢复高电平是为有效 反之则为抖动
					**up 触发后 与之相反
					**无需清除中断标志位，中断服务函数已经清除了
					**以上按键仅短按有效
*************************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY_Pin)
	{
		HAL_Delay(10);		
		if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin)==GPIO_PIN_SET)	
		{
			LED_Tog();
		}
	}
	  if(GPIO_Pin==NRF_IRQ_Pin)
	{
		osSemaphoreRelease(NrfIT_BinaryHandle);
	}		
	
}

/*无线接收atkpPacket*/
osStatus_t Link2Rem_ReceivePacket(uint8_t *p)
{
	assert_param(p);
	return osMessageQueueGet(rxQueueHandle, p,NULL,10);
}
osStatus_t Link2Rem_ReceivePacketBlocking(uint8_t *p)
{
	assert_param(p);
	return osMessageQueueGet(rxQueueHandle, p, NULL,osWaitForever);
}



uint8_t WaitTestSignal(void)
{
	//接收成功 
	if(NRF24L01_RxPacket(rx_buf) == 0)
	{
		if((rx_buf[0] == PACK_TEST)&&(rx_buf[11] == PACK_END))
			{			
				osEventFlagsSet(CanFlyEventHandle,LINK_Status_Flag);
				osDelay(500);
				//设置标志位 收到数据 连接成功
				return 0;
			}
			else 
				return 1;
	}
	else 
		return 2;

}


void Link2Remote_Init(void)
{
	//printf("\r\n开始接收\r\n");
	NRF24L01_RX_Mode();
	
	while(WaitTestSignal());
	 
	NRF24L01_TX_Mode();
	//进入自检程序 返回零时 自检通过
	while(AirCraft_TestSelf());
	
}


//接收任务
//接收的数据放入接收队列中

void radiolinkTask(void)
{			
	if(NRF24L01_RxPacket(rx_buf))
		{
			//接收失败 重新接收 超过次数断联·	
			//printf("接收失败\r\n");
			RxTaskFailedCount++;
			if(RxTaskFailedCount>50)
			{
				RxTaskFailedCount = 0;
				osEventFlagsClear(CanFlyEventHandle,LINK_Status_Flag);
			}
		}
		else
		{
			//接收成功 
			//printf("接收成功\r\n");
			RxTaskFailedCount = 0;
			osMessageQueuePut(rxQueueHandle,rx_buf,1,100);
		}

}


void link2rem_SendResult_of_Test(uint8_t Test_Status)
{
	uint8_t tx_buf[12];
	tx_buf[0] = Test_Status;
	tx_buf[11] = PACK_END;
	
	while(NRF24L01_TxPacket(tx_buf));
				
}
	 



