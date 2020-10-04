#include "queue.h"
#include "fcn.h"
#include "timer.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* startFcnF()
{
  double div = RAND_MAX / 100;
  fcnArgs* ret = malloc(sizeof(fcnArgs));
  ret->a = (double)rand()/div;
  ret->b = (double)rand()/div;
  ret->c = (double)rand()/div;
  ret->d = (double)rand()/div;
  return ret;
}
void* timerFcnF(void* userData)
{
  double a = (fcnArgs*)userData->a;
  double b = (fcnArgs*)userData->b;
  double c = (fcnArgs*)userData->c;
  double d = (fcnArgs*)userData->d;
  for (int i=0; i<100; i++)
  {
      for (int j=0; j<100; j++)
      {
        for (int k=0; k<100; k++)
        {
          if (a*i+b*j+c*k == d)
          {
            return (NULL);
          }
        }
      }
  }
}

void* stopFcnF()
{
  printf("Hello from stop function\n");
  return (NULL);
}

void* errorFcnF()
{
  printf("We got problem\n");
  return (NULL);
}
