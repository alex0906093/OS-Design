#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include "queue.h"

#define TRUE 1
#define FALSE 0

//make a new Queue
Queue *create_queue(){
	Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
	memset(q, 0, sizeof(struct Queue));
	q->isopen = TRUE;
	q->head = NULL;
	q->tail = NULL;
	q->length = 0;
	return q;

}

//get length of queue
int queue_length(Queue *q){
	int a = q->length;
	return a;
}

//add item to Queue
void enqueue(Queue *q, void *data){
	QueueItem *item = (QueueItem*)malloc(sizeof(struct QueueItem));
	memset(item, 0,  sizeof(struct QueueItem));
	item->data = data;
	item->next = NULL;

	if(q->length == 0){
		q->head = item;
		q->tail = item;
		q->length++;
	}
	else{
		q->tail->next = item;
		q->tail = item;
		q->length++;
	}
}

//get data from 
void *dequeue(Queue *q){
	if(q->length == 0){
		return NULL;
	}
	if(q->length == 1){
		QueueItem *head;
		head = q->head;
		void *data;
		data = head->data;
		free(head);
		q->length = 0;
		return data;
	}
	else{
		QueueItem *head;
		head = q->head;
		void *data;
		data = head->data;
		q->head = q->head->next;
		free(head);
		q->length--;
		return data;
	}
}

//destroy Queue
void destroy_queue(Queue *q){
	while(q->length > 0){
		dequeue(q);
	}
}
