#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

#include "local_button.h"
#include "local_leds.h"
#include "leds_control.h"
#include "send_to_serial.h"
#include "display_thread.h"
#include "read_thread.h"
#include "app_thread.h"

///////////////////////////////////////////////////////////////////////////////
///////////////// DECLARAION DES THREADS

//static void DefaultTask(void *argument);

osThreadId_t DisplayTaskHandle;
const osThreadAttr_t DisplayTask_attributes = { .name = "DisplayTask",
		.priority = (osPriority_t) osPriorityNormal, .stack_size = 1024 * 4 };

osThreadId_t sendToSerialTaskHandle;
const osThreadAttr_t sendToSerialTask_attributes = { .name = "sendToSerialTask",
		.priority = (osPriority_t) osPriorityRealtime7, .stack_size = 256 * 4 };



osThreadId_t ButtonReadTaskHandle;
const osThreadAttr_t ButtonReadTask_attributes = { .name = "ButtonReadTask",
		.priority = (osPriority_t) osPriorityRealtime5, .stack_size = 128 * 4 };


osThreadId_t AppTaskHandle;
const osThreadAttr_t AppTask_attributes = { .name = "AppTask",
		.priority = (osPriority_t) osPriorityNormal2, .stack_size = 512 * 4 };


//static void SendToSerialLogs(void *argument);
//
//osThreadId_t SendToSerialLogsHandle;
//const osThreadAttr_t SendToSerialLogs_attributes = { .name = "SendToSerialLogs",
//		.priority = (osPriority_t) osPriorityRealtime6, .stack_size = 128 * 4 };

///////////////////////////////////////////////////////////////////////////////
///////////////// DECLARATION DES QUEUES
osMessageQueueId_t receiveFromSerialHandle;
const osMessageQueueAttr_t receiveFromSerial_attributes = { .name =
		"receiveFromSerial" };

osMessageQueueId_t sendToSerialHandle;
const osMessageQueueAttr_t sendToSerial_attributes = { .name = "sendToSerial" };

osMessageQueueId_t sendLogsHandle;
const osMessageQueueAttr_t sendLogs_attributes = { .name = "sendLogs" };

///////////////////////////////////////////////////////////////////////////////
///////////////// DECLARATION DES SEMAPHORES
osSemaphoreId_t sendToSerialDoneHandle;
const osSemaphoreAttr_t sendToSerialDone_attributes = { .name =
		"sendToSerialDone" };

osSemaphoreId_t localButtonReleasedQueueHandle;
const osSemaphoreAttr_t localButtonReleasedQueue_attributes = { .name =
		"localButtonReleasedQueue" };

osSemaphoreId_t ButtonReadDoneTaskHandle;
const osSemaphoreAttr_t ButtonReadDoneTask_attributes = { .name =
		"ButtonReadDoneTask" };

osSemaphoreId_t SerialTaskHandle;
const osSemaphoreAttr_t SerialTask_attributes = { .name = "SerialTask" };

