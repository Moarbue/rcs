#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdio.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_NONE
} Logging_Level;

#define log_debug(fmt, ...)   log_message(LOG_DEBUG,   __FILE__, __LINE__, fmt, __VA_ARGS__)
#define log_info(fmt, ...)    log_message(LOG_INFO,    __FILE__, __LINE__, fmt, __VA_ARGS__)
#define log_warning(fmt, ...) log_message(LOG_WARNING, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define log_error(fmt, ...)   log_message(LOG_ERROR,   __FILE__, __LINE__, fmt, __VA_ARGS__)

void set_logging_level(Logging_Level level);
void set_logging_stream(FILE *stream);

void log_message(Logging_Level level, const char *loc, int line, const char *fmt, ...);

#endif // _LOGGING_H_