#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void eeprom_init(void);
bool eeprom_sequential_read(uint8_t* dst, uint16_t address, uint32_t len);
bool eeprom_page_write(uint8_t* src, uint16_t address, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif // EEPROM_H
