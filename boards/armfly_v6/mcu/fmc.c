#include <stdbool.h>
#include "stm32f4xx_hal.h"

SRAM_HandleTypeDef hsram1;   // FMC Bank 1 - NOR/PSRAM 2 -> expand io
SDRAM_HandleTypeDef hsdram1; // FMC Bank 5 - SDRAM 1

static bool shared_signals_inited = false;

static void fmc_shared_init(void)
{
    /* GPIO:
        PD0  -> FMC_D2
        PD1  -> FMC_D3
        PD4  -> FMC_NOE
        PD5  -> FMC_NWE
        PD8  -> FMC_D13
        PD9  -> FMC_D14
        PD10 -> FMC_D15
        PD14 -> FMC_D0
        PD15 -> FMC_D1

        PE0  -> FMC_NBL0
        PE1  -> FMC_NBL1
        PE7  -> FMC_D4
        PE8  -> FMC_D5
        PE9  -> FMC_D6
        PE10 -> FMC_D7
        PE11 -> FMC_D8
        PE12 -> FMC_D9
        PE13 -> FMC_D10
        PE14 -> FMC_D11
        PE15 -> FMC_D12

        PF0  -> FMC_A0
        PF1  -> FMC_A1
        PF2  -> FMC_A2
        PF3  -> FMC_A3
        PF4  -> FMC_A4
        PF5  -> FMC_A5
        PF11 -> FMC_NRAS
        PF12 -> FMC_A6
        PF13 -> FMC_A7
        PF14 -> FMC_A8
        PF15 -> FMC_A9

        PG0  -> FMC_A10
        PG1  -> FMC_A11
        PG4  -> FMC_BA0
        PG5  -> FMC_BA1
        PG8  -> FMC_SDCLK // TODO: separate pins for sdram and norsram
        PG9  -> FMC_NE2
        PG15 -> FMC_NCAS

        PH2  -> FMC_SDCKE0
        PH3  -> FMC_SDNE0
        PH5  -> FMC_SDNWE
        PH8  -> FMC_D16
        PH9  -> FMC_D17
        PH10 -> FMC_D18
        PH11 -> FMC_D19
        PH12 -> FMC_D20
        PH13 -> FMC_D21
        PH14 -> FMC_D22
        PH15 -> FMC_D23

        PI0  -> FMC_D24
        PI1  -> FMC_D25
        PI2  -> FMC_D26
        PI3  -> FMC_D27
        PI4  -> FMC_NBL2
        PI5  -> FMC_NBL3
        PI6  -> FMC_D28
        PI7  -> FMC_D29
        PI9  -> FMC_D30
        PI10 -> FMC_D31
    */
    GPIO_InitTypeDef gpiox_conf = {
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF12_FMC,
    };

    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    gpiox_conf.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
        | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &gpiox_conf);

    gpiox_conf.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
        | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpiox_conf);

    gpiox_conf.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11
        | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &gpiox_conf);

    gpiox_conf.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOG, &gpiox_conf);

    gpiox_conf.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
        | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH, &gpiox_conf);

    gpiox_conf.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
        | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI, &gpiox_conf);

    shared_signals_inited = true;
}

// override the native HAL FMC MSP functions (weak-defined)
void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram)
{
    if (!shared_signals_inited) {
        fmc_shared_init();
    }
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* hsdram)
{
    if (!shared_signals_inited) {
        fmc_shared_init();
    }
}
