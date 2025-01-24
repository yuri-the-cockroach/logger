#include "logger.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

enum loglevel CURRENT_LOG_LEVEL_CONSOLE = 4;
enum loglevel CURRENT_LOG_LEVEL_FILE    = 6;
char LOG_FILE_NAME[64];
bool BENCHMARKING        = false;
FILE *BENCH_LOG_FILE_PTR = 0;
FILE *LOG_FILE_PTR       = 0;

// This is an internal function.
// It is ment to be used through LOG macro, that filles in most of the data
void Logger(const char *restrict inFile, const char *restrict inFunc,
            const int onLine, const enum loglevel loglevel,
            const char *restrict format, ...) {

    if (CURRENT_LOG_LEVEL_FILE < loglevel &&
        CURRENT_LOG_LEVEL_CONSOLE < loglevel)
        return;

    if (loglevel == BENCH && !BENCHMARKING) return;

    constexpr int message_length       = 4096;
    char messageString[message_length] = "";

    va_list argptr;

    int argcount = 0;
    int i        = 0;
    char current;
    FILE *file =
        BENCHMARKING && loglevel == BENCH ? BENCH_LOG_FILE_PTR : LOG_FILE_PTR;

    // Count how many arguments we have
    // by counting format specifiers in a format string
    do {
        current = format[i];
        if (current == '%') argcount++;
        i++;
    } while (current != '\0');

    // Pass all the arguments to the vsprinf, which will fill it into the
    // message string. If argptr is 0, then it will just be ignored (hopefully)
    va_start(argptr, argcount);
    vsnprintf(messageString, message_length, format, argptr);
    va_end(argptr);

    struct timeval time = (struct timeval){ 0, 0 };
    gettimeofday(&time, 0);
    char formatedTime[64] = "";
    sprintf(formatedTime,
            "%.2ld-%.2ld-%.2ld.%ld",
            (time.tv_sec / 3600 + 3) % 24,
            time.tv_sec / 60 % 60,
            time.tv_sec % 60,
            time.tv_usec);
    // Actually print to stderr

    if (CURRENT_LOG_LEVEL_CONSOLE >= loglevel)
        fprintf(stderr,
                "[%s] %s %s->%s:%d %s\n",
                loglvlToString[loglevel],
                formatedTime,
                inFile,
                inFunc,
                onLine,
                messageString);

    fprintf(file,
            "[%s] %s %s->%s:%d %s\n",
            loglvlToStringNoColor[loglevel],
            formatedTime,
            inFile,
            inFunc,
            onLine,
            messageString);
}

int InitLogger(void) {
    time_t t           = time(NULL);
    struct tm timedate = *localtime(&t);

    struct stat statThingi;
    stat("logs", &statThingi);

    // If logs exists, but is not a directory
    // remove it and set errno to ENOENT
    // So the next condition gets triggered and creates a directory
    if (statThingi.st_mode / 010000 == 010) {
        remove("logs");
        errno = ENOENT;
    }

    if (errno == ENOENT) {
        mkdir("logs", 0755);
        errno = 0;
    }

    sprintf(LOG_FILE_NAME,
            "logs/asteroids-%d-%02d-%02d_%02d-%02d-%02d.log",
            timedate.tm_year + 1900,
            timedate.tm_mon + 1,
            timedate.tm_mday,
            timedate.tm_hour,
            timedate.tm_min,
            timedate.tm_sec);

    LOG_FILE_PTR = fopen(LOG_FILE_NAME, "a");
    errno        = 0;

    return 0;
}
