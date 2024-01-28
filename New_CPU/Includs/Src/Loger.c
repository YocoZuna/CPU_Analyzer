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

/**
  * @brief Lookuptable with string that coresponds to enum number 
  */
const char* logg_lvl[5] = {
    [DEBUG] = "DEBUG",
    [INFO] = "INFO" ,
    [WARRNING] = "WARRNING",
    [ERROR] = "ERROR",
}; 

/**
  * @brief Logs data to file  
  * @param messege that should be logged
  * 
  * @retval None
  */
void LogToFile(const char dataToLog[])
{
    FILE* pFile = NULL;
    pFile = fopen(FILETOLOG,"a+");
 
    if (pFile==NULL)
    {
        log_error("Cannot open file %s  %s",FILETOLOG,__func__);
        done =1;
        pthread_exit(0);
    }
    else
    {   
        time_t t;  
        time(&t);
        fprintf(pFile,"When occured:%s %s \n",ctime(&t),dataToLog);

    }
    fclose(pFile);
    pFile = NULL;
    
}

/**
  * @brief Inserting log data into circuralbuffer
  * @param format specifiers
  * @param arguments
  * @retval None
  */
inline void putToConsole( char* fmt,...)
{
    int ret = 0;
    char temp[MESSEGESIZE];
    va_list args;
    va_start(args,fmt);
    
    ret = vsnprintf(temp,MESSEGESIZE,fmt,args);
    if (ret>0)
    {
        pthread_mutex_lock(&bufforToPrint->mtx);
        
        ret = Circural_Enque(bufforToPrint,temp);
        if(ret!=0)
        {
            if (ret==-1)
                perror("Buffor is full\n");
            else if( ret == -2)
                perror("Message was to long\n");
        }
        
        pthread_cond_signal(&bufforToPrint->cond);
        pthread_mutex_unlock(&bufforToPrint->mtx);
        
    }
    else
    {
        perror("Could not write to logger buffor\n");
    }
    va_end(args);
}


/**
  * @brief main function of Logger thread that will be run 
  * @param circuralbuffer object 
  * @retval None
  */
 /*
void* Logger_Thread(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);

    circuralbuffer* loggerbuf = (circuralbuffer*)(arg);

    while(!done || !(Circural_CheckIfSomeThingToRead(loggerbuf)))
    {

        wdg[logger_thread]+=1;
        pthread_mutex_lock(&loggerbuf->mtx);
        if(!pthread_cond_timedwait(&loggerbuf->cond,&loggerbuf->mtx,&(struct timespec){.tv_sec=1}))
        {
            LogToFile(Circural_Deque(loggerbuf));
            wdg[logger_thread]+=1;

        }
        else
        {
            wdg[logger_thread]+=1;
        }
        pthread_mutex_unlock(&loggerbuf->mtx);
    }
    pthread_exit(0);
}
*/
void* Logger_Thread(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);

    circuralbuffer* loggerbuf = (circuralbuffer*)(arg);
    char ret = Circural_CheckIfSomeThingToRead(loggerbuf);
    while(!done || !ret )
    {
        wdg[logger_thread]+=1;
        char ret = Circural_CheckIfSomeThingToRead(loggerbuf);
        if(ret == 0)
        { 

            pthread_mutex_lock(&loggerbuf->mtx);
            LogToFile(Circural_Deque(loggerbuf));

            pthread_mutex_unlock(&loggerbuf->mtx);

        }
        thrd_sleep(&(struct timespec){.tv_sec=1},NULL);
    }
    pthread_exit(0);
}