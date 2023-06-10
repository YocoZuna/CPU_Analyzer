#include "Analyzer.h"


extern char loggerBufor[100];
static void CollectData(ProcStat_Typedef* Dest,ProcStat_Typedef* Source,int loops,long long int*pidle,long long int*pnonIdle,long long int*ptotal);
static void WirteToLogger(void* data,char* text);
void* Analzyer(void *DataFromReader)
{   

  int i;
  Reader_Typdef* ReaderStruct = DataFromReader;
  ProcStat_Typedef* Data = ReaderStruct->ptr;
  int howManyCPUs = (ReaderStruct->size);
  ProcStat_Typedef CurrentData[howManyCPUs];
  ProcStat_Typedef PrevData[howManyCPUs];
  char buffor[27];
  Analyzer_Typedef DataToPrinter[howManyCPUs];
  /* Variables to calculate % usage */
  long long int idle[howManyCPUs],nonIdle[howManyCPUs],total[howManyCPUs];
  long long int previdle[howManyCPUs],prevnonIdle[howManyCPUs],prevtotal[howManyCPUs];
  long long int idled[howManyCPUs],totald[howManyCPUs];
  
  while(!done)

  {
    ReaderStruct->WatchDog[1] +=1;
    /* Waiting for data from reader and blocking meamory*/
    sem_wait(&ReaderStruct->semDataReady);
    pthread_mutex_lock(&ReaderStruct->mutex);
    
    /* Collecting /proc/stat data */
    CollectData(PrevData,Data,howManyCPUs,previdle,prevnonIdle,prevtotal);

    /* Sleeping 1s beacouse we want thata to apear eevery 1 s*/
    if(done==0)
      sleep(1);
    ReaderStruct->WatchDog[1] +=1;
    /* Free semaphore and meamory*/
    sem_post(&ReaderStruct->semWaitForData);
    pthread_mutex_unlock(&ReaderStruct->mutex);

    /* Waiting for data from reader and blocking meamory*/
    sem_wait(&ReaderStruct->semDataReady);
    pthread_mutex_lock(&ReaderStruct->mutex);
    /* Collecting new data from /proc/stat*/
    CollectData(CurrentData,Data,howManyCPUs,idle,nonIdle,total);

    /* Calculation of % usage of CPU */
    for (i = 1; i< howManyCPUs;i++)
    {

      sem_wait(&ReaderStruct->semPrinterDone);
      pthread_mutex_lock(&ReaderStruct->mutexPrint);
      idled[i] = idle[i] -previdle[i];
      totald[i] = total[i]-prevtotal[i];
      
      DataToPrinter[i].cpu = CurrentData[i].cpu;

      DataToPrinter[i].total =(((float)totald[i]-(float)idled[i])/(float)totald[i])*100;
      
      /* Sending data to printer thru buffor */
      snprintf(buffor,27,"Core:%s,Usage:%0.2f%%",DataToPrinter[i].cpu,DataToPrinter[i].total);
      ReaderStruct->Printer = buffor;
      pthread_mutex_unlock(&ReaderStruct->mutexPrint);
      sem_post(&ReaderStruct->semPrinterStart);
    }


    pthread_mutex_unlock(&ReaderStruct->mutex);
    sem_post(&ReaderStruct->semWaitForData);
    
  }
  return NULL;
  /* Exiting thread*/ 
  pthread_exit(NULL);
}

static void CollectData(ProcStat_Typedef* Dest,ProcStat_Typedef* Source,int loops,long long int*pidle,long long int*pnonIdle,long long int*ptotal)
{
  for(int i = 0; i <loops;i++)
  {

    Dest[i]= Source[i];
    pidle[i] = Dest[i].idle + Dest[i].iowait;
    pnonIdle[i] = Dest[i].user+Dest[i].system+Dest[i].nice+Dest[i].irq+Dest[i].softirq+Dest[i].steal;
    ptotal[i] = pidle[i]+pnonIdle[i];
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