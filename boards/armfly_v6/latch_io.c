#include "stm32f4xx_hal.h"
#include <stdint.h>

#define LATCH_DN (*(uint32_t*)0x64001000) // 32-bit memory: FMC_NE2(0x64000000) + FMC_A11(0) << 13 + FMC_A10(1) << 12
static volatile uint32_t latch_qn;

extern SRAM_HandleTypeDef hsram1;

void latch_io_init(void)
{
    // SN74HC32PW + SN74HC574PW (latch) * 4
    // via sram memory controler, some attributes are invaild
    hsram1.Instance = FMC_NORSRAM_DEVICE;
    hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    hsram1.Init.NSBank = FMC_NORSRAM_BANK2;
    hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
    hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
    hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_32;
    hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
    hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram1.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
    hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
    hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
    hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
    hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE; // no single write timing
    hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
    hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;

    FMC_NORSRAM_TimingTypeDef timing = {
        .AddressSetupTime = 6 - 1, // (74HC32 Tpd + 74HC574 Tpd) - 1
        .AddressHoldTime = 1,      // ModeA access dont need
        .DataSetupTime = 2 + 1,    // 74HC574 Tsu + 1 fmc_ker_ck
        .BusTurnAroundDuration = 0,
        .CLKDivision = 2,
        .DataLatency = 2,
        .AccessMode = FMC_ACCESS_MODE_A,
    };
    (void)HAL_SRAM_Init(&hsram1, &timing, NULL);

    LATCH_DN = latch_qn;
}

uint8_t latch_io_read(uint32_t pin)
{
    return (latch_qn & pin) ? 1 : 0;
}

void latch_io_write(uint32_t pin, uint8_t level)
{
    latch_qn = level ? (latch_qn | pin) : (latch_qn & ~pin);
    LATCH_DN = latch_qn;
}

void latch_io_toggle(uint32_t pin)
{
    latch_qn ^= pin;
    LATCH_DN = latch_qn;
}
