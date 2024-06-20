/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED3_Pin GPIO_PIN_13
#define LED3_GPIO_Port GPIOC
#define NRF24_CE_Pin GPIO_PIN_0
#define NRF24_CE_GPIO_Port GPIOB
#define NRF24_CSN_Pin GPIO_PIN_1
#define NRF24_CSN_GPIO_Port GPIOB
#define NRF_IRQ_Pin GPIO_PIN_2
#define NRF_IRQ_GPIO_Port GPIOB
#define NRF_IRQ_EXTI_IRQn EXTI2_3_IRQn
#define LED1_Pin GPIO_PIN_3
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_5
#define KEY1_GPIO_Port GPIOB
#define KEY1_EXTI_IRQn EXTI4_15_IRQn
#define KEY2_Pin GPIO_PIN_6
#define KEY2_GPIO_Port GPIOB
#define KEY2_EXTI_IRQn EXTI4_15_IRQn
#define KEY3_Pin GPIO_PIN_7
#define KEY3_GPIO_Port GPIOB
#define KEY3_EXTI_IRQn EXTI4_15_IRQn
#define Joystick_L_Pin GPIO_PIN_8
#define Joystick_L_GPIO_Port GPIOB
#define Joystick_L_EXTI_IRQn EXTI4_15_IRQn
#define Joystick_R_Pin GPIO_PIN_9
#define Joystick_R_GPIO_Port GPIOB
#define Joystick_R_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
