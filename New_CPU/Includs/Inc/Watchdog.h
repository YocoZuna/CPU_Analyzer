
#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_
#include "main.h"

enum WatchDog_enum {reader_thread=2,analyzer_thread,printer_thread,logger_thread=1,watchdog_thread =0 };


void* WatchDog_Thread(void* arg);

#endif