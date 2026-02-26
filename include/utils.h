#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b) ({        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b;      \
})

#define MAX(a, b) ({        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b;      \
})

#define CLAMP(x, min, max) ({                   \
    __typeof__(x) _x = (x);                     \
    __typeof__(min) _min = (min);               \
    __typeof__(max) _max = (max);               \
    _x < _min ? _min : (_x > _max ? _max : _x); \
})

#ifdef DEBUG
#include "SEGGER_RTT.h"
// #include "cmsis_gcc.h"

#define INF0  "\x1B[32m[INFO]\x1B[0m"  // green
#define ERR0R "\x1B[31m[ERROR]\x1B[0m" // red

#define PRINTF(fmt, prefix, ...)                                                             \
    do {                                                                                     \
        SEGGER_RTT_printf(0, prefix "[%s:%d] " fmt, __FILE_NAME__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#endif // DEBUG

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
