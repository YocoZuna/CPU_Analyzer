#ifndef _READER_H_
#define _READER_H_

#include "main.h"
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#define FILETOOPEN "/proc/stat"

extern volatile sig_atomic_t done;

typedef struct 
{
    char cpu[4];
    long int user;
    long int nice;
    long int system;
    long int idle;
    long int iowait;
    long int irq;
    long int softirq;
    long int steal;

}ProcStat_Typedef;

typedef struct
{

    int size;
    void * ptr;
    sem_t semDataReady;
    sem_t semWaitForData;
    pthread_mutex_t mutex;
    void *Printer;
    sem_t semPrinterDone;
    sem_t semPrinterStart;
    pthread_mutex_t mutexPrint;
    int WatchDog[4];
}Reader_Typdef;

/*Function  to read how many CPUs machine has*/
int Reader_ReadHowManyCPUs(void);
/* Reader Thread*/
void* Reader_ReadDataFromProcStat(void* pArray);
#endif