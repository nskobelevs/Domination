#include "log.h"

/**
 * Opens the log file
 */
void initLog(void) {
    logFile = fopen("domination.log", "a");
}

/*
 * Returns a pointer to a string which stores the current time
 */
char* currentTime(void) {
    time_t timer;
    struct tm* tm_info;
    timer = time(NULL);
    tm_info = localtime(&timer);
    strftime(charString, 20, "%d-%m-%Y %H:%M:%S", tm_info);
    return charString;
}

/**
 * Closes log file
 */
void endLog(void) {
    fclose(logFile);
}

/**
 * Writes formatted message to log file
 * @param format Format string
 * @param ... Arguments for the string
 */
void LOG(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    fprintf(logFile, "[%s] ", currentTime());
    vfprintf(logFile, format, arg);
    va_end(arg);
    fflush(logFile);
}