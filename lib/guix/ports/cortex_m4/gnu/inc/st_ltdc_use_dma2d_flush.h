#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include "gx_api.h"

#ifdef __cplusplus
extern "C" {
#endif

// e.g.
#define GX_FLUSH_DMA2D_HAL_INCLUDE "stm32f4xx_hal.h"

#define ST_LTDC_FRAMEBUF_ADDR      0xC0000000
#define DISPLAY_ACTIVE_WIDTH       480

UINT st_ltdc_use_dma2d_flush_565rgb(GX_DISPLAY* display);

#ifdef __cplusplus
}
#endif

#endif // DISPLAY_DRIVER_H
