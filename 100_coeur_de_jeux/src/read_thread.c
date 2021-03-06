/*
 * read_thread.c
 *
 *  Created on: Apr 17, 2020
 *      Author: jonathan
 */

#include<stdio.h>
#include<stdint.h>
#include<unistd.h>
#include<pthread.h>

#include"debug.h"
#include"leds_control.h"

#include"queue.h"
#include"read_thread.h"

typedef void (*callback_t)(void*);

static callback_t SendInput_Callback = NULL;

int registerCallback(callback_t c) {
	SendInput_Callback = c;
	if (SendInput_Callback == NULL) return - 1;
	else return 0;
}

//Input reading and decoding thread handler.
void *thread_handler_input(void* args) {
	char button_read[READ_BUTTON_BUFFER_SIZE];
	uint8_t error = 0;
	uint8_t command[2];
	while (1) {
		//Get input.
		error = readbutton(button_read, (READ_BUTTON_BUFFER_SIZE + 1));
		if (error != 0)
			debug_printf(2, "Button error : %d\n", error);
		//Debug input print.
		if (button_read[3] == DOWN_PAD)
			debug_printf(2, "Button press read: %d %d %d %d\n", button_read[0],
					button_read[1], button_read[2], button_read[3]);
		//Process input.
		if (button_read[3] == DOWN_PAD && button_read[2] == UP_PAD) {
			command[0] = UP_COMMAND;
			if (button_read[1] == INPUT_PAD_1) {
				command[1] = PAD_1;
			} else if (button_read[1] == INPUT_PAD_2) {
				command[1] = PAD_2;
			}
		} else if (button_read[3] == DOWN_PAD && button_read[2] == DOWN_PAD) {
			command[0] = DOWN_COMMAND;
			if (button_read[1] == INPUT_PAD_1) {
				command[1] = PAD_1;
			} else if (button_read[1] == INPUT_PAD_2) {
				command[1] = PAD_2;
			}
		} else if (button_read[3] == DOWN_PAD && button_read[2] == LEFT_PAD) {
			command[0] = LEFT_COMMAND;
			if (button_read[1] == INPUT_PAD_1) {
				command[1] = PAD_1;
			} else if (button_read[1] == INPUT_PAD_2) {
				command[1] = PAD_2;
			}
		} else if (button_read[3] == DOWN_PAD && button_read[2] == RIGHT_PAD) {
			command[0] = RIGHT_COMMAND;
			if (button_read[1] == INPUT_PAD_1) {
				command[1] = PAD_1;
			} else if (button_read[1] == INPUT_PAD_2) {
				command[1] = PAD_2;
			}
		} else {
			usleep(100000);
			continue;
		}
		//Send processed command to read queue.
		if (SendInput_Callback != NULL) {
			SendInput_Callback(command);
		}
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return 0;
}
