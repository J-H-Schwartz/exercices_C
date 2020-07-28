/*
 * queue.h
 *
 *  Created on: Apr 7, 2020
 *      Author: jonathan
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include<stdint.h>
#include<semaphore.h>
#include"queue.h"
#include"game_p4.h"

#define QUEUE_ELEMENT_VALUE_SIZE 10

typedef enum {
	NO_WAIT, WAIT
} Receive_Wait_Status;

typedef enum Queue_id {
	QUEUE_READ, QUEUE_WRITE
} Queue_id;

struct Queue_list {
	struct Queue *read_queue, *write_queue;
};

// Élément de liste.
struct Queue_element {
	void* data;
	struct Queue_element* next;
	size_t size;
};

// Structure de file, contient le pointeur vers le premier
//élément (pour delete) et vers le dernier (pour add).
struct Queue {
	struct Queue_element *first, *last;
	pthread_mutex_t queue_mutex;
	sem_t semaphore;
};

struct Circle_element {
	int value;
	struct Circle_element *next, *prev;
};

struct Circle {
	struct Circle_element *first, *last;
	pthread_mutex_t circle_mutex;
};

void SendMessage(struct Queue* queue, void* msg_data, size_t size);

int ReceiveMessage(struct Queue* queue, void* msg_data, uint8_t wait_status,
		size_t size);

struct Queue* create_Queue(void);

struct Queue_element* create_Queue_element(void* msg_data, size_t size);

void add_to_Queue(struct Queue* queue, void* msg_data, size_t size);

int del_from_Queue(struct Queue* queue, void* msg_data, size_t size);

void print_Queue(struct Queue* queue);

struct Circle* create_Circle(void);

struct Circle_element* create_Circle_element(int value);

void add_to_Circle(struct Circle* circle, int value);

void del_from_Circle(struct Circle* circle);

void print_Circle(struct Circle* circle);

#endif /* QUEUE_H_ */
