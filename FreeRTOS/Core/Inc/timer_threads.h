/*
 * timer_threads.h
 *
 *  Created on: Apr 21, 2020
 *      Author: jonathan
 */

#ifndef TIMER_THREADS_H_
#define TIMER_THREADS_H_

typedef void (*callback_t)(void*);

typedef enum {
	RUN_TIMER, STOP_TIMER
} timer_signal_t;

typedef enum {
	TIMER_OFF, TIMER_ON
} timer_status_t;

typedef enum {
	FORCE_OFF, FORCE_ON
} force_timer_status;
typedef struct Timer_Struct{
	osThreadId_t thread_id;
	int timer_set;
	uint8_t timer_status;
	uint8_t timer_signal;
	uint8_t timer_force;
	callback_t Timer_Callback;
	void* callback_param;
}Timer_Struct;

void timer_init(Timer_Struct* timer, int timer_setting, void* callback_param);

void start_timer(Timer_Struct* timer);

void stop_timer(Timer_Struct* timer);

#endif /* TIMER_THREADS_H_ */
