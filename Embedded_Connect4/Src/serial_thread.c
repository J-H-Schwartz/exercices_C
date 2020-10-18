/*
 * send_to_serial.c
 *
 * Created on: 23 avr. 2020
 *      Author: gfr
 */

#include <serial_thread.h>
#include "main.h"
#include "cmsis_os.h"
#include "leds_control.h"
#include "game_p4.h"

extern osMessageQueueId_t sendToSerialHandle;
extern osMessageQueueId_t readFromSerialHandle;

extern UART_HandleTypeDef huart3;

void sendToSerial(char* msg) {
	osMessageQueuePut(sendToSerialHandle, msg, 10U, osWaitForever);
}

void readFromSerial(char* msg){
	osMessageQueueGet(readFromSerialHandle, msg, NULL, osWaitForever);

}

void* sendToSerialTask(void *argument) {
	for(;;){
		unsigned char msgRead[5];
		unsigned char msgSend[10];
		if (HAL_UART_Receive(&huart3, msgRead, SIZE_OF_PLAYER_COMMAND_BUFFER, 10) == HAL_OK){
			osMessageQueuePut(readFromSerialHandle, msgRead, 10U, 100);
		}
		if (osMessageQueueGetCount(sendToSerialHandle) > 0){
			osMessageQueueGet(sendToSerialHandle, msgSend, NULL, 10);
			HAL_UART_Transmit(&huart3, msgSend, SIZE_OF_LED_COMMAND_BUFFER, 100);
		}
	}

}

