#ifndef _PRINTER_H_
#define _PRINTER_H_
#include "main.h"


#define update() printf("\033[H\033[J")

extern volatile sig_atomic_t done;
extern int PID;
void* Printer(void* pDataToPrint);

#endif