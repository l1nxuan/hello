#include "stm32f4xx_hal.h"

LTDC_HandleTypeDef hltdc;

// override the native HAL LTDC MSP functions (weak-defined)
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
    (void)hltdc;

    /* GPIO:
        PI12 -> LTDC_HSYNC
        PI13 -> LTDC_VSYNC
        PI14 -> LTDC_CLK
        PI15 -> LTDC_R0

        PJ0  -> LTDC_R1
        PJ1  -> LTDC_R2
        PJ2  -> LTDC_R3
        PJ3  -> LTDC_R4
        PJ4  -> LTDC_R5
        PJ5  -> LTDC_R6
        PJ6  -> LTDC_R7
        PJ7  -> LTDC_G0
        PJ8  -> LTDC_G1
        PJ9  -> LTDC_G2
        PJ10 -> LTDC_G3
        PJ11 -> LTDC_G4
        PJ12 -> LTDC_B0
        PJ13 -> LTDC_B1
        PJ14 -> LTDC_B2
        PJ15 -> LTDC_B3

        PK0  -> LTDC_G5
        PK1  -> LTDC_G6
        PK2  -> LTDC_G7
        PK3  -> LTDC_B4
        PK4  -> LTDC_B5
        PK5  -> LTDC_B6
        PK6  -> LTDC_B7
        PK7  -> LTDC_DE
        */
    GPIO_InitTypeDef gpiox_conf = {
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF14_LTDC,
    };

    __HAL_RCC_LTDC_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    gpiox_conf.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOI, &gpiox_conf);

    __HAL_RCC_GPIOJ_CLK_ENABLE();
    gpiox_conf.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
        | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
        | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOJ, &gpiox_conf);

    __HAL_RCC_GPIOK_CLK_ENABLE();
    gpiox_conf.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
        | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOK, &gpiox_conf);
}
