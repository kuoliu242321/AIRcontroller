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
#include "link2remote.h"
#include "dataconfiguration.h"
#include "aircraft_manager.h"
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
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh2,
};
/* Definitions for Link2RemTask */
osThreadId_t Link2RemTaskHandle;
const osThreadAttr_t Link2RemTask_attributes = {
  .name = "Link2RemTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for ControlTask */
osThreadId_t ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes = {
  .name = "ControlTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for rxQueue */
osMessageQueueId_t rxQueueHandle;
const osMessageQueueAttr_t rxQueue_attributes = {
  .name = "rxQueue"
};
/* Definitions for NrfIT_Binary */
osSemaphoreId_t NrfIT_BinaryHandle;
const osSemaphoreAttr_t NrfIT_Binary_attributes = {
  .name = "NrfIT_Binary"
};
/* Definitions for CanFlyEvent */
osEventFlagsId_t CanFlyEventHandle;
const osEventFlagsAttr_t CanFlyEvent_attributes = {
  .name = "CanFlyEvent"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void linkTask(void *argument);
void controlTask(void *argument);

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
  /* creation of NrfIT_Binary */
  NrfIT_BinaryHandle = osSemaphoreNew(1, 1, &NrfIT_Binary_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of rxQueue */
  rxQueueHandle = osMessageQueueNew (10, 12, &rxQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of MainTask */
  MainTaskHandle = osThreadNew(StartDefaultTask, NULL, &MainTask_attributes);

  /* creation of Link2RemTask */
  Link2RemTaskHandle = osThreadNew(linkTask, NULL, &Link2RemTask_attributes);

  /* creation of ControlTask */
  ControlTaskHandle = osThreadNew(controlTask, NULL, &ControlTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of CanFlyEvent */
  CanFlyEventHandle = osEventFlagsNew(&CanFlyEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the MainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		Aircraft_Task_Manage();
		LED_Tog();
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_linkTask */
/**
* @brief Function implementing the Link2RemTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_linkTask */
void linkTask(void *argument)
{
  /* USER CODE BEGIN linkTask */
	Link2Remote_Init();
  /* Infinite loop */
  for(;;)
  {
		//printf("link Task running\r\n");
		radiolinkTask();
    osDelay(10);
  }
  /* USER CODE END linkTask */
}

/* USER CODE BEGIN Header_controlTask */
/**
* @brief Function implementing the ControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_controlTask */
void controlTask(void *argument)
{
  /* USER CODE BEGIN controlTask */
  /* Infinite loop */
  for(;;)
  {
		//printf("control Task running\r\n");
		AirCraft_Control_Task();
    osDelay(5);
  }
  /* USER CODE END controlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
