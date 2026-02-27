#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "utils.h"

#define EEPROM_I2C_ADDRESS     0xA0        // write form
#define EEPROM_PAGE_SIZE       64          // bytes
#define EEPROM_TOTAL_SIZE      (16 * 1024) // bytes
#define EEPROM_SELFTIMED_WRITE 5           // ms

extern I2C_HandleTypeDef hi2c1;

// TODO: ... AT24C128AN
void eeprom_init(void)
{
    if (hi2c1.State == HAL_I2C_STATE_RESET) {
        i2c1_init();
    }

    HAL_StatusTypeDef ret = HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDRESS, 1, 100);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", INF0, __FUNCTION__, ret);
    }
}

bool eeprom_sequential_read(uint8_t* dst, uint16_t address, uint32_t len)
{
    if (address + len > EEPROM_TOTAL_SIZE) { // len != 0
        return false;
    }

    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c1, EEPROM_I2C_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, dst, len,
        100);
    if (ret != HAL_OK) {
        PRINTF("%s failed, ret = %d.\n", INF0, __FUNCTION__, ret);
        return false;
    }

    return true;
}

bool eeprom_page_write(uint8_t* src, uint16_t address, uint32_t len)
{
    if (address + len > EEPROM_TOTAL_SIZE) { // len != 0
        return false;
    }

    uint32_t written = 0;
    uint16_t offset = address % EEPROM_PAGE_SIZE;
    while (written < len) {
        uint32_t chunk = MIN(len - written, EEPROM_PAGE_SIZE - offset);
        HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(&hi2c1, EEPROM_I2C_ADDRESS, address + written, I2C_MEMADD_SIZE_16BIT,
            src + written, chunk, 100);
        if (ret != HAL_OK) {
            PRINTF("%s failed, ret = %d.\n", INF0, __FUNCTION__, ret);
            return false;
        }

        HAL_Delay(EEPROM_SELFTIMED_WRITE);

        written += chunk;
        offset = 0;
    }

    return true;
}
