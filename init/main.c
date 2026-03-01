#include "stm32f4xx_hal.h"
#include "led.h"
#include "key.h"
#include "sdram.h"
#include "lcd.h"
#include "eeprom.h"
#include "touch.h"
#include "dac.h"
#include "adc.h"
#include "tx_api.h"
#include "utils.h"

static void system_clock_config(void)
{
    /* Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initializes the RCC Oscillators according to the specified parameters */
    RCC_OscInitTypeDef osc_conf = {
        .OscillatorType = RCC_OSCILLATORTYPE_HSE,
        .HSEState = RCC_HSE_ON,
        .PLL = {
            .PLLState = RCC_PLL_ON,
            .PLLSource = RCC_PLLSOURCE_HSE,
            .PLLM = 8,
            .PLLN = 336,
            .PLLP = RCC_PLLP_DIV2, // fSYSCLK = 168 MHz
            .PLLQ = 7,             // unused
        }
    };
    HAL_StatusTypeDef ret = HAL_RCC_OscConfig(&osc_conf);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }

    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitTypeDef clock_conf = {
        .ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
        .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider = RCC_SYSCLK_DIV1, // fHCLK (SystemCoreClock) = 168 MHz
        .APB1CLKDivider = RCC_HCLK_DIV4,  // fPCLK1 = 42 MHz, fAPB1TIMxCLK = 84 MHz
        .APB2CLKDivider = RCC_HCLK_DIV2,  // fPCLK2 = 84 MHz, fAPB2TIMxCLK = 168 MHz
    };
    ret = HAL_RCC_ClockConfig(&clock_conf, FLASH_LATENCY_5);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }
}

static void peripherals_clock_config(void)
{
    /* Initializes the peripherals clock */
    RCC_PeriphCLKInitTypeDef clock_conf = {
        .PeriphClockSelection = RCC_PERIPHCLK_LTDC,
        .PLLSAI = {
            .PLLSAIN = 120,
            .PLLSAIR = 2,
        },
        .PLLSAIDivR = RCC_PLLSAIDIVR_2, // fLCD-TFTCLK = 30Mhz
    };
    HAL_StatusTypeDef ret = HAL_RCCEx_PeriphCLKConfig(&clock_conf);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }
}

int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface [and the Systick]. */
    (void)HAL_Init();

    /* Configure the system clock, and the peripherals common clocks */
    system_clock_config();
    peripherals_clock_config();

    /* Initialize all configured peripherals */
    led_init();
    key_init();
    sdram_init();
    lcd_init();
    eeprom_init();
    touch_init();
    dac_init();
    adc_init();

#if (defined(ENABLE_TRACE_API))
    SEGGER_SYSVIEW_Conf();
#endif

    // subsequent scheduling occurs from the PendSV handling routine, priority = 255
    // Systick's periodic interrupt for the timer-related services, priority = 64
    tx_kernel_enter();

    while (1) { }
}

void HAL_MspInit(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE(); // SWD
    __HAL_RCC_GPIOH_CLK_ENABLE(); // HSE OSC

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    HAL_EnableCompensationCell();
}
