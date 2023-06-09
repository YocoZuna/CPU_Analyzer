#ifndef _ANALYZER_H_
#define _ANALYZER_H_
#include "main.h"

typedef struct 
{
    char* cpu;
    float total;
    
}Analyzer_Typedef;





extern volatile sig_atomic_t done;

void* Analzyer(void *DataFromReader);

#endif