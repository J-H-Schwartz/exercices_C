/*
 * leds.c
 *
 *  Created on: 21 avr. 2020
 *      Author: gfr
 */
#include <local_leds.h>
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal_gpio.h"


void LedGreenOn(void) {
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
}
void LedGreenOff(void) {
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
}

void LedBlueOn(void) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

void LedBlueOff(void) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void LedRedOn(void) {
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

void LedRedOff(void) {
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}
