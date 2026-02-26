#ifndef LATCH_IO_H
#define LATCH_IO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LATCH_IO_PIN_0  (1U << 0)
#define LATCH_IO_PIN_1  (1U << 1)
#define LATCH_IO_PIN_2  (1U << 2)
#define LATCH_IO_PIN_3  (1U << 3)
#define LATCH_IO_PIN_4  (1U << 4)
#define LATCH_IO_PIN_5  (1U << 5)
#define LATCH_IO_PIN_6  (1U << 6)
#define LATCH_IO_PIN_7  (1U << 7)
#define LATCH_IO_PIN_8  (1U << 8)
#define LATCH_IO_PIN_9  (1U << 9)
#define LATCH_IO_PIN_10 (1U << 10)
#define LATCH_IO_PIN_11 (1U << 11)
#define LATCH_IO_PIN_12 (1U << 12)
#define LATCH_IO_PIN_13 (1U << 13)
#define LATCH_IO_PIN_14 (1U << 14)
#define LATCH_IO_PIN_15 (1U << 15)
#define LATCH_IO_PIN_16 (1U << 16)
#define LATCH_IO_PIN_17 (1U << 17)
#define LATCH_IO_PIN_18 (1U << 18)
#define LATCH_IO_PIN_19 (1U << 19)
#define LATCH_IO_PIN_20 (1U << 20)
#define LATCH_IO_PIN_21 (1U << 21)
#define LATCH_IO_PIN_22 (1U << 22)
#define LATCH_IO_PIN_23 (1U << 23)
#define LATCH_IO_PIN_24 (1U << 24)
#define LATCH_IO_PIN_25 (1U << 25)
#define LATCH_IO_PIN_26 (1U << 26)
#define LATCH_IO_PIN_27 (1U << 27)
#define LATCH_IO_PIN_28 (1U << 28)
#define LATCH_IO_PIN_29 (1U << 29)
#define LATCH_IO_PIN_30 (1U << 30)
#define LATCH_IO_PIN_31 (1U << 31)

void latch_io_init(void);
uint8_t latch_io_read(uint32_t pin);
void latch_io_write(uint32_t pin, uint8_t level);
void latch_io_toggle(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif // LATCH_IO_H
