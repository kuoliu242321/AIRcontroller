/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "24l01.h"
#include "stdio.h"
#include "radiolink.h"
#include "remoter_ctrl.h"
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for MainTask */
osThreadId_t MainTaskHandle;
const osThreadAttr_t MainTask_attributes = {
  .name = "MainTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh7,
};
/* Definitions for Radiolink */
osThreadId_t RadiolinkHandle;
const osThreadAttr_t Radiolink_attributes = {
  .name = "Radiolink",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal1,
};
/* Definitions for Commandersend */
osThreadId_t CommandersendHandle;
const osThreadAttr_t Commandersend_attributes = {
  .name = "Commandersend",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for txQueue */
osMessageQueueId_t txQueueHandle;
const osMessageQueueAttr_t txQueue_attributes = {
  .name = "txQueue"
};
/* Definitions for nrfIT */
osSemaphoreId_t nrfITHandle;
const osSemaphoreAttr_t nrfIT_attributes = {
  .name = "nrfIT"
};
/* Definitions for REM_StatusFlag */
osEventFlagsId_t REM_StatusFlagHandle;
const osEventFlagsAttr_t REM_StatusFlag_attributes = {
  .name = "REM_StatusFlag"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void RadioLink(void *argument);
void CommanderSend(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of nrfIT */
  nrfITHandle = osSemaphoreNew(1, 1, &nrfIT_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of txQueue */
  txQueueHandle = osMessageQueueNew (10, 12, &txQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of MainTask */
  MainTaskHandle = osThreadNew(StartDefaultTask, NULL, &MainTask_attributes);

  /* creation of Radiolink */
  RadiolinkHandle = osThreadNew(RadioLink, NULL, &Radiolink_attributes);

  /* creation of Commandersend */
  CommandersendHandle = osThreadNew(CommanderSend, NULL, &Commandersend_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of REM_StatusFlag */
  REM_StatusFlagHandle = osEventFlagsNew(&REM_StatusFlag_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	Management_Prepare();
  /* Infinite loop */
  for(;;)
  {
	  LED3_Tog();
		LED2_Tog();
		LED1_Tog();
		//Task_Management();
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_RadioLink */
/**
* @brief Function implementing the Radiolink thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RadioLink */
void RadioLink(void *argument)
{
  /* USER CODE BEGIN RadioLink */
  /* Infinite loop */
  for(;;)
  {
    radiolinkTask();
		osDelay(10);
  }
  /* USER CODE END RadioLink */
}

/* USER CODE BEGIN Header_CommanderSend */
/**
* @brief Function implementing the Commandersend thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CommanderSend */
void CommanderSend(void *argument)
{
  /* USER CODE BEGIN CommanderSend */
  /* Infinite loop */
  for(;;)
  {
		//队列阻塞创建管理函数同时运行函数
	
		commanderTask();
    osDelay(10);
  }
  /* USER CODE END CommanderSend */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
