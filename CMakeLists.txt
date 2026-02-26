cmake_minimum_required(VERSION 3.21)

project(hello LANGUAGES C ASM)

# MCU specific flags and linker script
set(TARGET_FLAGS
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)
set(LD_SCRIPT "${PROJECT_SOURCE_DIR}/drivers/Device/ST/STM32F4xx/STM32F429XX_FLASH.ld")

# source files and include directories
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    # Cortex-M start: MSP@0x0000_0000, Reset@0x0000_0004; remapped by BOOT pins
    # Reset_Handler: defined in startup_xxx.s, calls SystemInit() and then main()
    ${PROJECT_SOURCE_DIR}/drivers/Device/ST/STM32F4xx/Source/gcc/startup_stm32f429xx.s
    ${PROJECT_SOURCE_DIR}/drivers/Device/ST/STM32F4xx/Source/system_stm32f4xx.c
    ${PROJECT_SOURCE_DIR}/init/main.c

    ${PROJECT_SOURCE_DIR}/init/syscalls.c
    ${PROJECT_SOURCE_DIR}/init/sysmem.c

    ${PROJECT_SOURCE_DIR}/app/tx_application_define.c
    ${PROJECT_SOURCE_DIR}/app/sample/sample.c
    ${PROJECT_SOURCE_DIR}/app/display/display_1.c
    ${PROJECT_SOURCE_DIR}/app/display/lv_log_print_cb.c
    ${PROJECT_SOURCE_DIR}/app/display/ui_data_binding.c
    ${PROJECT_SOURCE_DIR}/app/demo_waveform.c
    ${PROJECT_SOURCE_DIR}/app/eez_studio/src/ui.c
    ${PROJECT_SOURCE_DIR}/app/eez_studio/src/actions.c
    ${PROJECT_SOURCE_DIR}/app/eez_studio/src/images.c
    ${PROJECT_SOURCE_DIR}/app/eez_studio/src/screens.c
    ${PROJECT_SOURCE_DIR}/app/eez_studio/src/styles.c

    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu/adc.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu/dac.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu/dma.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu/fmc.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu/hal_assert.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu/hal_timebase.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu/ltdc.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu/nvic.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu/tim.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/key.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/latch_io.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/lcd.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/led.c
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/sdram.c

    ${PROJECT_SOURCE_DIR}/lib/RTT/RTT/SEGGER_RTT.c
    ${PROJECT_SOURCE_DIR}/lib/RTT/RTT/SEGGER_RTT_printf.c
    ${PROJECT_SOURCE_DIR}/lib/RTT/RTT/SEGGER_RTT_ASM_ARMv7M.S
    ${PROJECT_SOURCE_DIR}/lib/swtimer/swtimer.c

    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    # ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include

    ${PROJECT_SOURCE_DIR}/app
    ${PROJECT_SOURCE_DIR}/app/sample
    ${PROJECT_SOURCE_DIR}/app/display
    ${PROJECT_SOURCE_DIR}/app/eez_studio/src

    ${PROJECT_SOURCE_DIR}/boards/armfly_v6
    ${PROJECT_SOURCE_DIR}/boards/armfly_v6/mcu

    ${PROJECT_SOURCE_DIR}/lib/RTT/Config
    ${PROJECT_SOURCE_DIR}/lib/RTT/RTT
    ${PROJECT_SOURCE_DIR}/lib/swtimer

    ${PROJECT_SOURCE_DIR}/drivers/CMSIS/Core/Include
    ${PROJECT_SOURCE_DIR}/drivers/Device/ST/STM32F4xx/Include
    ${PROJECT_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Inc
)

# threadx
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/threadx/ports_arch/ARMv7-M/gnu)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/threadx/common)

# guix
# add_subdirectory(${PROJECT_SOURCE_DIR}/lib/guix/ports/cortex_m4/gnu)
# add_subdirectory(${PROJECT_SOURCE_DIR}/lib/guix/common)

# lvgl
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/lvgl)

# compiler settings
target_compile_definitions(${PROJECT_NAME} PRIVATE
    STM32F429xx
    USE_HAL_DRIVER
    TX_INCLUDE_USER_DEFINE_FILE
    # GX_INCLUDE_USER_DEFINE_FILE
    LV_LVGL_H_INCLUDE_SIMPLE
    LV_KCONFIG_IGNORE
    # LV_DISABLE_API_MAPPING
    $<$<CONFIG:Debug>:DEBUG>
)
target_compile_options(${PROJECT_NAME} PRIVATE
    ${TARGET_FLAGS}
    $<$<COMPILE_LANGUAGE:ASM>:
        -x assembler-with-cpp
        -MMD
        -MP
    >
    $<$<COMPILE_LANGUAGE:C>:
        -std=gnu11
        -Wall
        # -Wextra
        # -Wconversion
        -fdata-sections
        -ffunction-sections
    >
)

# link libraries and settings
target_link_libraries(${PROJECT_NAME} PRIVATE
    m   # math
)
target_link_options(${PROJECT_NAME} PRIVATE
    ${TARGET_FLAGS}
    "-T${LD_SCRIPT}"
    --specs=nano.specs
    -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
    -Wl,--gc-sections
    -Wl,--print-memory-usage
)

# add map file to clean target
set_target_properties(${PROJECT_NAME} PROPERTIES
    ADDITIONAL_CLEAN_FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map"
)
