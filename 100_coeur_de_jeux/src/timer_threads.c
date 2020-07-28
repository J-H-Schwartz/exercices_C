/*
 * timer_threads.c
 *
 *  Created on: Apr 21, 2020
 *      Author: jonathan
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

#include"timer_threads.h"
#include"debug.h"

static timer_struct_t* timer_table[10];
static int8_t active_timer_count = 0;

void* timer_handler(void* thread_id) {
	for (uint8_t i = 0; i < 10; i++) {
		timer_table[i] = NULL;
	}
	while (1) {
		for (int timer_id = 0; timer_id < 10; timer_id++) {
			if (timer_table[timer_id] != NULL) {
				if (timer_table[timer_id]->timer_status == TIMER_ON
						&& timer_table[timer_id]->timer_count != 0) {
					timer_table[timer_id]->timer_count -= 1;
				} else if (timer_table[timer_id]->timer_status == TIMER_ON
						&& timer_table[timer_id]->timer_count == 0) {
					timer_table[timer_id]->Timer_Callback(
							timer_table[timer_id]->callback_param);
					if (timer_table[timer_id]->timer_force == FORCE_ON) {
						timer_table[timer_id]->timer_count =
								timer_table[timer_id]->timer_max_count;
					} else {
						timer_table[timer_id]->timer_status = TIMER_OFF;
						active_timer_count -= 1;
					}
				}
				if (timer_table[timer_id]->timer_status == TIMER_ON
						&& timer_table[timer_id]->timer_signal == STOP_TIMER
						&& timer_table[timer_id]->timer_force == FORCE_OFF) {
					timer_table[timer_id]->timer_status = TIMER_OFF;
				}
			}
		}
		usleep(1000);
	}
	return 0;
}

void new_timer_init(timer_struct_t* timer, int timer_setting,
		void* callback_param) {
	timer->timer_id = DEFAULT_TIMER_ID;
	timer->timer_count = timer_setting;
	timer->timer_max_count = timer_setting;
	timer->timer_delay = timer->timer_status = TIMER_OFF;
	timer->timer_signal = STOP_TIMER;
	timer->timer_force = FORCE_OFF;
	timer->callback_param = callback_param;
}

uint8_t start_new_timer(timer_struct_t* timer) {
	if (timer->timer_status == TIMER_ON) {
		return -1;
	}
	for (int timer_id = 0; timer_id < 10; timer_id++) {
		if (timer_table[timer_id] != NULL) {
			timer_table[timer_id] = &(*timer);
			timer->timer_id = timer_id;
			timer->timer_signal = RUN_TIMER;
			timer->timer_status = TIMER_ON;
			return timer_id;
		}
	}
	return -1;
}

void stop_new_timer(int8_t timer_id) {
	if (timer_table[timer_id] == NULL) {
		return;
	}
	timer_table[timer_id]->timer_signal = STOP_TIMER;
	timer_table[timer_id]->timer_status = TIMER_OFF;
}

static void* thread_timer(void* timer_ptr) {
	Timer_Struct* timer = (Timer_Struct*) timer_ptr;
	int time_out = 0;
	timer->timer_signal = RUN_TIMER;
	while (timer->timer_signal != STOP_TIMER) {
		if (time_out == timer->timer_set) {
			if (timer->Timer_Callback != NULL) {
				timer->Timer_Callback(timer->callback_param);
			}
			if (timer->timer_force == FORCE_ON) {
				time_out = 0;
				continue;
			}
			break;
		} else {
			usleep(100000);
			time_out += 1;
		}
	}
	timer->timer_signal = STOP_TIMER;
	timer->timer_status = TIMER_OFF;
	pthread_exit(EXIT_SUCCESS);
}

void timer_init(Timer_Struct* timer, int timer_setting, void* callback_param) {
	timer->timer_set = timer_setting;
	timer->timer_status = TIMER_OFF;
	timer->timer_signal = STOP_TIMER;
	timer->timer_force = FORCE_OFF;
	timer->callback_param = callback_param;
}

void start_timer(Timer_Struct* timer) {
	uint8_t error = 0;
	if (timer->timer_status == TIMER_ON)
		return;
	if ((error = pthread_create(&timer->thread_id, NULL, thread_timer, timer))
			!= 0) {
		error_printf("Timer thread creation failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}
	timer->timer_status = TIMER_ON;
}

void stop_timer(Timer_Struct* timer) {
	uint8_t error = 0;
	if (timer->timer_status == TIMER_OFF)
		return;
	timer->timer_signal = STOP_TIMER;
	if ((error = pthread_join(timer->thread_id, EXIT_SUCCESS)) != 0) {
		error_printf("Timer thread join failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}
	timer->timer_status = TIMER_OFF;
}

