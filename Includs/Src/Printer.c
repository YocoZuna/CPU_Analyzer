#include "main.h"
extern char loggerBufor[100];
static void Printer_PrintToConsole(Reader_Typdef* DataToPrint,int loops);
static void WirteToLogger(void* data,char* text);
/*
*  Description - Printer thread 
*
*  Return  - None
*
*  Param  - Reader_Typedef* struct 
*/
void* Printer(void* pDataToPrint)
{
    int i = 0;
    Reader_Typdef* DataToPrint = pDataToPrint;
    int howManyCPUs = DataToPrint->size;

    while(!done)
    { 

        Printer_PrintToConsole(DataToPrint,howManyCPUs);
        
        sleep(1);
        /*Updating  console */
        update();
        
    }   
      /* Exiting thread*/ 
    pthread_exit(NULL);
}

/*
*  Description - Print data to console
*
*  Return  - None
*
*  Param  - Reader_Typedef* struct 
*/
static void Printer_PrintToConsole(Reader_Typdef* DataToPrint,int loops){

        DataToPrint->WatchDog[2] +=1;
        for(int i=0;i<loops-1;i++)
        {   
            sem_wait(&DataToPrint->semPrinterStart);
            pthread_mutex_lock(&DataToPrint->mutexPrint);
            if (i==0)
            {
               
                /*Printing header to console */
                printf("CPU analyzer PID:%d, Cores %d\n",PID,loops-1);
            }   
    
           
            
            printf("%s\n",(char*)DataToPrint->Printer);
            pthread_mutex_unlock(&DataToPrint->mutexPrint);
            sem_post(&DataToPrint->semPrinterDone);
            
        }
}

static void WirteToLogger(void* data,char* text)
{
    Reader_Typdef* dataToLog = data;
        pthread_mutex_lock(&dataToLog->mutexLog);
        
        sprintf(dataToLog->Logger,"%s",text);
        pthread_mutex_unlock(&dataToLog->mutexLog);
        sem_post(&dataToLog->semLogger);
}