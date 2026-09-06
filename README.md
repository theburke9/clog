# CLOG - A simple log library

Clog is a simple log library easy to use in your C programs.

## Features

- log levels (TRACE, DEBUG, INFO, WARN, ERROR, FATAL)
- no external dependencies
- supports ANSI color

## Installation

Copy `clog.h` into your project and include it:

```c
#include "clog.h"
```

## API 

| Function | Description |
|---|---|
| `clog_trace(fmt, ...)` | Log a trace message |
| `clog_debug(fmt, ...)` | Log a debug message |
| `clog_info(fmt, ...)` | Log an info message |
| `clog_warn(fmt, ...)` | Log a warning message |
| `clog_error(fmt, ...)` | Log an error message |
| `clog_fatal(fmt, ...)` | Log a fatal message |

## Usage

```c
#include "clog.h"

int main(void) {
    clog_info("hello, world!");
    
    int x = 5;
    clog_warn("x = %d", 5);
    
    return 0;
}
```

## Licence

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details