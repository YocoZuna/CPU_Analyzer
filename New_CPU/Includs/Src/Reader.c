/**
* @file 
* @author Dawid Żądło
* @version 1.0
*
* @section DESCRIPTION
*
*/


#include "Reader.h"
#include "Loger.h"
#include "Watchdog.h"

/**
  * @brief  maximum value of char characters that can be read during reading /proc/stat
*/
#define BUFFORMAXLEN 256

/**
  * @brief Initilize ReaderStruct  
  * @param number of cores this decides how much memory will be allocated
  * 
  * @retval Pointer to allocated memory 
  */
CPUstats* Read_InitReaderStruct(const char unsigned howManyCores)
{
    assert(howManyCores >0);
    CPUstats* temp = calloc(howManyCores,sizeof(CPUstats));

    return temp;
}

/**
  * @brief Denitilize ReaderStruct 
  * @param allocated object  
  * 
  * @retval None
  */
void Reader_DeInitReaderStruct(CPUstats* object[static  1])
{
    assert(*object!=NULL );
    free(*object);
    *object = NULL;
}   

/**
  * @brief Reads /proc/stat file in order to find number of CPU cores
  * @param None
  * 
  * @retval number of detected number of CPU cores
  */
unsigned char Reader_ReadCores(void)
{

    char buffor[BUFFORMAXLEN];
    char pattern[] = "cpu";
    unsigned char  res = 0;
    FILE* pFile = NULL;
    //home/dawid_zadlo/Desktop/Test/procstat.txt
    
    pFile = fopen("/proc/stat","r");
    if (pFile==NULL)
    {
        log_error("Cannot open file %s  %s",FILETOLOG,__func__);
        exit(EXIT_FAILURE);
    
    }
    else
    {   
        fgets(buffor,BUFFORMAXLEN,pFile); // skipping first line 
        /* Reading one by each line to the end of the file*/
        while(fgets(buffor,BUFFORMAXLEN,pFile))
        {
            /* Loooking for pattern "cpu" in buffor if it maches increment res*/
            if(strspn(buffor,pattern)==strlen(pattern))
            {
                res+=1;
            }
        }

    }
    fclose(pFile);
    pFile = NULL;
    if (res == 0)
    {
        log_error("Number cores that have been found %d",res);
        exit(EXIT_FAILURE);
    }
    return res;
}

/**
  * @brief Reads /proc/stat file and saves data in CPUStats struct 
  * @param CPUStats object
  * @param number of CPU cores
  * @retval None
  */
void Reader_ReadCoresInfo(CPUstats object[static 1],const unsigned char howManyCores)
{
    assert(object!=NULL);
    char buffor[BUFFORMAXLEN];
    FILE* pFile = NULL;
    pFile = fopen("/proc/stat","r");
    if(howManyCores==0)
    {
        log_error("Number of cores passed 0 %s",__func__);
        fclose(pFile);
        done=1;
        pthread_exit(0);
    }
    
    if (pFile==NULL)
    {
        log_error("Cannot open file %s  %s",FILETOLOG,__func__);
        fclose(pFile);
        done=1;
        pthread_exit(0);
    }

    else
    {   
        fgets(buffor,BUFFORMAXLEN,pFile); // skipping first line 
        
        /* Reading one by each line to the end of the file*/
        for(int i=0;i<howManyCores;i++)
        {
            fscanf(pFile,"%s %ld %ld %ld %ld %ld %ld %ld %ld",object->core,&object->user,&object->nice,&object->system,&object->idle,&object->iowait,&object->irq,&object->softirq,&object->steal);
            while(fgetc(pFile)!='\n'); // Has to be implemented because we dont read whole line so we have to skip last chars
            ++object;
        }
    
    }
    fclose(pFile);
    pFile = NULL;
}

/**
  * @brief main function of Reader thread that will be run 
  * @param CPUStats struct 
  * @retval None
  */
void* Reader_Thread(void*arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    CPUstats* cpuProcesInfo = (CPUstats*)(arg);
    if(cpuProcesInfo==NULL)
    {
        log_error("Object passed ==NULL %s",__func__);

        done=1;
        pthread_exit(0);
    }
    while(!done)
    {

  
        pthread_mutex_lock(&mutex);
        Reader_ReadCoresInfo(cpuProcesInfo,cpuCores);
        wdg[reader_thread]+=1;
      
        pthread_cond_signal(&analyze);
        pthread_mutex_unlock(&mutex);


        thrd_sleep(&(struct timespec){.tv_sec=1},NULL); 
    }
    cpuProcesInfo = NULL;
    pthread_exit(0);

}
