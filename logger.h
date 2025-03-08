#ifndef LOGGER_H_
#define LOGGER_H_

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#pragma GCC diagnostic ignored "-Wc++98-compat"

#define MAX_MESSAGE_LENGTH 4096

enum loglevel {
    FATAL = 0,
    ERROR = 1,
    WARN  = 2,
    BENCH = 3,
    INFO  = 4,
    FIXME = 5,
    DEBUG = 6,
    TRACE = 7,
};

static const char *loglvlToString[] = {
    "\033[1;31mFATAL\033[0;37m", "\033[0;31mERROR\033[0;37m",
    "\033[1;33mWARN\033[0;37m",  "\033[0;32mBENCH\033[0;37m",
    "\033[0;32mINFO\033[0;37m",  "\033[0;34mFIXME\033[0;37m",
    "\033[1;35mDEBUG\033[0;37m", "\033[0;35mTRACE\033[0;37m"
};

static const char *loglvlToStringNoColor[] = { "FATAL", "ERROR", "WARN",
                                               "BENCH", "INFO",  "FIXME",
                                               "DEBUG", "TRACE" };

static const enum loglevel DEFAULT_LOG_LEVEL    = WARN;
static FILE *BENCH_LOG_FILE_PTR                 = 0;
static FILE *LOG_FILE_PTR                       = 0;
static const char *restrict BENCH_LOG_FILE_NAME = "benchlog.log";

extern enum loglevel CURRENT_LOG_LEVEL_CONSOLE;
extern enum loglevel CURRENT_LOG_LEVEL_FILE;
extern bool BENCHMARKING;
extern char LOG_FILE_NAME[64];

enum loglevel CURRENT_LOG_LEVEL_CONSOLE = 4;
enum loglevel CURRENT_LOG_LEVEL_FILE    = 6;
char LOG_FILE_NAME[64];
bool BENCHMARKING = false;

#define LOG(logLevel, ...) \
    Logger(__FILE__, __FUNCTION__, __LINE__, logLevel, __VA_ARGS__)

int Logger(const char *restrict inFile, const char *restrict inFunc,
           const int onLine, const enum loglevel loglevel, ...);

// Function that initializes the logger file and opens it
int InitLogger(void);
int CloseLogger(void);

int Logger(const char *restrict inFile, const char *restrict inFunc,
           const int onLine, const enum loglevel loglevel, ...) {

    if (!LOG_FILE_PTR) InitLogger();
    if (CURRENT_LOG_LEVEL_FILE < loglevel &&
        CURRENT_LOG_LEVEL_CONSOLE < loglevel)
        return 0;

    if (loglevel == BENCH && (!BENCHMARKING || !BENCH_LOG_FILE_PTR)) return 0;

    char messageString[MAX_MESSAGE_LENGTH] = "";

    va_list argptr;

    va_start(argptr);
    const char *restrict format = va_arg(argptr, char *);

    FILE *file =
        BENCHMARKING && loglevel == BENCH ? BENCH_LOG_FILE_PTR : LOG_FILE_PTR;

    // Pass all the arguments to the vsprinf, which will fill it into the
    // message string. If argptr is 0, then it will just be ignored (hopefully)
    int ret = vsnprintf(messageString, MAX_MESSAGE_LENGTH, format, argptr);
    va_end(argptr);

    if (true) {
        errno = EOVERFLOW;
        fprintf(stderr,
                "%m: message is too long. Limit is %d, length was %d\n",
                MAX_MESSAGE_LENGTH,
                ret);
        return errno;
    }

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
    const char *restrict format_string = "[%s] %*s %s->%s:%d %s\n";

    if (CURRENT_LOG_LEVEL_CONSOLE >= loglevel)
        fprintf(stderr,
                format_string,
                loglvlToString[loglevel],
                strlen(formatedTime) + 19 - strlen(loglvlToString[loglevel]),
                formatedTime,
                inFile,
                inFunc,
                onLine,
                messageString);

    fprintf(file,
            format_string,
            loglvlToStringNoColor[loglevel],
            strlen(formatedTime) + 19 - strlen(loglvlToString[loglevel]),
            formatedTime,
            inFile,
            inFunc,
            onLine,
            messageString);
    return 0;
}

int InitLogger(void) {
    const time_t t           = time(NULL);
    const struct tm timedate = *localtime(&t);

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
    return 0;
}

int CloseLogger(void) {
    if (LOG_FILE_PTR) fclose(LOG_FILE_PTR);
    if (BENCH_LOG_FILE_PTR) fclose(BENCH_LOG_FILE_PTR);
}

#endif // LOGGER_H_
