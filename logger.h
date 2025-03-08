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

static const char *loglvlToString[10] = {
    "NOLOG",
    "\033[1;31mFATAL\033[0;37m",
    "\033[0;31mERROR\033[0;37m",
    "\033[1;33mWARNING\033[0;37m",
    "\033[0;32mBENCH\033[0;37m",
    "\033[0;32mINFO\033[0;37m",
    "\033[0;34mFIXME\033[0;37m",
    "\033[1;35mDEBUG\033[0;37m",
    "\033[0;35mTRACE\033[0;37m",
    "ALL"
};

static const char *loglvlToStringNoColor[10] = {
    "NOLOG",
    "FATAL",
    "ERROR",
    "WARNING",
    "BENCH",
    "INFO",
    "FIXME",
    "DEBUG",
    "TRACE",
    "ALL"
};


static const enum loglevel DEFAULT_LOG_LEVEL = WARNING;
extern enum loglevel CURRENT_LOG_LEVEL_CONSOLE;
extern enum loglevel CURRENT_LOG_LEVEL_FILE;
extern bool BENCHMARKING;
extern FILE *BENCH_LOG_FILE_PTR;
extern char LOG_FILE_NAME[64];
extern FILE *LOG_FILE_PTR;

static const char *restrict BENCH_LOG_FILE_NAME = "benchlog.log";

#define LOG(logLevel, format, ...) Logger(__FILE__, __FUNCTION__, __LINE__, logLevel, format, __VA_ARGS__)


void Logger(const char *restrict inFile, const char *restrict inFunc,
            const int onLine, const enum loglevel loglevel,
            const char *restrict format, ...);

int InitLogger(void);


#endif // LOGGER_H_
