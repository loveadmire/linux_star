#include "queue_my.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void DelQueue(Queue* queue)
{
	while (queue->head != queue->tail)
	{
		DeQueue(&queue);
	}

}
int Search_ip(Queue* queue, char *ip) {
	if (IsNull(queue)) {
		return 0;
	}
	if (ip == NULL) {
		return 0;
	}
	int statuse = 1;
	QueueNode* curNode = queue->head->next;
	while (curNode) {
		if (curNode->next != NULL)
		{
			if (strcmp(ip, curNode->ip) == 0) {
				statuse = 0;
			}
		}
		else
		{
			if (strcmp(ip, curNode->ip) == 0) {
				statuse = 0;
			}
		}
		curNode = curNode->next;
	}
	return statuse;
}

int LengQueue(Queue* queue) {
	int size = 0;
	if (IsNull(queue))
	{
		return size;
	}
	QueueNode* curNode = queue->head->next;
	while (curNode)
	{
		if (curNode->next != NULL)
		{
			size += 1;
		}
		else
		{
			return size + 1;
		}
		curNode = curNode->next;
	}
	return size;
}

void PrintQueue(Queue* queue)
{
	if (IsNull(queue))
	{
		printf("empty queue.\n");
		return;
	}
	QueueNode* curNode = queue->head->next;
	while (curNode)
	{
		if (curNode->next != NULL)
		{
			printf("ip is :%s ", curNode->ip);
			printf("%ld ", curNode->timestamp);
		}
		else
		{
			//�0�1�1�7�1�7�0�5�1�7�1�7�1�7�1�7�0�2�1�7�1�7
			printf(" it is %s:", curNode->ip);
			printf("it is %ld", curNode->timestamp);

		}
		curNode = curNode->next;
	}
	printf("\n");

}


void InitQueue(Queue* queue)
{
	queue->tail = (QueueNode*)malloc(sizeof(QueueNode));//warning
	queue->tail->ip = NULL;
	queue->tail->timestamp = 0;
	queue->head = queue->tail;
	queue->tail->next = NULL;
}

void EnQueue(Queue* queue, char *ip, long int timestamp)
{
	QueueNode * newNode = (QueueNode*)malloc(sizeof(QueueNode));//warning
	newNode->ip = ip;
	newNode->timestamp = timestamp;
	newNode->next = NULL;
	queue->tail->next = newNode;//2b �1�7�1�7�1�7�1�7�0�8�1�7�0�5�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�4
	queue->tail = newNode;
}

int DeQueue(Queue* queue)
{
	long int  popValue = queue->head->timestamp;
	//char *ip = queue->head->ip;
	QueueNode *popNode = queue->head;
	queue->head = queue->head->next;
	free(popNode);//warning
	return 0;
}
//1 means Null
int IsNull(Queue* queue)
{
	return (queue->head == queue->tail);
}

#if 1
int test_queue(void)
{
	Queue queue;
	InitQueue(&queue);
	//printf("IsNull = %d\n", IsNull(&queue));
	printf("enque 4 times and the elems: 1, 2, 3, 4\n");
	EnQueue(&queue, "aaaa",32);

	PrintQueue(&queue);
	printf("deque 1 times.\n");
	DeQueue(&queue);
	PrintQueue(&queue);

	printf("IsNull = %d\n", IsNull(&queue));
	DelQueue(&queue);
	printf("IsNull = %d\n", IsNull(&queue));

	return 0;
}
#endif
