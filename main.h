#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdio.h>
#include "Reader.h"
#include "Analyzer.h"
#include "Printer.h"
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "unistd.h"
#include <assert.h>
#include <semaphore.h>
#include <signal.h>
#define NUMOFTHREADS 3
#define NUMOFSEMA 4
#define NUMOFMUTEX 2
void ShutDownProgram(int signum);
#endif