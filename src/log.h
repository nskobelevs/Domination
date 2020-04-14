#ifndef DOMINATION_LOG_H
#define DOMINATION_LOG_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

FILE *logFile;        //File for debug output
char charString[64];    //Temp string storage

void initLog(void);

char* currentTime(void);

void endLog(void);

void LOG(const char *format, ...);

#endif //DOMINATION_LOG_H
