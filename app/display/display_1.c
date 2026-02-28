#include <stdint.h>
#include "tx_application_define.h"
#include "display_1.h"
#include "ui.h"
#include "screens.h"
#include "ui_data_binding.h"
#include "sdram.h"
#include "lcd.h"
#include "tx_api.h"
#include "lvgl.h"

TX_THREAD display_1_thread __attribute__((section(".ccmram")));
UCHAR display_1_thread_stack[DISPLAY_1_THREAD_STACK_SIZE] __attribute__((section(".ccmram")));

TX_EVENT_FLAGS_GROUP refresh_events; // couldnt place in CCM RAM

lv_display_t* display_1;

VOID display_1_thread_entry(ULONG thread_input)
{
    (void)thread_input;

    lv_init();
    lv_tick_set_cb(tx_time_get);

    // double-buffered direct mode
    display_1 = lv_st_ltdc_create_direct((void*)LCD_LAYER_1_FRAMEBUFFER, (void*)SDRAM_BANK2_ADDRESS, 0);
    ui_init();
    ui_data_binding();

    // Start
    (void)lv_timer_handler();
    lv_sleep_ms(330);
    lcd_backlight_on();

    (VOID) tx_event_flags_create(&refresh_events, "refresh events");

    while (1) {
        ULONG actual_events;
        UINT ret = tx_event_flags_get(&refresh_events, REFRESH_EVENTS_DATA_READY, TX_OR_CLEAR, &actual_events,
            TX_NO_WAIT);

        if (ret == TX_SUCCESS && (actual_events & REFRESH_EVENTS_DATA_READY)) {
            lv_chart_refresh(objects.waveform_area);
        }

        uint32_t time_till_next = lv_timer_handler();

        if (time_till_next == LV_NO_TIMER_READY) {
            time_till_next = LV_DEF_REFR_PERIOD; /* 33 ms by default in lv_conf.h */
        }
        lv_sleep_ms(time_till_next);
    }
}

void display_1_thread_create(void)
{
    // while LVGL timers
    (VOID) tx_thread_create(&display_1_thread, "display 1", display_1_thread_entry, 0,
        display_1_thread_stack, DISPLAY_1_THREAD_STACK_SIZE, DISPLAY_1_THREAD_PRIORITY, DISPLAY_1_THREAD_PRIORITY,
        TX_NO_TIME_SLICE, TX_AUTO_START);
}
