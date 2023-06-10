#include "main.h"


int killThread = 0;
static void CatchAlarm(void *Thread);
void* WatchDog(void* ThreadToWatch)
{

    while (!done)
    {
        if(killThread!=0)
        {
           pthread_exit(NULL);
        }
        sleep(1);
        CatchAlarm((void*)ThreadToWatch);

    }

}


static void CatchAlarm(void *Thread)
{  

 Reader_Typdef* ReaderStruct = Thread;
  
   for(int i= 0;i<NUMOFTHREADS-1;i++){
 
    if(ReaderStruct->WatchDog[i]!=0)
    {
        killThread=0;
    }
    else
    {
        killThread +=1;
    }
    ReaderStruct->WatchDog[i]=0;
   }
   if (killThread>0)
   {
        
        char buffor[20];
        sprintf(buffor,"kill %d",PID);
        system(buffor);
    
   }

}