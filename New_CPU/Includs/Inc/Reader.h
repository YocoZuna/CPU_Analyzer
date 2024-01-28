#ifndef _READER_H_
#define _READER_H_


#include "main.h"

//typedef enum procStatMeanings {core=0,user,nice,system,idle,iowait,irq,softirq,steal};

typedef struct procStat{
    char core[4];
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
    long steal;
}CPUstats;

CPUstats* Read_InitReaderStruct(const unsigned char howManyCores);
void Reader_ReadCoresInfo(CPUstats object[static 1],const unsigned char howManyCores);
void Reader_DeInitReaderStruct(CPUstats* object[static  1]);
unsigned char Reader_ReadCores(void);
void* Reader_Thread(void* arg);
#endif