#ifndef KEY_H
#define KEY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_SCAN_INTERVAL           10  // ms
#define KEY_DEBOUNCE_THRESHOLD      50  // ms
#define KEY_CLICK_TIMEOUT_THRESHOLD 500 // ms
// #define KEY_LONGPRESS_THRESHOLD     1000 // ms

typedef enum : uint8_t {
    KEY_BUTTON_1 = 0,    // S1
    KEY_BUTTON_2,        // S2
    KEY_BUTTON_3,        // S3
    KEY_JOYSTICK_DOWN,   // S5 DOWN
    KEY_JOYSTICK_SELECT, // S5 SELECT
    KEY_JOYSTICK_RIGHT,  // S5 RIGHT
    KEY_JOYSTICK_UP,     // S5 UP
    KEY_JOYSTICK_LEFT,   // S5 LEFT
    KEY_COUNT,
} key_t;

typedef enum : uint8_t {
    KEY_EVENT_KEYUP = 0,
    KEY_EVENT_KEYDOWN,
#ifdef KEY_CLICK_TIMEOUT_THRESHOLD
    KEY_EVENT_CLICK, // key up + key down
#endif
#ifdef KEY_LONGPRESS_THRESHOLD
    KEY_EVENT_LONGPRESS,
#endif
} key_event_t;

void key_init(void);
void key_scan(void);

// weak-defined
void key_on_press(key_t key);
void key_on_release(key_t key);
void key_on_click(key_t key, uint8_t clicks);
void key_on_longpress(key_t key, uint32_t duration_ms);

#ifdef __cplusplus
}
#endif

#endif // KEY_H
