#ifndef _MAIN_H_
#define _MAIN_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdatomic.h>
#include <time.h>
#include <stdarg.h>
#include "CircuralBuffer.h"




#define THREADSTOCALL 5
extern _Atomic(int)  wdg[THREADSTOCALL];

extern atomic_bool done;
extern unsigned char cpuCores;
extern pthread_mutex_t mutex;
extern pthread_mutex_t printer_mtx;
extern pthread_mutex_t logger;
extern pthread_cond_t analyze,print;
extern atomic_uint_fast8_t counter;
extern circuralbuffer* bufforToPrint;

typedef void* funPointer(void*);

#ifndef NDEBUG
#define UNDER_DEBUG
#endif

extern pid_t AppPid;

#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__) 
  #include <pthread.h>
  #include <threads.h>
#endif
#if __STDC_VERSION__ < 201112L || __STDC_NO_ATOMICS__ == 1
#error "I need atomics to work :("

#endif

void* Logger_Thread(void* arg);

#endif