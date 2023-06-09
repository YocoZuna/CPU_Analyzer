#include <stdio.h>
#include "main.h"
#include <signal.h>
#include <features.h>
/* Semaphores and mutex to synchronize Reader and Analyzer*/
sem_t semaphoreWaitForData;
sem_t semaphoreDataReady;
pthread_mutex_t mutex;

/* Semaphores and mutex to synchronize Aanalyzer and Readr*/
sem_t semaphorePrintStart;
sem_t semaphorePrintDone;
pthread_mutex_t mutexPrinter;

pthread_t threadID[NUMOFTHREADS];
sem_t* SemaphorArray[NUMOFSEMA] = {&semaphoreDataReady,&semaphorePrintDone,&semaphorePrintStart,&semaphoreWaitForData};
pthread_mutex_t * MutexesArray[NUMOFMUTEX] = {&mutexPrinter,&mutex};

volatile sig_atomic_t done = 0;
int PID = 0; // PID of this program 
void* pArray  =NULL;
Analyzer_Typedef* DataToPrinter = NULL;

int main(void){

    struct sigaction action;
    action.sa_handler = ShutDownProgram;

    sigaction(SIGTERM,&action,NULL);

    /* Initialization of mutexes*/
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutexPrinter,NULL);
    /* Initialization of semaphores*/
    sem_init(&semaphoreDataReady,0,0);
    sem_init(&semaphoreWaitForData,0,1);
    sem_init(&semaphorePrintStart,0,0);
    sem_init(&semaphorePrintDone,0,1);


    int t;
    /* How many CPUs are in machine*/
    int howManyCPUs  = Reader_ReadHowManyCPUs();

    /* Allocation ProcStat_Typdef for all of the CPUs*/
    pArray =  malloc(sizeof(ProcStat_Typedef)*howManyCPUs);
    PID = getpid();
    
    Reader_Typdef ProcStatData ={
        .size = howManyCPUs,
        .ptr = pArray,
        .semDataReady = semaphoreDataReady,
        .semWaitForData = semaphoreWaitForData,
        .mutex = mutex,
        .mutexPrint = mutexPrinter,
        .semPrinterDone = semaphorePrintDone,
        .semPrinterStart = semaphorePrintStart,
        .Printer = NULL,
    };
    Reader_Typdef* pProcStatData = &ProcStatData;
        /* Starting threads*/
    for (t=0;t<NUMOFTHREADS;t++)
    {   
        
        if(t==0)
        {
           if(pthread_create(&threadID[t],NULL,Reader_ReadDataFromProcStat,(void*)pProcStatData)!=0)
            printf("Could not creat tread %ld\n",threadID[t]);
        }
        else if (t==1)
        {
             if(pthread_create(&threadID[t],NULL,Analzyer,(void*)pProcStatData)!=0)
            printf("Could not creat tread %ld\n",threadID[t]);
        }
        else
        {
             if(pthread_create(&threadID[t],NULL,Printer,(void*)pProcStatData)!=0)
            printf("Could not creat tread %ld\n",threadID[t]);
        }

    }
    
    /* Infinit loop*/
    while(!done)
    {
        sleep(__INT_MAX__);
    }
}


void ShutDownProgram(int signum)
{
    /* Free allocated array*/


    done=1;

    for(int i=0;i<NUMOFTHREADS;i++)
    {
        pthread_join(threadID[i],NULL);
    }

    free(pArray);
    /* Destroying muexes and semaphores*/
    for(int i=0; i<NUMOFMUTEX;i++)
    {
        pthread_mutex_destroy(MutexesArray[i]);
    }

    for(int i = 0; i <NUMOFSEMA;i++)
    {
        sem_destroy(SemaphorArray[i]);
        
    }

    /* Exiting  program */
    exit(0);
}