/**
  ******************************************************************************
  * @file    stm32f407_lee_conf.h
  * @author  Cameron Lee
  * @brief   This file contains my personal configuration of peripherals.
  ******************************************************************************
  */ 
#ifndef Lee_Conf_H
#define Lee_Conf_H

#include "stdio.h"
#include "main.h"


//My  Define

/*     LED  Define  */
#define LED_ON()       HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET)
#define LED_OFF()      HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET)

#define LED_Tog()      HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin)        


#define BEEP_ON() 			HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET)
#define BEEP_OFF() 			HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET)


#endif  /* Lee_Conf_H   */
