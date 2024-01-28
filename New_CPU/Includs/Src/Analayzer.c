/**
* @file 
* @author Dawid Żądło
* @version 1.0
*
* @section DESCRIPTION
*
* 
*/

#include "Analayzer.h"
#include "Loger.h"
#include "Watchdog.h"


/**
  * @brief This calculates percentage usage of core 
  * @param CPUstats object with   
  * @param 
  *         
  * @retval percentage usage of core 
  */

float CalculatePercentageUsage( CPUstats*  object,const unsigned char howManyCores)
{
    assert(object!=NULL);


    float tempCalc = (float)((object->idle))/(object->iowait+object->irq+object->nice+object->softirq+object->steal+object->system+object->user);
 
    return fabs(tempCalc);
}


/**
  * @brief Initialize Analyzer_S struct
  * @param number of CPU cores
  * @param CPUstats object 
  *         
  * @retval pointer to allocated memory 
  */
Analyzer_S* Analyzer_InitAnalyzerStruct(const unsigned char howManyCores,CPUstats* object)
{
    if(howManyCores==0)
    {
        log_error("Number of cores passed 0 %s",__func__);
        done = 1;
        pthread_exit(0);
    }
    
    Analyzer_S* temp = malloc((sizeof(Analyzer_S)+(sizeof(float)*howManyCores))); 
    temp->cpuStats = object;
    return temp;
}

/**
  * @brief DeInitialize Analyzer_S struct
  * @param CPUstats object 
  *         
  * @retval None
  */
void Analyzer_DeInitAnalyzerStruct(Analyzer_S* object[static  1])
{
    assert(*object!=NULL );
    free(*object);
    *object = NULL;
}   

/**
  * @brief Calculate usage of CPU using raw data
  * @param CPUstats object previes read
  * @param CPUstats object new read
  * @param number of CPU cores        
  * @retval CPUStats object with calculated usage 
  */
CPUstats CalculateUsage(CPUstats prev[static 1],CPUstats new [static 1],const unsigned char howManyCores)
{
    CPUstats dest;

    if(new == NULL)
    {
        log_error("Object passed ==NULL %s",__func__);   
        done = 1;
        pthread_exit(0);
    }
    if(prev == NULL)
    {
        log_error("Object passed ==NULL %s",__func__);    
        done = 1;
        pthread_exit(0);
    }
    long user,nice,system,idle,iowait,irq,softirq,steal;
    char coreNumber[4];
    
    strcpy(dest.core,new[howManyCores].core);

    dest.user  = ((new[howManyCores].user) - (prev[howManyCores].user));
    dest.nice = (new[howManyCores].nice)- (prev[howManyCores].nice);
    dest.system = (new[howManyCores].system)- (prev[howManyCores].system);
    dest.idle = (new[howManyCores].idle)- (prev[howManyCores].idle);
    dest.iowait = (new[howManyCores].iowait)- (prev[howManyCores].iowait);
    dest.irq = (new[howManyCores].irq) -(prev[howManyCores].irq);
    dest.softirq = (new[howManyCores].softirq)- (prev[howManyCores].softirq);
    dest.steal = (new[howManyCores].steal)- (prev[howManyCores].steal);

    return dest;
    
}


void  AnalyzData( CPUstats* object[static 1],const unsigned char howManyCores)
{
    float resultArray[howManyCores];
    

    if(object == NULL)
    {
        log_error("Object passed ==NULL %s",__func__);
        done = 1;
        pthread_exit(0);
    }
    if(howManyCores==0)
    {
        log_error("Number of cores passed 0 %s",__func__);
        done = 1;
        pthread_exit(0);
    }

    for(int i = 0; i <howManyCores;i++)
    {
        resultArray[i] = CalculatePercentageUsage(*object,howManyCores);       
    }


}

/**
  * @brief main function of Analyzer thread that will be run 
  * @param Analzyer_S object 
  * @retval None
  */
void* Analyzer_Thread(void*arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    thread_local static unsigned char loop = 0;

    Analyzer_S* analyzer_handler = (Analyzer_S*)(arg);
    if(analyzer_handler==NULL)
    {
        log_error("Object passed ==NULL %s",__func__);
        done = 1;
        pthread_exit(0);
    }
    CPUstats* cpuProcesHandler = analyzer_handler->cpuStats;
    CPUstats firstRead[cpuCores];
    CPUstats secondRead[cpuCores];
    CPUstats final[cpuCores];
    float array[cpuCores];
    while(!done)
    {
        
        pthread_mutex_lock(&mutex);
        while(pthread_cond_wait(&analyze,&mutex));
        wdg[analyzer_thread]+=1;
        //Data after first read 
        if (loop==0)
        {
        

            for (int i=0; i <cpuCores;i++)
            {
                memcpy(&firstRead[i],(void*)(cpuProcesHandler+i),sizeof(CPUstats));
                
            }
            
            pthread_mutex_unlock(&mutex);
            loop+=1;
        }
        //Second read
        else 
        {
            for (int i=0; i <cpuCores;i++)
            {
                memcpy(&secondRead[i],(void*)(cpuProcesHandler+i),sizeof(CPUstats));
            }
            pthread_mutex_unlock(&mutex);


            pthread_mutex_lock(&printer_mtx);
            for (int i=0; i <cpuCores;i++)
            {
                final[i] = CalculateUsage(firstRead,secondRead,i);
                array[i] = CalculatePercentageUsage(&final[i],i);

                analyzer_handler->arayy[i]=array[i];
                
            }
            pthread_cond_signal(&print);
            pthread_mutex_unlock(&printer_mtx);
            
            memcpy(&firstRead,&secondRead,sizeof(firstRead));
        }


        
    }
    cpuProcesHandler = NULL;
    pthread_exit(0);

    
}

