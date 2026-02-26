#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "dac.h"
#include "utils.h"

DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma1;

static void dac_trigger_init(void)
{
    htim6.Instance = TIM6;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (tim_timebase_config(&htim6, DAC_FREQ_DEFAULT) != true) {
        PRINTF("%s failed!", ERR0R, __FUNCTION__);
    }
    (void)HAL_TIM_Base_Init(&htim6);

    TIM_MasterConfigTypeDef t6_trgo = {
        .MasterOutputTrigger = TIM_TRGO_UPDATE,
        .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
    };
    (void)HAL_TIMEx_MasterConfigSynchronization(&htim6, &t6_trgo);
}

// each DAC channel has a DMA capability
static void dac_ch1_dma_init(void)
{
    // no HAL_DMA_MspInit()
    __HAL_RCC_DMA1_CLK_ENABLE();

    hdma1.Instance = DMA1_Stream5;
    hdma1.Init.Channel = DMA_CHANNEL_7;
    hdma1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma1.Init.MemInc = DMA_MINC_ENABLE;
    hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma1.Init.Mode = DMA_CIRCULAR; // continuous output
    hdma1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    HAL_StatusTypeDef ret = HAL_DMA_Init(&hdma1);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }
}

void dac_init(void)
{
    hdac.Instance = DAC;
    (void)HAL_DAC_Init(&hdac);

    DAC_ChannelConfTypeDef ch1 = {
        .DAC_Trigger = DAC_TRIGGER_T6_TRGO, // U event
        .DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE,
    };
    (void)HAL_DAC_ConfigChannel(&hdac, &ch1, DAC_CHANNEL_1);

    dac_ch1_dma_init();
    __HAL_LINKDMA(&hdac, DMA_Handle1, hdma1);

    dac_trigger_init();
}

void dac_start(uint16_t* buffer, uint32_t len)
{
    // a DAC DMA request is generated when an external trigger occurs
    (void)HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)buffer, len, DAC_ALIGN_12B_R);
    (void)HAL_TIM_Base_Start(&htim6);
}

void dac_stop(void)
{
    (void)HAL_TIM_Base_Stop(&htim6);
    (void)HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
}

void dac_freq_update(dac_freq_t new_freq)
{
    // update the htim6 PSC and ARR
    if (tim_timebase_config(&htim6, new_freq) != true) {
        PRINTF("%s failed!", ERR0R, __FUNCTION__);
    }

    // preload registers transferred to active registers on U event
    __HAL_TIM_DISABLE(&htim6);
    __HAL_TIM_SET_PRESCALER(&htim6, htim6.Init.Prescaler);
    __HAL_TIM_SET_AUTORELOAD(&htim6, htim6.Init.Period);
    __HAL_TIM_ENABLE(&htim6);
}

// override the native HAL DAC MSP functions (weak-defined)
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    (void)hdac;

    /* GPIO:
        PA4 -> DAC_OUT1
    */
    GPIO_InitTypeDef gpiox_config = {
        .Pin = GPIO_PIN_4,
        .Mode = GPIO_MODE_ANALOG,
        .Pull = GPIO_NOPULL,
    };

    __HAL_RCC_DAC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &gpiox_config);
}
