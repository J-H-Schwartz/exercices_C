/*
 * read_thread.h
 *
 *  Created on: Apr 17, 2020
 *      Author: jonathan
 */

#ifndef READ_THREAD_H_
#define READ_THREAD_H_

#define READ_BUTTON_BUFFER_SIZE 4

//Players for pads attribution.
typedef enum {
	NO_PAD, PAD_1, PAD_2
} player_t;

//Game pads controls
typedef enum {
	INPUT_PAD_1 = 49,
	INPUT_PAD_2 = 50,
	DOWN_PAD = 100,
	LEFT_PAD = 108,
	RIGHT_PAD = 114,
	UP_PAD = 117
} Pads_controls;

int registerCallback(void (*my_func)(void*));
void thread_handler_input(void* args);

#endif /* READ_THREAD_H_ */
