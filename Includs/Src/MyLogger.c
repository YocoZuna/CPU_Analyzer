#include "MyLogger.h"
#include "time.h"
time_t currentTime = 0;
char loggerBufor[100];
void* Logger(void* DataToLog)
{
    Reader_Typdef* data = DataToLog;
    while(!done)
    {
        sem_wait(&data->semLogger);
        pthread_mutex_lock(&data->mutexLog);
        PrintToFile(data->Logger);

        pthread_mutex_unlock(&data->mutexLog);
        
        
    }
}
int PrintToFile( char* dataToLog)
{   
    time(&currentTime);
    FILE * pFile = NULL;
    pFile = fopen("Logger.txt","a+");
    if(pFile!=NULL)
    {
        fprintf(pFile,"%s  %s",ctime(&currentTime),dataToLog);
        return 0 ;
    }
    else
    {
        return -1;
    }
}