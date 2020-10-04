#ifndef QUEUE_H
#define QUEUE_H

#include "timer.h"
#include "fcn.h"
#include <pthread.h>
#include <sys/time.h>
#define QUEUESIZE 1000

typedef struct timer timer;
struct queue{
	timer* buf[QUEUESIZE];
	long head, tail;
	int producers;
	int full;
	int empty;
	pthread_mutex_t *mut;
	pthread_cond_t *notFull, *notEmpty;
};
typedef struct queue queue;

queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, timer* in);
void queueDel (queue *q, timer* out);

#endif