///////////////////////////////////////////////////////////////////////////////
///////////////// DECLARATION DES FONCTIONS DE GESTION DU MATERIEL
void SystemClock_Config(void);
static void EXTI15_10_IRQHandler_Config(void);
static void MX_GPIO_Init(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//static void SendToSerialLogs(void* argument) {
//	char message[50];
//	for (;;) {
//		osMessageQueueGet(sendLogsHandle, message, NULL, osWaitForever);
//		sendToSerial(message, 50);
//	}
//}

//static void ButtonReadTask(void *argument) {
////	char message[50] = "Led bleue active !";
//	for (;;) {
//		WaitForLocalButtonReleased();
//		LedBlueOn();
////		osMessageQueuePut(sendLogsHandle, message, 3, 3000);
//		osDelay(400);
//		LedBlueOff();
//	}
//}

//static void ButtonReadTask2(void *argument) {
//	char message[4];
//	char button_read[5];
//	for (;;) {
//		if (readbutton(button_read, 5) == LCRC_OK) {
//			if (button_read[3] == 'u' && button_read[2] == 'l') {
//				setLedColor(1, 1, 255, 0, 0);
//				osDelay(400);
//				setLedColor(1, 1, 0, 0, 0);
//			} else if (button_read[3] == 'u' && button_read[2] == 'r') {
//				setLedColor(1, 1, 0, 255, 0);
//				osDelay(400);
//				setLedColor(1, 1, 0, 0, 0);
//			}
//			osDelay(100);/* else {
//			 setLedColor(1,1, 0, 0, 255);
//			 osDelay(400);
//			 setLedColor(1,1, 0, 0, 0);
//			 }*/
////		osMessageQueuePut(sendLogsHandle, message, 1, 3000);
//		}
//	}
//}

void receive_from_serial(char *msg_ptr){
	MsgToSerial to_receive = EMPTY_MSG_TO_SERIAL;
	osMessageQueueGet(receiveFromSerialHandle, (void*)&to_receive, NULL, osWaitForever);
	memcpy(to_receive.params.sendMessage.msg, msg_ptr, to_receive.params.sendMessage.size);
}
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
//static void DefaultTask(void *argument) {
//	/* init code for USB_DEVICE */
//	/* USER CODE BEGIN 5 */
//	/* Infinite loop */
////	char message[50] = "Led rouge active !";
//	for (;;) {
////		LedRedOn();
//////		osMessageQueuePut(sendLogsHandle, &message, 1, osWaitForever);
////		osDelay(400);
////		LedRedOff();
//		osDelay(400);
//	}
//	/* USER CODE END 5 */
//}
int main(void) {
// Initialisation du matériel NE RIEN METTRE AVANT
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	EXTI15_10_IRQHandler_Config();

// Initialisation du micro noyau
	osKernelInitialize();

// Initialisation des IPC internes
	SerialTaskHandle = osSemaphoreNew(1, 0, &SerialTask_attributes);
	sendToSerialDoneHandle = osSemaphoreNew(1, 0, &sendToSerialDone_attributes);
	localButtonReleasedQueueHandle = osSemaphoreNew(1, 0,
			&localButtonReleasedQueue_attributes);
	receiveFromSerialHandle = osMessageQueueNew(16, sizeof(uint8_t),
			&receiveFromSerial_attributes);
	sendToSerialHandle = osMessageQueueNew(16, sizeof(MsgToSerial),
			&sendToSerial_attributes);

	sendLogsHandle = osMessageQueueNew(16, 50, &sendLogs_attributes);

// Initialisation des taches internes
	sendToSerialTaskHandle = osThreadNew(SendToSerialTask, NULL,
			&sendToSerialTask_attributes);

	/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
	ButtonReadDoneTaskHandle = osSemaphoreNew(3, 0,
			&ButtonReadDoneTask_attributes);
	//SerialTaskHandle = osSemaphoreNew(1, 1, &SerialTask_attributes);
	ButtonReadTaskHandle = osThreadNew(thread_handler_input, NULL,
			&ButtonReadTask_attributes);
	AppTaskHandle = osThreadNew(thread_handler_app, NULL,
			&AppTask_attributes);

	DisplayTaskHandle = osThreadNew(thread_handler_display, NULL, &DisplayTask_attributes);

	//SendToSerialLogsHandle = osThreadNew(SendToSerialLogs, NULL,
	//		&SendToSerialLogs_attributes);
	/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
	/* C'est entre c'est 2 double lignes de % que vous pouvez démarrer vos   */
	/* tâches et mettre votre code perso                                     */


	/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
	/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

// Démarrage du micro noyau. Cette fonction est en quelque sorte le micro noyau
// on ne sortira donc normalement JAMAIS de cette fonction.
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	while (1) {
	}
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

	/*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
	GPIO_InitStruct.Pin = RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin RMII_CRS_DV_Pin */
	GPIO_InitStruct.Pin =
	RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
	GPIO_InitStruct.Pin = LD1_Pin | LD3_Pin | LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : RMII_TXD1_Pin */
	GPIO_InitStruct.Pin = RMII_TXD1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(RMII_TXD1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : STLK_RX_Pin STLK_TX_Pin */
	GPIO_InitStruct.Pin = STLK_RX_Pin | STLK_TX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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

	/*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD0_Pin */
	GPIO_InitStruct.Pin = RMII_TX_EN_Pin | RMII_TXD0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

static void EXTI15_10_IRQHandler_Config(void) {
	/* Enable and set EXTI lines 10 to 15 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

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
