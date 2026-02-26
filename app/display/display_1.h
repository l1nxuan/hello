#ifndef DISPLAY_1_H
#define DISPLAY_1_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t refresh_events;

#define REFRESH_EVENTS_DATA_READY (1U << 0)

void display_1_thread_create(void);

#ifdef __cplusplus
}
#endif

#endif // DISPLAY_1_H
