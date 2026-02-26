#ifndef SDRAM_H
#define SDRAM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SDRAM_ADDRESS       ((uint32_t)0xC0000000) // FMC Bank 5

#define SDRAM_BANK_SIZE     (1U * 4096 * 256 * 32 / 8) // bytes -> 4MB
#define SDRAM_BANK1_ADDRESS (SDRAM_ADDRESS + SDRAM_BANK_SIZE * 0)
#define SDRAM_BANK2_ADDRESS (SDRAM_ADDRESS + SDRAM_BANK_SIZE * 1)
#define SDRAM_BANK3_ADDRESS (SDRAM_ADDRESS + SDRAM_BANK_SIZE * 2)
#define SDRAM_BANK4_ADDRESS (SDRAM_ADDRESS + SDRAM_BANK_SIZE * 3)

void sdram_init(void);
void sdram_testcase(void);

#ifdef __cplusplus
}
#endif

#endif // SDRAM_H
