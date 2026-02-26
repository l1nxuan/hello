#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "sdram.h"
#include "utils.h"

#define SDRAM_MODEREG_BURST_LENGTH_1          ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   ((uint16_t)0x0000)
#define SDRAM_MODEREG_CAS_LATENCY_3           ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE  ((uint16_t)0x0200)

extern SDRAM_HandleTypeDef hsdram1;

static void sdram_init_sequence(SDRAM_HandleTypeDef* sdram)
{
    FMC_SDRAM_CommandTypeDef cmd;

    // enable clock
    cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
    cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    cmd.AutoRefreshNumber = 1;
    cmd.ModeRegisterDefinition = 0;
    (void)HAL_SDRAM_SendCommand(sdram, &cmd, HAL_MAX_DELAY);

    // 100us min
    HAL_Delay(1);

    //  precharge all banks
    cmd.CommandMode = FMC_SDRAM_CMD_PALL;
    cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    cmd.AutoRefreshNumber = 1;
    cmd.ModeRegisterDefinition = 0;
    (void)HAL_SDRAM_SendCommand(sdram, &cmd, HAL_MAX_DELAY);

    // auto-refresh
    cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    cmd.AutoRefreshNumber = 2;
    cmd.ModeRegisterDefinition = 0;
    (void)HAL_SDRAM_SendCommand(sdram, &cmd, HAL_MAX_DELAY);

    // program mode register
    cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    cmd.AutoRefreshNumber = 1;
    cmd.ModeRegisterDefinition = SDRAM_MODEREG_BURST_LENGTH_1 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL
        | SDRAM_MODEREG_CAS_LATENCY_3 | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
    (void)HAL_SDRAM_SendCommand(sdram, &cmd, HAL_MAX_DELAY);

    // set the refresh rate counter = (4096 refresh cycles every 64ms) * (1 / 84Mhz) - 20
    // a refresh command must be inserted once every 1293 clock cycles
    (void)HAL_SDRAM_ProgramRefreshRate(sdram, 1293);
}

void sdram_init(void)
{
    // TODO: ... ISSI IS42S32400F-6TLI
    hsdram1.Instance = FMC_SDRAM_DEVICE;
    hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
    hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8; // column pins: log2(256) = 8 -> A0-A7
    hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;      // row pins: log2(4096) = 12 -> A0-A11
    hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
    hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4; // bank pins: log2(4) = 2 -> BA0-BA1
    hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;              // allowable operation frequency: 166MHz
    hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2; // SDRAM clock = fHCLK / 2 = 84Mhz
    hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
    hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;

    FMC_SDRAM_TimingTypeDef timing = {
        .LoadToActiveDelay = 2,
        .ExitSelfRefreshDelay = 7,
        .SelfRefreshTime = 4,
        .RowCycleDelay = 6,
        .WriteRecoveryTime = 2,
        .RPDelay = 2,
        .RCDDelay = 2,
    };
    (void)HAL_SDRAM_Init(&hsdram1, &timing);

    // Initialize ExtSDRAM and Load Mode Register
    sdram_init_sequence(&hsdram1);
}

void sdram_testcase(void)
{
    PRINTF("%s start...\n", INF0, __FUNCTION__);

    uint32_t sdram_size = 4U * SDRAM_BANK_SIZE;

    // 1. word-level access test: verify basic read/write and address decoding
    uint32_t* word = (uint32_t*)SDRAM_ADDRESS;
    for (uint32_t i = 0; i < sdram_size / 4; i++) {
        *word = i;
        word++;
    }

    word = (uint32_t*)SDRAM_ADDRESS;
    for (uint32_t i = 0; i < sdram_size / 4; i++) {
        if (*word != i) {
            PRINTF("%s word-level access test failed.\n", ERR0R, __FUNCTION__);
        }
        word++;
    }

    // 2. invert in-place test: verify every data bit can toggle between 0 and 1
    word = (uint32_t*)SDRAM_ADDRESS;
    for (uint32_t i = 0; i < sdram_size / 4; i++) {
        *word = ~(*word);
        word++;
    }

    word = (uint32_t*)SDRAM_ADDRESS;
    for (uint32_t i = 0; i < sdram_size / 4; i++) {
        if (*word != ~i) {
            PRINTF("%s invert in-place test failed.\n", ERR0R, __FUNCTION__);
        }
        word++;
    }

    // 3. byte-level access test: verify FSMC_NBL0/NBL1 byte lane enables
    const uint8_t bytes[] = { 0x55, 0xA5, 0x5A, 0xAA };
    uint8_t* byte = (uint8_t*)SDRAM_ADDRESS;
    for (uint8_t i = 0; i < sizeof(bytes); i++) {
        *byte = bytes[i];
        byte++;
    }

    byte = (uint8_t*)SDRAM_ADDRESS;
    for (uint8_t i = 0; i < sizeof(bytes); i++) {
        if (*byte != bytes[i]) {
            PRINTF("%s byte-level access test failed.\n", ERR0R, __FUNCTION__);
        }
        byte++;
    }

    PRINTF("%s passed!\n", INF0, __FUNCTION__);
}
