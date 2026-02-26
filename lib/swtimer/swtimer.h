#ifndef SWTIMER_H
#define SWTIMER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// x-macro to define software timer IDs
typedef enum : uint8_t {
#define SWTIMER_ID(name) SWTIMER_ID_##name,
#include "swtimer.def" // e.g. SWTIMER_ID(KEY_SCAN)
#undef SWTIMER_ID
    SWTIMER_COUNT,
} swtimer_id_t;

typedef enum : uint8_t {
    SWTIMER_MODE_ONESHOT = 0,
    SWTIMER_MODE_PERIODIC,
} swtimer_mode_t;

bool swtimer_start(swtimer_id_t id, uint32_t period, swtimer_mode_t mode);
void swtimer_update(void);
bool swtimer_stop(swtimer_id_t id);
bool swtimer_is_expired(swtimer_id_t id);

#ifdef __cplusplus
}
#endif

#endif // SWTIMER_H
