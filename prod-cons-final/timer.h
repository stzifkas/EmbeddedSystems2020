#ifndef TIMER_H
#define TIMER_H
#include "queue.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
FILE* producerTimeLog;
typedef struct queue queue;

struct timer{
	int period;
  int tasksToExecute;
  int startDelay;
  void* (*startFcn)(void *);
  void* (*stopFcn)(void *);
  void* (*timerFcn)(void *);
  void* (*errorFcn)(void *);
  void* userData;
	queue* q;
	pthread_t pro;
};
typedef struct timer timer;
timer* timerInit(int period, queue* q);
void start(timer* t);
#endif
