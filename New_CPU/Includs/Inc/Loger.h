#ifndef _LOGER_H_
#define _LOGER_H_


#include "main.h"

#define str0(X) #X
#define STRING(X) str0(X)

#define FILETOLOG "/home/dawid_zadlo/Desktop/Test/Src/Logs.txt"


extern pthread_mutex_t logger;
extern atomic_bool done;

extern const char* logg_lvl[5];


#define log_info(F,...) putToConsole("INFO" " File:" __FILE__ " Line:" STRING(__LINE__) " " F "",__VA_ARGS__)
#define log_debug(F,...) putToConsole("DEBUG" " File:" __FILE__ " Line:" STRING(__LINE__) " " F "",__VA_ARGS__)
#define log_warrning(F,...) putToConsole("WARRNING" " File:" __FILE__ " Line:" STRING(__LINE__) " " F "",__VA_ARGS__)
#define log_error(F,...) putToConsole("ERROR" " File:" __FILE__ " Line:" STRING(__LINE__) " " F "",__VA_ARGS__)






typedef enum {
    DEBUG,
    INFO,
    WARRNING,
    ERROR,
}loger_enum;


void putToConsole( char* data,...);
void LogToFile(const char dataToLog[]);

void* Logger_Thread(void* arg);


#endif