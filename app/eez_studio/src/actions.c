#include "actions.h"

void action_hook_division_lines(lv_event_t* e)
{
    lv_draw_task_t* draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t* base_dsc = (lv_draw_dsc_base_t*)lv_draw_task_get_draw_dsc(draw_task);

    if (base_dsc->part != LV_PART_MAIN || lv_draw_task_get_type(draw_task) != LV_DRAW_TASK_TYPE_LINE) {
        return;
    }

    lv_draw_line_dsc_t* line_dsc = (lv_draw_line_dsc_t*)lv_draw_task_get_draw_dsc(draw_task);
    line_dsc->width = 1;
    line_dsc->color = lv_color_hex(0xffffffff);

    /* Vertical line */
    if (line_dsc->p1.x == line_dsc->p2.x) {
        if (base_dsc->id1 == 7) {
            line_dsc->dash_gap = 0;
            line_dsc->dash_width = 0;
        } else {
            line_dsc->dash_gap = 5;
            line_dsc->dash_width = 5;
        }
    }
    /* Horizontal line */
    else {
        if (base_dsc->id1 == 4) {
            line_dsc->dash_gap = 0;
            line_dsc->dash_width = 0;
        } else {
            line_dsc->dash_gap = 5;
            line_dsc->dash_width = 5;
        }
    }
}
