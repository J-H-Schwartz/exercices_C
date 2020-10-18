/*
 * timer_threads.h
 *
 *  Created on: Apr 21, 2020
 *      Author: jonathan
 */

#ifndef TIMER_THREADS_H_
#define TIMER_THREADS_H_

#define DEFAULT_TIMER_ID 10

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

typedef struct {
	uint8_t timer_id;
	int timer_count;
	int timer_max_count;
	int timer_delay;
	uint8_t timer_status;
	uint8_t timer_signal;
	uint8_t timer_force;
	callback_t Timer_Callback;
	void* callback_param;
} timer_struct_t;

uint8_t start_new_timer(timer_struct_t* timer);

void stop_new_timer(timer_struct_t* timer);

void new_timer_init(timer_struct_t* timer, int timer_setting, void* callback_param);

void* timer_handler(void* thread_id);


#endif /* TIMER_THREADS_H_ */
