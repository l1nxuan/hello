#include "lvgl.h"

#if defined(LV_USE_LOG) && defined(LV_LOG_PRINT_CB)
#include "SEGGER_RTT.h"

void lv_log_printf_rtt(lv_log_level_t level, const char* buf)
{
    const char* color = "";
    const char* prefix = "";

    switch (level) {
    case LV_LOG_LEVEL_TRACE:
        color = "\x1B[90m"; // Gray
        prefix = "[TRACE]";
        break;
    case LV_LOG_LEVEL_INFO:
        color = "\x1B[32m"; // Green
        prefix = "[INFO]";
        break;
    case LV_LOG_LEVEL_WARN:
        color = "\x1B[33m"; // Yellow
        prefix = "[WARN]";
        break;
    case LV_LOG_LEVEL_ERROR:
        color = "\x1B[31m"; // Red
        prefix = "[ERROR]";
        break;
    case LV_LOG_LEVEL_USER:
        color = "\x1B[36m"; // Cyan
        prefix = "[USER]";
        break;
    default:
        break;
    }

    SEGGER_RTT_printf(0, "%s%s\x1B[0m %s", color, prefix, buf);
}
#endif
