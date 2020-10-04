#ifndef FCN_H
#define FCN_H

typedef struct {
	double a,b,c,d;
} fcnArgs;
void* startFcnF();
void* timerFcnF (void* userData);
void* stopFcnF();
void* errorFcnF();

#endif
