#include <stdint.h>
#include "tx_application_define.h"
#include "display_1.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tx_api.h"
#include "utils.h"

#include "demo_waveform.h"
#include "dac.h"

TX_THREAD convert_thread __attribute__((section(".ccmram")));
UCHAR convert_thread_stack[SAMPLE_THREAD_STACK_SIZE] __attribute__((section(".ccmram")));

TX_SEMAPHORE convcplt_sem __attribute__((section(".ccmram")));

#define SAMPLE_POINTS_PERIOD 128
static uint16_t _adc_buffer[SAMPLE_POINTS_PERIOD];

static volatile uint8_t serie_1_buffer_idx = 512 / 128; // ConvCplt times
extern int32_t chart_1_serie_1_buffer[512];

VOID sample_thread_entry(ULONG thread_input)
{
    (VOID) thread_input;

    // TODO: Waveform Generator
    static uint16_t _dac_buffer[DEMO_WAVEFORM_LEN];
    (void)demo_squarewave_gen(_dac_buffer, 0XCCC, 0x333, 50);
    dac_start(_dac_buffer, DEMO_WAVEFORM_LEN); // f = DAC_FREQ_DEFAULT / DEMO_WAVEFORM_LEN = 78.125Hz

    // trigger: Tx_CCx | record: DMAx_Streamx
    adc_start(&hadc1, _adc_buffer, SAMPLE_POINTS_PERIOD);

    (VOID) tx_semaphore_create(&convcplt_sem, "convcplt sem", 0);

    while (1) {
        UINT ret = tx_semaphore_get(&convcplt_sem, TX_WAIT_FOREVER);
        if (ret != TX_SUCCESS) {
            PRINTF("%s no success, ret = %d.\n", INF0, "tx_semaphore_get", ret);
        }
    }
}

void sample_thread_create(void)
{
    (VOID) tx_thread_create(&convert_thread, "convert", sample_thread_entry, 0,
        convert_thread_stack, SAMPLE_THREAD_STACK_SIZE, SAMPLE_THREAD_PRIORITY, SAMPLE_THREAD_PRIORITY,
        TX_NO_TIME_SLICE, TX_AUTO_START);
}

// override the native HAL ADC DMA transfer callbacks (weak-defined)
static inline void copy_reverse(int32_t* dst, uint16_t* src, uint16_t len)
{
    const uint16_t* s = src + len;
    int32_t* d = dst;

    while (s > src) {
        *d++ = (int32_t)*(--s);
    }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1) {
        uint16_t offset = (serie_1_buffer_idx - 1) * SAMPLE_POINTS_PERIOD;
        copy_reverse(chart_1_serie_1_buffer + offset + (SAMPLE_POINTS_PERIOD >> 1), _adc_buffer,
            SAMPLE_POINTS_PERIOD >> 1);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1) {
        uint16_t offset = (serie_1_buffer_idx - 1) * SAMPLE_POINTS_PERIOD;
        copy_reverse(chart_1_serie_1_buffer + offset, _adc_buffer + (SAMPLE_POINTS_PERIOD >> 1),
            SAMPLE_POINTS_PERIOD >> 1);

        serie_1_buffer_idx--;
        if (serie_1_buffer_idx == 0) {
            serie_1_buffer_idx = 512 / 128;
            tx_event_flags_set(&refresh_events, REFRESH_EVENTS_DATA_READY, TX_OR);
        }
    }
}
