
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <defs_common.h>
#include <logging.h>

static volatile uint32_t gLoggerLogLevel = LOG_LEVEL_DEFAULT; // LOG_LEVEL_WARN;

static inline pid_t _gettid()
{
  return syscall(__NR_gettid);
}

static inline void _get_log_timestamp(char *buff)
{
    struct tm tmBuf;
    char timeBuf[32] = {0, };
    char prefixBuf[64] = {0, };

    struct timespec when;
    long ms;
    
    clock_gettime(CLOCK_REALTIME, &when);
    ms = when.tv_nsec / 1000000UL;
    //ms = round(when.tv_nsec / 1000000UL);
    if (ms > 999) {
        when.tv_sec++;
        ms -= 1000;
    }

    localtime_r(&(when.tv_sec), &tmBuf);

    strftime(timeBuf, sizeof(timeBuf), "%m-%d %H:%M:%S", &tmBuf);

    // ex 0513_160944_686
    snprintf(prefixBuf, sizeof(prefixBuf),"%s.%03ld", timeBuf, ms);

    strcpy(buff, prefixBuf);
    buff[strlen(buff)] = '\0';

    return;
}

static inline void _get_log_timestamp2(char *buff, uint32_t * out_len)
{
    struct tm tmBuf;
    char timeBuf[32] = {0, };
    char prefixBuf[64] = {0, };

    struct timespec when;
    long ms;
    uint32_t ret_len = 0;
    
    clock_gettime(CLOCK_REALTIME, &when);
    ms = when.tv_nsec / 1000000UL;
    //ms = round(when.tv_nsec / 1000000UL);
    if (ms > 999) {
        when.tv_sec++;
        ms -= 1000;
    }

    localtime_r(&(when.tv_sec), &tmBuf);

    strftime(timeBuf, sizeof(timeBuf), "%m-%d %H:%M:%S", &tmBuf);

    // ex 0513_160944_686
    snprintf(prefixBuf, sizeof(prefixBuf),"%s.%03ld ", timeBuf, ms);

    strcpy(buff, prefixBuf);
    ret_len = strlen(buff);
    buff[ret_len] = '\0';

    if (out_len != NULL) {
        *out_len = ret_len;
    }

    return;
}

VOID loggerSetLogLevel(UINT32 targetLoggerLevel)
{
    gLoggerLogLevel = targetLoggerLevel;
}

UINT32 loggerGetLogLevel()
{
    return (UINT32)gLoggerLogLevel;
}

const PCHAR getLogLevelStr(UINT32 loglevel)
{
    switch (loglevel) {
        case LOG_LEVEL_VERBOSE:
            return LOG_LEVEL_VERBOSE_STR;
        case LOG_LEVEL_DEBUG:
            return LOG_LEVEL_DEBUG_STR;
        case LOG_LEVEL_INFO:
            return LOG_LEVEL_INFO_STR;
        case LOG_LEVEL_WARN:
            return LOG_LEVEL_WARN_STR;
        case LOG_LEVEL_ERROR:
            return LOG_LEVEL_ERROR_STR;
        case LOG_LEVEL_FATAL:
            return LOG_LEVEL_FATAL_STR;
        case LOG_LEVEL_PROFILE:
            return LOG_LEVEL_PROFILE_STR;
        default:
            return LOG_LEVEL_SILENT_STR;
    }
}

VOID addLogMetadata(const PCHAR buffer, UINT32 bufferLen, const PCHAR tag, const PCHAR fmt, UINT32 logLevel)
{
#define MIN_LOG_TAG_WIDTH       (8)

    UINT32 timeStrLen = 0;
    /* space for "yyyy-mm-dd HH:MM:SS.MMMMMM" + space + null */
    CHAR timeString[64];
    STATUS retStatus = STATUS_SUCCESS;
    UINT32 offset = 0;

    CHAR pidString[32];
    
    pid_t pid = getpid();
    SNPRINTF(pidString, ARRAY_SIZE(pidString), "%5d", pid);

    CHAR tidString[32];
    pid_t tid = _gettid();
    SNPRINTF(tidString, ARRAY_SIZE(tidString), "%5d", tid);

    CHAR levelString[32];
    SNPRINTF(levelString, ARRAY_SIZE(levelString), "%s", getLogLevelStr(logLevel));

    CHAR tagString[64];
    SNPRINTF(tagString, ARRAY_SIZE(tagString), "%s", tag);

    _get_log_timestamp2(timeString, &timeStrLen);
    
    offset = (UINT32) SNPRINTF(buffer
                            , timeStrLen + MAX_LOG_LEVEL_STRLEN + 2
                            , "%s"
                            , timeString);
    
    offset += SNPRINTF(buffer + offset, ARRAY_SIZE(pidString), "%s ", pidString);
    offset += SNPRINTF(buffer + offset, ARRAY_SIZE(tidString), "%s ", tidString);

    offset += SNPRINTF(buffer + offset, ARRAY_SIZE(levelString)
                            , "%-*s "
                            , MAX_LOG_LEVEL_STRLEN, getLogLevelStr(logLevel));

    offset += SNPRINTF(buffer + offset, ARRAY_SIZE(tagString), "%*s : ", MIN_LOG_TAG_WIDTH, tagString);

    SNPRINTF(buffer + offset, bufferLen - offset, "%s\n", fmt);

    UNUSED(retStatus);
}

//
// Default logger function
//
VOID defaultLogPrint(UINT32 level, const PCHAR tag, const PCHAR fmt, ...)
{
    CHAR logFmtString[256] = {0, };
    UINT32 logLevel = loggerGetLogLevel();

    if (logLevel != LOG_LEVEL_SILENT && level >= logLevel) {
        addLogMetadata(logFmtString, (UINT32) ARRAY_SIZE(logFmtString), tag, fmt, level);

        va_list valist;
        va_start(valist, fmt);
        vprintf(logFmtString, valist);
        va_end(valist);
    }
}

logPrintFunc getGlobalCustomLogPrintFn(VOID)
{
    return defaultLogPrint;
}
