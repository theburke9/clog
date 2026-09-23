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

#include <stdarg.h>
#include <unistd.h>
#include <time.h>

#define CLOG_INTERNAL_BUFFER_SIZE 8192

FILE* clog_output_file = NULL;

static int use_color_cache = -1;

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

#ifdef CLOG_ANSI_COLOR
static const char *clog_text_color(enum clog_level level, int use_color) {
    if (!use_color) return "";
    switch (level) {
        case CLOG_LEVEL_ERROR: return "\033[0;31m";
        case CLOG_LEVEL_WARN:  return "\033[0;33m";
        case CLOG_LEVEL_DEBUG: return "\033[0;36m";
        default:               return "";
    }
}

static const char *clog_bg_color(enum clog_level level, int use_color) {
    if (!use_color) return "";
    switch (level) {
        case CLOG_LEVEL_FATAL: return "\033[41m";
        default:               return "";
    }
}

static const char* clog_reset_color(int use_color) {
    if (!use_color) return "";
    return "\033[0m";
}
#endif

static int clog_hydrate_buffer(
    char *buffer,
    size_t buffer_size,
    const char *bg_color,
    const char *text_color,
    const struct tm *tm_info,
    const char *level_str,
    const char *tag,
    const char *file,
    int line,
    const char *fmt,
    va_list args,
    int use_color
) {
    if (buffer_size == 0) return -1;
    buffer[0] = '\0';

    int n1 = snprintf(
        buffer, buffer_size - 1,
        "%s%s%04d/%02d/%02d %02d:%02d:%02d %s (%s) %s:%d ->  ",
        bg_color, text_color,
        tm_info->tm_year+1900, tm_info->tm_mon+1, 
        tm_info->tm_mday, tm_info->tm_hour, 
        tm_info->tm_min, tm_info->tm_sec, 
        level_str, tag, file, line);

    if (n1 < 0) { return -1; }
    size_t pos = ((size_t)n1 < buffer_size) ? (size_t)n1 : buffer_size - 1;
    
    int n2 = vsnprintf(buffer + pos, buffer_size - pos, fmt, args);
    if (n2 < 0) { return -1; }
    pos = ((pos + n2) < buffer_size) ? pos + n2 : buffer_size - 1;

#ifdef CLOG_ANSI_COLOR
    int n3 = snprintf(buffer + pos, buffer_size - pos, "%s\n", clog_reset_color(use_color));
#else
    (void)use_color;
    int n3 = snprintf(buffer + pos, buffer_size - pos, "\n");
#endif
    if (n3 < 0) { return -1; }
    pos = ((pos + n3) < buffer_size) ? pos + n3 : buffer_size - 1;

    buffer[buffer_size - 1] = '\0';
    return pos;
}

void clog_log(enum clog_level level, const char* file, int line, const char* tag, const char *fmt, ...) {
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);

    FILE* out = clog_get_output(level);

#ifdef CLOG_ANSI_COLOR
    int fd = fileno(out);
    if (use_color_cache == -1) {
        use_color_cache = (fd != -1) && isatty(fd);
    }
    int use_color = use_color_cache;
    
    const char *text_color = clog_text_color(level, use_color);
    const char *bg_color = clog_bg_color(level, use_color);
#else
    int use_color = 0;
    const char *text_color = "";
    const char *bg_color = "";
#endif

    const char *level_str = clog_level_str(level);

    char buffer[CLOG_INTERNAL_BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);
    int status = clog_hydrate_buffer(
        buffer,
        CLOG_INTERNAL_BUFFER_SIZE,
        bg_color,
        text_color,
        &tm_info,
        level_str,
        tag,
        file,
        line,
        fmt,
        args,
        use_color
    );
    va_end(args);

    if (status > 0) {
        fwrite(buffer, sizeof(char), status, out);
    }

    if (level >= CLOG_LEVEL_ERROR) {
        fflush(out);
    } 
}

void clog_set_output(FILE* out) {
    if (out == NULL) return;
    clog_output_file = out;
    use_color_cache = -1;
}
