#ifndef DAC_H
#define DAC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum : uint32_t {
    DAC_FREQ_10KHZ = 10000,
    DAC_FREQ_DEFAULT = DAC_FREQ_10KHZ,
} dac_freq_t; // hz

void dac_init(void);
void dac_start(uint16_t* buffer, uint32_t len);
void dac_stop(void);
void dac_freq_update(dac_freq_t new_freq);

#ifdef __cplusplus
}
#endif

#endif // DAC_H
