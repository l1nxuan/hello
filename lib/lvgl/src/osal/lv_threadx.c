#include "lv_os_private.h"

#if LV_USE_OS == LV_OS_THREADX
#include <stddef.h>
#include <stdint.h>
#include "../misc/lv_timer.h"
#include "../stdlib/lv_mem.h"
#include "tx_api.h"

static uint32_t lv_thread_prio_map(lv_thread_prio_t prio)
{
    const uint32_t lv_max = LV_THREAD_PRIO_HIGHEST;
    const uint32_t tx_max = TX_MAX_PRIORITIES - 1;

    if (prio > lv_max) {
        prio = lv_max;
    }
    return (tx_max * (lv_max - prio)) / lv_max;
}

lv_result_t lv_thread_init(lv_thread_t* thread, const char* const name, lv_thread_prio_t prio, void (*callback)(void*),
    size_t stack_size, void* user_data)
{
    /* All allocation sizes and addresses are aligned to 4 bytes. */
    void* stack_start = lv_malloc(stack_size);

    // e.g. LV_DRAW_THREAD_PRIO (LV_THREAD_PRIO_HIGH) -> 7 when TX_MAX_PRIORITIES == 32
    uint32_t stack_prio = lv_thread_prio_map(prio);

    UINT ret = tx_thread_create(thread, (CHAR*)name, (VOID (*)(ULONG))callback, (ULONG)user_data, stack_start,
        stack_size, stack_prio, stack_prio, TX_NO_TIME_SLICE, TX_AUTO_START);
    if (ret != TX_SUCCESS) {
        return LV_RESULT_INVALID;
    }

    return LV_RESULT_OK;
}

// Since the specified thread must be in a terminated or completed state,
// this service cannot be called from a thread attempting to delete itself.
lv_result_t lv_thread_delete(lv_thread_t* thread)
{
    UINT ret = tx_thread_delete(thread);
    if (ret != TX_SUCCESS) {
        return LV_RESULT_INVALID;
    }

    lv_free(thread->tx_thread_stack_start);

    return LV_RESULT_OK;
}

lv_result_t lv_mutex_init(lv_mutex_t* mutex)
{
    UINT ret = tx_mutex_create(mutex, "lv_mutex", TX_INHERIT);
    if (ret != TX_SUCCESS) {
        return LV_RESULT_INVALID;
    }

    return LV_RESULT_OK;
}

lv_result_t lv_mutex_lock(lv_mutex_t* mutex)
{
    UINT ret = tx_mutex_get(mutex, TX_NO_WAIT);
    if (ret != TX_SUCCESS) {
        return LV_RESULT_INVALID;
    }

    return LV_RESULT_OK;
}

lv_result_t lv_mutex_unlock(lv_mutex_t* mutex)
{
    UINT ret = tx_mutex_put(mutex);
    if (ret != TX_SUCCESS) {
        return LV_RESULT_INVALID;
    }

    return LV_RESULT_OK;
}

lv_result_t lv_thread_sync_init(lv_thread_sync_t* sync)
{
    UINT ret = tx_semaphore_create(sync, "lv_sync", 0);
    if (ret != TX_SUCCESS) {
        return LV_RESULT_INVALID;
    }

    return LV_RESULT_OK;
}

lv_result_t lv_thread_sync_wait(lv_thread_sync_t* sync)
{
    UINT ret = tx_semaphore_get(sync, TX_WAIT_FOREVER);
    if (ret != TX_SUCCESS) {
        return LV_RESULT_INVALID;
    }

    return LV_RESULT_OK;
}

lv_result_t lv_thread_sync_signal(lv_thread_sync_t* sync)
{
    UINT ret = tx_semaphore_put(sync);
    if (ret != TX_SUCCESS) {
        return LV_RESULT_INVALID;
    }

    return LV_RESULT_OK;
}

lv_result_t lv_thread_sync_signal_isr(lv_thread_sync_t* sync)
{
    return lv_thread_sync_signal(sync);
}

lv_result_t lv_thread_sync_delete(lv_thread_sync_t* sync)
{
    UINT ret = tx_semaphore_delete(sync);
    if (ret != TX_SUCCESS) {
        return LV_RESULT_INVALID;
    }

    return LV_RESULT_OK;
}

uint32_t lv_os_get_idle_percent(void)
{
    return lv_timer_get_idle();
}

void lv_sleep_ms(uint32_t ms)
{
    uint32_t sleep_ticks = ms * TX_TIMER_TICKS_PER_SECOND / 1000;
    tx_thread_sleep(sleep_ticks);
}

#endif // LV_USE_OS == LV_OS_THREADX
