/**
* @file 
* @author Dawid Żądło
* @version 1.0
*
* @section DESCRIPTION
*
*/

#include "Loger.h"
#include "Watchdog.h"
#include "Printer.h"



#define testOutput
#ifdef testOutput
    extern char outputstream[256];
#endif
#undef testOutput
/**
  * @brief Printing data to stdout
  * @param Analyzer_S object 
  * @param number of CPU cores
  * @retval None
  */
void Printer_PrintDataToSTDOUT(Analyzer_S* object,const unsigned char howManyCores)
{
    if(object==NULL)
    {
        log_error("Object passed ==NULL %s",__func__);
        done=1;
        pthread_exit(0);
    }
    if(howManyCores==0)
    {
        log_error("Number of cores passed 0 %s",__func__);
        done=1;
        pthread_exit(0);
    }

    /* We do this way only if we are sure that cores thata will be always in good order*/
    for(int i = 0 ; i < howManyCores;i++)
    {
        float data = object->arayy[i];
        #ifdef testOutput
            snprintf(outputstream,30,"Core %d Usage:%.2f %%\n",i,data);
        #else
            printf("Core %d Usage:%.2f %%\n",i,data);
        #endif
    }

}
/**
  * @brief main function of Printer thread that will be run 
  * @param Analyzer_S object 
  * @retval None
  */
void* Printer_Thread(void* arg)
{
    
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    Analyzer_S* bufforToPrint = (Analyzer_S*)(arg);
    if(bufforToPrint==NULL)
    {
        log_error("Object passed ==NULL %s",__func__);
        done=1;
        pthread_exit(0);
    }
    while (!done)
    {    
        
        pthread_mutex_lock(&printer_mtx);
        pthread_cond_wait(&print,&printer_mtx);

        
        wdg[printer_thread]+=1;
        
        printf("Program PID to kill: %d\n",AppPid);

        Printer_PrintDataToSTDOUT(bufforToPrint,cpuCores);
        update(); // cleaning stdout
        pthread_mutex_unlock(&printer_mtx);
    }
    bufforToPrint = NULL;
    pthread_exit(0);
}