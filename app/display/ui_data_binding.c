#include <stdint.h>
#include "screens.h"
#include "lvgl.h"

int32_t chart_1_serie_1_buffer[512]; // LTR

void ui_data_binding(void)
{
    // specific
    lv_chart_set_div_line_count(objects.waveform_area, 9, 15);
    lv_obj_add_flag(objects.waveform_area, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

    lv_chart_set_type(objects.waveform_area, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(objects.waveform_area, 512);
    lv_chart_set_axis_range(objects.waveform_area, LV_CHART_AXIS_PRIMARY_Y, 0, 0x117B); // ~3.6V

    lv_chart_series_t* serie_1 = lv_chart_add_series(objects.waveform_area, lv_color_hex(0xFFFF0000),
        LV_CHART_AXIS_PRIMARY_Y);

    // default values
    lv_chart_set_series_ext_y_array(objects.waveform_area, serie_1, chart_1_serie_1_buffer);
}
