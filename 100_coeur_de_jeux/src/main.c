/*
 *****************************************************************************

 File        : main.cpp

 The MIT License (MIT)
 Copyright (c) 2019 STMicroelectronics

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 *****************************************************************************
 */

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<pthread.h>

#include"debug.h"
#include"queue.h"
#include"display_thread.h"
#include"read_thread.h"
#include"app_thread.h"
#include"timer_threads.h"

//Création des threads Input, Application, et Display.
int main(void) {
	int error = 0;
	debug_setlevel(2);
	debug_printf(1, "Lancement du programme Puissance 4.\n");
	pthread_t thread_id_input;
	pthread_t thread_id_display;
	pthread_t thread_id_app;
	pthread_t thread_id_timer;

	if ((error = pthread_create(&thread_id_input, NULL, thread_handler_input,
			(void*) &thread_id_input)) != 0) {
		error_printf("Input thread creation failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}

	if ((error = pthread_create(&thread_id_display, NULL,
			thread_handler_display, (void*) &thread_id_display)) != 0) {
		error_printf("Display thread creation failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}

	if ((error = pthread_create(&thread_id_app, NULL, thread_handler_app,
			(void*) &thread_id_app)) != 0) {
		error_printf("App thread creation failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}

	if ((error = pthread_create(&thread_id_timer, NULL, timer_handler,
			(void*) &thread_id_timer)) != 0) {
		error_printf("Timer thread creation failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}

	if ((error = pthread_join(thread_id_app, NULL)) != 0) {
		error_printf("App thread join failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}
	debug_printf(1, "Fin du programme.\n");
	return 0;
}

