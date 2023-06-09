#include <stdio.h>
#include "main.h"

/* Semaphores and mutex to synchronize Reader and Analyzer*/
sem_t semaphoreWaitForData;
sem_t semaphoreDataReady;
pthread_mutex_t mutex;

pthread_t threadID[NUMOFTHREADS];
sem_t* SemaphorArray[NUMOFSEMA] = {&semaphoreDataReady,&semaphoreWaitForData};
pthread_mutex_t * MutexesArray[NUMOFMUTEX] = {&mutex};

volatile sig_atomic_t done = 0;
void* pArray  =NULL;

int main(int, char**){

    struct sigaction action;
    action.sa_handler = ShutDownProgram;

    sigaction(SIGTERM,&action,NULL);

    /* Initialization of mutexes*/
    pthread_mutex_init(&mutex,NULL);

    /* Initialization of semaphores*/
    sem_init(&semaphoreDataReady,0,0);
    sem_init(&semaphoreWaitForData,0,1);


    int t;
    /* How many CPUs are in machine*/
    int howManyCPUs  = Reader_ReadHowManyCPUs();

    /* Allocation ProcStat_Typdef for all of the CPUs*/
    pArray =  malloc(sizeof(ProcStat_Typedef)*howManyCPUs);

    Reader_Typdef ProcStatData ={
        .size = howManyCPUs,
        .ptr = pArray,
        .semDataReady = semaphoreDataReady,
        .semWaitForData = semaphoreWaitForData,
        .mutex = mutex,
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

    for(int i=0;i<3;i++)
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