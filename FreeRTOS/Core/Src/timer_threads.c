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
#include "cmsis_os.h"

#include"timer_threads.h"


const osThreadAttr_t TimerThread_attributes = { .name = "TimerThread",
		.priority = (osPriority_t) osPriorityNormal2, .stack_size = 128 * 4 };

static void thread_timer(void* timer_ptr) {
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
			osDelay(100);
			time_out += 1;
		}
	}
	timer->timer_signal = STOP_TIMER;
	timer->timer_status = TIMER_OFF;
}

void timer_init(Timer_Struct* timer, int timer_setting, void* callback_param) {
	timer->timer_set = timer_setting;
	timer->timer_status = TIMER_OFF;
	timer->timer_signal = STOP_TIMER;
	timer->timer_force = FORCE_OFF;
	timer->callback_param = callback_param;
}

void start_timer(Timer_Struct* timer) {
	if (timer->timer_status == TIMER_ON)
		return;
	timer->thread_id = osThreadNew(thread_timer, timer,
				&TimerThread_attributes);
//	if ((error = pthread_create(&timer->thread_id, NULL, thread_timer, timer))
//			!= 0) {
//		error_printf("Timer thread creation failed. Error = %d (%s)\n", error,
//				strerror(error));
//		exit(EXIT_FAILURE);
//	}
	timer->timer_status = TIMER_ON;
}

void stop_timer(Timer_Struct* timer) {
	if (timer->timer_status == TIMER_OFF)
		return;
	timer->timer_signal = STOP_TIMER;
	while (timer->timer_status != TIMER_OFF){
		osDelay(100);
	}
//	if ((error = pthread_join(timer->thread_id, EXIT_SUCCESS)) != 0) {
//		error_printf("Timer thread join failed. Error = %d (%s)\n", error,
//				strerror(error));
//		exit(EXIT_FAILURE);
//	}
	timer->timer_status = TIMER_OFF;
}

