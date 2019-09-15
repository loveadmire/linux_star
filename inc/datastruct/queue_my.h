#ifndef QUEUE_MYH
#define QUEUE_MYH

#include <stdio.h>


typedef struct tagNode
{
	char *ip;
	long int timestamp;
	struct tagNode* next;
}QueueNode;


typedef struct queue
{
	QueueNode* head;
	QueueNode* tail;
}Queue;

void InitQueue(Queue*);
int Search_ip(Queue*, char *);

int LengQueue(Queue*);
void EnQueue(Queue*, char*, long int);
int DeQueue(Queue* queue);
void PrintQueue(Queue* queue);
int IsNull(Queue* queue);
void DelQueue(Queue* queue);

int test_queue(void);
#endif //QUEUE_H
