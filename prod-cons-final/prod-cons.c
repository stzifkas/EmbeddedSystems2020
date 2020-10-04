#include "queue.h"
#include "fcn.h"
#include "timer.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define PRODUCERS 10

void* producer(void* t)
{
  FILE* producertimes = fopen("producertimes","a");
  timer* tim = t;
  Queue* fifo = tim->q;
  struct timeval taskstart;
  for (int i=0;i<tim->tasksToExecute;i++)
  {
    gettimeofday(&taskstart,NULL);
    pthread_mutex_lock(fifo->mut);
    while(fifo->full)
    {
      errorFcnF();
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }
    queueAdd(fifo,tim);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);
    struct timeval taskend;
    gettimeofday(&taskend,NULL);
    long long dif = (taskend.tv_sec-taskstart.tv_sec)*1000000ll + taskend.tv_userc - taskstart.tv_usec;
    if (dif < tim->period*1000)
    {
      usleep(tim->period*1000-dif);
    }
    fprintf(producertimes,"%lld\n",dif)
  }
  stopFcnF();
  return (NULL);
}

void* consumer (void *q)
{
  FILE* consumertimes = fopen("consumertimes","a");
	queue* fifo;
	void* result = NULL;
  fifo = (queue *)q;
	while (1)
	{
		pthread_mutex_lock (fifo->mut);
		while (fifo->empty)
		{
			pthread_cond_wait (fifo->notEmpty, fifo->mut);
		}
		if (fifo->empty && fifo->producers==0)
		{
			pthread_mutex_unlock(fifo->mut);
			return (NULL);
		}
		struct timeval taskstart;
		gettimeofday(&taskstart,NULL);
		timer* job;
    queueDel(fifo, job);
		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal (fifo->notFull);
		job->timerFcn(out->userData);
    struct timeval taskend;
		gettimeofday(&taskend,NULL);
    long long dif = (taskend.tv_sec-taskstart.tv_sec)*1000000ll +taskend.tv_usec-taskstart.tv_usec;
		fprintf(consumertimes,"%lld\n",dif);
    //print drift
    printf("%lld\n",-1000*job->period +(taskstart.tv_sec-out->lastStart.tv_sec)*1000000ll +taskstart.tv_usec-out->lastStart.tv_usec);
		job->lastStart = taskstart;
	}
	return (NULL);
}

int main ()
{
  queue *fifo;
  pthread_t pro, con;
  int p,q,i;
  p = PRODUCERS;
  period = 10;
  fifo->producers = p;
  pthread_t con;
  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
  timer* timers[p];
  for (i=0;i<p;i++)
  {
    timers[i] = timerInit(period,fifo,5000000,0);
    start(timer[i],producer);
  }
  pthread_create(&con,NULL,consumer,fifo);

  for (i=0;i<p;i++)
  {
    pthread_join(timers[i]->producer,NULL);
  }
  fifo->producers == 0;
  pthread_cond_broadcast(fifo->notEmpty);
	pthread_join (con, NULL);
	queueDelete (fifo);
  return 0;
}
