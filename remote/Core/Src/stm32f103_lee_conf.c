#include "stm32f103_lee_conf.h"


/*串口相关*/

uint8_t USART_RX_BUF[USART_REC_LEN]; 	  //接收数据
uint8_t aRxBuffer;											//接收�?�?缓冲
//接收状�?
//bit15�?	接收完成标志
//bit14�?	接收�?0x0d
//bit13~0�?	接收到的有效字节数目
uint16_t USART_RX_STA = 0;							//接收缓冲计数


extern UART_HandleTypeDef huart1;


/*------------------------------------------------------------------------*/

/* ================      USART1 重定�?       =============        */
/***************************************************************************************/
//重定向c库函数printf到串�?DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一�?字节数据到串�?DEBUG_USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);	
	
	return (ch);
}

//重定向c库函数scanf到串�?DEBUG_USART，重写向后可使用scanf、getchar等函�?
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
									
	@brief 串口回调函数(接收�?�?已经处理完毕)  
	
	@param  *huart �?�?串口
								
	@retval  none
			
	@postscript						
	**使用前必须使能串口中�?  放在main.c�?
	**使用后也需再开�?串口�?�?，因为使能中�?仅一次有�?
	**此函数为每接收一�?就产生一次中�?，但优点�?�?接收不定长内容（以回车结尾）
	**�?�?适应发送接收帧来更�?
	**外加�?�?


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
		if((USART_RX_STA&0x8000)==0)//接收�?完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer!=0x0a)USART_RX_STA=0;//接收错�??,重新开�?
				else 
				{
					USART_RX_STA|=0x8000;	//接收完成�? 
					//要进行什么操作在这里面哦
					HAL_UART_Transmit(&huart1,(uint8_t*)USART_RX_BUF,USART_RX_STA&0x3fff,1000);	//发送接收到的数�?
					
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
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错�??,重新开始接�?	  
				}		 
			}
		}
	}	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开�?接收�?�?
}
/*******************************************************************************/
 
/*-------------------------------------------------------------*/


/******************  =========================================== 
	@funcation	void assertFail(char *exp, char *file, int line)
									
	@brief  �?言驱动代码
	
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







