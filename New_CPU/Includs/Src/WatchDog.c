/**
* @file 
* @author Dawid Żądło
* @version 1.0
*
* @section DESCRIPTION
*
*/

#include "Watchdog.h"
#include "Loger.h"
/**
  * @brief Lookuptable with string that coresponds to enum number 
  */
const char* lookUpTable[] = {
    [reader_thread]  = "READER" ,
    [analyzer_thread] = "ANALAZYER",
    [printer_thread] = "PRINTER",
    [logger_thread] = "LOGGER",
};

/**
  * @brief main function of Watchdog thread that will be run 
  * @param circuralbuffer object 
  * @retval None
  */
void* WatchDog_Thread(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    while(!done)
    {
        thrd_sleep(&(struct timespec){.tv_sec=2},NULL);
        for(int i=1; i<THREADSTOCALL-1;i++)
        {
            if(wdg[i]>0)
            {
                wdg[i] = 0;
            }
            else
            {
      
                log_warrning("Thread %s died\n",lookUpTable[i]);

               // done =1;
            };
        }
    }
    pthread_exit(0);
}