/*
 * send_to_serial.c
 *
 * Created on: 23 avr. 2020
 *      Author: gfr
 */

#include "send_to_serial.h"

#include "main.h"
#include "cmsis_os.h"
#include "usbd_cdc_if.h"
#include "leds_control.h"

extern osMessageQueueId_t sendToSerialHandle;
extern osSemaphoreId_t sendToSerialDoneHandle;

void sendToSerial(const char * const msg, const size_t size) {
	MsgToSerial toSerial = EMPTY_MSG_TO_SERIAL;

	toSerial.type = MTST_SEND_MESSAGE;

	const char * msgToSend = msg;
	size_t remainsToSend = size;
	do {
		size_t toSend =
				remainsToSend > MTST_SEND_MESSAGE_MAX_SIZE ?
						MTST_SEND_MESSAGE_MAX_SIZE : remainsToSend;

		toSerial.params.sendMessage.size = toSend;
		memcpy(toSerial.params.sendMessage.msg, msgToSend, toSend);
		osMessageQueuePut(sendToSerialHandle, &toSerial, 1, osWaitForever);

		remainsToSend -= toSend;
		msgToSend += toSend;
	} while (remainsToSend > 0);
}

void SendToSerialTask(void *argument) {
	openLink();
	osDelay(1000);
	for (;;) {
		MsgToSerial msg;
		osMessageQueueGet(sendToSerialHandle, &(msg), NULL, osWaitForever);
		if (CDC_Transmit_FS((uint8_t *) msg.params.sendMessage.msg,
				msg.params.sendMessage.size) != USBD_BUSY) {
			osSemaphoreAcquire(sendToSerialDoneHandle, osWaitForever);
		}
	}
}
