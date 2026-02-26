#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include <stdint.h>
#include "latch_io.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LED_1   LATCH_IO_PIN_8  // LD1
#define LED_2   LATCH_IO_PIN_9  // LD2
#define LED_3   LATCH_IO_PIN_10 // LD3
#define LED_4   LATCH_IO_PIN_11 // LD4
#define LED_ALL (LED_1 | LED_2 | LED_3 | LED_4)

void led_init(void);
void led_on(uint32_t leds);
void led_off(uint32_t leds);
void led_toggle(uint32_t leds);
bool led_is_on(uint32_t led); // one led at a time

#ifdef __cplusplus
}
#endif

#endif // LED_H
