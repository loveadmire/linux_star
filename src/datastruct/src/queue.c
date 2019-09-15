#include <stdio.h>
#include <stdlib.h>
#include "../inc/queue.h"



int queue_test_01(void)
{
	QUEUE *q = createQueue(90);

	enQueue(23 ,q);
	printf(" %08x \n",q->array);
	enQueue(24, q);
	enQueue(25, q);
	enQueue(103, q);
	printf("01 %d %d \n", frontAndDequeue(q),q->size);
	printf("02 %d %d \n", frontAndDequeue(q),q->size);
	printf("03 %d %d \n", frontAndDequeue(q),q->size);
	printf("04 %d %d \n", front(q), q->size);
	enQueue(104, q);
	enQueue(105, q);
	enQueue(107, q);
	printf("05 %d\n", frontAndDequeue(q));
    printf("20 %d %d \n", front(q),q->size);
	deQueue(q);
	printf("06 %d  %d \n", frontAndDequeue(q), q->size);
	printf("07 %d  %d \n", frontAndDequeue(q),q->size);
	enQueue(108, q);
	enQueue(109, q);
	printf("08 %d\n", frontAndDequeue(q));
	printf("09 %d\n", frontAndDequeue(q));
	printf("10 %d\n", frontAndDequeue(q));


	disposeQueue(q);

	return 0;
}



int isEmpty(QUEUE *q)
{
	return (q->size == 0);
}
int isFull(QUEUE *q)
{
	return (q->size == q->capacity);
}
QUEUE *createQueue(int maxElements)
{
	if(maxElements < MinQueueSize) {
		printf("queue size is too small\n");	
		return NULL;
	} else {
		QUEUE *q = (QUEUE *)malloc(sizeof(struct queueRecord));
		q->array = (elementType *)malloc(sizeof(elementType) * maxElements);
		q->capacity = maxElements;
		makeEmpty(q);
		return q;
	}
}
void disposeQueue(QUEUE *q)
{
	if(q != NULL) {
		free(q->array);	
		free(q);
	}	
}
void makeEmpty(QUEUE *q)
{
	if(q != NULL) {
		q->front = 1;	
		q->rear = 0;
		q->size = 0;
	}
}
static int succ(int value, QUEUE *q) 
{
	if(++value == q->capacity) 
		value = 0;
	return value;
}
void enQueue(elementType element, QUEUE *q)
{
	if(!isFull(q)) {
		q->size++;	
		q->rear = succ(q->rear, q);
		q->array[q->rear] = element;
	} else {
		printf("Full queue\n");	
	}
}
elementType front(QUEUE *q)
{
	if(!isEmpty(q)) {
		return q->array[q->front];	
	} else {
		printf("empty queue\n");	
	}
}
void deQueue(QUEUE *q)
{
	if(!isEmpty(q)) {
		q->size--;
		q->front = succ(q->front, q);
	} else {
		printf("empty queue\n");	
	}
}
elementType frontAndDequeue(QUEUE *q)
{
    elementType data;
	if(!isEmpty(q)) {
		data = q->array[q->front];
		q->size--;
		q->front = succ(q->front, q);
		return data ;
	} else {
		printf("empty queue\n");	
		return 0;
	}
}

