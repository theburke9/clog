#include "clog.h"

#define TAG "main"
#define OUTPUT_LOG_FILE "logs.log"

int main(void) {
    FILE* f = fopen(OUTPUT_LOG_FILE, "a");

    if (f == NULL) {
        return 1;
    }

    clog_set_output(f);

    CLOG_FATAL(TAG, "Oops, something went wrong!");
    CLOG_ERROR(TAG, "Unexpected error");
    CLOG_WARN(TAG, "This function is deprecated");
    CLOG_INFO(TAG, "Server started on port %s", "8080");
    CLOG_DEBUG(TAG, "WiFi status = %d", 1);
    CLOG_TRACE(TAG, "This is a trace");
    return 0;
}