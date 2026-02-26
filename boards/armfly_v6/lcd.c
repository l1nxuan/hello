#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "lcd.h"

#define LCD_ACTIVE_WIDTH  800
#define LCD_HSYNC_WIDTH   48
#define LCD_HBP           88
#define LCD_HFP           40
#define LCD_ACTIVE_HEIGHT 480
#define LCD_VSYNC_WIDTH   3
#define LCD_VBP           32
#define LCD_VFP           13

// PWM -> PT4110E89E
extern TIM_HandleTypeDef htim5;

extern LTDC_HandleTypeDef hltdc;

static void lcd_backlight_init(void)
{
    RCC_ClkInitTypeDef clock_conf;
    uint32_t latency;
    HAL_RCC_GetClockConfig(&clock_conf, &latency);
    uint32_t clock = clock_conf.APB1CLKDivider == RCC_HCLK_DIV1 ? HAL_RCC_GetPCLK1Freq() : 2 * HAL_RCC_GetPCLK1Freq();

    htim5.Instance = TIM5;
    htim5.Init.Prescaler = (clock / 1000000) - 1; // have TIM5 counter clock equal to 1MHz
    htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim5.Init.Period = (1000000 / 100) - 1; // (1MHz counter clock / 100Hz tick rate) - 1
    htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    // HAL_TIM_PWM_Init()
    (void)HAL_TIM_Base_Init(&htim5);

    TIM_ClockConfigTypeDef clock_src = {
        .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    (void)HAL_TIM_ConfigClockSource(&htim5, &clock_src);

    TIM_MasterConfigTypeDef trgo = {
        .MasterOutputTrigger = TIM_TRGO_RESET,
        .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
    };
    (void)HAL_TIMEx_MasterConfigSynchronization(&htim5, &trgo);

    // PWM gen on TIM5_CH1, duty cycle = brightness / 1000
    TIM_OC_InitTypeDef oc_conf = {
        .OCMode = TIM_OCMODE_PWM1,
        .Pulse = LCD_DEFAULT_BRIGHTNESS,
        .OCFastMode = TIM_OCFAST_DISABLE,
        .OCPolarity = TIM_OCPOLARITY_HIGH, // active level
    };
    (void)HAL_TIM_PWM_ConfigChannel(&htim5, &oc_conf, TIM_CHANNEL_1);
}

void lcd_init(void)
{
    // TODO: ... OTD9960 + OTA7001
    hltdc.Instance = LTDC;
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc.Init.HorizontalSync = LCD_HSYNC_WIDTH - 1;
    hltdc.Init.VerticalSync = LCD_VSYNC_WIDTH - 1;
    hltdc.Init.AccumulatedHBP = LCD_HSYNC_WIDTH + LCD_HBP - 1;
    hltdc.Init.AccumulatedVBP = LCD_VSYNC_WIDTH + LCD_VBP - 1;
    hltdc.Init.AccumulatedActiveW = LCD_HSYNC_WIDTH + LCD_HBP + LCD_ACTIVE_WIDTH - 1;
    hltdc.Init.AccumulatedActiveH = LCD_VSYNC_WIDTH + LCD_VBP + LCD_ACTIVE_HEIGHT - 1;
    hltdc.Init.TotalWidth = LCD_HSYNC_WIDTH + LCD_HBP + LCD_ACTIVE_WIDTH + LCD_HFP - 1;
    hltdc.Init.TotalHeigh = LCD_VSYNC_WIDTH + LCD_VBP + LCD_ACTIVE_HEIGHT + LCD_VFP - 1;
    hltdc.Init.Backcolor.Blue = 255; // background
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;
    (void)HAL_LTDC_Init(&hltdc);

    LTDC_LayerCfgTypeDef layer = {
        .WindowX0 = 0,
        .WindowX1 = LCD_ACTIVE_WIDTH,
        .WindowY0 = 0,
        .WindowY1 = LCD_ACTIVE_HEIGHT,
        .PixelFormat = LTDC_PIXEL_FORMAT_RGB565,
        .Alpha = 255,
        .BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA,
        .BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA, // 1 * C + (1 - 1) * BG_C
        .FBStartAdress = LCD_LAYER_1_FRAMEBUFFER,
        .ImageWidth = LCD_ACTIVE_WIDTH,
        .ImageHeight = LCD_ACTIVE_HEIGHT,
        .Alpha0 = 255, // default color in the format ARGB
        .Backcolor = {
            .Blue = 0,
            .Green = 0,
            .Red = 0,
        },
    };
    (void)HAL_LTDC_ConfigLayer(&hltdc, &layer, LTDC_LAYER_1);

    // enable LTDC global interrupt
    HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(LTDC_IRQn);

    lcd_backlight_init();
}

void lcd_backlight_on(void)
{
    (void)HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
}
