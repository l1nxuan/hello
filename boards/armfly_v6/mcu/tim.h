#ifndef TIM_H
#define TIM_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

bool tim_timebase_config(TIM_HandleTypeDef* htim, uint32_t freq);

#ifdef __cplusplus
}
#endif

#endif // TIM_H
