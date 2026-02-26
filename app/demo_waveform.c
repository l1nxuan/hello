#include <stdint.h>
#include "demo_waveform.h"
#include "utils.h"

const uint16_t _sinewave[DEMO_WAVEFORM_LEN] = {
    2047, 2147, 2248, 2347, 2446, 2544, 2641, 2737, 2830, 2922,
    3012, 3099, 3184, 3266, 3346, 3422, 3494, 3564, 3629, 3691,
    3749, 3803, 3852, 3897, 3938, 3974, 4006, 4033, 4055, 4072,
    4084, 4092, 4094, 4092, 4084, 4072, 4055, 4033, 4006, 3974,
    3938, 3897, 3852, 3803, 3749, 3691, 3629, 3564, 3494, 3422,
    3346, 3266, 3184, 3099, 3012, 2922, 2830, 2737, 2641, 2544,
    2446, 2347, 2248, 2147, 2047, 1947, 1846, 1747, 1648, 1550,
    1453, 1357, 1264, 1172, 1082, 995, 910, 828, 748, 672,
    600, 530, 465, 403, 345, 291, 242, 197, 156, 120,
    88, 61, 39, 22, 10, 2, 0, 2, 10, 22,
    39, 61, 88, 120, 156, 197, 242, 291, 345, 403,
    465, 530, 600, 672, 748, 828, 910, 995, 1082, 1172,
    1264, 1357, 1453, 1550, 1648, 1747, 1846, 1947
};

uint32_t demo_sinewave_gen(uint16_t* buffer, uint16_t vpp)
{
    uint32_t scale = ((uint32_t)vpp << 16) / 0xFFF; // 12-bit mode
    for (uint8_t i = 0; i < DEMO_WAVEFORM_LEN; i++) {
        uint32_t value = (_sinewave[i] * scale) >> 16;
        buffer[i] = value < 0xFFF ? value : 0xFFF;
    }

    return DEMO_WAVEFORM_LEN;
}

uint32_t demo_squarewave_gen(uint16_t* buffer, uint16_t high_level, uint16_t low_level, uint8_t duty_cycle)
{
    uint8_t high_cnt = (duty_cycle * DEMO_WAVEFORM_LEN + 50) / 100;
    uint8_t low_cnt = DEMO_WAVEFORM_LEN - high_cnt;

    while (high_cnt--) {
        *(buffer++) = high_level;
    }

    while (low_cnt--) {
        *buffer++ = low_level;
    }

    return DEMO_WAVEFORM_LEN;
}

uint32_t demo_trianglewave_gen(uint16_t* buffer, uint16_t high_level, uint16_t low_level, uint8_t duty_cycle)
{
    uint8_t rise_cnt = CLAMP(DEMO_WAVEFORM_LEN * duty_cycle / 100, 1, DEMO_WAVEFORM_LEN - 1);
    uint8_t fall_cnt = DEMO_WAVEFORM_LEN - rise_cnt;
    uint16_t vpp = high_level - low_level;

    // rise edge
    uint32_t step = ((uint32_t)vpp << 16) / rise_cnt;
    for (uint8_t i = 0; i < rise_cnt; i++) {
        buffer[i] = low_level + ((step * i + (1 << 15)) >> 16);
    }

    // fall edge
    step = ((uint32_t)vpp << 16) / fall_cnt;
    for (uint8_t i = 0; i < fall_cnt; i++) {
        buffer[rise_cnt + i] = high_level - ((step * i + (1 << 15)) >> 16);
    }

    return DEMO_WAVEFORM_LEN;
}
