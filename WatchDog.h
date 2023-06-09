#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include "main.h"
void* WatchDog(void* ThreadToWatch);

void CatchAlarm(void *Thread);

#endif