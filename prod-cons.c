#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define QUEUESIZE 100
#define LOOP 20000
#define CONSUMERS 100
#define PRODUCERS 10

void *producer (void *args);
void *consumer (void *args);
void *sayHello (void *yes);

struct workFunction {
  void * (*work)(void *);
  void * arg;
  struct timeval start;
};

typedef struct {
  struct workFunction buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;

queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, struct workFunction in);
void queueDel (queue *q, struct workFunction *out);

int main ()
{
  queue *fifo;
  pthread_t pro, con;
  int p,q,i;
  p = PRODUCERS;
  q = CONSUMERS;
  pthread_t producerArr[p];
  pthread_t consumerArr[q];
  fifo = queueInit ();
  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
  for(i=0;i<p;i++)
  {
    pthread_create(&producerArr[i],NULL,producer,fifo);
  }
  for(i=0;i<q;i++)
  {
    pthread_create(&consumerArr[i],NULL,consumer,fifo);
  }
  for(i=0;i<p;i++)
  {
    pthread_join(producerArr[i],NULL);
  }
  for (i=0;i<q;i++)
  {
    pthread_join(consumerArr[i],NULL);
  }
  queueDelete (fifo);
  return 0;
}

void *producer (void *q)
{
  queue *fifo;
  int i;
  struct workFunction *newJob;
  newJob = malloc(sizeof(*newJob));
  fifo = (queue *)q;
  for (i = 0; i < LOOP; i++) {
    newJob->work = sayHello;
    newJob->arg = (void *)i;
    pthread_mutex_lock (fifo->mut);
    while (fifo->full) {
      printf ("producer: queue FULL.\n");
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }
    queueAdd (fifo, *newJob);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);
  }
  return (NULL);
}

void * sayHello (void *yes)
{
  int i;
  int sum=0;
  printf("Hello. Actually I'm doing nothing\n");
  for (i=0;i<1000;i++)
  {
    sum+=i;
  }
  printf("Done\n");
}
void *consumer (void *q)
{
  queue *fifo;
  int i;
  struct workFunction d;

  fifo = (queue *)q;

  while(1) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty) {
      printf ("consumer: queue EMPTY.\n");
      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
    queueDel (fifo, &d);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);
    d.work(d.arg);
  }
  return (NULL);
}

queue *queueInit (void)
{
  queue *q;

  q = (queue *)malloc (sizeof (queue));
  if (q == NULL) return (NULL);

  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;
  q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (q->mut, NULL);
  q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notFull, NULL);
  q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notEmpty, NULL);

  return (q);
}

void queueDelete (queue *q)
{
  pthread_mutex_destroy (q->mut);
  free (q->mut);
  pthread_cond_destroy (q->notFull);
  free (q->notFull);
  pthread_cond_destroy (q->notEmpty);
  free (q->notEmpty);
  free (q);
}

void queueAdd (queue *q, struct workFunction in)
{
  struct timeval start;
  gettimeofday(&start,NULL);
  in.start = start;
  q->buf[q->tail] = in;
  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

  return;
}

void queueDel (queue *q, struct workFunction *out)
{
  struct timeval end;
  *out = q->buf[q->head];
  gettimeofday(&end,NULL);
  long micros =  end.tv_usec - out->start.tv_usec;
  printf("%ld\n", micros);
  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;

  return;
}
