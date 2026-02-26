#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "tim.h"
#include "adc.h"
#include "utils.h"

ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma2;

ADC_HandleTypeDef hadc2;

ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma3;

extern TIM_HandleTypeDef htim2;

static void adc_trigger_init(void)
{
    // HAL_TIM_PWM_Init()
    htim2.Instance = TIM2;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (tim_timebase_config(&htim2, ADC_FREQ_DEFAULT) != true) {
        PRINTF("%s failed!", ERR0R, __FUNCTION__);
    }
    (void)HAL_TIM_Base_Init(&htim2);

    TIM_ClockConfigTypeDef clock_src = {
        .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    (void)HAL_TIM_ConfigClockSource(&htim2, &clock_src);

    // unrelated
    TIM_MasterConfigTypeDef trgo = {
        .MasterOutputTrigger = TIM_TRGO_RESET,
        .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
    };
    (void)HAL_TIMEx_MasterConfigSynchronization(&htim2, &trgo);

    // to trigger ADC1&3 at each CC event
    TIM_OC_InitTypeDef oc_conf = {
        .OCMode = TIM_OCMODE_PWM1,
        .Pulse = htim2.Init.Period / 2, // 50% duty cycle for ADC1 trigger
        .OCPolarity = TIM_OCPOLARITY_LOW,
        .OCFastMode = TIM_OCFAST_DISABLE,
    };
    (void)HAL_TIM_PWM_ConfigChannel(&htim2, &oc_conf, TIM_CHANNEL_2);

    // almost 100% duty cycle for ADC3 trigger
    oc_conf.Pulse = htim2.Init.Period - 1;
    (void)HAL_TIM_PWM_ConfigChannel(&htim2, &oc_conf, TIM_CHANNEL_4);
}

static void adc1_dma_init(void)
{
    // no HAL_DMA_MspInit()
    __HAL_RCC_DMA2_CLK_ENABLE();

    hdma2.Instance = DMA2_Stream0;
    hdma2.Init.Channel = DMA_CHANNEL_0;
    hdma2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma2.Init.MemInc = DMA_MINC_ENABLE;
    hdma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma2.Init.Mode = DMA_CIRCULAR;
    hdma2.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    HAL_StatusTypeDef ret = HAL_DMA_Init(&hdma2);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }

    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void adc1_init(void)
{
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; // fADC = 21Mhz, max: 36MHz when VDDA = 2.4 to 3.6V
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC2;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1; // regular group
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    (void)HAL_ADC_Init(&hadc1);

    ADC_ChannelConfTypeDef ch13 = {
        .Channel = ADC_CHANNEL_13,
        .Rank = 1,
        .SamplingTime = ADC_SAMPLETIME_3CYCLES, // tCONV = (3 + 12) cycles @ 21MHz → max ~1.4MSPS
    };
    (void)HAL_ADC_ConfigChannel(&hadc1, &ch13);

    adc1_dma_init();
    __HAL_LINKDMA(&hadc1, DMA_Handle, hdma2);

    if (htim2.State == HAL_TIM_STATE_RESET) {
        adc_trigger_init();
    }
}

void adc2_init(void)
{
    hadc2.Instance = ADC2;
    hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; // fADC = 21Mhz, max: 36MHz when VDDA = 2.4 to 3.6V
    hadc2.Init.Resolution = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode = DISABLE;
    hadc2.Init.ContinuousConvMode = DISABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion = 1; // regular group
    hadc2.Init.DMAContinuousRequests = DISABLE;
    hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    (void)HAL_ADC_Init(&hadc2);

    ADC_ChannelConfTypeDef ch10 = {
        .Channel = ADC_CHANNEL_10,
        .Rank = 1,
        .SamplingTime = ADC_SAMPLETIME_480CYCLES, // tCONV = (480 + 12) cycles @ 21MHz → max ~42.7kSPS
    };
    (void)HAL_ADC_ConfigChannel(&hadc2, &ch10);
}

static void adc3_dma_init(void)
{
    // no HAL_DMA_MspInit()
    __HAL_RCC_DMA2_CLK_ENABLE();

    hdma3.Instance = DMA2_Stream1;
    hdma3.Init.Channel = DMA_CHANNEL_2;
    hdma3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma3.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma3.Init.MemInc = DMA_MINC_ENABLE;
    hdma3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma3.Init.Mode = DMA_CIRCULAR;
    hdma3.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    hdma3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    HAL_StatusTypeDef ret = HAL_DMA_Init(&hdma3);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }
}

