

#ifndef _PRINTER_H_
#define _PRINTER_H_

#include "main.h"
#include "Analayzer.h"
#define update() printf("\033[H\033[J")


void Printer_PrintDataToSTDOUT(Analyzer_S* object,const unsigned char howManyCores);
void* Printer_Thread(void* arg);
#endif

