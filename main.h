#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdio.h>
#include "Reader.h"
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "unistd.h"
#include <assert.h>
#include <semaphore.h>
#include <signal.h>
#define NUMOFTHREADS 1
#define NUMOFSEMA 2
#define NUMOFMUTEX 1
void ShutDownProgram(int signum);
#endif