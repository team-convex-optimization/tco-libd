#include <stdarg.h>
#include <stdio.h>

#include <time.h>
#include <string.h>
#include <fcntl.h>

#include "tco_libd.h"

static FILE *log_file;
static char *caller_name;
extern int log_level; /* Should be set to some combination of "log_flags" OR'd together */

static void log_format(const char *tag, const char *message, va_list args)
{
    time_t now;
    time(&now);
    char *date = ctime(&now);
    date[strlen(date) - 1] = '\0';
    fprintf(log_file, " %s [%s - %s] ", date, caller_name, tag);
    vfprintf(log_file, message, args);
    fprintf(log_file, "\n");
}

int log_init(char *caller, const char *log_path)
{
    if (log_level != LOG_OFF)
    {
        caller_name = caller;
        log_file = fopen(log_path, "a");
        if (log_file == NULL)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

void log_error(const char *message, ...)
{
    if (log_level & LOG_ERROR)
    {
        va_list args;
        va_start(args, message);
        log_format("error", message, args);
        va_end(args);
    }
}

void log_info(const char *message, ...)
{
    if (log_level & LOG_INFO)
    {
        va_list args;
        va_start(args, message);
        log_format("info", message, args);
        va_end(args);
    }
}

void log_debug(const char *message, ...)
{
    if (log_level & LOG_DEBUG)
    {
        va_list args;
        va_start(args, message);
        log_format("debug", message, args);
        va_end(args);
    }
}
