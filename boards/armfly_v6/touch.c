#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stmpe811.h"
#include "stmpe811_conf.h"
#include "i2c.h"
#include "utils.h"

#define TOUCH_I2C_ADDRESS 0x82 // write form
#define TOUCH_I2C_TIMEOUT 100  // ms

extern I2C_HandleTypeDef hi2c1;
static STMPE811_Object_t htouch;

int32_t touch_i2c_write(void* handle, uint16_t reg, uint8_t* data, uint16_t len)
{
    (void)handle;

    HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(&hi2c1, TOUCH_I2C_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, len,
        TOUCH_I2C_TIMEOUT);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", INF0, __FUNCTION__, ret);
        return STMPE811_ERROR;
    }

    return STMPE811_OK;
}

int32_t touch_i2c_read(void* handle, uint16_t reg, uint8_t* data, uint16_t len)
{
    (void)handle;

    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, TOUCH_I2C_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, data, len,
        TOUCH_I2C_TIMEOUT);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", INF0, __FUNCTION__, ret);
        return STMPE811_ERROR;
    }

    return STMPE811_OK;
}

int32_t touch_gettick(void)
{
    return HAL_GetTick();
}

// TODO: ... STMPE811QTR
void touch_init(void)
{
    if (hi2c1.State == HAL_I2C_STATE_RESET) {
        i2c1_init();
    }

    // single point reading
    htouch.IO.GetTick = touch_gettick;
    htouch.Ctx.ReadReg = touch_i2c_read;
    htouch.Ctx.WriteReg = touch_i2c_write;
    htouch.IsInitialized = 1; // hardcode
    int32_t ret = STMPE811_TS_Init(&htouch);
    if (ret != STMPE811_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }

    // GPIO_ALT_FUNCT Reset: 0x00
    (void)STMPE811_IO_Start(&htouch,
        STMPE811_GPIO_PIN_0 | STMPE811_GPIO_PIN_1 | STMPE811_GPIO_PIN_2 | STMPE811_GPIO_PIN_3);

    // detect TOUCH event
    GPIO_InitTypeDef gpiox_conf = {
        .Pin = GPIO_PIN_7,
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };
    __HAL_RCC_GPIOH_CLK_ENABLE();
    HAL_GPIO_Init(GPIOH, &gpiox_conf);

    /* Enable TS fifo threshold IT */
    ret = STMPE811_TS_EnableIT(&htouch);
    if (ret != STMPE811_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }

    // verify touch panel
    ret = STMPE811_IO_ReadPin(&htouch,
        STMPE811_GPIO_PIN_0 | STMPE811_GPIO_PIN_1 | STMPE811_GPIO_PIN_2 | STMPE811_GPIO_PIN_3);
    if (ret != 0x00000004) { // 5" 800x480
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }
}

bool touch_get_coord(uint16_t* x, uint16_t* y)
{
    STMPE811_State_t state;
    int32_t ret = STMPE811_TS_GetState(&htouch, &state);
    if (ret != STMPE811_OK) {
        PRINTF("%s failed, ret = %d.\n", ERR0R, __FUNCTION__, ret);
    }

    // TODO: R touchscreen calibrate
    if (state.TouchDetected) {
        *x = (uint16_t)((state.TouchX * STMPE811_MAX_X_LENGTH) >> 12); // 12-bit ADC
        *y = (uint16_t)((state.TouchY * STMPE811_MAX_Y_LENGTH) >> 12);
        return true;
    }

    return false;
}
