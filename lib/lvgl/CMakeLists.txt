target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/lv_init.c

    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_group.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_class.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_draw.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_event.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_id_builtin.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_pos.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_property.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_scroll.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_style_gen.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_style.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj_tree.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_obj.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core/lv_refr.c

    ${CMAKE_CURRENT_LIST_DIR}/src/display/lv_display.c

    # common + convert(sw) + dma2d + sw
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_3d.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_arc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_blur.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_buf.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_image.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_label.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_line.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_mask.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_rect.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_triangle.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw_vector.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_draw.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/lv_image_decoder.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/convert/lv_draw_buf_convert.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/dma2d/lv_draw_dma2d_fill.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/dma2d/lv_draw_dma2d_img.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/dma2d/lv_draw_dma2d.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend_to_a8.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend_to_al88.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888_premultiplied.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend_to_argb8888.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend_to_i1.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend_to_l8.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend_to_rgb565_swapped.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend_to_rgb565.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend_to_rgb888.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/blend/lv_draw_sw_blend.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_arc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_blur.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_border.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_box_shadow.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_fill.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_grad.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_img.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_letter.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_line.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_mask_rect.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_mask.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_transform.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_triangle.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw_vector.c
    ${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/lv_draw_sw.c

    # ltdc
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/display/st_ltdc/lv_st_ltdc.c

    ${CMAKE_CURRENT_LIST_DIR}/src/font/binfont_loader/lv_binfont_loader.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/fmt_txt/lv_font_fmt_txt.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_dejavu_16_persian_hebrew.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_8.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_10.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_12.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_14_aligned.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_14.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_16.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_18.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_20.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_22.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_24.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_26.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_28_compressed.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_28.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_30.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_32.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_34.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_36.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_38.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_40.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_42.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_44.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_46.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_montserrat_48.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_source_han_sans_sc_14_cjk.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_source_han_sans_sc_16_cjk.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_unscii_8.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font_unscii_16.c
    ${CMAKE_CURRENT_LIST_DIR}/src/font/lv_font.c

    ${CMAKE_CURRENT_LIST_DIR}/src/indev/lv_indev_gesture.c
    ${CMAKE_CURRENT_LIST_DIR}/src/indev/lv_indev_scroll.c
    ${CMAKE_CURRENT_LIST_DIR}/src/indev/lv_indev.c

    ${CMAKE_CURRENT_LIST_DIR}/src/layouts/flex/lv_flex.c
    ${CMAKE_CURRENT_LIST_DIR}/src/layouts/grid/lv_grid.c
    ${CMAKE_CURRENT_LIST_DIR}/src/layouts/lv_layout.c

    # 3RD PARTS LIBRARIES
    ${CMAKE_CURRENT_LIST_DIR}/src/libs/bin_decoder/lv_bin_decoder.c

    ${CMAKE_CURRENT_LIST_DIR}/src/misc/cache/class/lv_cache_lru_ll.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/cache/class/lv_cache_lru_rb.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/cache/class/lv_cache_sc_da.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/cache/instance/lv_image_cache.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/cache/instance/lv_image_header_cache.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/cache/lv_cache_entry.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/cache/lv_cache.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_anim_timeline.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_anim.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_area.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_array.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_async.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_bidi.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_circle_buf.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_color_op.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_color.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_event.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_fs.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_grad.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_iter.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_ll.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_log.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_lru.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_math.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_matrix.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_palette.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_profiler_builtin_posix.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_profiler_builtin.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_rb.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_style_gen.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_style.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_templ.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_text_ap.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_text.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_timer.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_tree.c
    ${CMAKE_CURRENT_LIST_DIR}/src/misc/lv_utils.c

    # tx
    ${CMAKE_CURRENT_LIST_DIR}/src/osal/lv_os_none.c
    ${CMAKE_CURRENT_LIST_DIR}/src/osal/lv_os.c
    ${CMAKE_CURRENT_LIST_DIR}/src/osal/lv_threadx.c

    # FEATURE CONFIGURATION

    # common + built-in
    ${CMAKE_CURRENT_LIST_DIR}/src/stdlib/lv_mem.c
    ${CMAKE_CURRENT_LIST_DIR}/src/stdlib/builtin/lv_mem_core_builtin.c
    ${CMAKE_CURRENT_LIST_DIR}/src/stdlib/builtin/lv_sprintf_builtin.c
    ${CMAKE_CURRENT_LIST_DIR}/src/stdlib/builtin/lv_string_builtin.c
    ${CMAKE_CURRENT_LIST_DIR}/src/stdlib/builtin/lv_tlsf.c

    ${CMAKE_CURRENT_LIST_DIR}/src/themes/default/lv_theme_default.c
    ${CMAKE_CURRENT_LIST_DIR}/src/themes/mono/lv_theme_mono.c
    ${CMAKE_CURRENT_LIST_DIR}/src/themes/simple/lv_theme_simple.c
    ${CMAKE_CURRENT_LIST_DIR}/src/themes/lv_theme.c

    ${CMAKE_CURRENT_LIST_DIR}/src/tick/lv_tick.c

    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/3dtexture/lv_3dtexture.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/animimage/lv_animimage.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/arc/lv_arc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/arclabel/lv_arclabel.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/bar/lv_bar.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/button/lv_button.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/buttonmatrix/lv_buttonmatrix.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/calendar/lv_calendar_chinese.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/calendar/lv_calendar_header_arrow.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/calendar/lv_calendar_header_dropdown.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/calendar/lv_calendar.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/canvas/lv_canvas.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/chart/lv_chart.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/checkbox/lv_checkbox.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/dropdown/lv_dropdown.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/image/lv_image.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/imagebutton/lv_imagebutton.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/keyboard/lv_keyboard.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/label/lv_label.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/led/lv_led.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/line/lv_line.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/list/lv_list.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/lottie/lv_lottie.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/menu/lv_menu.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/msgbox/lv_msgbox.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/objx_templ/lv_objx_templ.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_animimage_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_dropdown_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_image_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_keyboard_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_label_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_obj_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_roller_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_slider_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_style_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/property/lv_textarea_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/roller/lv_roller.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/scale/lv_scale.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/slider/lv_slider.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/span/lv_span.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/spinbox/lv_spinbox.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/spinner/lv_spinner.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/switch/lv_switch.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/table/lv_table.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/tabview/lv_tabview.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/textarea/lv_textarea.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/tileview/lv_tileview.c
    ${CMAKE_CURRENT_LIST_DIR}/src/widgets/win/lv_win.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
