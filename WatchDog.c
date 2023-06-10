#include "main.h"


int killThread = 0;
static void CatchAlarm(void *Thread);

/*
*  Description - Runs watchdog thread
*
*  Return  - None
*
*  Param  - Reader_Typedef* struct 
*/
void* WatchDog(void* ThreadToWatch)
{
    
    while (!done)
    {
        sleep(1);
        if(killThread!=0)
        {
           pthread_exit(NULL);
        }
        
        CatchAlarm((void*)ThreadToWatch);
        sleep(1);
    }

}

/*
*  Description - Checks if any of called threads incremented data if not application is killed 
*
*  Return  - None
*
*  Param  - Reader_Typedef* struct 
*/
static void CatchAlarm(void *Thread)
{  

    Reader_Typdef* ReaderStruct = Thread;

   /* NUum of thread -1 because last thread is watchdog */
   for(int i= 0;i<NUMOFTHREADS-1;i++){
 
    /* REading data from struct*/
    if(ReaderStruct->WatchDog[i]!=0)
    {
        killThread=0;
    }
    else
    {
        printf("Thread %d failed\n",i);
        killThread +=1;
    }
    ReaderStruct->WatchDog[i]=0;
   }
   if (killThread>0)
   {
        /* If any of data is equal to 0 send command kill {PID of application}*/
        char buffor[20];
        sprintf(buffor,"kill %d",PID);
        system(buffor);
    
   }

}