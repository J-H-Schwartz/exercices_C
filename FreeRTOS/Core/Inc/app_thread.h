/*
 * app_thread.h
 *
 *  Created on: Apr 17, 2020
 *      Author: jonathan
 */

#ifndef APP_THREAD_H_
#define APP_THREAD_H_

#define SHORT_TIMER_SETTING 50 // * 0,1 second.
#define LONG_TIMER_SETTING 150 // * 0,1 second.

typedef enum {
	PADS_ACTIVATED, PADS_DEACTIVATED
} pads_activation_status;

typedef enum {
	NO_PLAYER_PAD, PLAYER_PAD_1, PLAYER_PAD_2
}player_pad_attrib_t;

void TimeOutAutoplay(void*);
void SendInputToApp(void*);
void thread_handler_app(void* args);

#endif /* APP_THREAD_H_ */
