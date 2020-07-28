/*
 * local_button.c
 *
 *  Created on: 21 avr. 2020
 *      Author: gfr
 */
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal_gpio.h"

extern osSemaphoreId_t localButtonReleasedQueueHandle;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)  {
	osSemaphoreRelease(localButtonReleasedQueueHandle);
}

void WaitForLocalButtonReleased(void) {
	osSemaphoreAcquire(localButtonReleasedQueueHandle, osWaitForever);
}
