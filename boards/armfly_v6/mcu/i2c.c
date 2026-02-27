#include "stm32f4xx_hal.h"
#include "utils.h"

I2C_HandleTypeDef hi2c1; // slave: AT24C128AN(eeprom), STMPE811QTR(touch)

void i2c1_init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000; // Standard
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_StatusTypeDef ret = HAL_I2C_Init(&hi2c1);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }

    (void)HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE);
    (void)HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0); // Disable
}

// override the native HAL I2C MSP functions (weak-defined)
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    (void)hi2c;

    /* GPIO:
        PB6 -> I2C1_SCL
        PB9 -> I2C1_SDA
    */
    GPIO_InitTypeDef gpiox_conf = {
        .Pin = GPIO_PIN_6 | GPIO_PIN_9,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = GPIO_AF4_I2C1,
    };

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &gpiox_conf);
}
