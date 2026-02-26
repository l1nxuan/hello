#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "tim.h"

extern TIM_HandleTypeDef htim7;

// override the native HAL timebase functions (weak-defined)
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    htim7.Instance = TIM7;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    // uwTickFreq is of type HAL_TickFreqTypeDef
    (void)tim_timebase_config(&htim7, (1000 / uwTickFreq));

    // start TIM7 as HAL timebase
    (void)HAL_TIM_Base_Init(&htim7);
    (void)HAL_TIM_Base_Start_IT(&htim7);

    // enable the TIM7 global interrupt
    if (TickPriority >= (1 << __NVIC_PRIO_BITS)) {
        return HAL_ERROR;
    }
    HAL_NVIC_SetPriority(TIM7_IRQn, TickPriority, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
    uwTickPrio = TickPriority;

    return HAL_OK;
}

void HAL_SuspendTick(void)
{
    __HAL_TIM_DISABLE_IT(&htim7, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void)
{
    __HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE);
}
