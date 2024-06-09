/* 
 * Copyright (c) 2024 Litbig
 * Litbig PROPRIETARY/CONFIDENTIAL 
 */

#ifndef _COMMON_LOGGER_H_
#define _COMMON_LOGGER_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>

#include <defs_common.h>

/**
 * In the top of your file, write as follow
 * 
 * ex1) use log
 *  
 *  #define LOG_TAG "app-hello"
 *  #define LOG_NDEBUG 0
 *  #include <common/logging.h>
 *
 *  
 * ex2) disable log
 * 
 *  #define LOG_TAG "app-hello"
 *  #define LOG_NDEBUG 1
 *  #include <common/logging.h>
 * 
 * ex) use log, but disable function name print
 * 
 *  #define LOG_TAG "app-hello"
 *  #define LOG_NDEBUG 0
 *  #define LOG_NSHOW_FUNC 1
 *  #include <common/logging.h>
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//=============================

#ifndef LOG_TAG
#define LOG_TAG "null"
#endif

#ifdef DETECTED_GIT_HASH
#define GIT_HASH SDK_VERSION
#else
#define GIT_HASH EMPTY_STRING
#endif

// Log print function definition
typedef VOID (*logPrintFunc)(UINT32, const PCHAR, const PCHAR, ...);

//
// Default logger function
//
PUBLIC_API VOID defaultLogPrint(UINT32 level, const PCHAR tag, const PCHAR fmt, ...);

extern VOID loggerSetLogLevel(UINT32 targetLoggerLevel);
extern UINT32 loggerGetLogLevel(VOID);
extern logPrintFunc getGlobalCustomLogPrintFn(VOID);

#ifdef USE_ANDROID_LOGGER
#include <cutils/log.h>
    #if LOG_NDEBUG
        #define __ASSERT(cond, ...)                 do { if (0) { ALOG_ASSERT(cond, __VA_ARGS__); } } while (0)
        #define __LOG(prio, tag, fmt, ...)          do { if (0) { android_printLog(prio, tag, fmt, ##__VA_ARGS__); } } while (0)
    #else
        #define __ASSERT(cond, ...)                 ALOG_ASSERT(cond, __VA_ARGS__)
        #define __LOG(prio, tag, fmt, ...) do { \
                UINT32 logLevel = loggerGetLogLevel(); \
                if (logLevel != LOG_LEVEL_SILENT && prio >= logLevel) { \
                    android_printLog(prio, tag, fmt, ##__VA_ARGS__);    \
                }                                                       \
            } while (0)
    #endif
#else
    #define __ASSERT(p1, p2, p3, ...)   assert(p1)
    #define __LOG                       getGlobalCustomLogPrintFn()
#endif /* USE_ANDROID_LOGGER */

#define LOG_LEVEL_UNKNOWN (0)
#define LOG_LEVEL_DEFAULT (1)
#define LOG_LEVEL_VERBOSE (2)
#define LOG_LEVEL_DEBUG   (3)
#define LOG_LEVEL_INFO    (4)
#define LOG_LEVEL_WARN    (5)
#define LOG_LEVEL_ERROR   (6)
#define LOG_LEVEL_FATAL   (7)
#define LOG_LEVEL_SILENT  (8)

// Adding this after LOG_LEVEL_SILENT to ensure we do not break backward compat
#define LOG_LEVEL_PROFILE 8

#define LOG_LEVEL_VERBOSE_STR (const PCHAR) "V"     // "VERBOSE"
#define LOG_LEVEL_DEBUG_STR   (const PCHAR) "D"     // "DEBUG"
#define LOG_LEVEL_INFO_STR    (const PCHAR) "I"     // "INFO"
#define LOG_LEVEL_WARN_STR    (const PCHAR) "W"     // "WARN"
#define LOG_LEVEL_ERROR_STR   (const PCHAR) "E"     // "ERROR"
#define LOG_LEVEL_FATAL_STR   (const PCHAR) "F"     // "FATAL"
#define LOG_LEVEL_SILENT_STR  (const PCHAR) "S"     // "SILENT"
#define LOG_LEVEL_PROFILE_STR (const PCHAR) "P"     // "PROFILE"

// LOG_LEVEL_VERBOSE_STR string length
#define MAX_LOG_LEVEL_STRLEN 1

// Extra logging macros
#ifndef DLOGE
    #if LOG_NSHOW_FUNC
        #define DLOGE(fmt, ...) __LOG(LOG_LEVEL_ERROR, (const PCHAR) LOG_TAG, (const PCHAR) fmt, ##__VA_ARGS__)
    #else
        #define DLOGE(fmt, ...) __LOG(LOG_LEVEL_ERROR, (const PCHAR) LOG_TAG, (const PCHAR) "%s() " fmt, __FUNCTION__, ##__VA_ARGS__)
    #endif
#endif

#ifndef DLOGW
    #if LOG_NSHOW_FUNC
        #define DLOGW(fmt, ...) __LOG(LOG_LEVEL_WARN, (const PCHAR) LOG_TAG, (const PCHAR) fmt, ##__VA_ARGS__)
    #else
        #define DLOGW(fmt, ...) __LOG(LOG_LEVEL_WARN, (const PCHAR) LOG_TAG, (const PCHAR) "%s() " fmt, __FUNCTION__, ##__VA_ARGS__)
    #endif
#endif

