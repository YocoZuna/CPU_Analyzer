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