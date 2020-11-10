/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "ctype.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include"queue.h"
#include"display_thread.h"
#include"read_thread.h"
#include"app_thread.h"
#include"timer_threads.h"
#include"leds_control.h"
#include"serial_thread.h"
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
ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;
UART_HandleTypeDef huart7;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* Definitions for defaultTask */
osThreadId_t serialTaskHandle;
const osThreadAttr_t serialTask_attributes = { .name = "serialTask", .priority =
		(osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };
/* Definitions for task_read */
osThreadId_t task_readHandle;
const osThreadAttr_t task_read_attributes = { .name = "task_read", .priority =
		(osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };
/* Definitions for task_app */
osThreadId_t task_appHandle;
const osThreadAttr_t task_app_attributes = { .name = "task_app", .priority =
		(osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };
/* Definitions for task_display */
osThreadId_t task_displayHandle;
const osThreadAttr_t task_display_attributes = { .name = "task_display",
		.priority = (osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };
/* Definitions for task_timer */
osThreadId_t task_timerHandle;
const osThreadAttr_t task_timer_attributes = { .name = "task_timer", .priority =
		(osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };
/* Definitions for app_queue */
osMessageQueueId_t app_queueHandle;
const osMessageQueueAttr_t app_queue_attributes = { .name = "app_queue" };
/* Definitions for display_queue */
osMessageQueueId_t display_queueHandle;
const osMessageQueueAttr_t display_queue_attributes =
		{ .name = "display_queue" };
/* Definitions for send */
osMessageQueueId_t sendToSerialHandle;
const osMessageQueueAttr_t sendToSerial_attributes = { .name = "sendToSerial" };
/* Definitions for read */
osMessageQueueId_t readFromSerialHandle;
const osMessageQueueAttr_t readFromSerial_attributes = { .name =
		"readFromSerial" };
/* Definitions for start_signal */
SemaphoreHandle_t start_signalHandle;

/* Definitions for reset_signal */
SemaphoreHandle_t reset_signalHandle;

/* Definitions for game_end_anim */
SemaphoreHandle_t game_end_animHandle;

/* Definitions for valid_anim */
SemaphoreHandle_t valid_animHandle;

/* USER CODE BEGIN PV */
osMutexId_t timer_mutexHandle;
const osMutexAttr_t timer_mutex_attributes = { .name = "timer_mutex" };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_UART7_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_NVIC_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ETH_Init();
	MX_USART3_UART_Init();
	MX_UART7_Init();
	MX_USB_OTG_FS_PCD_Init();
	/* USER CODE BEGIN 2 */
	MX_NVIC_Init();
	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* Create the semaphores(s) */
	/* creation of start_signal */
	start_signalHandle = xSemaphoreCreateBinary();

	/* creation of reset_signal */
	reset_signalHandle = xSemaphoreCreateBinary();

	/* creation of game_end_anim */
	game_end_animHandle = xSemaphoreCreateBinary();

	/* creation of valid_anim */
	valid_animHandle = xSemaphoreCreateBinary();

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the queue(s) */
	/* creation of app_queue */
	app_queueHandle = osMessageQueueNew(16, sizeof(Msg_t),
			&app_queue_attributes);

	/* creation of display_queue */
	display_queueHandle = osMessageQueueNew(16, sizeof(Msg_t),
			&display_queue_attributes);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	sendToSerialHandle = osMessageQueueNew(16, SIZE_OF_LED_COMMAND_BUFFER,
			&sendToSerial_attributes);

	/* creation of display_queue */
	readFromSerialHandle = osMessageQueueNew(16, SIZE_OF_PLAYER_COMMAND_BUFFER,
			&readFromSerial_attributes);
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation de serialTask */
	serialTaskHandle = osThreadNew((void*) sendToSerialTask, NULL,
			&serialTask_attributes);

	/* creation of task_read */
	task_readHandle = osThreadNew((void*) thread_handler_input, NULL,
			&task_read_attributes);

	/* creation of task_app */
	task_appHandle = osThreadNew((void*) thread_handler_app, NULL,
			&task_app_attributes);

	/* creation of task_display */
	task_displayHandle = osThreadNew((void*) thread_handler_display, NULL,
			&task_display_attributes);

	/* creation of task_timer */
	task_timerHandle = osThreadNew((void*) timer_handler, NULL,
			&task_timer_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */
		osDelay(10000);
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE()
	;
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ETH Initialization Function
 * @param None
 * @retval None
 */
static void MX_ETH_Init(void) {

	/* USER CODE BEGIN ETH_Init 0 */

	/* USER CODE END ETH_Init 0 */

	/* USER CODE BEGIN ETH_Init 1 */

	/* USER CODE END ETH_Init 1 */
	heth.Instance = ETH;
	heth.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
	heth.Init.PhyAddress = LAN8742A_PHY_ADDRESS;
	heth.Init.MACAddr[0] = 0x00;
	heth.Init.MACAddr[1] = 0x80;
	heth.Init.MACAddr[2] = 0xE1;
	heth.Init.MACAddr[3] = 0x00;
	heth.Init.MACAddr[4] = 0x00;
	heth.Init.MACAddr[5] = 0x00;
	heth.Init.RxMode = ETH_RXPOLLING_MODE;
	heth.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
	heth.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;

	/* USER CODE BEGIN MACADDRESS */

	/* USER CODE END MACADDRESS */

	if (HAL_ETH_Init(&heth) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ETH_Init 2 */

	/* USER CODE END ETH_Init 2 */

}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void) {

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

/**
 * @brief USB_OTG_FS Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_OTG_FS_PCD_Init(void) {

	/* USER CODE BEGIN USB_OTG_FS_Init 0 */

	/* USER CODE END USB_OTG_FS_Init 0 */

	/* USER CODE BEGIN USB_OTG_FS_Init 1 */

	/* USER CODE END USB_OTG_FS_Init 1 */
	hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
	hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
	hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
	hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
	hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
	hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
	hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
	if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USB_OTG_FS_Init 2 */

	/* USER CODE END USB_OTG_FS_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;
	__HAL_RCC_GPIOE_CLK_ENABLE()
	;
	__HAL_RCC_GPIOG_CLK_ENABLE()
	;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, LD1_Pin | LD3_Pin | LD2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin : USER_Btn_Pin */
	GPIO_InitStruct.Pin = USER_Btn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
	GPIO_InitStruct.Pin = LD1_Pin | LD3_Pin | LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : USB_PowerSwitchOn_Pin */
	GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : USB_OverCurrent_Pin */
	GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

static void MX_NVIC_Init(void) {
	/* EXTI15_10_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
static int last_message_tick = 0;
static uint8_t trame_type = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	int message_tick = HAL_GetTick();
	if (message_tick - last_message_tick <= 1000){
		return;
	}
	last_message_tick = message_tick;
	char message[60] = "Mooooordu ! Mordu ! Mordu ! Mordu !\0";
	char message_buffer[60];
	char buffer[62];
	int message_size = 0;
	if (trame_type == 0) {
		int k = 0;
		while (message[k] != '\0') {
			message_buffer[k] = message[k];
			k += 1;
			message_size +=1;
		}
		message_buffer[k] = '\0';
		trame_type += 1;
	} else if (trame_type == 1) {
		int i = 0;
		while (message[i] != '\0') {
			if (message[i] >= 97 && message[i] <= 122) {
				message_buffer[i] = message[i] - 32;
			} else {
				message_buffer[i] = message[i];
			}
			i += 1;
			message_size +=1;
		}
		message_buffer[i] = '\0';
		trame_type += 1;
	} else if (trame_type == 2) {
		int j = 0;
		while (message[j] != '\0') {
			if (message[j] >= 65 && message[j] <= 90) {
				message_buffer[j] = message[j] + 32;
			} else {
				message_buffer[j] = message[j];
			}
			j += 1;
			message_size +=1;
		}
		message_buffer[j] = '\0';
		trame_type +=1 ;
	} else if (trame_type == 3) {
		int l = 0;
		int m = 0;
		while (message[m] != '\0') {
			m +=1;
		}
		m -= 1;
		while (m >= 0) {
			message_buffer[l] = message[m];
			l += 1;
			m -= 1;
			message_size +=1;
		}
		message_buffer[l] = '\0';
		trame_type = 0;
	}
	char beg = 'd';
	char end = '\n';
	int i = 1;
	buffer[0] = beg;
	while (message_buffer[i - 1] != '\0') {
		buffer[i] = message_buffer[i - 1];
		i += 1;
	}
	buffer[i] = end;
	if (GPIO_Pin == USER_Btn_Pin) {
		HAL_UART_Transmit(&huart7, (uint8_t*)buffer, message_size + 2, 100);
	}
}

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM1) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief UART7 Initialization Function^M
 * @param None^M
 * @retval None^M
 */
static void MX_UART7_Init(void) {

	/* USER CODE BEGIN UART7_Init 0 */

	/* USER CODE END UART7_Init 0 */

	/* USER CODE BEGIN UART7_Init 1 */

	/* USER CODE END UART7_Init 1 */
	huart7.Instance = UART7;
	huart7.Init.BaudRate = 115200;
	huart7.Init.WordLength = UART_WORDLENGTH_8B;
	huart7.Init.StopBits = UART_STOPBITS_1;
	huart7.Init.Parity = UART_PARITY_NONE;
	huart7.Init.Mode = UART_MODE_TX_RX;
	huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart7.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart7) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN UART7_Init 2 */

	/* USER CODE END UART7_Init 2 */

}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
