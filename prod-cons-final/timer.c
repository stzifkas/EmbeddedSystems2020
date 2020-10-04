#include "timer.h"
#include "fcn.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

timer* timerInit(int period, queue* q, int tasksToExecute, int startDelay)
{
	timer* t = malloc(sizeof(Timer));
	t->period = period;
	t->q = q;
	t->tasksToExecute = tasksToExecute;
	t->startDelay = startDelay;
	t->timerFcn = timerFcnF;
	t->userData = startFcnF();
	return t;
}

void start(timer* t, void (*producer)(void*))
{
	usleep(1000*t->startDelay);
	pthread_create (&t->pro, NULL, producer, t);
}
