/*
 * queue.c
 *
 *  Created on: Apr 7, 2020
 *      Author: jonathan
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>

#include"debug.h"

#include"queue.h"

//Send message through queues, inter-thread communication.
void SendMessage(struct Queue* queue, void* msg_data, size_t size) {
	add_to_Queue(queue, msg_data, size);
	sem_post(&queue->semaphore);
	debug_printf(2, "Message sent.\n");
}

//Receive message through queues, inter-thread communication.
int ReceiveMessage(struct Queue* queue, void* msg_data, uint8_t wait_status,
		size_t size) {
	if (wait_status == WAIT) {
		sem_wait(&queue->semaphore);
		debug_printf(2, "Message received.\n");
	}
	uint8_t status = del_from_Queue(queue, msg_data, size);
	return status;

}

// Créer un nouvel élément de queue.
struct Queue_element* create_Queue_element(void* msg_data, size_t size) {
	int error = 0;
	struct Queue_element* tmp = malloc(sizeof(struct Queue_element));
	if (tmp == NULL) {
		error = errno;
		error_printf(
				"Queue element memory allocation failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	tmp->data = malloc(size);
	if (tmp->data == NULL) {
		error = errno;
		error_printf("Queue data memory allocation failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	tmp->size = size;
	memcpy(tmp->data, msg_data, size);
	tmp->next = NULL;
	return tmp;
}

struct Circle_element* create_Circle_element(int value) {
	int error = 0;
	struct Circle_element* tmp = (struct Circle_element*) malloc(
			sizeof(struct Circle_element));
	if (tmp == NULL) {
		error = errno;
		error_printf(
				"Queue element memory allocation failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	tmp->value = value;
	tmp->next = NULL;
	tmp->prev = NULL;
	return tmp;
}

// Créer une queue vide.
struct Queue* create_Queue(void) {
	int error = 0;
	struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
	if (queue == NULL) {
		error = errno;
		error_printf("Queue memory allocation failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}
	queue->first = queue->last = NULL;
	queue->queue_mutex = (pthread_mutex_t ) PTHREAD_MUTEX_INITIALIZER;
	sem_init(&queue->semaphore, 0, 0);
	return queue;
}

struct Circle* create_Circle(void) {
	int error = 0;
	struct Circle* circle = (struct Circle*) malloc(sizeof(struct Circle));
	if (circle == NULL) {
		error = errno;
		error_printf("Queue memory allocation failed. Error = %d (%s)\n", error,
				strerror(error));
		exit(EXIT_FAILURE);
	}
	circle->first = circle->last = NULL;
	circle->circle_mutex = (pthread_mutex_t ) PTHREAD_MUTEX_INITIALIZER;
	return circle;
}

// Créer un élément de queue et l'ajouter à la queue voulue.
void add_to_Queue(struct Queue* queue, void* msg_data, size_t size) {
	// Nouvel élément
	struct Queue_element* element_to_add = create_Queue_element(msg_data, size);
	int error = 0;
	if ((error = pthread_mutex_lock(&queue->queue_mutex)) != 0) {
		error_printf("\"Add to Queue\" mutex lock failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	// Si la queue est vide, le nouvel élément devient le premier et le dernier.
	if (queue->last == NULL) {
		queue->first = queue->last = element_to_add;
		if ((error = pthread_mutex_unlock(&queue->queue_mutex)) != 0) {
			error_printf(
					"\"Add to Queue\" mutex unlock failed. Error = %d (%s)\n",
					error, strerror(error));
			exit(EXIT_FAILURE);
		}
		return;
	}
	// Ajouter le nouvel élément à la queue et le définir comme étant le dernier.
	queue->last->next = element_to_add;
	queue->last = element_to_add;
	if ((error = pthread_mutex_unlock(&queue->queue_mutex)) != 0) {
		error_printf("\"Add to Queue\" mutex unlock failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
}

void add_to_Circle(struct Circle* circle, int value) {
	struct Circle_element* element_to_add = create_Circle_element(value);
	int error = 0;
	if ((error = pthread_mutex_lock(&circle->circle_mutex)) != 0) {
		error_printf("\"Add to Circle\" mutex lock failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	if (circle->first == NULL) {
		circle->first = circle->last = element_to_add;
		if ((error = pthread_mutex_unlock(&circle->circle_mutex)) != 0) {
			error_printf(
					"\"Add to Queue\" mutex unlock failed. Error = %d (%s)\n",
					error, strerror(error));
			exit(EXIT_FAILURE);
		}
		return;
	}
	circle->last->next = element_to_add;
	element_to_add->prev = circle->last;
	element_to_add->next = circle->first;
	circle->last = element_to_add;
	if ((error = pthread_mutex_unlock(&circle->circle_mutex)) != 0) {
		error_printf("\"Add to Queue\" mutex unlock failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
}

// Supprimer un élément de la queue.
int del_from_Queue(struct Queue* queue, void* msg_data, size_t size) {
	// Si la queue est vide, aucun retour.
	int error = 0;
	if ((error = pthread_mutex_lock(&queue->queue_mutex)) != 0) {
		error_printf("\"Del from Queue\" mutex lock failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	if (queue->first == NULL) {
		if ((error = pthread_mutex_unlock(&queue->queue_mutex)) != 0) {
			error_printf(
					"\"Del from Queue\" mutex unlock failed. Error = %d (%s)\n",
					error, strerror(error));
			exit(EXIT_FAILURE);
		}
		return -1;
	}
	// Récupère l'addresse du premier élement, puis le second élément devient le premier.
	struct Queue_element* element_to_delete = queue->first;
	queue->first = queue->first->next;
	// Si le premier devient nul, le dernier aussi.
	if (queue->first == NULL)
		queue->last = NULL;
	if ((error = pthread_mutex_unlock(&queue->queue_mutex)) != 0) {
		error_printf(
				"\"Del from Queue\" mutex unlock failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	memcpy(msg_data, element_to_delete->data, element_to_delete->size);
	free(element_to_delete->data);
	free(element_to_delete);
	return 0;
}

void del_from_Circle(struct Circle* circle) {
	if (circle->first == NULL)
		return;
	int error = 0;
	if ((error = pthread_mutex_lock(&circle->circle_mutex)) != 0) {
		error_printf("\"Del from Circle\" mutex lock failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	struct Circle_element* element_to_delete = circle->first;
	circle->first = circle->first->next;
	circle->first->prev = circle->last;
	circle->last->next = circle->first;
	if (circle->first == NULL)
		circle->last = NULL;
	if ((error = pthread_mutex_unlock(&circle->circle_mutex)) != 0) {
		error_printf(
				"\"Del from Queue\" mutex unlock failed. Error = %d (%s)\n",
				error, strerror(error));
		exit(EXIT_FAILURE);
	}
	free(element_to_delete);
}

/*void print_Queue(struct Queue* queue) {
 if (queue->first != NULL) {
 printf("My Queue : ");
 struct Queue_element* my_element = queue->first;
 do {
 printf("%d ", my_element->data);
 my_element = my_element->next;
 } while (my_element != NULL);
 printf("//\n");
 }
 }*/

void print_Circle(struct Circle* circle) {
	if (circle->first != NULL) {
		printf("My Circle : ");
		struct Circle_element* my_element = circle->first;
		do {
			printf("%d ", my_element->value);
			my_element = my_element->next;
		} while (my_element != circle->first);
		printf("//\n");
	}
}
