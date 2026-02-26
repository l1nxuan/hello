#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "swtimer.h"

typedef _Atomic uint8_t atomic_uint8_t;

typedef struct {
    uint32_t reload;
    uint32_t current; // down-counter for remaining time
    swtimer_mode_t mode; // TODO: change to reschedule_ticks
    atomic_uint8_t count; // number of times timer has expired
    atomic_bool enable;
    // 1 byte padding
} swtimer_t;

static swtimer_t timers[SWTIMER_COUNT];

bool swtimer_start(swtimer_id_t id, uint32_t period, swtimer_mode_t mode)
{
    swtimer_t* timer = timers + id;
    if (period == 0 || atomic_load_explicit((bool*)&timer->enable, memory_order_acquire)) {
        return false;
    }

    timer->reload = period;
    timer->current = period;
    timer->mode = mode;
    atomic_store_explicit((uint8_t*)&timer->count, 0, memory_order_relaxed);
    atomic_store_explicit((bool*)&timer->enable, true, memory_order_release);
    return true;
}

// attach this to a reloadic interrupt (e.g., SysTick) to update timers
void swtimer_update(void)
{
    for (uint8_t i = 0; i < SWTIMER_COUNT; i++) {
        swtimer_t* timer = timers + i;
        if (!atomic_load_explicit((bool*)&timer->enable, memory_order_acquire)) {
            continue;
        }

        if (--timer->current == 0) {
            atomic_fetch_add_explicit((uint8_t*)&timer->count, 1, memory_order_relaxed);
            if (timer->mode == SWTIMER_MODE_PERIODIC) {
                timer->current = timer->reload;
            } else {
                atomic_store_explicit((bool*)&timer->enable, false, memory_order_release);
            }
        }
    }
}

bool swtimer_stop(swtimer_id_t id)
{
    swtimer_t* timer = timers + id;
    atomic_store_explicit((bool*)&timer->enable, false, memory_order_seq_cst);
    atomic_store_explicit((uint8_t*)&timer->count, 0, memory_order_relaxed);
    return true; // best-effort
}

// check if timer has expired and clear the flag, thread-safe
bool swtimer_is_expired(swtimer_id_t id)
{
    swtimer_t* timer = timers + id;
    uint8_t expected = atomic_load_explicit((uint8_t*)&timer->count, memory_order_relaxed);
    do {
        if (expected == 0) {
            return false;
        }
    } while (!atomic_compare_exchange_weak_explicit((uint8_t*)&timer->count, &expected, expected - 1,
        memory_order_relaxed, memory_order_relaxed));
    return true;
}
