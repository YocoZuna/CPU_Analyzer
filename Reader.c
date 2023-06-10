#include "Reader.h"

/*
*  Description - This function read /proc/stat file and returns number of avaiable CPUs
*
*  Return  - number of avaiable CPUs
*
*  Param  - None
*/

int Reader_ReadHowManyCPUs(void)
{
    /* Pattern to look for */
    char pattern[4] = "cpu";
    FILE *pFile= NULL;
    pFile = fopen(FILETOOPEN,"r");
     
    int p = (int)strlen(pattern); 
    int res = 0;int i,j,c;
    char string[400];
    i=0;
    while((c=getc(pFile))!='i')
    {
        
        string[i] = (char)c;
        i+=1;
    }
    fclose(pFile);
    int s = strlen(string); 
    for (i=0; i <=s-p;i++)
    {
        for(j=0;j<p;j++)
        
            if (string[i+j] != pattern[j])
                break;
        if(j==p)
            res++;
        
    }
    pFile = NULL;
    return res;
}

/*
*  Description - Runs Reader thread that reads proc stat
*
*  Return  - None
*
*  Param  - Reader_Typedef* struct 
*/



void* Reader_ReadDataFromProcStat(void* pArray)
{

    Reader_Typdef* ReaderStruct = NULL;
    ProcStat_Typedef* Data = NULL;
    FILE *pFile= NULL;
    
    char dummyBuffor[100];
    int i,dummy;
    while(!done)
    {
        

        
        
        ReaderStruct = pArray;
        sem_wait(&ReaderStruct->semWaitForData);
        pthread_mutex_lock(&ReaderStruct->mutex);
        ReaderStruct->WatchDog[0] +=1;
        Data = ReaderStruct->ptr;
        int howManyCPUs = ReaderStruct->size;
        pFile = fopen(FILETOOPEN,"r");
        for (i = 0; i< howManyCPUs;i++)
        {   
            
            int retval  = fscanf(pFile,"%s %ld %ld %ld %ld %ld %ld %ld %ld \n",Data[i].cpu,&Data[i].user,&Data[i].nice,&Data[i].system,&Data[i].idle,&Data[i].iowait,&Data[i].irq,&Data[i].softirq,&Data[i].steal);

            fseek(pFile,4,SEEK_CUR); 
            
        }
        fclose(pFile);
        sem_post(&ReaderStruct->semDataReady);
        pthread_mutex_unlock(&ReaderStruct->mutex);
    }

    pthread_exit(NULL);
}