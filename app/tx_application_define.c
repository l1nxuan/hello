#include "tx_application_define.h"
#include "sample.h"
#include "display_1.h"
#include "tx_api.h"
#include "utils.h"

static UCHAR app_memory_pool_buffer[APP_MEMORY_POOL_SIZE];
TX_BYTE_POOL app_memory_pool;

// called by _tx_initialize_kernel_enter when CPSID   i
VOID tx_application_define(VOID* first_unused_memory)
{
    (void)first_unused_memory;

    // static-pool-based dynamic allocation for general use
    UINT ret = tx_byte_pool_create(&app_memory_pool, "app memory pool", app_memory_pool_buffer,
        APP_MEMORY_POOL_SIZE);
    if (ret != TX_SUCCESS) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }

    sample_thread_create();
    display_1_thread_create();
}
