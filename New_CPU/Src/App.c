/**
* @file 
* @author Dawid Żądło
* @version 1.0
*
* @section DESCRIPTION
*
*   MAIN PROGRAM OF APLICATION 
*/
#include "main.h"
#include "Analayzer.h"
#include "Printer.h"
#include "Loger.h"
#include "Reader.h"
#include "Watchdog.h"

pthread_mutex_t mutex,printer_mtx;
pthread_cond_t analyze,print;
unsigned char cpuCores = 0;
_Atomic(int) wdg[THREADSTOCALL];
atomic_uint_fast8_t counter = 0;
atomic_bool done= 0;
Analyzer_S* analyzer_struct = NULL;
circuralbuffer*  bufforToPrint = NULL;
CPUstats* cpuProcStatInfo  = NULL;
pid_t AppPid;
static __thrd_t thread[THREADSTOCALL];



funPointer* lookUpFunctionTable[THREADSTOCALL] = 
{
    [watchdog_thread] = WatchDog_Thread,
    [logger_thread] = Logger_Thread,
    [reader_thread]  = Reader_Thread ,
    [analyzer_thread] = Analyzer_Thread,
    [printer_thread] = Printer_Thread,

};

/**
  * @brief Starts thread
  * @param which thread to run : 
  *                             @arg watchdog_thread  = WatchDog_Thread,
  *                             @arg logger_thread    = Logger_Thread,
  *                             @arg reader_thread    = Reader_Thread ,
  *                             @arg analyzer_thread  = Analyzer_Thread,
  *                             @arg printer_thread   = Printer_Thread,
  * 
  * @param object thats should be passed to thread
  * 
  * @retval None
  */
static void RunThread(int threadToRun,void* param[static 1])
{
    if(pthread_create(&thread[threadToRun],NULL,lookUpFunctionTable[threadToRun],param))  
    {
        log_error("Could not create thread %ld\n",threadToRun);
        done = 1;   
    }
    log_info("Thread %ld just started\n",threadToRun);
}

/**
  * @brief Cleans up allocated memory threads mutexs etc
  * @retval None
  */
void CleanUP(void)
{

    
    for(int i=0; i <THREADSTOCALL;i++)
    {
        pthread_cancel(thread[i]);
        pthread_join(thread[i],NULL);

    }
    

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&printer_mtx);
    
    Reader_DeInitReaderStruct(&cpuProcStatInfo);
    Circural_DeInitBuffer(&bufforToPrint);
    Analyzer_DeInitAnalyzerStruct(&analyzer_struct);
    bufforToPrint = NULL;
    cpuProcStatInfo  = NULL;
    analyzer_struct = NULL;

}


/**
  * @brief SIGTERM ISR
  *
  * 
  * @param signum 
  * 
  * @retval None
  */
void signal_handler(int singnum)
{
    done  =1;
    log_info("%s","SIGTERM has been detected closing application\n");
    sleep(1);
  
}



int main(void)
{
    AppPid  =getpid();
    atexit(&CleanUP);
    
    struct sigaction action={.sa_handler = signal_handler};
    sigaction(SIGTERM,&action,NULL);

    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&printer_mtx,NULL);
    pthread_cond_init(&analyze,NULL);
    pthread_cond_init(&print,NULL);
    

    cpuCores = Reader_ReadCores();
    cpuProcStatInfo = Read_InitReaderStruct(cpuCores);
    analyzer_struct = Analyzer_InitAnalyzerStruct(cpuCores,cpuProcStatInfo);
    bufforToPrint  = Circural_InitBuffer(cpuCores);
    
    
    RunThread(watchdog_thread,(void*)bufforToPrint);

    RunThread(logger_thread,(void*)bufforToPrint);
    
    RunThread(reader_thread,(void*)cpuProcStatInfo);
    
    RunThread(analyzer_thread,(void*)analyzer_struct);
    
    RunThread(printer_thread,(void*)analyzer_struct);

    
    while(!done)
    {
        sleep(__UINT32_MAX__);
    }
}