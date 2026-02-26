#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "key.h"

#define KEY_LEVEL_UP               1
#define KEY_LEVEL_DOWN             0

#define KEY_STATE_BIT_CURR_LEVEL   ((uint8_t)0x01)
#define KEY_STATE_BIT_PREV_LEVEL   ((uint8_t)0x02)
#define KEY_STATE_BIT_FIRSTUP_FLAG ((uint8_t)0x04) // first state must be "UP"

#if KEY_LEVEL_UP == 1
#define KEY_STATE_UP (KEY_STATE_BIT_FIRSTUP_FLAG | KEY_STATE_BIT_PREV_LEVEL | KEY_STATE_BIT_CURR_LEVEL)
#else
#define KEY_STATE_UP (KEY_STATE_BIT_FIRSTUP_FLAG)
#endif // KEY_LEVEL_UP
#define KEY_STATE_KEYDOWN (KEY_STATE_UP ^ KEY_STATE_BIT_CURR_LEVEL)
#define KEY_STATE_DOWN    (KEY_STATE_KEYDOWN ^ KEY_STATE_BIT_PREV_LEVEL)
#define KEY_STATE_KEYUP   (KEY_STATE_DOWN ^ KEY_STATE_BIT_CURR_LEVEL)

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t state;
    // 1 byte padding or no padding
#ifdef KEY_CLICK_TIMEOUT_THRESHOLD
    uint8_t click_count;
    uint32_t prev_click_tick;
#endif
#ifdef KEY_LONGPRESS_THRESHOLD
    uint32_t longpress_detected_tick;
#endif
    uint32_t level_changed_tick;
    uint32_t prev_level_changed_tick;
} key_snap_t;

static key_snap_t keys[KEY_COUNT] = {
    [KEY_BUTTON_1] = { .port = GPIOI, .pin = GPIO_PIN_8 },
    [KEY_BUTTON_2] = { .port = GPIOC, .pin = GPIO_PIN_13 },
    [KEY_BUTTON_3] = { .port = GPIOH, .pin = GPIO_PIN_4 },
    [KEY_JOYSTICK_DOWN] = { .port = GPIOF, .pin = GPIO_PIN_10 },
    [KEY_JOYSTICK_SELECT] = { .port = GPIOI, .pin = GPIO_PIN_11 },
    [KEY_JOYSTICK_RIGHT] = { .port = GPIOG, .pin = GPIO_PIN_7 },
    [KEY_JOYSTICK_UP] = { .port = GPIOG, .pin = GPIO_PIN_2 },
    [KEY_JOYSTICK_LEFT] = { .port = GPIOG, .pin = GPIO_PIN_3 },
};

void key_init(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    GPIO_InitTypeDef gpiox_conf = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };

    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        key_snap_t* key = keys + i;
        gpiox_conf.Pin = key->pin;
        HAL_GPIO_Init(key->port, &gpiox_conf);
    }
}

void key_scan(void)
{
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        key_snap_t* key = keys + i;
        uint8_t curr_level = HAL_GPIO_ReadPin(key->port, key->pin);
        uint32_t curr_tick = HAL_GetTick();

        // This is to prevent initial detected state on hardware errors,
        // or when button is kept pressed after the system/lib reset.
        if (!(key->state & KEY_STATE_BIT_FIRSTUP_FLAG)) {
            if (curr_level == KEY_LEVEL_DOWN) {
                continue;
            }
            key->state = KEY_STATE_UP;
        }

#ifdef KEY_CLICK_TIMEOUT_THRESHOLD
        // handle "CLICK" event - after certain timeout
        if (key->click_count > 0 && curr_tick - key->prev_click_tick >= KEY_CLICK_TIMEOUT_THRESHOLD) {
            key_on_click(i, key->click_count);
            key->click_count = 0;
        }
#endif

        // debouce and record tick on level change
        if (curr_level ^ (key->state & KEY_STATE_BIT_CURR_LEVEL)) { // curr level must be bit-0
            // fsm: KEY_STATE_UP -> KEY_STATE_KEYDOWN or KEY_STATE_DOWN -> KEY_STATE_KEYUP
            key->state ^= KEY_STATE_BIT_CURR_LEVEL;
            key->level_changed_tick = curr_tick;
            continue;
        }

        // action on detecting unchanged level
        switch (key->state) {
        case KEY_STATE_UP:
            break;
        case KEY_STATE_KEYDOWN:
            // handle "KEYDOWN" event
            if (curr_tick - key->level_changed_tick >= KEY_DEBOUNCE_THRESHOLD) {
                // fsm: KEY_STATE_KEYDOWN -> KEY_STATE_DOWN
                key->state ^= KEY_STATE_BIT_PREV_LEVEL;
                key->prev_level_changed_tick = key->level_changed_tick;
#ifdef KEY_LONGPRESS_THRESHOLD
                key->longpress_detected_tick = key->level_changed_tick;
#endif
                key_on_press(i);
            }
            break;
        case KEY_STATE_DOWN:
#ifdef KEY_LONGPRESS_THRESHOLD
            // handle "LONGPRESS" event
            while (curr_tick - key->longpress_detected_tick >= KEY_LONGPRESS_THRESHOLD) {
                key_on_longpress(i, curr_tick - key->longpress_detected_tick);
                key->longpress_detected_tick += KEY_LONGPRESS_THRESHOLD;
            }
#endif
            break;
        case KEY_STATE_KEYUP:
            // handle "KEYUP" event
            if (curr_tick - key->level_changed_tick >= KEY_DEBOUNCE_THRESHOLD) {
#ifdef KEY_CLICK_TIMEOUT_THRESHOLD
                // handle "CLICK" event - count click times
                if (key->level_changed_tick - key->prev_level_changed_tick < KEY_CLICK_TIMEOUT_THRESHOLD) {
                    key->click_count++;
                    key->prev_click_tick = key->level_changed_tick;
                }
#endif
                // fsm: KEY_STATE_KEYUP -> KEY_STATE_UP
                key->state ^= KEY_STATE_BIT_PREV_LEVEL;
                key->prev_level_changed_tick = key->level_changed_tick;
                key_on_release(i);
            }
            break;
        }
    }
}

// #define __weak __attribute__((weak))
__WEAK void key_on_press(key_t key)
{
    (void)key;
}

__WEAK void key_on_release(key_t key)
{
    (void)key;
}

__WEAK void key_on_click(key_t key, uint8_t clicks)
{
    (void)key;
}

__WEAK void key_on_longpress(key_t key, uint32_t duration_ms)
{
    (void)key;
    (void)duration_ms;
}
