#include "clog.h"

int main(void) {
    CLOG_FATAL("main", "Oops, something went wrong!");
    CLOG_ERROR("main", "Unexpected error");
    CLOG_WARN("main", "This function is deprecated");
    CLOG_INFO("main", "Server started on port %s", "8080");
    CLOG_DEBUG("main", "WiFi status = %d", 1);
    CLOG_TRACE("main", "This is a trace");
    return 0;
}