#ifndef TX_APPLICATION_DEFINE_H
#define TX_APPLICATION_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

// extern TX_BYTE_POOL app_memory_pool;
#define APP_MEMORY_POOL_SIZE        (1 * 1024) // bytes

// threadx stack size
#define DISPLAY_1_THREAD_STACK_SIZE (4 * 1024) // bytes
#define SAMPLE_THREAD_STACK_SIZE    (4 * 1024)

// threadx priority
#define DISPLAY_1_THREAD_PRIORITY   30
#define SAMPLE_THREAD_PRIORITY      0

#ifdef __cplusplus
}
#endif

#endif // TX_APPLICATION_DEFINE_H
