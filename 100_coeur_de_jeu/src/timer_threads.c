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

static pthread_mutex_t timer_table_mutex;

void* timer_handler(void* thread_id) {
	for (uint8_t i = 0; i < 10; i++) {
		timer_table[i] = NULL;
	}
	while (1) {
		for (int timer_id = 0; timer_id < 10; timer_id++) {
			if (timer_table[timer_id] != NULL) {
				if (timer_table[timer_id]->timer_status == TIMER_ON
						&& timer_table[timer_id]->timer_count > 0) {
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
					}
				}
			}
		}
		usleep(1000);
	}
	return 0;
}

void new_timer_init(timer_struct_t* timer, int timer_setting,
		void* callback_param) {
	int error = 0;
	if (timer_table[timer->timer_id] != NULL) {
		return;
	}
	timer->timer_id = DEFAULT_TIMER_ID;
	timer->timer_count = timer_setting;
	timer->timer_max_count = timer_setting;
	timer->timer_delay = timer->timer_status = TIMER_OFF;
	timer->timer_signal = STOP_TIMER;
	timer->timer_force = FORCE_OFF;
	timer->callback_param = callback_param;
	if ((error = pthread_mutex_lock(&timer_table_mutex)) != 0) {
		error_printf("Timer_Table mutex lock failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}
	for (int timer_id = 0; timer_id < 10; timer_id++) {
		if (timer_table[timer_id] == NULL) {
			timer_table[timer_id] = timer;
			timer->timer_id = timer_id;
			if ((error = pthread_mutex_unlock(&timer_table_mutex)) != 0) {
					error_printf("Timer_Table mutex unlock failed. Error = %d (%s)\n",
							error, strerror(error));
					exit(EXIT_FAILURE);
				}
			return;
		}
	}
	if ((error = pthread_mutex_unlock(&timer_table_mutex)) != 0) {
		error_printf("Timer_Table mutex unlock failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	return;
}

uint8_t start_new_timer(timer_struct_t* timer) {
	if (timer->timer_status == TIMER_ON) {
		return -1;
	}

	timer_table[timer->timer_id]->timer_status = TIMER_ON;
	timer_table[timer->timer_id]->timer_signal = RUN_TIMER;
	return 0;
}

void stop_new_timer(timer_struct_t* timer) {
	if (timer_table[timer->timer_id] == NULL) {
		return;
	}
	timer_table[timer->timer_id]->timer_status = TIMER_OFF;
	timer_table[timer->timer_id]->timer_signal = STOP_TIMER;
	timer_table[timer->timer_id]->timer_force = FORCE_OFF;
	timer_table[timer->timer_id]->timer_count =
			timer_table[timer->timer_id]->timer_max_count;
}

