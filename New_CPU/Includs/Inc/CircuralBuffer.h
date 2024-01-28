#ifndef _CIRCURAL_BUFFER_H_
#define _CIRCURAL_BUFFER_H_
#include "main.h"
//#define CircuralType(X)  _Generic((X),float : float(X),char : char(X),int : int(X),default : float(X))(X)     

#define MESSEGESIZE 250

#define BUFFERSIZE 10

typedef struct circural{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    int tail;
    int head;
    char buffer[][MESSEGESIZE];

}circuralbuffer;

circuralbuffer* Circural_InitBuffer();

char Circural_Enque(circuralbuffer* object, const char data[]);

char*  Circural_Deque(circuralbuffer* object);

void Circural_DeInitBuffer(circuralbuffer* object [static  1]);

char Circural_CheckIfSomeThingToRead(circuralbuffer* object);

#endif 