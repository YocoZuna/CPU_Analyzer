#pragma once
#ifndef _ANALAZYER_H_
#define _ANALAYZER_H_

#include "Reader.h"
#include "main.h"

typedef struct analayzer_s {
    int size;
    CPUstats * cpuStats;
    float arayy[];
}Analyzer_S;



void AnalyzData( CPUstats* object[static 1],const unsigned char howManyCores);

CPUstats CalculateUsage( CPUstats prev[static 1],CPUstats  new [static 1],const unsigned char howManyCores);

float CalculatePercentageUsage( CPUstats*  object,const unsigned  char howManyCores);

void* Analyzer_Thread(void*arg);

Analyzer_S*  Analyzer_InitAnalyzerStruct(const unsigned char howManyCores,CPUstats* object);

void Analyzer_DeInitAnalyzerStruct(Analyzer_S* object[static  1]);

#endif