#ifndef DEMO_WAVEFORM_H
#define DEMO_WAVEFORM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEMO_WAVEFORM_LEN 128

uint32_t demo_sinewave_gen(uint16_t* buffer, uint16_t vpp);
uint32_t demo_squarewave_gen(uint16_t* buffer, uint16_t high_level, uint16_t low_level, uint8_t duty_cycle);
uint32_t demo_trianglewave_gen(uint16_t* buffer, uint16_t high_level, uint16_t low_level, uint8_t duty_cycle);

#ifdef __cplusplus
}
#endif

#endif // DEMO_WAVEFORM_H
