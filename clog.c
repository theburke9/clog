/**
* Copyright 2026 jayson.mourier@proton.me
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include "clog.h"

FILE* clog_output_file = NULL;

void clog_set_output(FILE* out) {
    if (out == NULL) return;
    clog_output_file = out;
}

static FILE* clog_get_output(enum clog_level level) {
    return clog_output_file ? clog_output_file : (level < CLOG_LEVEL_WARN ? stderr : stdout);
}

static const char *clog_level_str(enum clog_level level) {
    switch (level) {
        case CLOG_LEVEL_FATAL: return "FATAL";
        case CLOG_LEVEL_ERROR: return "ERROR";
        case CLOG_LEVEL_WARN:  return "WARN";
        case CLOG_LEVEL_INFO:  return "INFO";
        case CLOG_LEVEL_DEBUG: return "DEBUG";
        case CLOG_LEVEL_TRACE: return "TRACE";
        default:               return "UNKNOWN";
    }
}

static const char *clog_text_color(enum clog_level level, int use_color) {
#ifdef CLOG_ANSI_COLOR
    if (!use_color) return "";
    switch (level) {
        case CLOG_LEVEL_ERROR: return "\033[0;31m";
        case CLOG_LEVEL_WARN:  return "\033[0;33m";
        case CLOG_LEVEL_DEBUG: return "\033[0;36m";
        default:               return "";
    }
#else
    return "";
#endif
}

static const char *clog_bg_color(enum clog_level level, int use_color) {
#ifdef CLOG_ANSI_COLOR
    if (!use_color) return "";
    switch (level) {
        case CLOG_LEVEL_FATAL: return "\033[41m";
        default:               return "";
    }
#else
    return "";
#endif
}

static const char* clog_reset_color(int use_color) {
#ifdef CLOG_ANSI_COLOR
    if (!use_color) return "";
    return "\033[0m";
#else
    return "";
#endif
}

void clog_log(enum clog_level level, const char* file, int line, const char* tag, const char *fmt, ...) {
    time_t timestamp = time(NULL);
    struct tm * time_infos = localtime(&timestamp);

    FILE* out = clog_get_output(level);

#ifdef CLOG_ANSI_COLOR
    int fd = fileno(out);
    int use_color = (fd != -1) && isatty(fd);
#else
    int use_color = 0;
#endif

    const char *level_str = clog_level_str(level);
    const char *text_color = clog_text_color(level, use_color);
    const char *bg_color = clog_bg_color(level, use_color);
    
    fprintf(
        out,
        "%s%s%04d/%02d/%02d %02d:%02d:%02d %s (%s) %s:%d -> ",
        bg_color,
        text_color,
        time_infos->tm_year+1900, 
        time_infos->tm_mon+1, 
        time_infos->tm_mday, 
        time_infos->tm_hour, 
        time_infos->tm_min, 
        time_infos->tm_sec, 
        level_str, 
        tag, 
        file, 
        line
    );

    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);

    fprintf(out, "%s\n", clog_reset_color(use_color));
}
