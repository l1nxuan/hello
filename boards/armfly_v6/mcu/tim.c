#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

// clang-format off
#define IS_APB1_TIM(INSTANCE)  (((INSTANCE) == TIM2)  || \
                                ((INSTANCE) == TIM3)  || \
                                ((INSTANCE) == TIM4)  || \
                                ((INSTANCE) == TIM5)  || \
                                ((INSTANCE) == TIM6)  || \
                                ((INSTANCE) == TIM7)  || \
                                ((INSTANCE) == TIM12) || \
                                ((INSTANCE) == TIM13) || \
                                ((INSTANCE) == TIM14))

#define IS_32bARR_TIM(INSTANCE)  (((INSTANCE) == TIM2)  || \
                                  ((INSTANCE) == TIM5))
// clang-format on

TIM_HandleTypeDef htim2; // core/adc trigger
TIM_HandleTypeDef htim5; // lcd backlight
TIM_HandleTypeDef htim6; // core/dac trigger
TIM_HandleTypeDef htim7; // HAL timebase

bool tim_timebase_config(TIM_HandleTypeDef* htim, uint32_t freq)
{
    // retrieve current clock configuration
    RCC_ClkInitTypeDef clock_conf;
    uint32_t flash_latency;
    HAL_RCC_GetClockConfig(&clock_conf, &flash_latency);
    uint32_t apb_div = IS_APB1_TIM(htim->Instance) ? clock_conf.APB1CLKDivider : clock_conf.APB2CLKDivider;

    // get timer clock frequency
    uint32_t tim_clock = IS_APB1_TIM(htim->Instance) ? HAL_RCC_GetPCLK1Freq() : HAL_RCC_GetPCLK2Freq();
    if (apb_div != RCC_HCLK_DIV1) {
        tim_clock *= 2;
    }

    // determine max auto-reload value
    uint32_t max_arr = IS_32bARR_TIM(htim->Instance) ? 0xFFFFFFFF : 0xFFFF;
    const uint32_t max_psc = 0xFFFF;

    // calculate required clock division factor, prescaler and auto-reload value
    uint32_t target_freq_div = tim_clock / freq;
    if (target_freq_div - 1 <= max_arr) {
        htim->Init.Prescaler = 0;
        htim->Init.Period = target_freq_div - 1;
    } else {
        uint32_t min_psc = ((uint64_t)target_freq_div + max_arr) / ((uint64_t)max_arr + 1) - 1;
        if (min_psc > max_psc) {
            return false; // target frequency too low for this timer configuration
        }

        htim->Init.Prescaler = min_psc;
        htim->Init.Period = target_freq_div / ((uint64_t)min_psc + 1) - 1;
    }
    return true;
}

// override the native HAL TIM MSP functions (weak-defined)
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
    if (htim == &htim7) {
        __HAL_RCC_TIM7_CLK_ENABLE();
    }

    else if (htim == &htim5) {
        /* GPIO:
            PA0 -> TIM5_CH1
        */
        GPIO_InitTypeDef gpiox_conf = {
            .Pin = GPIO_PIN_0,
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
            .Alternate = GPIO_AF2_TIM5,
        };

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_TIM5_CLK_ENABLE();
        HAL_GPIO_Init(GPIOA, &gpiox_conf);
    }

    else if (htim == &htim6) {
        __HAL_RCC_TIM6_CLK_ENABLE();
    }

    else if (htim == &htim2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
}
