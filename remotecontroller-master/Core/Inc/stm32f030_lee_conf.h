/**
  ******************************************************************************
  * @file    stm32f407_lee_conf.h
  * @author  Cameron Lee
  * @brief   This file contains my personal configuration of peripherals.
  ******************************************************************************
  */ 
#ifndef Lee_Conf_H
#define Lee_Conf_H


#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "remoter_ctrl.h"


#define USART_REC_LEN 200     					//最大接收字节数


#define LED1_ON()       HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET)
#define LED1_OFF()      HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)

#define LED2_ON()       HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET)
#define LED2_OFF()      HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET)

#define LED3_ON()       HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET)
#define LED3_OFF()      HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET)

#define LED1_Tog()      HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin)        
#define LED2_Tog()      HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin) 
#define LED3_Tog()      HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin)


#define ASSERT(e)  if (e) ; \
        else assertFail( #e, __FILE__, __LINE__ )
			
				
void assertFail(char *exp, char *file, int line);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);
#endif  /* Lee_Conf_H   */