void adc3_init(void)
{
    hadc3.Instance = ADC3;
    hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; // fADC = 21Mhz, max: 36MHz when VDDA = 2.4 to 3.6V
    hadc3.Init.Resolution = ADC_RESOLUTION_12B;
    hadc3.Init.ScanConvMode = DISABLE;
    hadc3.Init.ContinuousConvMode = DISABLE;
    hadc3.Init.DiscontinuousConvMode = DISABLE;
    hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC4;
    hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc3.Init.NbrOfConversion = 1; // regular group
    hadc3.Init.DMAContinuousRequests = ENABLE;
    hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    (void)HAL_ADC_Init(&hadc3);

    static ADC_AnalogWDGConfTypeDef awd_adc3_conf = {
        .WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG,
        .Channel = ADC_CHANNEL_10,
        .ITMode = ENABLE, // AWD interrupt request
        .HighThreshold = 0x0FFF,
        .LowThreshold = 0x0000,
    };
    (void)HAL_ADC_AnalogWDGConfig(&hadc3, &awd_adc3_conf);

    ADC_ChannelConfTypeDef ch10 = {
        .Channel = ADC_CHANNEL_10,
        .Rank = 1,
        .SamplingTime = ADC_SAMPLETIME_3CYCLES, // tCONV = (3 + 12) cycles @ 21MHz → max ~1.4MSPS
    };
    (void)HAL_ADC_ConfigChannel(&hadc3, &ch10);

    adc3_dma_init();
    __HAL_LINKDMA(&hadc3, DMA_Handle, hdma3);

    // enable ADCx GLOBAL interrupt
    HAL_NVIC_SetPriority(ADC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    if (htim2.State == HAL_TIM_STATE_RESET) {
        adc_trigger_init();
    }
}

void adc_freq_update(adc_freq_t new_freq)
{
    // update the htim2 PSC and ARR
    if (tim_timebase_config(&htim2, new_freq) != true) {
        PRINTF("%s failed!", ERR0R, __FUNCTION__);
    }

    // preload registers transferred to active registers on U event
    __HAL_TIM_DISABLE(&htim2);
    __HAL_TIM_SET_PRESCALER(&htim2, htim2.Init.Prescaler);
    __HAL_TIM_SET_AUTORELOAD(&htim2, htim2.Init.Period);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, htim2.Init.Period / 2);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, htim2.Init.Period - 1);
    __HAL_TIM_ENABLE(&htim2);
}

void adc_start(ADC_HandleTypeDef* hadc, uint16_t* buffer, uint32_t len)
{
    // a ADC DMA request is generated when an external trigger occurs
    (void)HAL_ADC_Start_DMA(hadc, (uint32_t*)buffer, len);

    // start PWM [output] for ADC trigger
    if (hadc == &hadc1) {
        (void)HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    } else {
        (void)HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
    }
}

void adc_stop(ADC_HandleTypeDef* hadc)
{
    if (hadc == &hadc1) {
        (void)HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
    } else {
        (void)HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
    }
    (void)HAL_ADC_Stop_DMA(hadc);
}

// override the native HAL ADC MSP functions (weak-defined)
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    /* GPIO:
        PC3 -> ADC1_IN13
    */
    if (hadc == &hadc1) {
        GPIO_InitTypeDef gpiox_conf = {
            .Pin = GPIO_PIN_3,
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_ADC1_CLK_ENABLE();
        HAL_GPIO_Init(GPIOC, &gpiox_conf);
    }

    /* GPIO:
        PC0 -> ADC2_IN10
    */
    else if (hadc == &hadc2) {
        GPIO_InitTypeDef gpiox_config = {
            .Pin = GPIO_PIN_0,
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_ADC2_CLK_ENABLE();
        HAL_GPIO_Init(GPIOC, &gpiox_config);
    }

    /* GPIO:
        PC0 -> ADC3_IN10
    */
    else if (hadc == &hadc3) {
        GPIO_InitTypeDef gpiox_config = {
            .Pin = GPIO_PIN_0,
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_ADC3_CLK_ENABLE();
        HAL_GPIO_Init(GPIOC, &gpiox_config);
    }
}

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
    // TODO: handle ADC3 AWD event
}
