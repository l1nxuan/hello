
target_sources(${PROJECT_NAME} PRIVATE
    # {{BEGIN_TARGET_SOURCES}}
    ${CMAKE_CURRENT_LIST_DIR}/gnu/src/st_ltdc_use_dma2d_flush.c
    # {{END_TARGET_SOURCES}}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/gnu/inc
)
