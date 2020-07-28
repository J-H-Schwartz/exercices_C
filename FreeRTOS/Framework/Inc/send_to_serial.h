/*
 * send_to_serial.h
 *
 *  Created on: 23 avr. 2020
 *      Author: gfr
 */

#ifndef INC_SEND_TO_SERIAL_H_
#define INC_SEND_TO_SERIAL_H_

#include <stddef.h>

#define MTST_SEND_MESSAGE_MAX_SIZE 50
#define EMPTY_MSG_TO_SERIAL { .type = MTST_NONE }

typedef enum {
	MTST_NONE,
	MTST_SEND_MESSAGE
} MsgToSerialType;


typedef struct {
	MsgToSerialType type;
	union {
		struct {
			char msg[MTST_SEND_MESSAGE_MAX_SIZE];
			size_t size;
		} sendMessage;
	} params;
} MsgToSerial;

void sendToSerial(const char * const msg, const size_t size);

void SendToSerialTask(void *argument);

#endif /* INC_SEND_TO_SERIAL_H_ */
