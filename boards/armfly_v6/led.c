#include <stdbool.h>
#include <stdint.h>
#include "latch_io.h"
#include "led.h"

#define LED_LEVEL_ON  0
#define LED_LEVEL_OFF 1

void led_init(void)
{
    latch_io_init();
    led_off(LED_ALL);
}

void led_on(uint32_t leds)
{
    latch_io_write(leds, LED_LEVEL_ON);
}

void led_off(uint32_t leds)
{
    latch_io_write(leds, LED_LEVEL_OFF);
}

void led_toggle(uint32_t leds)
{
    latch_io_toggle(leds);
}

bool led_is_on(uint32_t led)
{
    return latch_io_read(led) == LED_LEVEL_ON;
}
