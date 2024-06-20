#include "stm32f030_lee_conf.h"


/*串口相关*/

uint8_t USART_RX_BUF[USART_REC_LEN]; 	  //接收数据
uint8_t aRxBuffer;											//接收中断缓冲
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA = 0;							//接收缓冲计数


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


/**====================================================   
	@funcation	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)						
									
	@brief 串口回调函数(接收中断已经处理完毕)  
	
	@param  *huart 哪个串口
								
	@retval  none
			
	@postscript						
	**使用前必须使能串口中断  放在main.c中
	**使用后也需再开启串口中断，因为使能中断仅一次有效
	**此函数为每接收一个就产生一次中断，但优点是可接收不定长内容（以回车结尾）
	**可自适应发送接收帧来更改
	**外加剪切


-------------->  	main.c 
****************************************************************
extern uint8_t aRxBuffer;
HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
****************************************************************

**************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
 if(huart->Instance==USART1)
  {
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else 
				{
					USART_RX_STA|=0x8000;	//接收完成了 
					//要进行什么操作在这里面哦
					HAL_UART_Transmit(&huart1,(uint8_t*)USART_RX_BUF,USART_RX_STA&0x3fff,1000);	//发送接收到的数据
					
					USART_RX_STA=0;					
				}							
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
}
/*******************************************************************************/
 
/*-------------------------------------------------------------*/


/******************  =========================================== 
	@funcation	void assertFail(char *exp, char *file, int line)
									
	@brief  断言驱动代码
	
	@param  char *exp 
					char	*file
					int line
								
	@retval  
			
	@PS			
					**
					
*************************************************************************************/
void assertFail(char *exp, char *file, int line)
{
	//printf("Assert failed %s:%d\n", file, line);
	while (1);
}

/*************************************************************************************/







