#include <stdint.h>
#include "st_ltdc_use_dma2d_flush.h"
#include GX_FLUSH_DMA2D_HAL_INCLUDE
#include "gx_api.h"
#include "gx_display.h"

VOID display_buffer_565rgb_flush(GX_CANVAS* canvas, GX_RECTANGLE* dirty_area)
{
    // copy only the dirty area from managed canvas to lcd gram
    uint16_t* src = (uint16_t*)canvas->gx_canvas_memory
        + dirty_area->gx_rectangle_top * canvas->gx_canvas_x_resolution + dirty_area->gx_rectangle_left;
    uint16_t* dst = (uint16_t*)ST_LTDC_FRAMEBUF_ADDR
        + (canvas->gx_canvas_display_offset_y + dirty_area->gx_rectangle_top) * DISPLAY_ACTIVE_WIDTH
        + canvas->gx_canvas_display_offset_x + dirty_area->gx_rectangle_left;
    uint32_t width = dirty_area->gx_rectangle_right - dirty_area->gx_rectangle_left + 1;
    uint32_t height = dirty_area->gx_rectangle_bottom - dirty_area->gx_rectangle_top + 1;

    // TODO: add cache cleaning
    DMA2D->CR = DMA2D_M2M;
    DMA2D->FGMAR = (uint32_t)src;
    DMA2D->OMAR = (uint32_t)dst;
    DMA2D->FGOR = canvas->gx_canvas_x_resolution - width; // pixels
    DMA2D->OOR = DISPLAY_ACTIVE_WIDTH - width;            // pixels
    DMA2D->FGPFCCR = DMA2D_INPUT_RGB565;                  // 16-bit per pixel
    DMA2D->OPFCCR = DMA2D_OUTPUT_RGB565;                  // 16-bit per pixel
    DMA2D->NLR = width << 16 | height;                    // pixel per line | number of lines
    DMA2D->CR |= DMA2D_CR_START;
    while (DMA2D->CR & DMA2D_CR_START) { }
}

UINT st_ltdc_use_dma2d_flush_565rgb(GX_DISPLAY* display)
{
    _gx_display_driver_565rgb_setup(display, GX_NULL, display_buffer_565rgb_flush);

    return GX_SUCCESS;
}
