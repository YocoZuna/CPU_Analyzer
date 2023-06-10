#include "main.h"

static void Printer_PrintToConsole(Reader_Typdef* DataToPrint,int loops);

void* Printer(void* pDataToPrint)
{
    int i = 0;
    Reader_Typdef* DataToPrint = pDataToPrint;
    int howManyCPUs = DataToPrint->size;


    while(!done)
    { 

        Printer_PrintToConsole(DataToPrint,howManyCPUs);
        
        sleep(1);
        /*Updating buffo console */
        update();
        
    }   
      /* Exiting thread*/ 
    pthread_exit(NULL);
}
static void Printer_PrintToConsole(Reader_Typdef* DataToPrint,int loops){
        DataToPrint->WatchDog[2] +=1;
        for(int i=0;i<loops-1;i++)
        {   
            sem_wait(&DataToPrint->semPrinterStart);
            pthread_mutex_lock(&DataToPrint->mutexPrint);
            if (i==0)
            {
                sleep(4000);
                printf("CPU analyzer PID:%d, CPU %d\n",PID,loops-1);
            }   
            if(done==1)
                pthread_exit(NULL);

            printf("%s\n",(char*)DataToPrint->Printer);
            pthread_mutex_unlock(&DataToPrint->mutexPrint);
            sem_post(&DataToPrint->semPrinterDone);
            
        }
}