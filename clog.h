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

#ifndef CLOG_H
#define CLOG_H

#include <stdio.h>

#define CLOG_VERSION "1.0.0"

#ifndef CLOG_LEVEL
    #define CLOG_LEVEL 3
#endif

enum clog_level { 
    CLOG_LEVEL_FATAL, 
    CLOG_LEVEL_ERROR, 
    CLOG_LEVEL_WARN, 
    CLOG_LEVEL_INFO, 
    CLOG_LEVEL_DEBUG, 
    CLOG_LEVEL_TRACE, 
};

#define CLOG_LOG(level, tag, fmt, ...) \
    do { \
        if (CLOG_LEVEL >= (level)) { \
            clog_log((level), __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define CLOG_TRACE(tag, fmt, ...) \
    CLOG_LOG(CLOG_LEVEL_TRACE, tag, fmt, ##__VA_ARGS__)

#define CLOG_DEBUG(tag, fmt, ...) \
    CLOG_LOG(CLOG_LEVEL_DEBUG, tag, fmt, ##__VA_ARGS__)

#define CLOG_INFO(tag, fmt, ...) \
    CLOG_LOG(CLOG_LEVEL_INFO, tag, fmt, ##__VA_ARGS__)

#define CLOG_WARN(tag, fmt, ...) \
    CLOG_LOG(CLOG_LEVEL_WARN, tag, fmt, ##__VA_ARGS__)

#define CLOG_ERROR(tag, fmt, ...) \
    CLOG_LOG(CLOG_LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)

#define CLOG_FATAL(tag, fmt, ...) \
    CLOG_LOG(CLOG_LEVEL_FATAL, tag, fmt, ##__VA_ARGS__)

void clog_log(
    enum clog_level level, 
    const char* file, 
    int line, 
    const char* tag, 
    const char *fmt, 
    ...
);

void clog_set_output(FILE* out);

#endif // CLOG_H