#ifndef DLOGI
    #if LOG_NSHOW_FUNC
        #define DLOGI(fmt, ...) __LOG(LOG_LEVEL_INFO, (const PCHAR) LOG_TAG, (const PCHAR) fmt, ##__VA_ARGS__)
    #else
        #define DLOGI(fmt, ...) __LOG(LOG_LEVEL_INFO, (const PCHAR) LOG_TAG, (const PCHAR) "%s() " fmt, __FUNCTION__, ##__VA_ARGS__)
    #endif
#endif

#ifndef DLOGD
    #if LOG_NSHOW_FUNC
        #define DLOGD(fmt, ...) __LOG(LOG_LEVEL_DEBUG, (const PCHAR) LOG_TAG, (const PCHAR) fmt, ##__VA_ARGS__)
    #else
        #define DLOGD(fmt, ...) __LOG(LOG_LEVEL_DEBUG, (const PCHAR) LOG_TAG, (const PCHAR) "%s() " fmt, __FUNCTION__, ##__VA_ARGS__)
    #endif
#endif

#ifndef DLOGV
    #if LOG_NSHOW_FUNC
        #define DLOGV(fmt, ...) __LOG(LOG_LEVEL_VERBOSE, (const PCHAR) LOG_TAG, (const PCHAR) fmt, ##__VA_ARGS__)
    #else
        #define DLOGV(fmt, ...) __LOG(LOG_LEVEL_VERBOSE, (const PCHAR) LOG_TAG, (const PCHAR) "%s() " fmt, __FUNCTION__, ##__VA_ARGS__)
    #endif
#endif

#ifndef DLOGP
    #if LOG_NSHOW_FUNC
        #define DLOGP(fmt, ...) __LOG(LOG_LEVEL_PROFILE, (const PCHAR) LOG_TAG, (const PCHAR) fmt, ##__VA_ARGS__)
    #else
        #define DLOGP(fmt, ...) __LOG(LOG_LEVEL_PROFILE, (const PCHAR) LOG_TAG, (const PCHAR) "%s() " fmt, __FUNCTION__, ##__VA_ARGS__)
    #endif
#endif

#ifndef DV
#define DV(x, ...) DLOGV(x, ##__VA_ARGS__)
#endif

#ifndef DD
#define DD(x, ...) DLOGD(x, ##__VA_ARGS__)
#endif

#ifndef DI
#define DI(x, ...) DLOGI(x, ##__VA_ARGS__)
#endif

#ifndef DW
#define DW(x, ...) DLOGW(x, ##__VA_ARGS__)
#endif

#ifndef DE
#define DE(x, ...) DLOGE(x, ##__VA_ARGS__)
#endif

#ifndef ENTER
#define ENTER(...) DLOGV(">>>>> Enter : " __VA_ARGS__)
#endif

#ifndef LEAVE
#define LEAVE(...) DLOGV("<<<<< Leave : " __VA_ARGS__)
#endif

#ifndef ENTERS
#define ENTERS() DLOGS("Enter")
#endif

#ifndef LEAVES
#define LEAVES() DLOGS("Leave")
#endif

// Optionally log very verbose data (>200 msgs/second) about the streaming process
#ifndef DLOGS
#ifdef LOG_STREAMING
#define DLOGS DLOGV
#else
#define DLOGS(...)                                                                                                                                   \
    do {                                                                                                                                             \
    } while (0)
#endif
#endif

// Assertions
#ifndef CONDITION
#ifdef __GNUC__
#define CONDITION(cond) (__builtin_expect((cond) != 0, 0))
#else
#define CONDITION(cond) ((cond) == TRUE)
#endif
#endif
#ifndef LOG_ALWAYS_FATAL_IF
#define LOG_ALWAYS_FATAL_IF(cond, ...) ((CONDITION(cond)) ? ((void) __ASSERT(FALSE, (const PCHAR) LOG_TAG, ##__VA_ARGS__)) : (void) 0)
#endif

#ifndef LOG_ALWAYS_FATAL
#define LOG_ALWAYS_FATAL(...) (((void) __ASSERT(FALSE, (const PCHAR) LOG_TAG, ##__VA_ARGS__)))
#endif

#ifndef SANITIZED_FILE
#define SANITIZED_FILE (STRRCHR(__FILE__, '/') ? STRRCHR(__FILE__, '/') + 1 : __FILE__)
#endif
#ifndef CRASH
#define CRASH(fmt, ...) LOG_ALWAYS_FATAL("%s::%s: " fmt, (const PCHAR) LOG_TAG, __FUNCTION__, ##__VA_ARGS__)
#endif
#ifndef CHECK
#define CHECK(x) LOG_ALWAYS_FATAL_IF(!(x), "%s::%s: ASSERTION FAILED at %s:%d: " #x, (const PCHAR) LOG_TAG, __FUNCTION__, SANITIZED_FILE, __LINE__)
#endif
#ifndef CHECK_EXT
#define CHECK_EXT(x, fmt, ...)                                                                                                                       \
    LOG_ALWAYS_FATAL_IF(!(x), "%s::%s: ASSERTION FAILED at %s:%d: " fmt, (const PCHAR) LOG_TAG, __FUNCTION__, SANITIZED_FILE, __LINE__,            \
                        ##__VA_ARGS__)
#endif

#ifndef LOG_GIT_HASH
#define LOG_GIT_HASH() DLOGI("SDK version: %s", GIT_HASH)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _COMMON_LOGGER_H_ */
