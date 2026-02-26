#ifndef LV_THREADX_H
#define LV_THREADX_H

#include "tx_api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef TX_THREAD lv_thread_t;

typedef TX_MUTEX lv_mutex_t;

typedef TX_SEMAPHORE lv_thread_sync_t;

#ifdef __cplusplus
}
#endif

#endif // LV_THREADX_H
