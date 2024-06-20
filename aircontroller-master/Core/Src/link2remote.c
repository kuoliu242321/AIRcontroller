#include "link2remote.h"


extern osSemaphoreId_t NrfIT_BinaryHandle;
extern osMessageQueueId_t rxQueueHandle;
extern osEventFlagsId_t CanFlyEventHandle;

static	uint8_t rx_buf[12]; 
static  uint8_t RxTaskFailedCount = 0;
/*-------------------------------------------------------------*/

/******************  =========================================== 
	@funcation	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)		
									
	@brief GPIO�ⲿ�жϻص����� 
	
	@param  GPIO_Pin �ĸ�IO
								
	@retval  none
			
	@PS			
					**key0 1 2 up ��Ϊ�����ش��� 0 1 2 Ϊ���� upΪ���� ��������˼· 
					**0 1 2 ������ IO �ָ��ߵ�ƽ��Ϊ��Ч ��֮��Ϊ����
					**up ������ ��֮�෴
					**��������жϱ�־λ���жϷ������Ѿ������
					**���ϰ������̰���Ч
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

/*���߽���atkpPacket*/
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
	//���ճɹ� 
	if(NRF24L01_RxPacket(rx_buf) == 0)
	{
		if((rx_buf[0] == PACK_TEST)&&(rx_buf[11] == PACK_END))
			{			
				osEventFlagsSet(CanFlyEventHandle,LINK_Status_Flag);
				osDelay(500);
				//���ñ�־λ �յ����� ���ӳɹ�
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
	//printf("\r\n��ʼ����\r\n");
	NRF24L01_RX_Mode();
	
	while(WaitTestSignal());
	 
	NRF24L01_TX_Mode();
	//�����Լ���� ������ʱ �Լ�ͨ��
	while(AirCraft_TestSelf());
	
}


//��������
//���յ����ݷ�����ն�����

void radiolinkTask(void)
{			
	if(NRF24L01_RxPacket(rx_buf))
		{
			//����ʧ�� ���½��� ��������������	
			//printf("����ʧ��\r\n");
			RxTaskFailedCount++;
			if(RxTaskFailedCount>50)
			{
				RxTaskFailedCount = 0;
				osEventFlagsClear(CanFlyEventHandle,LINK_Status_Flag);
			}
		}
		else
		{
			//���ճɹ� 
			//printf("���ճɹ�\r\n");
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
	 



