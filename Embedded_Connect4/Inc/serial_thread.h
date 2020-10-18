/*
 * send_to_serial.h
 *
 *  Created on: 23 avr. 2020
 *      Author: gfr
 */

#ifndef INC_SERIAL_THREAD_H_
#define INC_SERIAL_THREAD_H_

#include <stddef.h>

#define MTST_SEND_MESSAGE_MAX_SIZE 50
#define EMPTY_MSG_TO_SERIAL { .type = MTST_NONE }



void sendToSerial(char * msg);

void readFromSerial(char * msg);

void* sendToSerialTask(void *argument);

#endif /* INC_SERIAL_THREAD_H_ */
