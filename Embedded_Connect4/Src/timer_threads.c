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
#include"cmsis_os.h"
#include"app_thread.h"

#include"timer_threads.h"

timer_struct_t short_timer;
timer_struct_t long_timer;
timer_struct_t play_token_blink;
timer_struct_t victory_blink;
timer_struct_t valid_move;



static timer_struct_t* timer_table[10];
extern osMutexId_t timer_mutexHandle;

void* timer_handler(void* thread_id) {
	for (uint8_t i = 0; i < 10; i++) {
		timer_table[i] = NULL;
	}
	new_timer_init(&short_timer, SHORT_TIMER_SETTING);
	new_timer_init(&long_timer, LONG_TIMER_SETTING);
	new_timer_init(&play_token_blink, 250);
	new_timer_init(&victory_blink, 500);
	new_timer_init(&valid_move, 200);
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
		osDelay(1);
	}
	return 0;
}

void new_timer_init(timer_struct_t* timer, int timer_setting) {
	timer->timer_id = DEFAULT_TIMER_ID;
	timer->timer_count = timer_setting;
	timer->timer_max_count = timer_setting;
	timer->timer_delay = timer->timer_status = TIMER_OFF;
	timer->timer_signal = STOP_TIMER;
	timer->timer_force = FORCE_OFF;

	for (int timer_id = 0; timer_id < 10; timer_id++) {
		if (timer_table[timer_id] == NULL) {
			timer_table[timer_id] = timer;
			timer->timer_id = timer_id;

			return;
		}
	}

	return;
}

uint8_t start_new_timer(timer_struct_t* timer, uint8_t force_status) {
	if (timer->timer_status == TIMER_ON) {
		return -1;
	}
	if (force_status) {
		timer_table[timer->timer_id]->timer_force = FORCE_ON;
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

