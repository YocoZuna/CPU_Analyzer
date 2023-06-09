#include "main.h"
#include "WatchDog.h"
#include <signal.h>
#include <unistd.h>

void* WatchDog(void* ThreadToWatch)
{

    while (!done)
    {
        sleep(1);
        CatchAlarm((void*)ThreadToWatch);

    }
    



}


void CatchAlarm(void *Thread)
{  
    int kill = 0;
 Reader_Typdef* ReaderStruct = Thread;
  
   for(int i= 0;i<NUMOFTHREADS-1;i++){
    printf("%d",ReaderStruct->WatchDog[0]);
    if(ReaderStruct->WatchDog[i]!=0)
    {
        ReaderStruct->WatchDog[i]=0;
    }
    else
    {
        kill +=1;
    }
   }
   if (kill>0)
   {

        char buffor[20];
        sprintf(buffor,"kill %d",PID);
        system(buffor);
    
   }

}