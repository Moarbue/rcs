#include "logging.h"

#include <stdarg.h>
#include <time.h>

static const char *MESSAGE_TYPE_TO_STRING[] = {
    [LOG_DEBUG]   = "DEBUG  ",
    [LOG_INFO]    = "INFO   ",
    [LOG_WARNING] = "WARNING",
    [LOG_ERROR]   = "ERROR  ",
};

FILE *f = NULL;
Logging_Level logging_level = LOG_INFO;

void set_logging_level(Logging_Level level)
{
    logging_level = level;
}

// Important: the stream must be opened first
void set_logging_stream(FILE *stream)
{
    f = stream;
}

// Convert current time to string
void get_time_formated(char *time_str, int len)
{
    // minimal length of string
    if (len < 9) return;

    time_t current_time;
    struct tm *time_info;

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(time_str, len, "%H:%M:%S", time_info);
}

void log_message(Logging_Level level, const char *loc, int line, const char *fmt, ...)
{
    if (level < logging_level) return;

    // default to stderr
    if (f == NULL) f = stderr;

    char time_str[9];
    get_time_formated(time_str, sizeof(time_str));

    va_list args;
    va_start(args, fmt);

    fprintf (f, "%s %s %s:%d: ", time_str, MESSAGE_TYPE_TO_STRING[level], loc, line);
    vfprintf(f, fmt, args);
    fprintf (f, "\n");

    va_end(args);
}