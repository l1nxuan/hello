#ifndef LCD_H
#define LCD_H

#include "sdram.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_LAYER_1_FRAMEBUFFER SDRAM_BANK1_ADDRESS

// TODO: lcd_backlight_update()
#define LCD_DEFAULT_BRIGHTNESS  88 // range: [0, 100)

void lcd_init(void);
void lcd_backlight_on(void);

#ifdef __cplusplus
}
#endif

#endif // LCD_H
