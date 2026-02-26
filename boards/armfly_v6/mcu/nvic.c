#include "stm32f4xx_hal.h"

extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma2;
extern LTDC_HandleTypeDef hltdc;

// called by DMA_IT_TC | DMA_IT_HT | ..., priority = 1 << 4 = 16
void DMA2_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma2);
}

// called by ADC3 AWD event and DMA underrun error, priority = 2 << 4 = 32
void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&hadc3);
}

// called by LTDC global error interrupt, priority = 3 << 4 = 48
// [Reload interrupt enabled by lv_st_ltdc/flush_cb]
void LTDC_IRQHandler(void)
{
    HAL_LTDC_IRQHandler(&hltdc);
}

// #if USE_RTOS
#if 1
extern TIM_HandleTypeDef htim7;

// called by TIM7 U event, priority = TICK_INT_PRIORITY (default: 0x0F) << 4 = 240
void TIM7_IRQHandler(void)
{
    // HAL_TIM_IRQHandler(&htim7);
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
    HAL_IncTick();
}
#else
#include "swtimer.h"

void SysTick_Handler(void)
{
    HAL_IncTick();
    swtimer_update();
}
#endif // USE_RTOS
