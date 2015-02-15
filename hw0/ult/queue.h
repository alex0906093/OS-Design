#ifndef QUEUE_H
#define QUEUE_H
#define QUEUE_SCAN(e,Q,i) QueueItemPtr e=Q.head;for (int i=0;e!=Q.head&&i>0;i++,e=e.next)

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
