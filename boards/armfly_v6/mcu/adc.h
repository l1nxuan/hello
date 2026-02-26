#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

typedef enum : uint32_t {
    ADC_FREQ_10KHZ = 10000,
    ADC_FREQ_DEFAULT = ADC_FREQ_10KHZ,
} adc_freq_t; // hz

void adc1_init(void);
void adc2_init(void);
void adc3_init(void);

#define adc_init()   \
    do {             \
        adc1_init(); \
        adc2_init(); \
        adc3_init(); \
    } while (0)

void adc_start(ADC_HandleTypeDef* hadc, uint16_t* buffer, uint32_t len);
void adc_stop(ADC_HandleTypeDef* hadc);
void adc_freq_update(adc_freq_t new_freq);

#ifdef __cplusplus
}
#endif

#endif // ADC_H
