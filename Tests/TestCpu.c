#include "Analyzer.h"
#include "Reader.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>

#define FILETOOPEN "/proc/stat"


int Reader_ReadHowManyCPUs(void)
{
    char pattern[4] = "cpu";
    FILE *pFile= NULL;
    pFile = fopen(FILETOOPEN,"r");
    int p = strlen(pattern); 
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

int main(void)
{
    int cpu = Read
}