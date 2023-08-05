#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdio.h>
#include "Reader.h"
#include "Analyzer.h"
#include "Printer.h"
#include "WatchDog.h"

#include "MyLogger.h"

#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <semaphore.h>
#include <signal.h>
#define NUMOFTHREADS 5
#define NUMOFSEMA 5
#define NUMOFMUTEX 3
void ShutDownProgram(int signum);
#endif