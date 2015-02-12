#ifndef QUEUE_H
#define QUEUE_H


typedef struct QueueItem* QueueItemPtr;

typedef struct QueueItem{
	void *data;
	QueueItemPtr next;
} QueueItem;

typedef struct Queue{
	QueueItem *head;
	QueueItem *tail;
	int isopen;
	int length;
}Queue;

Queue *create_queue();
int queue_length(Queue *queue);
void enqueue(Queue *queue, void *data);
void *dequeue(Queue *queue);
void destroy_queue(Queue *queue);

#endif
