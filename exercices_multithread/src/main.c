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
#include<stdint.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include"queue.h"
#include"leds_control.h"
#include"numbers_module.h"

static struct Queue* read_queue;
static struct Queue* write_queue;
static pthread_t thread_id;
static pthread_t write_thread_id_set;
static pthread_t write_thread_id_blink;


void SendMessage(Queue_id queue, char* message, int message_length){
	if (queue == QUEUE_READ){

		int command = (int)*message;
		add_to_Queue(read_queue, command);
		sem_post(&read_queue->semaphore);

	} else if (queue == QUEUE_WRITE){

		int number_to_add = (int) *message;
		add_to_Queue(write_queue, number_to_add);
		sem_post(&write_queue->semaphore);

	}
	printf("message sent\n");
}
int ReceiveMessage(Queue_id queue, char* message, int message_length){

	if (queue == QUEUE_WRITE){
		sem_wait(&write_queue->semaphore);
		del_from_Queue(write_queue, message);

	} else if (queue == QUEUE_READ) {

		sem_wait(&read_queue->semaphore);
		del_from_Queue(read_queue, message);
	}
	printf("Message received\n");
	return 1;
}


void *thread_handler_read(void* args) {
	char button_read[4];
	while (1) {
		int error;
		int command;
		error = readbutton(button_read, 5);
		if (error != 0) printf("%d\n", error);
		if (button_read[3] == 100) printf("Button press read: %d %d %d %d\n", button_read[0], button_read[1], button_read[2],
				button_read[3]);
		if (button_read[3] == 100 && button_read[2] == 117) {
			command = 1;
		} else if (button_read[3] == 100 && button_read[2] == 100) {
			command = 0;
		} else continue;
		SendMessage(QUEUE_READ, (char*)&command, 1);
		printf("\n");
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return 0;
}

void *thread_handler_write(void* args) {
	while(1) {
		char tmp_message = '0';
		if (pthread_self() == write_thread_id_set){
			ReceiveMessage(QUEUE_WRITE, &tmp_message, 1);
			Setnumber((int)tmp_message);
			SetLedMatrice();
		} else if (pthread_self() == write_thread_id_blink);
		SetBlackMatrice();
		sleep(1);
		SetMatriceBack();
		sleep(1);
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return 0;
}

void *thread_handler_app(void* args) {
	int number_to_display = 5;
	char tmp_message = '0';
	while (1) {
		ReceiveMessage(QUEUE_READ, &tmp_message, 1);
		if (tmp_message == 0){
			number_to_display -= 1;
			if (number_to_display < 0) number_to_display = 9;
		} else if (tmp_message == 1){
			number_to_display += 1;
			if (number_to_display > 9) number_to_display = 0;
		}
		SendMessage(QUEUE_WRITE, (char*)&number_to_display, 1);
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return 0;
}

int main(void) {
	openLink();
	read_queue = create_Queue();
	write_queue = create_Queue();


	pthread_create(&thread_id, NULL, thread_handler_read, (void*)&thread_id);
	pthread_create(&write_thread_id_set, NULL, thread_handler_write, (void*)&write_thread_id_set);
	pthread_create(&write_thread_id_blink, NULL, thread_handler_write, (void*)&write_thread_id_blink);
	pthread_create(&thread_id, NULL, thread_handler_app, (void*)&thread_id);

	pthread_join(thread_id, NULL);

	printf("Fin du programme.");
	closeLink();
	return 0;
}

