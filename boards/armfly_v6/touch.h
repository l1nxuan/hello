#ifndef TOUCH_H
#define TOUCH_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void touch_init(void);
bool touch_get_coord(uint16_t* x, uint16_t* y);

#ifdef __cplusplus
}
#endif

#endif // TOUCH_H
